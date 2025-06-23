#include <opencv2/opencv.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <exception>

using namespace cv;

class helloTest : public ::testing::Test {
public:
    helloTest() {
    }
    ~helloTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST(heloTest, heloOpenCv)
{
    std::string imgPath = RES_DIR + std::string("/1.jpg");
    cv::Mat img = cv::imread(imgPath);
    cv::imshow("Test", img);
    cv::waitKey(0);
}

/* 生成颜色变化的 255 ~ 0 的 Mat */
void createAlphaMat(cv::Mat& mat)
{
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            Vec4b& rgba = mat.at<Vec4b>(i, j);
            rgba[0] = 0; //蓝色通道
            //绿色通道
            rgba[1] = saturate_cast<uchar>((float(mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX);
            //红色
            rgba[2] = saturate_cast<uchar>((float(mat.rows - i)) / ((float)mat.rows) * UCHAR_MAX);
            //透明度
            rgba[3] = saturate_cast<uchar>(0.5 * (rgba[1] + rgba[2]));
        }
    }
}

TEST(heloTest, showAlphMat)
{
    //创建带alpha通道的Mat
    Mat mat(480, 640, CV_8UC4);
    createAlphaMat(mat);

    std::vector<int> compression_params;
    compression_params.push_back(IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(0);

    //显示图片
    try {
        imwrite(RES_DIR + std::string("/透明Alpha值图.png"), mat, compression_params); //最后一个参数表示压缩参数，9是最高压缩
        imshow("生成的png图", mat);
        fprintf(stdout, "PNG图片文件的alpha数据保存完毕~\n可以在工程目录下查看由imwrite函数生成的图片\n");
        waitKey(0);
    } catch (std::runtime_error& ex) {
        fprintf(stderr, "图像转换成PNG格式发生错误：%s\n", ex.what());
        return;
    }
}

//混合图像
TEST(helloTest, imgMix)
{
    Mat image = imread(RES_DIR + std::string("/dota.jpg"), 1); //1 表示以彩色图片读取
    Mat logo = imread(RES_DIR + std::string("/logo.jpg"));

    //载入后先显示
    namedWindow("【2】原画图");
    imshow("【2】原画图", image);

    namedWindow("【3】logo图");
    imshow("【3】logo图", logo);

    Mat imageROI;
    imageROI = image(Rect(800, 350, logo.cols, logo.rows));
    // result(x, y) = alpha * image(x, y) + beta * imageROI(x, y) + gamma
    //0.5 0.3 图像权重占比，0. 偏移量， 最后一个参数，保存位置
    addWeighted(imageROI, 0.5, logo, 0.3, 0., imageROI);
    //显示结果
    namedWindow("【4】原画+logo图");
    imshow("【4】原画+logo图", image);

    waitKey(0);
    imwrite(RES_DIR + std::string("/dota_logo.png"), image);
}

#define WINDOW_NAME "滑动条"

const int g_nMaxAlphaValue = 100;//Alpha值的最大值
int g_nAlphaValueSlider;//滑动条对应的变量
double g_dAlphaValue;
double g_dBetaValue;
//声明存储图像的变量
Mat g_srcImage1;
Mat g_srcImage2;
Mat g_dstImage;

void on_Trackbar(int, void *) {
    //求出当前alpha值相对于最大值的比例
    g_dAlphaValue = (double) g_nAlphaValueSlider / g_nMaxAlphaValue;
    //则beta值为1减去alpha值
    g_dBetaValue = (1.0 - g_dAlphaValue);

    //根据alpha和beta值进行线性混合
    addWeighted(g_srcImage1, g_dAlphaValue, g_srcImage2, g_dBetaValue, 0.0, g_dstImage);

    //显示效果图
    imshow(WINDOW_NAME, g_dstImage);
}

TEST(helloCV, trackbar)
{
    //加载图像 (两图像的尺寸需相同)
    g_srcImage1 = imread(RES_DIR + std::string("/trackBar1.jpg"));
    g_srcImage2 = imread(RES_DIR + std::string("/trackBar2.jpg"));

    //设置滑动条初值为70
    g_nAlphaValueSlider = 70;

    //创建窗体
    namedWindow(WINDOW_NAME, 1);

    //在创建的窗体中创建一个滑动条控件
    char TrackbarName[50];
    sprintf(TrackbarName, "透明值 %d", g_nMaxAlphaValue);

    createTrackbar(TrackbarName, WINDOW_NAME, &g_nAlphaValueSlider, g_nMaxAlphaValue, on_Trackbar);

    //结果在回调函数中显示
    on_Trackbar(g_nAlphaValueSlider, 0);

    // 获取当前轨迹条的位置
    int pos = getTrackbarPos(TrackbarName,WINDOW_NAME);
    std::cout << pos << std::endl;

    //按任意键退出
    waitKey(0);
}

Rect g_rectangle;
bool g_bDrawingBox = false;//是否进行绘制
RNG g_rng(12345);

void DrawRectangle( cv::Mat& img, cv::Rect box )
{
    //随机颜色
    cv::rectangle(img,box.tl(), box.br(), cv::Scalar(g_rng.uniform(0, 255), g_rng.uniform(0,255), g_rng.uniform(0,255)));
}

void on_MouseHandle(int event, int x, int y, int flags, void* param)
{
    Mat& image = *(cv::Mat*)param;
    switch(event) {
        //鼠标移动消息
        case EVENT_MOUSEMOVE: {
            if( g_bDrawingBox ) { //如果是否进行绘制的标识符为真，则记录下长和宽到RECT型变量中
                g_rectangle.width = x - g_rectangle.x;
                g_rectangle.height = y - g_rectangle.y;
            }
            break;
        }
        case EVENT_LBUTTONDOWN: { //左键按下消息
            g_bDrawingBox = true;
            g_rectangle =Rect(x, y, 0, 0);//记录起始点
            break;
        }
        case EVENT_LBUTTONUP: { //左键抬起消息
            g_bDrawingBox = false;//置标识符为false
            //对宽和高小于0的处理
            if(g_rectangle.width < 0) {
                g_rectangle.x += g_rectangle.width;
                g_rectangle.width *= -1;
            }
            if(g_rectangle.height < 0) {
                g_rectangle.y += g_rectangle.height;
                g_rectangle.height *= -1;
            }
            //调用函数进行绘制
            DrawRectangle(image, g_rectangle);
            break;
        }
    }
}

// 使用鼠标事件绘制矩形
TEST(helloCV, testMouse)
{
    //【1】准备参数
    g_rectangle = Rect(-1,-1,0,0);
    Mat srcImage(600, 800,CV_8UC3), tempImage;

    // deep copy
    srcImage.copyTo(tempImage);
    g_rectangle = Rect(-1,-1,0,0);
    srcImage = Scalar::all(0);

    //【2】设置鼠标操作回调函数
    namedWindow(WINDOW_NAME);
    // srcImage 为 on_MouseHandle 函数的参数
    setMouseCallback(WINDOW_NAME, on_MouseHandle, (void*)&srcImage);

    //【3】程序主循环，当进行绘制的标识符为真时，进行绘制
    while(1) {
        srcImage.copyTo(tempImage);//拷贝源图到临时变量
        if (g_bDrawingBox) {
            DrawRectangle(tempImage, g_rectangle);//当进行绘制的标识符为真，则进行绘制
        }
        imshow( WINDOW_NAME, tempImage );
        if (waitKey(10) == 27) {
            break;//按下ESC键，程序退出
        }
    }
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}