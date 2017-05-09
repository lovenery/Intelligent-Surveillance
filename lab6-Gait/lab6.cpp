#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;

int main()
{

	const char* path = "video.avi";
	VideoCapture cap(path);
	if (!cap.isOpened())
		return -1;


	cv::BackgroundSubtractorMOG background_model;
	int frameCount = 0;
	Mat before, finalresult(240, 320, CV_8U, Scalar(0, 0, 0));
	while (true)
	{
		Mat frame, foreground, without_shadow;
		Mat ROI, diff_image(240, 320, CV_8U);
		Mat newROI(240, 320, CV_8U, Scalar(0, 0, 0));
		cap >> frame;
		if (frame.empty() == true) {
			break;
		}

		// Step1. find foreground image
		background_model.operator()(frame, foreground, 0.05);
		threshold(foreground, without_shadow, 127, 255, THRESH_BINARY);   // 去除陰影
		erode(without_shadow, without_shadow, Mat());           // 侵蝕運算
		dilate(without_shadow, without_shadow, Mat());           //  膨脹運算

		// Step2. find connected components
		vector<std::vector<cv::Point> > contours;
		Mat contours_foreground = foreground.clone();
		findContours(contours_foreground, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		vector<Rect> boundRect(contours.size());
		for (int i = 0; i < contours.size(); i++) {

			int n = contourArea(contours[i]);

			if (n > 1500)
			{
				boundRect[i] = boundingRect(contours[i]);
				//rectangle(frame, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0));

				ROI = without_shadow(cv::Rect(boundRect[i].tl().x,
					boundRect[i].tl().y,
					boundRect[i].width,
					boundRect[i].height));

				//imshow("video", ROI);


				// Step3. align foreground image
				int mean = 0;
				int counter = 0;
				for (int col = 0; col < ROI.cols; col++)
				{
					for (int row = 0; row < 30; row++)
					{
						if (ROI.at<uchar>(row, col) == 255)
						{
							mean = mean + col;
							counter++;
						}
					}
				}
				int average_x = mean / counter;

				Mat paste_region(newROI, Rect(160 - average_x, 50, ROI.cols, ROI.rows));  // 在 src (50,50) 的位置上new一個新的mat
				ROI.copyTo(paste_region);        // 並將ROI複製到這個新的mat上
				//imshow("video2", newROI);
			}


		}

		//imshow("video2", newROI);

		// Step4. absolute difference of images
		if (!before.empty())
		{
			cv::absdiff(before, newROI, diff_image);
		}
		before = newROI.clone();
		//imshow(std::to_string(frameCount), diff_image);

		// Step5. add all difference of images to find MEI image
		if (frameCount > 50)
		{

			cv::add(finalresult, diff_image, finalresult);
		}

		frameCount += 1;
		if (frameCount == 74) {
			imshow("Aligned", newROI);
			imshow("MEI", finalresult);
		}

		if (waitKey(30) >= 0) break;
	}


	return 0;
}

