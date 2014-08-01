#ifndef IMAGE_H
#define IMAGE_H
#include <opencv2/opencv.hpp>  
using namespace cv;
class Lines
{
public: 
	CvPoint start;
	CvPoint end;
	Lines(CvPoint a, CvPoint b);
	Lines();
};
class Image
{
private:
	Vector<Lines> lineCollector; 
public:
	Vector<Lines>* houghTransform(IplImage *src, int threshold);
	void icvHoughLinesStandard( const CvMat* img, float rho, float theta, int threshold, CvSeq *lines, int linesMax );
	void HoughDraw(IplImage *dst, Vector<Lines> *lineCollector);
	void GrayStretch(IplImage *src);
	void Binaryzation(IplImage*, int, int);
	void Graying(IplImage *, IplImage *);
};
#endif