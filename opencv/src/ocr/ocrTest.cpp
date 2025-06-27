#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <exception>
#include "ocr.h"

using namespace cv;
using namespace std;

class ocrTest : public ::testing::Test {
public:
    ocrTest() {
    }
    ~ocrTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(ocrTest, resize)
{
    OCR ocr;
    std::string imgPath = RES_DIR + std::string("/image.png");

    cv::Mat src = cv::imread(imgPath, cv::IMREAD_COLOR);
    cv::Mat dst;
    ocr.resize(src, dst, 500, 0);

    std::cout << "dst.size() = " << dst.size() << std::endl;

    std::string afterResizeImg = RES_DIR + std::string("/afterResize.png");
    cv::imwrite(afterResizeImg, dst);

    std::cout << "RES_DIR: " << imgPath << std::endl;
}

TEST_F(ocrTest, ocrRun)
{
    OCR ocr;
    std::string imgPath = RES_DIR + std::string("/image.png");
    std::string result;
    ocr.Run(imgPath, result);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}