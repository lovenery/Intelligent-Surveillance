#include <opencv2/core/core.hpp> //basic
#include <opencv2/highgui/highgui.hpp> //imread or …
#include <opencv2/nonfree/nonfree.hpp>  //sift or surf <- important
#include <opencv2/legacy/legacy.hpp>  //matcher

using namespace cv;

int main()
{
	// 兩張原圖
	Mat ori_image_obj = imread("obj.jpg");
	Mat ori_image_scene = imread("scene.jpg");
	//imshow("ori", ori_image_obj);

	// 第一surf
	SURF surf(300);
	Mat gray_image = imread("obj.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat descriptor;
	std::vector<KeyPoint> keypoints;
	surf.operator()(gray_image, noArray(), keypoints, descriptor, 0);
	// draw key points for debug
	Mat key_point_obj;
	drawKeypoints(ori_image_obj, keypoints, key_point_obj, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	//imshow("hiih", key_point_obj);


	// 第二張surf
	SURF surf_scene(300);
	Mat gray_image_scene = imread("scene.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat descriptor_scene;
	std::vector<KeyPoint> keypoints_scene;
	surf_scene.operator()(gray_image_scene, noArray(), keypoints_scene, descriptor_scene, 0);
	// draw key points for debug
	Mat key_point_scene;
	drawKeypoints(ori_image_scene, keypoints_scene, key_point_scene, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	//imshow("hiih2", key_point_scene);


	// 合體
	// Match the same feature points from the input images.
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptor, descriptor_scene, matches);


	// by 助教 變成good_matches
	double min_dist = 1000;
	for(int i = 0; i < (int)matches.size(); i++) { // find the min_distance
		double dist = matches[i].distance;
		if (dist < min_dist)
			min_dist = dist;
	} // for 
	std::vector< DMatch > good_matches; // store a better matches
	for (int i = 0; i < (int)matches.size(); i++) {
		if (matches[i].distance < 3 * min_dist) // you can chage the number "3 "
			good_matches.push_back(matches[i]);
	} // for


	// 第一張結果
	Mat img_matches;
	drawMatches(ori_image_obj, keypoints, ori_image_scene, keypoints_scene, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	imshow("Draw Matches", img_matches);
	imwrite("DrawMatches.jpg", img_matches);

	// by 助教 為了轉第二張圖片
	std::vector<Point2f> src;
	std::vector<Point2f> target;
	for (int i = 0; i < good_matches.size(); i++) {
		src.push_back(keypoints[good_matches[i].queryIdx].pt);
		target.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}
	Mat H = findHomography(target, src, CV_RANSAC);
	// 轉過來
	Mat after_perspective_ori_image_scene;
	warpPerspective(ori_image_scene, after_perspective_ori_image_scene, H, Size(ori_image_scene.cols * 2, ori_image_scene.rows));
	//imshow("test", after_perspective_ori_image_scene);


	// 第二張結果
	Mat target_in_big_mat(after_perspective_ori_image_scene, Rect(0, 0, after_perspective_ori_image_scene.cols, after_perspective_ori_image_scene.rows));
	//after_perspective_ori_image_scene.copyTo(target_in_big_mat);
	ori_image_obj.copyTo(target_in_big_mat(Rect(0, 0, ori_image_obj.cols, ori_image_obj.rows)));
	imshow("Merging Result", target_in_big_mat);
	imwrite("MergingResult.jpg", target_in_big_mat);


	cv::waitKey(0);

	return 0;
}

