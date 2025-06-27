#ifndef _OPCV_OCR_H_
#define _OPCV_OCR_H_ 

#include <opencv2/opencv.hpp>
#include <string>

class OCR {
public:
    OCR() {}
    ~OCR() {}

    int Run(const std::string& strImage, std::string& result);

// private:
    int resize(cv::Mat& src, cv::Mat& dst, int width = 0,  int height = 0, int interp = cv::INTER_LINEAR);

private:
    float ratio;

};

#endif