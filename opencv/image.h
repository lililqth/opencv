#ifndef IMAGE_H
#define IMAGE_H
#include <opencv2/opencv.hpp>  
using namespace cv;
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
	void houghTransform(IplImage*, CvSeq*, int);
	void icvHoughLinesStandard( const CvMat* img, float rho, float theta, int threshold, CvSeq *lines, int linesMax);
	void HoughDraw(IplImage *dst, Vector<Lines> *lineCollector);
	void GrayStretch(IplImage *src);
	void Binaryzation(IplImage*, int, int);
	void Graying(IplImage *, IplImage *);
};
#endif