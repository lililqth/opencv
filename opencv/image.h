#ifndef IMAGE_H
#define IMAGE_H
#include <opencv2/opencv.hpp>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace cv;
using namespace std;

class Lines
{
public:
	CvPoint start;
	CvPoint end;
	double theta;
	double r;
	Lines(CvPoint a, CvPoint b);
	Lines(double theta, double r);
	Lines();
};

class Image
{
public:
	void houghTransform(IplImage *, vector<Lines>*, int);	//霍夫变换
	void GrayStretch(IplImage *);							//灰度拉伸
	void Binaryzation(IplImage *, int, int);				//二值化
	void Graying(IplImage *, IplImage *);					//灰度化
	void medianFilter(IplImage *);							//中值滤波
	void sobel(IplImage *, IplImage *);						//sobel边缘检测
	void blur(IplImage *, IplImage *);						//BoxFilter
	void adaptiveThreshold(IplImage *, IplImage *);			//自适应二值化
};
#endif