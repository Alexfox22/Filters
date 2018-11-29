#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include <fstream>
#include <ctime>
using namespace cv;
using namespace std;


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
				//circle(dst_norm_scaled, Point(j, i), 2, 255,  2, 8, 0);
				circle(src, Point(j, i), 2, 255, 2, 8, 0);
				circle(dst, Point(j, i), 1, 255, 2, 8, 0);
			}
		}
	}
	
	return src;
}
void Clamp(Mat base)
{
	
	for (int i=0;i< base.rows;i++)
		for (int j=0;j<base.cols;j++)
		{/*
			if (base.at<uchar>(i, j) < 0) base.at<uchar>(i, j) = 0;
			if (base.at<uchar>(i, j) >255) base.at<uchar>(i, j) = 255;*/
			//base.at<uchar>(i, j)=std::to_int(base.at<uchar>(i, j));
		}
}
int main()
{
	cout << "Enter filename" << endl ;
	string inputf;
	cin >> inputf;
	
	string a = "C:\\";
	Mat input = imread(a + inputf + ".jpg");//, IMREAD_GRAYSCALE);           //1 читаем картинку с котиками
	imshow("Input", input);
	if (input.empty())
	{
		cout << "Could not open or find the image" << endl;
		cin.get(); //wait for any key press
		return -1;
	}

	//Mat outputKONTR = Mat::zeros(input.size(), CV_8UC1);
	//Mat outputPOINT = Mat::zeros(input.size(), CV_8UC1);
	//Mat points = Mat::zeros(input.size(), CV_8UC1);

	unsigned int start_time = clock();

	Mat gray;
	cvtColor(input, gray, COLOR_BGR2GRAY);				//2 фигачим изображение в полутоновое


	Mat hist_equalized_image;
	equalizeHist(gray, hist_equalized_image);			//3 улучшаем контраст
	imshow("Linears", hist_equalized_image);

	Mat outputCANNY;
	Canny(gray,outputCANNY, 10, 100, 3);               //4 метод Канни
	imshow("Canny", outputCANNY);

	Mat corners = outputCANNY;
	vector<Point2f> points;
	goodFeaturesToTrack(outputCANNY, points, 100, 0.05, 3);               //5 угловые точки и дибильные кружочки
	for (int i = 0; i < 100; ++i)
		circle(corners, points[i], 2, 255, 2, 8, 0);
//	circle(src, Point(j, i), );
	imshow("Corners", corners);

	//int thresh = 100;
	//int max_thresh = 255;
	//
	//points=cornerHarris_demo(outputCANNY,thresh);           //5
	//imshow("POINTS", points);

	//cv::Mat dist;
	//cv::distanceTransform(points, dist, CV_DIST_L2, 3);
	//cv::normalize(dist, dist, 0, 1., cv::NORM_MINMAX);
	//imshow("Dist", dist);           //6

	//unsigned int end_time = clock(); // конечное время
//	unsigned int search_time = end_time - start_time; // искомое время
	//cout << "LINE ----------> " << search_time << endl;


	cvWaitKey(0);
	return 0;
}

