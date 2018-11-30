#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
using namespace cv;
using namespace std;



int main()
{
	auto begin = chrono::high_resolution_clock::now();
	double k = 255;
	cout << "Enter filename" << endl ;
	string inputf;
	cin >> inputf;
	
	string a = "C:\\";
	Mat input = imread(a + inputf + ".jpg");//, IMREAD_GRAYSCALE);           //1 читаем картинку с котиками
	imshow("Input", input);
	if (input.empty())
	{
		cout << "Could not open or find the image" << endl;
		cin.get(); 
		return -1;
	}

	Mat gray;
	cvtColor(input, gray, COLOR_BGR2GRAY);				//2 изображение в полутоновое

	Mat hist_equalized_image;
	equalizeHist(gray, hist_equalized_image);			//3 улучшаем контраст
//	imshow("Contrast", hist_equalized_image);

	Mat outputCANNY;
	Canny(gray,outputCANNY, 10, 100, 3);               //4 метод  анни
//	imshow("Canny", outputCANNY);

	Mat corners = outputCANNY;
	vector<Point2f> points;
	goodFeaturesToTrack(outputCANNY, points, 100, 0.05, 3);               //5 угловые точки и кружочки
	for (int i = 0; i < points.size(); ++i)
		circle(corners, points[i], 2, 255, 2, 8, 0);
	//imshow("Corners", corners);


	Mat dist;
	distanceTransform(255-corners, dist, CV_DIST_L2, 5);
	normalize(dist, dist, 0, 255, NORM_MINMAX);
//	imshow("Distance", dist);												  //6 карта рассто€ний
	dist.convertTo(dist, CV_8U);
	normalize(dist, dist, 0, 1, NORM_MINMAX);// 1., NORM_MINMAX);

	Mat integ;
	integral(input, integ, -1);
	int height = input.size().height;
	int width = input.size().width;
	int channels = input.channels();

	Mat filtered = input.clone();
	for (int ch = 0; ch < channels; ch++)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int kernelDiam = dist.at<uchar>(i, j);
				if (kernelDiam % 2 == 0)
					kernelDiam++;
				if ((i <= kernelDiam / 2) || (j <= kernelDiam / 2) || (j > width - kernelDiam / 2 - 1) || (i > height - kernelDiam / 2 - 1) || (kernelDiam <= 1))
					continue;
				float sum = (integ.at<Vec3f>(i - kernelDiam / 2 - 1, j - kernelDiam / 2 - 1)(ch)
					- integ.at<Vec3f>(i - kernelDiam / 2 - 1, j + kernelDiam / 2)(ch)
					+ integ.at<Vec3f>(i + kernelDiam / 2, j + kernelDiam / 2)(ch)
					- integ.at<Vec3f>(i + kernelDiam / 2, j - kernelDiam / 2 - 1)(ch));
				filtered.at<Vec3b>(i, j)(ch) = sum / (float)(pow(kernelDiam, 2));
			}
		}
	}
	imshow("Filtered", filtered);

	Mat integ;
	norm7(gray, dist, integ, k);
	imshow("7", integ);

	auto end = chrono::high_resolution_clock::now();
	auto elapsed = end - begin;
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
	cout << endl << "Time spent: " << milliseconds << endl;
	float Fps = 6 / (milliseconds / 1000.f);
	cout << "Fps: " << Fps << endl;

	cvWaitKey(0);
	return 0;
}

