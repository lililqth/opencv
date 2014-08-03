#include "Image.h"
using std::vector;

typedef struct node
{
	double theta;
	double r;
	int count;
}LinePolar;

Lines::Lines(CvPoint a, CvPoint b)
	:start(a),end(b),theta(0.0), r(0.0)
{
}
Lines::Lines()
	:start(cvPoint(0, 0)),
	end(cvPoint(0, 0)),
	theta(0.0), r(0.0)
{
}
Lines::Lines(double x, double y)
	:theta(x), r(y), start(cvPoint(0, 0)), end(cvPoint(0, 0))
{
}


bool cmp(LinePolar a, LinePolar b)
{
	return a.count > b.count; 
}
void Image::houghTransform(IplImage *src, vector<Lines> *lineCollector, int threshold)
{
	double cos[180] = {1, 0.999848, 0.999391, 0.99863, 0.997564, 0.996195, 0.994522, 0.992546, 0.990268, 0.987688, 0.984808, 0.981627, 0.978148, 0.97437, 
		0.970296, 0.965926, 0.961262, 0.956305, 0.951057, 0.945519, 0.939693, 0.93358, 0.927184, 0.920505, 0.913545, 0.906308, 0.898794, 0.891007, 0.882948,
		0.87462, 0.866025, 0.857167, 0.848048, 0.838671, 0.829038, 0.819152, 0.809017, 0.798636, 0.788011, 0.777146, 0.766044, 0.75471, 0.743145, 0.731354,
		0.71934, 0.707107, 0.694658, 0.681998, 0.669131, 0.656059, 0.642788, 0.62932, 0.615661, 0.601815, 0.587785, 0.573576, 0.559193, 0.544639, 0.529919, 
		0.515038, 0.5, 0.48481, 0.469472, 0.45399, 0.438371, 0.422618, 0.406737, 0.390731, 0.374607, 0.358368, 0.34202, 0.325568, 0.309017, 0.292372, 0.275637,
		0.258819, 0.241922, 0.224951, 0.207912, 0.190809, 0.173648, 0.156434, 0.139173, 0.121869, 0.104528, 0.0871557, 0.0697565, 0.052336, 0.0348995, 0.0174524, 
		6.12323e-017, -0.0174524, -0.0348995, -0.052336, -0.0697565, -0.0871557, -0.104528, -0.121869, -0.139173, -0.156434, -0.173648, -0.190809, -0.207912, 
		-0.224951, -0.241922, -0.258819, -0.275637, -0.292372, -0.309017, -0.325568, -0.34202, -0.358368, -0.374607, -0.390731, -0.406737, -0.422618, -0.438371,
		-0.45399, -0.469472, -0.48481, -0.5, -0.515038, -0.529919, -0.544639, -0.559193, -0.573576, -0.587785, -0.601815, -0.615661, -0.62932, -0.642788, -0.656059, 
		-0.669131, -0.681998, -0.694658, -0.707107, -0.71934, -0.731354, -0.743145, -0.75471, -0.766044, -0.777146, -0.788011, -0.798636, -0.809017, -0.819152, -0.829038, 
		-0.838671, -0.848048, -0.857167, -0.866025, -0.87462, -0.882948, -0.891007, -0.898794, -0.906308, -0.913545, -0.920505, -0.927184, -0.93358, -0.939693, -0.945519,
		-0.951057, -0.956305, -0.961262, -0.965926, -0.970296,
		-0.97437, -0.978148, -0.981627, -0.984808, -0.987688, -0.990268, -0.992546, -0.994522, -0.996195, -0.997564, -0.99863, -0.999391, -0.999848};
	double sin[180] = {
		0, 0.0174524, 0.0348995, 0.052336, 0.0697565, 0.0871557, 0.104528, 0.121869, 0.139173, 0.156434, 0.173648, 0.190809, 0.207912,
		0.224951, 0.241922, 0.258819, 0.275637, 0.292372, 0.309017, 0.325568, 0.34202, 0.358368, 0.374607, 0.390731, 0.406737, 0.422618, 
		0.438371, 0.45399, 0.469472, 0.48481, 0.5, 0.515038, 0.529919, 0.544639, 0.559193, 0.573576, 0.587785, 0.601815, 0.615661, 0.62932,
		0.642788, 0.656059, 0.669131, 0.681998, 0.694658, 0.707107, 0.71934, 0.731354, 0.743145, 0.75471, 0.766044, 0.777146, 0.788011,
		0.798636, 0.809017, 0.819152, 0.829038, 0.838671, 0.848048, 0.857167, 0.866025, 0.87462, 0.882948, 0.891007, 0.898794, 0.906308, 
		0.913545, 0.920505, 0.927184, 0.93358, 0.939693, 0.945519, 0.951057, 0.956305, 0.961262, 0.965926, 0.970296, 0.97437, 0.978148, 0.981627,
		0.984808, 0.987688, 0.990268, 0.992546, 0.994522, 0.996195, 0.997564, 0.99863, 0.999391, 0.999848, 1, 0.999848, 0.999391, 0.99863,
		0.997564, 0.996195, 0.994522, 0.992546, 0.990268, 0.987688, 0.984808, 0.981627, 0.978148, 0.97437, 0.970296, 0.965926, 0.961262, 0.956305,
		0.951057, 0.945519, 0.939693, 0.93358, 0.927184, 0.920505, 0.913545, 0.906308, 0.898794, 0.891007, 0.882948, 0.87462, 0.866025, 
		0.857167, 0.848048, 0.838671, 0.829038, 0.819152, 0.809017, 0.798636, 0.788011, 0.777146, 0.766044, 0.75471, 0.743145, 0.731354, 
		0.71934, 0.707107, 0.694658, 0.681998, 0.669131, 0.656059, 0.642788, 0.62932, 0.615661, 0.601815, 0.587785, 0.573576, 0.559193, 0.544639,
		0.529919, 0.515038, 0.5, 0.48481, 0.469472, 0.45399, 0.438371, 0.422618, 0.406737, 0.390731, 0.374607, 0.358368, 0.34202, 0.325568,
		0.309017, 0.292372, 0.275637, 0.258819, 0.241922, 0.224951, 
		0.207912, 0.190809, 0.173648, 0.156434, 0.139173, 0.121869, 0.104528, 0.0871557, 0.0697565, 0.052336, 0.0348995, 0.0174524, 
	};
	const int limit = 100;
	const int maxLength = 1000;
	int width = src->width;
	int height = src->height;
	int (*statisticRecord)[maxLength * 2] = new int[190][maxLength*2];   
	memset(statisticRecord, 0, 190 * (maxLength * 2) * sizeof(int));
	// 统计
	for(int i=0; i<width; i++)
	{
		for(int j=0; j<height; j++)
		{
			uchar gray=((uchar*)(src->imageData + src->widthStep*j))[i];  
			if(gray == 255)
			{
				for(int k=0; k<=180; k++)
				{
					double theta = (double)((double)k/180.0)*CV_PI;
					double costheta = cos[k];
					double sintheta = sin[k];
					int length = (int)((double)i*(costheta)+(double)j*sintheta) + maxLength;
					if(length < maxLength * 2 - 1)
					{
						statisticRecord[k][length]++;
					}
				}
			}
		}
	}

	//保存每一条直线
	vector<LinePolar>linesPolarCollector;
	int linesTotal = 0;
	for(int i = 0;i <= 180; i++)
	{
		for(int j = 0; j < maxLength * 2; j++)
		{
			if(statisticRecord[i][j] > threshold && j-1>0 && i-1>0 && j+1 < maxLength*2 && i+1 < 180
			&& statisticRecord[i][j] > statisticRecord[i][j-1] && statisticRecord[i][j] > statisticRecord[i][j+1]
			&& statisticRecord[i][j] > statisticRecord[i-1][j] && statisticRecord[i][j] > statisticRecord[i+1][j]
			&& statisticRecord[i][j] > statisticRecord[i-1][j-1] && statisticRecord[i][j] > statisticRecord[i+1][j+1])
			{
				LinePolar lp;
				lp.theta = (double)i /180.0 * CV_PI;
				lp.r = j - maxLength;
				lp.count = statisticRecord[i][j];
				linesPolarCollector.push_back(lp);
				linesTotal++;
			}
		}
	}

	sort(linesPolarCollector.begin(), linesPolarCollector.end(), cmp);
	int lineMax = MIN(linesTotal, limit);
	lineCollector->clear();
	for(int i=0; i<lineMax; i++)
	{
		Lines l = Lines(linesPolarCollector[i].theta, linesPolarCollector[i].r);
		lineCollector->push_back(l);
	}
	delete []statisticRecord;  	
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
			if(v > x1 && v < x2)
			{
				((uchar*)(src->imageData + src->widthStep*j))[i] = k2 * (v - x1) + y1;             
			}
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