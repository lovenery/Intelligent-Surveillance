// To get or set the value of each pixels in an image.
int h = lena.rows;
int w = lena.cols;
for (int i = 0; i < w; i++)
{
	for (int j = 0; j < h; j++)
	{
		if (lena.channels() == 1) {
			lena.at<uchar>(j, i) = 255;
		} 
		else if (lena.channels() == 3) {
			lena.at<cv::Vec3b>(j, i)[0] = 255;
			lena.at<cv::Vec3b>(j, i)[1] = 255;
			lena.at<cv::Vec3b>(j, i)[2] = 255;
		}
	}
}