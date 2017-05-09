#include "opencv2/opencv.hpp"
#include <opencv2/objdetect/objdetect.hpp>

using namespace cv;

int main()
{
	const char* path = "SRC.mp4";
	VideoCapture cap(path);
	if (!cap.isOpened())
		return -1;

	// 臉
	String face_cascade_name = "haarcascade_frontalface_alt.xml";
	CascadeClassifier face_cascade;
	face_cascade.load(face_cascade_name);
	if (!face_cascade.load(face_cascade_name)) {
		printf("--(!)Error loading\n");
		return -1;
	}
	std::vector<cv::Rect> FACES;

	// 眼睛
	String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
	CascadeClassifier eyes_cascade;
	eyes_cascade.load(eyes_cascade_name);
	if (!eyes_cascade.load(eyes_cascade_name)) {
		printf("--(!)Error loading\n");
		return -1;
	}
	std::vector<cv::Rect> EYES;

	int frameCount = 0;
	while (true)
	{
		Mat frame, gray_frame;
		cap >> frame;
		if (frame.empty() == true) {
			break;
		}

		cvtColor(frame, gray_frame, CV_BGR2GRAY);
		face_cascade.detectMultiScale(gray_frame, FACES, 1.1, 3, 0, Size(100, 100));

		// draw
		for (int i = 0; i < FACES.size(); i++)
		{
			cv::rectangle(frame, FACES[i], Scalar(0, 255, 0), 4, 8, 0); // rect are faces[i]!
			Mat faceROI = gray_frame(FACES[i]);
			eyes_cascade.detectMultiScale(faceROI, EYES, 1.1, 3, 0, Size(.1, .1));

			for (int j = 0; j < EYES.size(); j++)
			{
				Point center(FACES[i].x + EYES[j].x + EYES[j].width*0.5, FACES[i].y + EYES[j].y + EYES[j].height*0.5);
				int radius = cvRound((EYES[j].width + EYES[j].height)*0.25);
				cv::circle(frame, center, radius, Scalar(255, 0, 0), 3, 8, 0);
			}
		}

		frameCount += 1;
		if (frameCount == 100) {
			imshow("100", frame);
			imwrite("100.jpg", frame);
		}
		if (frameCount == 300) {
			imshow("300", frame);
			imwrite("300.jpg", frame);
		}

		imshow("video", frame);

		if (waitKey(30) >= 0) break;
	}

    return 0;
}

