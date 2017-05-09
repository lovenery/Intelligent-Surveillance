#include "opencv2/opencv.hpp"

using namespace cv;

int main()
{
	const char* path = "input.mp4";
	VideoCapture cap(path);
	if (!cap.isOpened())
		return -1;

	// 要放在外面
	std::vector<std::vector<Point> > contours; // init contours

	int frameCount = 0;
	for (;;)
	{
		Mat frame, origin, result;
		cap >> frame; // get a new frame from camera
		if (frame.empty() == true) {
			break;
		}
		origin = frame.clone();

		// use Gaussian Blur to get smooth video
		// 為了減少noise
		GaussianBlur(frame, frame, Size(5, 5), 2, 0);
		
		// Convert the color space to YCbCr
		cvtColor(frame, frame, CV_BGR2YCrCb);

		// Extract the skin color pixels with the values in Cb channel and Cr channel
		// 把手的顏色去掉
		inRange(frame, Scalar(0, 133, 98), Scalar(255, 177, 122), frame);

		// 去除陰影
		threshold(frame, frame, 128, 255, THRESH_BINARY);

		// mask放上去
		origin.copyTo(result, frame);

		// 計算邊界
		findContours(frame, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //calculate contours
		for (int i = 0; i < contours.size(); i++)
		{
			//calculate  area of contour
			// 輪廓的面積
			int n = contourArea(contours[i]);

			// 大於一定的大小(手)
			if (n > 5000)
			{
				// draw contours[i] on image_gbr
				// 手描紅邊
				drawContours(result, contours, i, Scalar(0, 0, 255));

				// 藍色正方形
				Rect rect = boundingRect(contours[i]);
				rectangle(result, rect.tl(), rect.br(), Scalar(255, 0, 0));

				// 數字
				putText(result, "pixels " + std::to_string(n), rect.tl(), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));
			}

		}


		frameCount += 1;
		if (frameCount == 100) {
			imshow("100", result);
			imwrite("100.jpg", result);
		}
		if (frameCount == 200) {
			imshow("200", result);
			imwrite("200.jpg", result);
		}

		imshow("video", result);
		if (waitKey(30) >= 0) break;
	}


    return 0;
}

