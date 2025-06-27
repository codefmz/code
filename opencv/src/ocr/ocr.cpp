#include "ocr.h"

// const std::string filePath = RES_DIR + std::string("/out.png");

int OCR::Run(const std::string &strImage, std::string &result)
{
    std::string filePath = RES_DIR + std::string("/afterCandy2.png");
    cv::Mat src = cv::imread(strImage, cv::IMREAD_GRAYSCALE);
    if (src.empty()) {
        return -1;
    }

    resize(src, src, 0, 500);
    cv::GaussianBlur(src, src, cv::Size(5, 5), 0);
    cv::Canny(src, src, 75, 100);

    cv::imwrite(filePath, src);


    return 0;
}

int OCR::resize(cv::Mat &src, cv::Mat &dst, int width, int height, int interp)
{
    if (src.empty()) {
        return -1;
    }

    int srcWidth = src.cols;
    int srcHeight = src.rows;
    if (width > 0) {
        ratio = (float)srcWidth / width;
        height = (int)(srcHeight / ratio);
        cv::resize(src, dst, cv::Size(width, height), 0, 0, interp);
        return 0;
    }

    if (height > 0) {
        ratio = (float)srcHeight / height;
        width = (int)(srcWidth / ratio);
        cv::resize(src, dst, cv::Size(width, height), 0, 0, interp);
        return 0;
    }

    return 0;
}