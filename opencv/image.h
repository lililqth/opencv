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
	void houghTransform(IplImage *, vector<Lines>*, int);	//����任
	void GrayStretch(IplImage *);							//�Ҷ�����
	void Binaryzation(IplImage *, int, int);				//��ֵ��
	void Graying(IplImage *, IplImage *);					//�ҶȻ�
	void medianFilter(IplImage *);							//��ֵ�˲�
	void sobel(IplImage *, IplImage *);						//sobel��Ե���
	void blur(IplImage *, IplImage *);						//BoxFilter
	void adaptiveThreshold(IplImage *, IplImage *);			//����Ӧ��ֵ��
};
#endif