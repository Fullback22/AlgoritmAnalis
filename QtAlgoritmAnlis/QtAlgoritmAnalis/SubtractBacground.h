#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include "iostream"
#include <opencv2/video/background_segm.hpp>


class SubtractBacground
{
	cv::Mat coreMorfologis;
	cv::Size rectSize;
	std::vector<cv::Rect> detectObj;
	cv::Ptr<cv::BackgroundSubtractorMOG2> pBackSub = cv::createBackgroundSubtractorMOG2();
	//BackgroundSubtractorMOG2 pBackSub;
	int thresh;
	int medianBlurSize;
	int thresholdSize;
	int morfologiSize;
	int thresholdType;
	double learnSpeed;
public:
	SubtractBacground();
	SubtractBacground(int thresh,int rectSize,cv::Size coreSize);
	~SubtractBacground();
	void work(cv::Mat &inputImg);
	void draw(cv::Mat &inOutputImg);
	void clean();
	int getDetectedObj();
	int getObjX(int N);
	int getObjY(int N);
	int getObjHigth(int N);
	int getObjWigth(int N);
	void setParams(int thresh, int medianBlurSize, int ThresholdSize, int morfologiSize, int ThresholdType, double learnSpeed, int sizeRectObj);
	void getPredictCoordinate(std::vector<int>& pred_x, std::vector<int>& pred_y, std::vector<int>& pred_width, std::vector<int>& pred_heigth);
};

