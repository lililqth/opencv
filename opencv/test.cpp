#include "image.h"

const int thresholdTheta = 10;				/*��֮֡��ĽǶȲ�����*/
const int thresholdLength = 10;				/*��֮֡���R������*/
const int offsetThresholdTheta = 15;		/*���ʼ�Ƕ�ֵƫ�������*/
const int offsetThresholdLength = 15;		/*���ʼRƫ�������*/
vector<int> thetaStatus(180);
vector<int> lengthStatus(1000);
void ShowStatus(IplImage *canvas, vector<int> *status)
{
	int buttom = canvas->height-2;	
	int i=0;
	for( vector<int>::iterator iter = status->begin(); iter!= status->end(); iter++)
	{
		cvLine(canvas, cvPoint(i, buttom), cvPoint(i, buttom - *iter), CV_RGB(0, 0, 255),1, CV_AA, 0);
		i++;
	}
}

bool cmp(int a, int b)
{
	return a>b;
}

inline bool Judge(int actual, int standard, int threshold)
{
	if(abs(standard - actual) < threshold)
	{
		return true;
	}
	return false;
}

int main(int argc, char *argv[])  
{  
	CvCapture* capture=cvCreateFileCapture("../13.avi");  
//	VideoWriter writer("VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(640, 480));  
	IplImage *thetaPic = cvCreateImage(cvSize(180, 500),IPL_DEPTH_8U,1);//�������ڻ��ƽǶȷֲ�ͼ
	IplImage *lengthPic = cvCreateImage(cvSize(1000, 500),IPL_DEPTH_8U,1);//�������ڻ��ƽǶȷֲ�ͼ
	IplImage* frame;    //��Ƶͼ��
	IplImage* dst;		//���ͼ��
	Image img;
	//���ļ���ȡƵ�����ĽǶ�	
	std::vector<int> maxTheta;
	fstream fin("recordTheta.txt", ios::in || ios::out);
	char buffer[100];
	while(!(fin.eof() || !fin.is_open()))
	{
		fin>>buffer;
		if(strcmp(buffer,""))
		{
			maxTheta.push_back(atoi(buffer));
		}
	}
	fin.close();

	//���ļ���ȡƵ�����ĳ���
	vector<int> maxLength;
	fin.open("recordLength.txt", ios::in || ios::out);
	while(!(fin.eof() || !fin.is_open()))
	{
		fin>>buffer;
		if(strcmp(buffer,""))
		{
			maxLength.push_back(atoi(buffer));

		}
	}
	fin.close();

	bool firstTime = true;  
	int originThetaRight;
	int originThetaLeft;
	int originLengthRight;
	int originLengthLeft;
	int leftThetaPre = originThetaLeft = maxTheta[0];
	int rightThetaPre = originThetaRight= maxTheta[1];
	int leftLengthPre = originLengthLeft = maxLength[0];
	int rightLengthPre = originLengthRight = maxLength[1];
	bool findFlag[2] = {false, false};
	Lines leftLinePre = Lines(), rightLinePre = Lines();
	while(1)  
	{  

		if(firstTime == false)
		{
			maxTheta.clear();
			maxLength.clear();
			if(!Judge(leftThetaPre, originThetaLeft, offsetThresholdTheta))
			{
				leftThetaPre = originThetaLeft;
			}
			if(!Judge(rightThetaPre, originThetaRight, offsetThresholdTheta))
			{
				rightThetaPre = originThetaRight;
			}
			if(!Judge(leftLengthPre, originLengthLeft, offsetThresholdLength))
			{
				leftLengthPre = originLengthLeft;
			}
			if(!Judge(rightLengthPre, originLengthRight, offsetThresholdLength))
			{
				rightLengthPre = originLengthRight;
			}
			maxTheta.push_back(leftThetaPre);
			maxTheta.push_back(rightThetaPre);
			maxLength.push_back(leftLengthPre);
			maxLength.push_back(rightLengthPre);
		} 
		frame=cvQueryFrame(capture);  
		if(!frame) 
		{
			break;  
		}
		dst = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1) ;    

		//�ҶȻ�
		img.Graying(frame, dst);

		//����ͼ����°벿��
		CvSize size = cvSize(cvGetSize(dst).width, cvGetSize(dst).height/2);
		cvSetImageROI(dst, cvRect(0 ,size.height, size.width, size.height));
		IplImage* halfDst = cvCreateImage(size,dst->depth,dst->nChannels);	//����Ŀ��ͼ��
		cvCopy(dst, halfDst);												//halfDst�д�ŵ���ͼ����°벿��
		cvResetImageROI(dst);
		cvReleaseImage(&dst);
		dst = cvCreateImage(cvGetSize(halfDst),IPL_DEPTH_8U,1) ;    
		cvCopy(halfDst, dst);
		cvReleaseImage(&halfDst);

		//��ֵ�˲���Ϊsobel���ӽ���˸�˹ƽ��������Ҫ�ٽ��и�˹ƽ��
		img.medianFilter(dst);

		//��ֵ��ȥ�����õ���Ϣ
		img.Binaryzation(dst, 60, 150);

		//֮������Ӧ��ֵ����ȡ�߽�
		img.adaptiveThreshold(dst, dst);

		//sobel��Ե��ǿ
		IplImage *sobelOut8 = cvCreateImage(cvGetSize(dst),IPL_DEPTH_8U,1);
		img.sobel(dst, sobelOut8);
		cvCopy(sobelOut8, dst);
		cvReleaseImage(&dst);
		dst = cvCloneImage(sobelOut8);
		cvReleaseImage(&sobelOut8);

		//����任
		vector<Lines> *linesSeq = new vector<Lines>;
		if(maxTheta.size() == 0 || maxLength.size() == 0)
		{
			img.houghTransform(dst, linesSeq, 60);
		}
		else
		{
			img.houghTransform(dst, linesSeq, 10);
		}
		for(vector<Lines>::iterator iter = linesSeq->begin(); iter != linesSeq->end() && (findFlag[0]==false || findFlag[1] == false); iter++)  
		{  
			double rho = iter->r;
			double theta = iter->theta/CV_PI*180.0;
			//����ǵ�һ�����У��Ͷ�Ƶ�ʽ���ͳ�ơ�
			if(maxTheta.size() == 0)
			{
				//ͳ�ƽǶȷֲ�
				thetaStatus[(int)theta]++;
				ShowStatus(thetaPic, &thetaStatus);
				cvShowImage("�Ƕ�ͳ����Ϣ", thetaPic);
			}
			else if(maxLength.size() == 0)
			{
				if(rho+500>=0 && rho+500<1000)
				{
					lengthStatus[(int)(rho + 500)]++;
					ShowStatus(lengthPic, &lengthStatus);
					cvShowImage("����ͳ����Ϣ", lengthPic);
				}
			}
			else
			{
				//������ǵ�һ�����о͸��ݳ���Ƶ������2��������й��ˡ�
				for(int i=0; i<(int)maxTheta.size(); i++)
				{
					if(findFlag[i]==false && Judge((int)theta, maxTheta[i], thresholdTheta) && Judge((int)rho, maxLength[i]-500, thresholdLength))
					{		
						findFlag[i] = true;
						if(i == 0)
						{
							leftThetaPre = (int)theta;
							leftLengthPre = (int)rho + 500;
						}
						else if(i == 1)
						{
							rightThetaPre = (int)theta;
							rightLengthPre = (int)rho + 500;
						}
						CvPoint pt1, pt2;  
						double a = cos(theta/180*CV_PI), b = sin(theta/180*CV_PI);  
						double x0 = a*rho, y0 = b*rho;  
						pt1.x = cvRound(x0 + 1000*(-b));

						pt1.y = cvRound(y0 + 1000*(a)) + size.height;  
						while(pt1.y < size.height)
						{
							pt1.x -= 100*(-(int)b);
							pt1.y -= 100*((int)a);
						}
						pt2.x = cvRound(x0 - 1000*(-b));  
						pt2.y = cvRound(y0 - 1000*(a)) + size.height;  
						while(pt2.y < size.height)
						{
							pt2.y += 100*((int)a);
							pt2.x += 100*(-(int)b);
						}
						cvLine( frame, pt1, pt2, CV_RGB(255, 255, 255), 3, CV_AA, 0);  
						if(i==0)
						{
							leftLinePre = Lines(pt1, pt2);
						}
						if(i==1)
						{
							rightLinePre = Lines(pt1, pt2);
						}
						break;
					}
				}
			}
		} 
		if(findFlag[0] == false)
		{
			cvLine( frame, leftLinePre.start, leftLinePre.end, CV_RGB(255, 255, 255), 3, CV_AA, 0);  
		}
		if(findFlag[1] == false)
		{
			cvLine( frame, rightLinePre.start, rightLinePre.end, CV_RGB(255, 255, 255), 3, CV_AA, 0);  
		}
		firstTime = false;
		findFlag[0] = false;
		findFlag[1] = false;
		cvShowImage("ʶ����",frame);  

		cvReleaseImage(&dst);
		//writer << frame;
		char c=cvWaitKey(10);
		if(c==27) break;  
	}  

	//���ƽǶȳ���ͳ����Ϣ �ҳ�������ߵ�  ��д���ļ�
	vector<int> statusCopy(thetaStatus);
	ofstream fout;
	bool flag = false;
	if(maxTheta.size()==0)
	{
		flag = true;
		sort(statusCopy.begin(), statusCopy.end(),cmp);
		fout.open("recordTheta.txt", ios::out);
		for(size_t i=0; i<thetaStatus.size(); i++)
		{
			if(thetaStatus[i] == statusCopy[0] || thetaStatus[i] == statusCopy[1] 
			|| thetaStatus[i] == statusCopy[2] || thetaStatus[i] == statusCopy[3])
			{
				fout<<i<<endl;
			}
		}
		fout.close();
	}
	if(maxLength.size() == 0 && flag == false)
	{
		statusCopy = lengthStatus;
		sort(statusCopy.begin(), statusCopy.end(),cmp);
		ofstream fout;
		fout.open("recordLength.txt", ios::out);
		for(size_t i=0; i<lengthStatus.size(); i++)
		{
			if(lengthStatus[i] == statusCopy[0] || lengthStatus[i] == statusCopy[1] 
			|| lengthStatus[i] == statusCopy[2] || lengthStatus[i] == statusCopy[3])
			{
				fout<<i<<endl;
			}
		}
		fout.close();	
	}
	cvReleaseCapture(&capture);  
	cvReleaseImage(&thetaPic);
	cvReleaseImage(&frame);
	cvReleaseImage(&lengthPic);
	cvDestroyAllWindows();
	return 0;  
}  