#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <exception>

using namespace cv;
using namespace std;

class coreTest : public ::testing::Test {
public:
    coreTest() {
    }
    ~coreTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(coreTest, useMat)
{
    /**************** 显示创建 Mat 的七种方法 *******************/
    // 1. Mat()
    Mat M(2, 2, CV_8UC3, Scalar(0, 0, 255));
    cout << "M = " << endl << " " << M << endl << endl;

    // 2. 构造函数初始化
    int sz[3] = {2, 2, 2};
    Mat L(3, sz, CV_8UC1, Scalar::all(0));

    // 3. IplImage 方式
    IplImage *img = cvLoadImage(std::string(RES_DIR + std::string("/1.jpg")).c_str());
    Mat mtimg = cvarrToMat(img); // IplImage* -> img

    // 4. Create() 只是改变尺寸重新开辟空间
    M.create(4, 4, CV_8UC2);
    cout << "M = " << endl << " " << M << endl << endl;

    // Matlab 初始化
    Mat E = Mat::eye(4, 4, CV_64F);
    cout << "E = " << endl << " " << E << endl << endl;

    Mat O = Mat::ones(2, 2, CV_32F);
    cout << "O = " << endl << " " << O << endl << endl;

    Mat Z = Mat::zeros(3, 3, CV_8UC1);
    cout << "Z = " << endl << " " << Z << endl << endl;

    // 对小矩阵进行逗号分隔符初始化
    Mat C = (Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    cout << "C = " << endl << " " << C << endl << endl;

    // 对矩阵进行截取复制
    Mat RowClone = C.row(1).clone();
    cout << "RowClone = " << endl << " " << RowClone << endl << endl;

    Mat I = Mat::eye(4, 4, CV_64F);
    I.at<double>(1, 1) = CV_PI;
    cout << "\nI = " << I << ";\n" << endl;

    /**************** 格式化输出方法 *******************/
    Mat r = Mat(10, 3, CV_8UC3);
    randu(r, Scalar::all(0), Scalar::all(255));


    cout << "r (OpenCV默认风格) = " << r << ";" << endl << endl;
    cout << "r (Python风格) = " << format(r, Formatter::FMT_PYTHON) << ";" << endl << endl;
    cout << "r (Numpy风格) = " << format(r, Formatter::FMT_NUMPY) << ";" << endl << endl;
    cout << "r (逗号分隔风格) = " << format(r, Formatter::FMT_CSV) << ";" << endl << endl;
    cout << "r (C语言风格) = " << format(r, Formatter::FMT_C) << ";" << endl << endl;

    Point ipt(1,3);
    cout << "【2维点】p = " << ipt << ";\n" << endl;
    Point2f p(6, 2);
    cout << "【2维点】p = " << p << ";\n" << endl;

    Point3f p3f(8, 2, 0);
    cout << "【3维点】p3f = " << p3f << ";\n" << endl;

    vector<float> v;
    v.push_back(3);
    v.push_back(5);
    v.push_back(7);

    cout << "【基于Mat的vector】shortvec = " << Mat(v) << ";\n" << endl;

    vector<Point2f> points(20);
    for (size_t i = 0; i < points.size(); ++i)
        points[i] = Point2f((float) (i * 5), (float) (i % 7));

    cout << "【二维点向量】points = " << points << ";\n";

    Size is(4,5);
    cout << "【二维size向量】size = " << is << ";\n";

    Scalar s(255,255,0);
    cout << "【三维Scalar向量】size = " << s << ";\n";

    Rect rect1(0,0,10,10);
    Rect rect2(5,5,10,10);
    Rect rec3 = rect1 & rect2;
    Rect rec4 = rect1 + ipt;
    Rect rec5 = rect1 + is;

    getchar();//按任意键退出

}

#define WINDOW_NAME1 "【绘制图1】"        //为窗口标题定义的宏
#define WINDOW_NAME2 "【绘制图2】"        //为窗口标题定义的宏
#define WINDOW_WIDTH 600//定义窗口大小的宏

void DrawLine(Mat img, Point start, Point end)
{
    int thickness = 2;
    int lineType = 8;
    line(img, start, end, Scalar( 0, 0, 0 ), thickness, lineType);
}

void DrawPolygon( Mat img )
{
    int lineType = 8;

    //创建一些点
    Point rookPoints[1][20];
    rookPoints[0][0] = Point(WINDOW_WIDTH / 4, 7 * WINDOW_WIDTH / 8);
    rookPoints[0][1] = Point(3 * WINDOW_WIDTH / 4, 7 * WINDOW_WIDTH / 8 );
    rookPoints[0][2] = Point(3 * WINDOW_WIDTH/4,  13*WINDOW_WIDTH/16 );
    rookPoints[0][3]  = Point( 11*WINDOW_WIDTH/16, 13*WINDOW_WIDTH/16 );
    rookPoints[0][4]  = Point( 19*WINDOW_WIDTH/32,  3*WINDOW_WIDTH/8 );
    rookPoints[0][5]  = Point(  3*WINDOW_WIDTH/4,   3*WINDOW_WIDTH/8 );
    rookPoints[0][6]  = Point(  3*WINDOW_WIDTH/4,     WINDOW_WIDTH/8 );
    rookPoints[0][7]  = Point( 26*WINDOW_WIDTH/40,    WINDOW_WIDTH/8 );
    rookPoints[0][8]  = Point( 26*WINDOW_WIDTH/40,    WINDOW_WIDTH/4 );
    rookPoints[0][9]  = Point( 22*WINDOW_WIDTH/40,    WINDOW_WIDTH/4 );
    rookPoints[0][10] = Point( 22*WINDOW_WIDTH/40,    WINDOW_WIDTH/8 );
    rookPoints[0][11] = Point( 18*WINDOW_WIDTH/40,    WINDOW_WIDTH/8 );
    rookPoints[0][12] = Point( 18*WINDOW_WIDTH/40,    WINDOW_WIDTH/4 );
    rookPoints[0][13] = Point( 14*WINDOW_WIDTH/40,    WINDOW_WIDTH/4 );
    rookPoints[0][14] = Point( 14*WINDOW_WIDTH/40,    WINDOW_WIDTH/8 );
    rookPoints[0][15] = Point(    WINDOW_WIDTH/4,     WINDOW_WIDTH/8 );
    rookPoints[0][16] = Point(    WINDOW_WIDTH/4,   3*WINDOW_WIDTH/8 );
    rookPoints[0][17] = Point( 13*WINDOW_WIDTH/32,  3*WINDOW_WIDTH/8 );
    rookPoints[0][18] = Point(  5*WINDOW_WIDTH/16, 13*WINDOW_WIDTH/16 );
    rookPoints[0][19] = Point(    WINDOW_WIDTH/4,  13*WINDOW_WIDTH/16 );

    const Point* ppt[1] = { rookPoints[0] };
    int npt[] = { 20 };

    fillPoly(img, ppt, npt, 1, Scalar( 255, 255, 255 ), lineType);
}

void DrawEllipse( Mat img, double angle )
{
    int thickness = 2;
    int lineType = 8;

    ellipse(img, Point(WINDOW_WIDTH / 2, WINDOW_WIDTH / 2), Size(WINDOW_WIDTH / 4, WINDOW_WIDTH /16),
        angle, 0, 360, Scalar( 255, 129, 0 ), thickness, lineType);
}

void DrawFilledCircle( Mat img, Point center )
{
    int thickness = -1;
    int lineType = 8;

    circle(img, center, WINDOW_WIDTH / 32, Scalar(0, 0, 255), thickness, lineType);
}

GTEST_TEST_F(coreTest, drawImage)
{
  // 创建空白的Mat图像
    Mat atomImage = Mat::zeros(WINDOW_WIDTH, WINDOW_WIDTH, CV_8UC3);
    Mat rookImage = Mat::zeros(WINDOW_WIDTH, WINDOW_WIDTH, CV_8UC3);

    // ---------------------<1>绘制化学中的原子示例图------------------------
    //【1.1】先绘制出椭圆
    DrawEllipse(atomImage, 90);
    DrawEllipse(atomImage, 0);
    DrawEllipse(atomImage, 45);
    DrawEllipse(atomImage, -45);

    //【1.2】再绘制圆心
    DrawFilledCircle(atomImage, Point(WINDOW_WIDTH / 2, WINDOW_WIDTH / 2));

    // ----------------------------<2>绘制组合图-----------------------------
    //【2.1】先绘制出椭圆
    DrawPolygon(rookImage);

    // 【2.2】绘制矩形
    rectangle(rookImage, Point(0, 7 * WINDOW_WIDTH / 8), Point(WINDOW_WIDTH, WINDOW_WIDTH),
        Scalar(0, 255, 255), -1, 8);

    // 【2.3】绘制一些线段
    DrawLine(rookImage, Point(0, 15 * WINDOW_WIDTH / 16), Point(WINDOW_WIDTH, 15*WINDOW_WIDTH / 16));
    DrawLine(rookImage, Point(WINDOW_WIDTH / 4, 7 * WINDOW_WIDTH / 8), Point(WINDOW_WIDTH / 4, WINDOW_WIDTH));
    DrawLine(rookImage, Point(WINDOW_WIDTH / 2, 7 * WINDOW_WIDTH / 8), Point(WINDOW_WIDTH / 2, WINDOW_WIDTH));
    DrawLine(rookImage, Point(3 * WINDOW_WIDTH / 4, 7 * WINDOW_WIDTH / 8), Point(3 * WINDOW_WIDTH / 4, WINDOW_WIDTH));

    // ---------------------------<3>显示绘制出的图像------------------------
    imshow(WINDOW_NAME1, atomImage);
    moveWindow(WINDOW_NAME1, 0, 200);
    imshow(WINDOW_NAME2, rookImage);
    moveWindow(WINDOW_NAME2, WINDOW_WIDTH, 200);

    waitKey( 0 );
}

// C 操作符
void colorReduceByPointer(Mat& inputImage, Mat& outputImage, int div)
{
    //参数准备
    outputImage = inputImage.clone();  //拷贝实参到临时变量
    int rowNumber = outputImage.rows;  //行数
    int colNumber = outputImage.cols*outputImage.channels();  //列数 x 通道数=每一行元素的个数

    //双重循环，遍历所有的像素值
    for(int i = 0;i < rowNumber;i++) { //行循环
        uchar* data = outputImage.ptr<uchar>(i); { //获取第i行的首地址
        for(int j = 0;j < colNumber;j++)   //列循环
            // ---------【开始处理每个像素】-------------
            data[j] = data[j] / div * div + div / 2;
            // ----------【处理结束】---------------------
        }  //行处理结束
    }
}

//迭代器
void colorReduceByIterator(Mat& inputImage, Mat& outputImage, int div)
{
    //参数准备
    outputImage = inputImage.clone();  //拷贝实参到临时变量
    //获取迭代器
    Mat_<Vec3b>::iterator it = outputImage.begin<Vec3b>();  //初始位置的迭代器
    Mat_<Vec3b>::iterator itend = outputImage.end<Vec3b>();  //终止位置的迭代器

    //存取彩色图像像素
    for(; it != itend; ++it)
    {
        // ------------------------【开始处理每个像素】--------------------
        (*it)[0] = (*it)[0]/div*div + div/2;
        (*it)[1] = (*it)[1]/div*div + div/2;
        (*it)[2] = (*it)[2]/div*div + div/2;
        // ------------------------【处理结束】----------------------------
    }
}

// 使用at 函数动态获取
void colorReduceByDynamicAddr(Mat& inputImage, Mat& outputImage, int div)
{
    //参数准备
    outputImage = inputImage.clone();  //拷贝实参到临时变量
    int rowNumber = outputImage.rows;  //行数
    int colNumber = outputImage.cols;  //列数

    //存取彩色图像像素
    for(int i = 0;i < rowNumber;i++)
    {
        for(int j = 0;j < colNumber;j++)
        {
            // ------------------------【开始处理每个像素】--------------------
            outputImage.at<Vec3b>(i,j)[0] =  outputImage.at<Vec3b>(i,j)[0]/div*div + div/2;  //蓝色通道
            outputImage.at<Vec3b>(i,j)[1] =  outputImage.at<Vec3b>(i,j)[1]/div*div + div/2;  //绿色通道
            outputImage.at<Vec3b>(i,j)[2] =  outputImage.at<Vec3b>(i,j)[2]/div*div + div/2;  //红是通道
            // -------------------------【处理结束】----------------------------
        }  // 行处理结束
    }
}

TEST_F(coreTest, colorReduce)
{
    //【1】创建原始图并显示
    Mat srcImage = imread(RES_DIR + std::string("/colorReduce.jpg"));
    imshow("原始图像",srcImage);

    //【2】按原始图的参数规格来创建创建效果图
    Mat dstImage;
    //效果图的大小、类型与原图片相同
    dstImage.create(srcImage.rows, srcImage.cols, srcImage.type());

    //【3】记录起始时间
    double time0 = static_cast<double>(getTickCount());

    //【4】调用颜色空间缩减函数
    // colorReduceByPointer(srcImage, dstImage, 32);
    colorReduceByDynamicAddr(srcImage, dstImage, 32);

    //【5】计算运行时间并输出
    time0 = ((double)getTickCount() - time0) / getTickFrequency();
    cout << "\t此方法运行时间为： "<< time0 << "秒" << endl;  //输出运行时间

    //【6】显示效果图
    imshow("效果图", dstImage);
    cv::imwrite(RES_DIR + std::string("/colorReduce_after.jpg"), dstImage);
    waitKey(0);
}

TEST_F(coreTest, linearBlend)
{
    //【0】定义一些局部变量
    double alphaValue = 0.5;
    double betaValue;
    Mat dstImage;
    Mat srcImage2= imread(RES_DIR + std::string("/rain.jpg"), 1);
    Mat srcImage3= imread(RES_DIR + std::string("/mogu.jpg"), 1);
    // 【2】进行图像混合加权操作
    betaValue = ( 1.0 - alphaValue );
    // dst = src1 * alphaValue + src2 * betaValue + gamma
    addWeighted(srcImage2, alphaValue, srcImage3, betaValue, 0.0, dstImage);

    imshow("【2】进行图像混合加权操作", dstImage);
    waitKey(0);
    imwrite(RES_DIR + std::string("/mogu_rain.jpg"), dstImage);
}

/* 分隔图像为3个通道的图像 */
TEST_F(coreTest, splitChannel)
{
    //【0】定义相关变量
    Mat srcImage;
    Mat logoImage;
    vector<Mat> channels;
    Mat imageBlueChannel;

    //=================【蓝色通道部分】=================
    //	描述：多通道混合-蓝色分量部分
    //============================================

    // 【1】读入图片
    logoImage= imread(RES_DIR + std::string("/dota_logo.jpg"),0);
    srcImage= imread(RES_DIR + std::string("/dota_jugg.jpg"));

    //【2】把一个3通道图像转换成3个单通道图像
    // mv[c] = src[:,:,c]
    // channels: vector<Mat>
    split(srcImage,channels);//分离色彩通道

    //【3】将原图的蓝色通道引用返回给imageBlueChannel，注意是引用，相当于两者等价，修改其中一个另一个跟着变
    imageBlueChannel= channels.at(0);
    //【4】将原图的蓝色通道的（500,250）坐标处右下方的一块区域和logo图进行加权操作，将得到的混合结果存到imageBlueChannel中
    addWeighted(imageBlueChannel(Rect(500,250,logoImage.cols,logoImage.rows)),1.0,
        logoImage,0.5,0,imageBlueChannel(Rect(500,250,logoImage.cols,logoImage.rows)));

    //【5】将三个单通道重新合并成一个三通道
    merge(channels,srcImage);

    //【6】显示效果图
    namedWindow(" <1>游戏原画+logo蓝色通道");
    imshow(" <1>游戏原画+logo蓝色通道",srcImage);
    waitKey(0);

    //=================【绿色通道部分】=================
    //	描述：多通道混合-绿色分量部分
    //============================================

    //【0】定义相关变量
    Mat imageGreenChannel;

    //【1】重新读入图片
    logoImage= imread(RES_DIR + std::string("/dota_logo.jpg"), 0);
    srcImage= imread(RES_DIR + std::string("/dota_jugg.jpg"));

    //【2】将一个三通道图像转换成三个单通道图像
    split(srcImage,channels);//分离色彩通道

    //【3】将原图的绿色通道的引用返回给imageBlueChannel，注意是引用，相当于两者等价，修改其中一个另一个跟着变
    imageGreenChannel= channels.at(1);
    //【4】将原图的绿色通道的（500,250）坐标处右下方的一块区域和logo图进行加权操作，将得到的混合结果存到imageGreenChannel中
    addWeighted(imageGreenChannel(Rect(500,250,logoImage.cols,logoImage.rows)),1.0,
                logoImage,0.5,0.,imageGreenChannel(Rect(500,250,logoImage.cols,logoImage.rows)));

    //【5】将三个独立的单通道重新合并成一个三通道
    merge(channels,srcImage);

    //【6】显示效果图
    namedWindow("<2>游戏原画+logo绿色通道");
    imshow("<2>游戏原画+logo绿色通道",srcImage);

    //=================【红色通道部分】=================
    //	描述：多通道混合-红色分量部分
    //============================================

    //【0】定义相关变量
    Mat  imageRedChannel;

    //【1】重新读入图片
    logoImage= imread(RES_DIR + std::string("/dota_logo.jpg"),0);
    srcImage= imread(RES_DIR + std::string("/dota_jugg.jpg"));


    //【2】将一个三通道图像转换成三个单通道图像
    split(srcImage,channels);//分离色彩通道
    std::cout << " channels.size() = " << channels.size() << "IS EMPTY = " << logoImage.empty() << std::endl;

    //【3】将原图的红色通道引用返回给imageBlueChannel，注意是引用，相当于两者等价，修改其中一个另一个跟着变
    imageRedChannel= channels.at(2);
    //【4】将原图的红色通道的（500,250）坐标处右下方的一块区域和logo图进行加权操作，将得到的混合结果存到imageRedChannel中
    addWeighted(imageRedChannel(Rect(500,250,logoImage.cols,logoImage.rows)),1.0,
                logoImage,0.5,0.,imageRedChannel(Rect(500,250,logoImage.cols,logoImage.rows)));

    //【5】将三个独立的单通道重新合并成一个三通道
    merge(channels,srcImage);

    //【6】显示效果图
    namedWindow("<3>游戏原画+logo红色通道 ");
    imshow("<3>游戏原画+logo红色通道 ",srcImage);
    waitKey(0);
}

/* 修改亮度与对比度 */
int g_nContrastValue; //对比度值
int g_nBrightValue;  //亮度值
Mat g_srcImage, g_dstImage;

//-----------------------------【ContrastAndBright( )函数】------------------------------------
//	描述：改变图像对比度和亮度值的回调函数
//-----------------------------------------------------------------------------------------------
static void ContrastAndBright(int, void *) {
    // 创建窗口
    namedWindow("【原始图窗口】", 1);
    // 三个for循环，执行运算 g_dstImage(i,j) = a*g_srcImage(i,j) + b
    for( int y = 0; y < g_srcImage.rows; y++ ) {
        for( int x = 0; x < g_srcImage.cols; x++ ) {
            for( int c = 0; c < 3; c++ ) {
                // saturate_cast<uchar>() 防止数据溢出
                g_dstImage.at<Vec3b>(y, x)[c] = saturate_cast<uchar>( (g_nContrastValue* 0.01)*( g_srcImage.at<Vec3b>(y,x)[c] ) + g_nBrightValue);
            }
        }
    }

    // 显示图像
    imshow("【原始图窗口】", g_srcImage);
    imshow("【效果图窗口】", g_dstImage);
}

TEST_F(coreTest, changeContrastAndBright)
{
    // 读入用户提供的图像
    g_srcImage = imread(RES_DIR + std::string("/contrastAndBright.jpg"));
    if( !g_srcImage.data ) {
        printf("读取g_srcImage图片错误~！ \n");
        return;
    }
    g_dstImage = Mat::zeros(g_srcImage.size(), g_srcImage.type());

    //设定对比度和亮度的初值
    g_nContrastValue=80;
    g_nBrightValue=80;

    //创建窗口
    namedWindow("【效果图窗口】", 1);

    //创建轨迹条
    createTrackbar("对比度：", "【效果图窗口】", &g_nContrastValue, 300,ContrastAndBright );
    createTrackbar("亮   度：", "【效果图窗口】", &g_nBrightValue, 200,ContrastAndBright );

    //调用回调函数
    ContrastAndBright(g_nContrastValue,0);
    ContrastAndBright(g_nBrightValue,0);

    //按下“q”键时，程序退出
    while(char(waitKey(1)) != 'q') {}
}

TEST_F(coreTest, dft)
{
    //【1】以灰度模式读取原始图像并显示
    Mat srcImage = imread(RES_DIR + std::string("/dft.jpg"), 0);
    if(!srcImage.data ) {
        printf("读取图片错误，请确定目录下是否有imread函数指定图片存在~！ \n");
        return;
    }
    imshow("原始图像" , srcImage);

    //【2】将输入图像延扩到最佳的尺寸，边界用0补充
    int m = getOptimalDFTSize(srcImage.rows);
    int n = getOptimalDFTSize(srcImage.cols);
    //将添加的像素初始化为0.
    Mat padded;
    // dst_size = (src.cols + left + right, src.rows + top + bottom)
    // top/bottom/left/right 各个方向需要的 pad
    copyMakeBorder(srcImage, padded, 0, m - srcImage.rows, 0, n - srcImage.cols, BORDER_CONSTANT, Scalar::all(0));

    //【3】为傅立叶变换的结果(实部和虚部)分配存储空间。
    //将planes数组组合合并成一个多通道的数组complexI
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);

    //【4】进行 inplace 离散傅里叶变换
    // dst = dft(src)
    dft(complexI, complexI);

    //【5】将复数转换为幅值，即=> log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    // 将多通道数组complexI分离成几个单通道数组，planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    split(complexI, planes);
    // magnitude = sqrt(x^2 + y^2)
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magnitudeImage = planes[0];

    //【6】进行对数尺度(logarithmic scale)缩放
    magnitudeImage += Scalar::all(1); // +1 防止 log0 的无效值
    log(magnitudeImage, magnitudeImage);//求自然对数

    //【7】剪切和重分布幅度图象限
    //若有奇数行或奇数列，进行频谱裁剪
    magnitudeImage = magnitudeImage(Rect(0, 0, magnitudeImage.cols & -2, magnitudeImage.rows & -2));
    //重新排列傅立叶图像中的象限，使得原点位于图像中心
    int cx = magnitudeImage.cols/2;
    int cy = magnitudeImage.rows/2;
    Mat q0(magnitudeImage, Rect(0, 0, cx, cy));   // ROI区域的左上
    Mat q1(magnitudeImage, Rect(cx, 0, cx, cy));  // ROI区域的右上
    Mat q2(magnitudeImage, Rect(0, cy, cx, cy));  // ROI区域的左下
    Mat q3(magnitudeImage, Rect(cx, cy, cx, cy)); // ROI区域的右下

    //交换象限（左上与右下进行交换）
    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    //交换象限（右上与左下进行交换）
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    //【8】归一化，用0到1之间的浮点值将矩阵变换为可视的图像格式
    // dst = norm(src) alpha: 归一化后的最小值，归一化后的最大值
    normalize(magnitudeImage, magnitudeImage, 0, 1, NORM_MINMAX);

    //【9】显示效果图
    imshow("频谱幅值", magnitudeImage);
    waitKey();
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}