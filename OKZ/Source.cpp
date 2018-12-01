#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
using namespace cv;
using namespace std;
using namespace std::chrono;


int main()
{
	auto start = high_resolution_clock::now();
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
	imshow("Contrast", hist_equalized_image);

	Mat outputCANNY;
	Canny(gray,outputCANNY, 10, 100, 3);               //4 метод  анни
	imshow("Canny", outputCANNY);

	Mat corners = outputCANNY;
	vector<Point2f> points;
	goodFeaturesToTrack(outputCANNY, points, 100, 0.05, 3);               //5 угловые точки и кружочки
	for (int i = 0; i < points.size(); ++i)
		circle(corners, points[i], 2, 255, 2, 8, 0);
	imshow("Corners", corners);


	Mat dist;
	distanceTransform(255-outputCANNY, dist, CV_DIST_L2, 5);
	normalize(dist, dist, 0, 1, NORM_MINMAX);
	imshow("Distance", dist);	//6 карта рассто€ний
	dist.convertTo(dist, CV_8U);

	Mat integ;
	integral(gray, integ, -1);

	Mat filtered = gray;
	for (int i = 0; i < input.rows; i++)
		{
			for (int j = 0; j < input.cols; j++)
			{
				int kernel = dist.at<uchar>(i, j);
				if (kernel % 2 == 0)
					kernel++;
				double kernel2 = kernel / 2;
				if ((kernel <= 1) || (i <= kernel2) || (j <= kernel2) || (i > input.rows - kernel2- 1) || (j > input.cols - kernel2 - 1)) continue;
				float sum = (integ.at<uchar>(i - kernel2 - 1, j - kernel2 - 1) - integ.at<uchar>(i - kernel2 - 1, j + kernel2)
						   + integ.at<uchar>(i + kernel2, j + kernel2) - integ.at<uchar>(i + kernel2, j - kernel2 - 1));    //7 фильтраци€
				filtered.at<uchar>(i, j) = sum / (float)(pow(kernel, 2));
			}
	}
	imshow("Filtered", filtered);

	auto end = high_resolution_clock::now();
	auto conv = duration_cast<milliseconds>(end - start).count();
	cout << endl << "Time spent: " << conv << endl;
	cout << "Fps: " << 6 / (conv / 1000.f) << endl;						//9 Fps

	cvWaitKey(0);
	return 0;
}

