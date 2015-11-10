// TEST.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"  
#include "cxcore.h"   
#include "cv.h"   
#include "highgui.h"  

void FillInternalContours(IplImage *pBinary, double dAreaThre)   
{   
	double dConArea;   
	CvSeq *pContour = NULL;   
	CvSeq *pConInner = NULL;   
	CvMemStorage *pStorage = NULL;   
	// ִ������   
	if (pBinary)   
	{   
		// ������������   
		pStorage = cvCreateMemStorage(0);   
		cvFindContours(pBinary, pStorage, &pContour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);   
		// �����������   
		cvDrawContours(pBinary, pContour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 2, CV_FILLED, 8, cvPoint(0, 0));  
		// ������ѭ��   
		int wai = 0;  
		int nei = 0;  
		for (; pContour != NULL; pContour = pContour->h_next)   
		{   
			wai++;  
			// ������ѭ��   
			for (pConInner = pContour->v_next; pConInner != NULL; pConInner = pConInner->h_next)   
			{   
				nei++;  
				// ���������   
				dConArea = fabs(cvContourArea(pConInner, CV_WHOLE_SEQ));  
				printf("%f\n", dConArea);   
			}  
			CvRect rect = cvBoundingRect(pContour,0);
			cvRectangle(pBinary, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),CV_RGB(255,255, 255), 1, 8, 0);
		}   

		printf("wai = %d, nei = %d", wai, nei);  
		cvReleaseMemStorage(&pStorage);   
		pStorage = NULL;   
	}   
}   
int Otsu(IplImage* src)      
{      
	int height=src->height;      
	int width=src->width;          

	//histogram      
	float histogram[256] = {0};      
	for(int i=0; i < height; i++)    
	{      
		unsigned char* p=(unsigned char*)src->imageData + src->widthStep * i;      
		for(int j = 0; j < width; j++)     
		{      
			histogram[*p++]++;      
		}      
	}      
	//normalize histogram      
	int size = height * width;      
	for(int i = 0; i < 256; i++)    
	{      
		histogram[i] = histogram[i] / size;      
	}      

	//average pixel value      
	float avgValue=0;      
	for(int i=0; i < 256; i++)    
	{      
		avgValue += i * histogram[i];  //����ͼ���ƽ���Ҷ�    
	}       

	int threshold;        
	float maxVariance=0;      
	float w = 0, u = 0;      
	for(int i = 0; i < 256; i++)     
	{      
		w += histogram[i];  //���赱ǰ�Ҷ�iΪ��ֵ, 0~i �Ҷȵ�����(��������ֵ�ڴ˷�Χ�����ؽ���ǰ������) ��ռ����ͼ��ı���    
		u += i * histogram[i];  // �Ҷ�i ֮ǰ������(0~i)��ƽ���Ҷ�ֵ�� ǰ�����ص�ƽ���Ҷ�ֵ    

		float t = avgValue * w - u;      
		float variance = t * t / (w * (1 - w) );      
		if(variance > maxVariance)     
		{      
			maxVariance = variance;      
			threshold = i;      
		}      
	}      

	return threshold;      
}     

int main()  
{  
	IplImage *img = cvLoadImage("F://pp.jpg", 0);  
	IplImage *bin = cvCreateImage(cvGetSize(img), 8, 1); 
   

	int thresh = Otsu(img);  
	cvThreshold(img, bin, thresh, 255, CV_THRESH_BINARY);  

	FillInternalContours(bin, 200);  

	cvNamedWindow("img");  
	cvShowImage("img", img);  

	cvNamedWindow("result");  
	cvShowImage("result", bin);  

	cvWaitKey(-1);  

	cvReleaseImage(&img);  
	cvReleaseImage(&bin);  
	return 0;  
}  

