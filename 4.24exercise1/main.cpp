#include <iostream>
#include<opencv2/opencv.hpp>
#include<cmath>

using namespace std;
using namespace cv;

//函数声明
void calc_Hist(cv::Mat Mat1, float *hist, int dim, int size)
{
	int nX = Mat1.cols / size;
	int nY = Mat1.rows / size;
	int Angle = 360 / dim;
	//计算梯度与角度
	cv::Mat gx, gy;
	cv::Mat mag, angle;
	cv::Sobel(Mat1, gx, CV_32F, 1, 0, 1);
	cv::Sobel(Mat1, gy, CV_32F, 0, 1, 1);
	cv::cartToPolar(gx, gy, mag, angle, true);
	int Num = 0;
	for (int i = 0; i < nX; i++)
	{
		for (int j = 0; j < nY; j++)
		{
			for (int m = 0; m < size; m++)
			{
				for (int n = 0; n < size; n++)
				{
					int num = (angle.at<float>(j*size + n, i*size + m)) / Angle;
					hist[Num * 8 + num] += mag.at<float>(j*size + m, i*size + n);
				}
			}
			Num++;
		}
	}
}

float com_Mat(float *hist1, float *hist2, int size)
{
	float sum = 0;
	for (int i = 0; i < size; i++)
	{
		sum += (hist1[i] - hist2[i])*(hist1[i] - hist2[i]);
	}
	sum = sqrt(sum);
	return sum;
}

int main()
{
	cv::Mat srcMat = imread("E:\\PIC\\image.jpg", 0);
	cv::Mat firstMat = imread("E:\\PIC\\image1.jpg", 0);
	cv::Mat secondMat = imread("E:\\PIC\\image2.jpg", 0);
	
	int cellSize = 16;
	int Dim = 8;
	int nX = srcMat.cols / cellSize;
	int nY = srcMat.rows / cellSize;
	int bins = nX * nY * Dim;
	float * src_hist = new float[bins];
	memset(src_hist, 0, sizeof(float)*bins);
	float * f_hist = new float[bins];
	memset(f_hist, 0, sizeof(float)*bins);
	float * s_hist = new float[bins];
	memset(s_hist, 0, sizeof(float)*bins);
	
	calc_Hist(srcMat, src_hist, Dim, cellSize);
	calc_Hist(firstMat, f_hist, Dim, cellSize);
	calc_Hist(secondMat, s_hist, Dim, cellSize);
	
	float result1 = com_Mat(src_hist, f_hist, bins);
	float result2 = com_Mat(src_hist, s_hist, bins);
	
	if (result1 <= result2)
	{
		std::cout << "相似的图为图1" << endl;
	}
	else
	{
		std::cout << "相似的图为图2" << endl;
	}

	delete[] src_hist;
	delete[] f_hist;
	delete[] s_hist;

	return 0;
}
