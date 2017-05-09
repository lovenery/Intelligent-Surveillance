#include "opencv2/opencv.hpp"

using namespace cv;

int main()
{
	const char* path = "video.mp4";
	VideoCapture cap(path); // read video

	// Gaussian Mixture Model
    // BackgroundSubtractorMOG2(int history=20, float varThreshold=16, bool bShadowDetection=true )
	BackgroundSubtractorMOG2 bgSubtractor(20, 16, true);

	// 檢查影片是否開啟成功
	if (!cap.isOpened())
		return -1;

    // 楨數
	int frameCount = 0;

	Mat gmm, foreground, morphology, result;
	namedWindow("video", 1);
    
    // 無窮迴圈
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		if (frame.empty() == true) {
			break;
		}

		// void BackgroundSubtractorMOG2::operator()(Mat update, Mat foreground, double learningRate)
		bgSubtractor(frame, gmm, 0.01);

		// 去除陰影
		threshold(gmm, foreground, 128, 255, THRESH_BINARY);

		// 雜訊
        // 侵蝕在擴張
		erode(foreground, morphology, Mat());
		dilate(morphology, morphology, Mat());

        // 楨數 150 200 做事
		frameCount +=1;
		if (frameCount == 150) {
			imshow("origin", frame);
			imshow("foreground", foreground);
			imshow("morphology", morphology);
			frame.copyTo(result, morphology); // morphology 是 mask
			imshow("result", result);

			imwrite("origin.jpg", frame);
			imwrite("foreground.jpg", foreground);
			imwrite("morphology.jpg", morphology);
			imwrite("result.jpg", result);
		}

		if (frameCount == 200) {
			imshow("origin2", frame);
			imshow("foreground2", foreground);
			imshow("morphology2", morphology);
			frame.copyTo(result, morphology); // morphology 是 mask
			imshow("result2", result);

			imwrite("origin2.jpg", frame);
			imwrite("foreground2.jpg", foreground);
			imwrite("morphology2.jpg", morphology);
			imwrite("result2.jpg", result);
		}

		imshow("video", gmm);
		if (waitKey(30) >= 0) break;
	}
  
	return 0;
}
