#include "Image.h"
using std::vector;
Lines::Lines(CvPoint a, CvPoint b)
	:start(a),end(b)
{
}
Lines::Lines()
	:start(cvPoint(0, 0)),
	end(cvPoint(0, 0))
{
}
Vector<Lines> *Image::houghTransform(IplImage *src, int threshold)
{
	const int maxLength = 1000;
	int width = src->width;
	int height = src->height;
	int statisticRecord[190][maxLength];//统计每一对（ R，theta ）的出现次数
	memset(statisticRecord,0,sizeof(statisticRecord));
	for(int i=0; i<width; i++)
	{
		for(int j=0; j<height; j++)
		{
			int gray = cvGet2D(src, j, i).val[0];
			if(gray < 60)
			{
				continue;
			}
			for(int k=0; k<=180; k++)
			{
				double theta = (double)((double)k/180.0)*3.1415926;
				double costheta = cos(theta);
				double sintheta = sin(theta);
				double length;
				//如果角度大于90
				if(k > 90)
				{
					length = (int)((double)i*(-costheta)+(double)j*sintheta);                   
				}
				else
				{
					length = (int)((double)i*costheta+(double)j*sintheta);                   
				}
				if(cvRound(length)<maxLength-5)
				{
					statisticRecord[k][cvRound(length)]++;
				}
			}
		}
	}
	lineCollector.clear();
	for(int i=0;i<=180; i++)
	{
		for(int j=0; j<maxLength; j++)
		{
			if(statisticRecord[i][j] > threshold
			&& statisticRecord[i][j] > statisticRecord[i][j-1] && statisticRecord[i][j] > statisticRecord[i][j+1]
			&& statisticRecord[i][j] > statisticRecord[i-1][j] && statisticRecord[i][j] > statisticRecord[i+1][j]
			&& statisticRecord[i][j] > statisticRecord[i-1][j-1] && statisticRecord[i][j] > statisticRecord[i+1][j+1])
			{
				double a = cos(i), b = sin(i);  
				double x0 = a*j, y0 = b*j;  
				CvPoint pt1, pt2;
				pt1.x = cvRound(x0 + 1000*(-b));  
				pt1.y = cvRound(y0 + 1000*(a)) ;  
				pt2.x = cvRound(x0 - 1000*(-b));  
				pt2.y = cvRound(y0 - 1000*(a)) ;  
				Lines l = Lines(pt1, pt2);
				lineCollector.push_back(l);
			}
		}
	}
	return &lineCollector;
}

void Image::HoughDraw(IplImage *dst, Vector<Lines> *lineCollector)
{
	for(Vector<Lines>::iterator iter = lineCollector->begin(); iter != lineCollector->end(); iter++)
	{
		cvLine( dst, iter->start, iter->end, CV_RGB(255,255,255), 3, CV_AA, 0 );  
	}
}

/************************************************* 
Function:      灰度拉伸 
src:		   8位灰度图像     
*************************************************/ 
void Image::GrayStretch(IplImage *src)
{
	int max = -1, min = 0x3f3f3f3f;
	int x1 = 20, y1= 0, x2 = 150, y2 = 255;
	double k1 = y1/x1;
	double k2 = (y2-y1)/(x2-x1);
	double k3 = (255-y2)/(255-x2);
	for(int i=0; i< src->width; i++)
	{
		for(int j=0; j<src->height; j++)
		{
			uchar v=((uchar*)(src->imageData + src->widthStep*j))[i];  
		/*	if(v < x1)
			{
				((uchar*)(src->imageData + src->widthStep*j))[i] = k1 * v;             
			}
			else */if(v > x1 && v < x2)
			{
				((uchar*)(src->imageData + src->widthStep*j))[i] = k2 * (v - x1) + y1;             
			}
			/*else
			{
				((uchar*)(src->imageData + src->widthStep*j))[i] = k3 * (v - x2) + y2;             
			}*/
		}
	}
}

void Image::Graying(IplImage *src, IplImage *dst)
{
	for (int i=0; i<src->width; i++)  
	{  
		for (int j=0; j<src-> height; j++)
		{  
			uchar r=((uchar*)(src->imageData + src->widthStep*j))[i*src->nChannels + 0];
			uchar g=((uchar*)(src->imageData + src->widthStep*j))[i*src->nChannels + 1];
			uchar b=((uchar*)(src->imageData + src->widthStep*j))[i*src->nChannels + 2];
			((uchar*)(dst->imageData + dst->widthStep*j))[i] = (r*38 + g*75 + b*15) >> 7;
		}  
	}  
}
void Image::Binaryzation(IplImage *src, int low, int high)
{
	for(int i=0; i<src->width; i++)  
	{  
		for(int j=0; j<src-> height; j++){  
			uchar v=((uchar*)(src->imageData + src->widthStep*j))[i];  
			if(v >= low && v <=high)
			{
				((uchar*)(src->imageData + src->widthStep*j))[i] = 255;             
			}
			else
			{
				((uchar*)(src->imageData + src->widthStep*j))[i] = 0;
			}
		}  
	}  
}