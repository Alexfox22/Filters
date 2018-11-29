#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include <fstream>
#include <ctime>
using namespace cv;
using namespace std;


int main()
{
	cout << "Enter filename" << endl ;
	string inputf;
	cin >> inputf;
	
	string a = "C:\\";
	Mat input = imread(a + inputf + ".jpg");//, IMREAD_GRAYSCALE);           //1 читаем картинку с котиками
	//imshow("Input", input);
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
	imshow("Corners", corners);

	Mat dist;
	distanceTransform(255-corners, dist, CV_DIST_L2, 5);
	normalize(dist, dist, 0, 1, NORM_MINMAX);// 1., NORM_MINMAX);
	imshow("Dist", dist);												  //6 карта расстояний


	


	//int thresh = 100;
	//int max_thresh = 255;
	//
	//points=cornerHarris_demo(outputCANNY,thresh);           //5
	//imshow("POINTS", points);



	//unsigned int end_time = clock(); // конечное время
//	unsigned int search_time = end_time - start_time; // искомое время
	//cout << "LINE ----------> " << search_time << endl;


	cvWaitKey(0);
	return 0;
}

