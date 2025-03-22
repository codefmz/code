#include "gtest/gtest.h"
#include "base.h"
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class textureTest : public ::testing::Test {
protected:
    textureTest() {
    }
    ~textureTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(textureTest, texture1)
{
    initGLFW();
    GLFWwindow* window = createWindow();
    Shader shader(R"(E:\Code\opengl\OpenGlTest1\src\texture\shader.vs)",
        R"(E:\Code\opengl\OpenGlTest1\src\texture\shader.fs)");

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    int width, height, nrChannels;
    unsigned char *data = stbi_load(R"(E:\Code\opengl\OpenGlTest1\src\texture\container.jpg)",
        &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };


    unsigned int indices[] = {
        0, 1, 2, // 第一个三角形
        2, 3, 0  // 第二个三角形
    };

    //1. 生成顶点数组对象与顶点缓冲对象
    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    //2. 绑定顶点数组对象与顶点缓冲对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // 3. 复制顶点数组到缓冲中供OpenGL使用
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4. 解析顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //启用顶点属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); //启用顶点属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清空颜色缓冲区
        glClear(GL_COLOR_BUFFER_BIT);
        // draw our first triangle
        shader.use();
        // 绑定 VBO
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
}
