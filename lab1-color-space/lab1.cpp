#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

int main()
{
	// path
	const char* path = "lena.jpg";

	// read
	cv::Mat lena = cv::imread(path, CV_LOAD_IMAGE_COLOR);

	// show origin
	cv::namedWindow("Display", CV_WINDOW_NORMAL);
	cv::imshow("Display", lena);

	// 1
	// convert RGB to HSV color space 
	cv::Mat hsvImg;
	cv::cvtColor(lena, hsvImg, cv::COLOR_BGR2HSV);
	cv::imshow("HSV", hsvImg);
	//cv::imwrite("HSV.jpg", hsvImg);

	// 2
	// split HSV image to three 8-bit channels.
	std::vector<cv::Mat> v;
	// get v[0] v[1] v[2]
	cv::split(hsvImg, v);
	for (int i = 0; i < 3; i++)
	{
		cv::imshow(std::to_string(i), v[i]);
		//cv::imwrite(std::to_string(i) + ".jpg", v[i]);
	}

	// 3-1
	// perform "log" gray-level transformation on H,S,and V plane.
	cv::Mat loghsv;
	for (int i = 0; i < 3; i++)
	{
		v[i].convertTo(loghsv, CV_32F);
		// 運算
		loghsv = loghsv + 1;
		cv::log(loghsv, loghsv);
		loghsv = 32 * loghsv / log(2);
		cv::convertScaleAbs(loghsv, loghsv);

		cv::imshow("log-" + std::to_string(i+1), loghsv);
		//cv::imwrite("log-" + std::to_string(i+1) + ".jpg", loghsv);
	}

	// 3-2
	// perform "inverse-log" gray-level transformation on H,S,and V plane.
	cv::Mat iloghsv;
	for (int i = 0; i < 3; i++)
	{
		v[i].convertTo(iloghsv, CV_64F);
		// 運算
		// cv::pow(iloghsv / 32, 2, iloghsv); // it's wrong
		for (int k = 0; k < iloghsv.rows; k++) {
			for (int j = 0; j < iloghsv.cols; j++) {
				iloghsv.at<double>(k, j) = pow(2, iloghsv.at<double>(k, j) / 32);
			}
		}
		iloghsv = iloghsv - 1;
		cv::convertScaleAbs(iloghsv, iloghsv);

		cv::imshow("ilog-" + std::to_string(i+1), iloghsv);
		//cv::imwrite("ilog-" + std::to_string(i+1) + ".jpg", iloghsv);
	}

	// keep GUI before return
	cv::waitKey(0);

	return 0;
}