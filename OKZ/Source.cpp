#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include <fstream>
#include <ctime>
using namespace cv;
using namespace std;

void linears(Mat source, Mat result)
{
	//unsigned int start_time = clock();
	for (int i = 0; i < result.rows; ++i)
		for (int j = 0; j < result.cols; ++j)
			result.at<uchar>(i, j) = source.at<uchar>(i, j);
	int *hist;
	hist = new int[256];
	memset(hist, 0, 256 * sizeof(int));
	int min = INT_MAX;
	int max = INT_MIN;

	for (int i = 0; i < source.rows; i++)
	{
		for (int j = 0; j < source.cols; j++)
		{
			hist[source.at<uchar>(i, j)]++;
		}
	}
	//painthist(source);


	int i = 0;
	while (i < 256 && hist[i] == 0) i++;
	min = i;
	i = 255;
	while (i > 0 && hist[i] == 0) i--;
	max = i;

	double buf = 0;
	for (int i = 0; i < result.rows; i++)
	{
		for (int j = 0; j < result.cols; j++)
		{
			result.at<uchar>(i, j) = round(255 * (result.at<uchar>(i, j) - min) / (max - min));
		}
	}
	//unsigned int end_time = clock(); // конечное время
	//unsigned int search_time = end_time - start_time; // искомое время
	//cout << search_time << endl;
	//painthist(result);

}
Mat cornerHarris_demo(Mat src,int thresh)
{
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;
	Mat dst = Mat::zeros(src.size(), CV_32FC1);
	cornerHarris(src, dst, blockSize, apertureSize, k);
	Mat dst_norm, dst_norm_scaled;
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled);
	for (int i = 0; i < dst_norm.rows; i++)
	{
		for (int j = 0; j < dst_norm.cols; j++)
		{
			if ((int)dst_norm.at<float>(i, j) > thresh)
			{
				//circle(dst_norm_scaled, Point(j, i), 2, 255,  2, 8, 0);//Scalar(0), 2, 8, 0);
				circle(src, Point(j, i), 2, 255, 2, 8, 0);//Scalar(0), 2, 8, 0);
				circle(dst, Point(j, i), 1, 255, 2, 8, 0);//Scalar(0), 2, 8, 0);
			}
		}
	}
	//namedWindow("Corners detected");
	//imshow("Corners detected", dst_norm_scaled);

	return src;
}

int main()
{
	Mat input;
	cout << "Enter filename" << endl ;
	string inputf;
	cin >> inputf;
	
	string a = "C:\\";
	input = imread(a + inputf + ".jpg", IMREAD_GRAYSCALE);
	imshow("Input", input);

	Mat outputKONTR = Mat::zeros(input.size(), CV_8UC1);
	Mat outputCANNY = Mat::zeros(input.size(), CV_8UC1);
	Mat outputPOINT = Mat::zeros(input.size(), CV_8UC1);
	Mat points = Mat::zeros(input.size(), CV_8UC1);

	unsigned int start_time = clock();

	linears(input, outputKONTR);
	imshow("Linears", outputKONTR);

	Canny(input,outputCANNY, 10, 100, 3);
	imshow("Canny", outputCANNY);

	int thresh = 100;
	int max_thresh = 255;
	
	points=cornerHarris_demo(outputCANNY,thresh);
	imshow("POINTS", points);

	cv::Mat dist;
	cv::distanceTransform(points, dist, CV_DIST_L2, 3);
	cv::normalize(dist, dist, 0, 1., cv::NORM_MINMAX);
	imshow("Dist", dist);

	//unsigned int end_time = clock(); // конечное время
//	unsigned int search_time = end_time - start_time; // искомое время
	//cout << "LINE ----------> " << search_time << endl;


	cvWaitKey(0);
	return 0;
}

