#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "iostream"
#include <math.h>
#include <vector>

class TempDetect
{
	std::vector<cv::Rect> detectObj;
	std::vector<cv::Rect> drawObj;
	int thresher;
	cv::Mat tempImg;
	cv::Size rectSize;
	int medianBlurSize;
	int firstThresholdType;
	int tempType;

public:
	TempDetect();
	TempDetect(int thresher, cv::Mat const &tempImg, int rectSize);
	~TempDetect();
	void work(cv::Mat inputImg);
	void draw(cv::Mat &inOutputImg);
	int getDetectedObj();
	int getObjX(int N);
	int getObjY(int N);
	int getObjHigth(int N);
	int getObjWigth(int N);
	void setParams(int medianBlurSize, int firstThresholdType, int rectSize, int TempType, int thresher);
	void setTempImg(cv::Mat &tempImg);
	void getPredictCoordinate(std::vector<int>& pred_x, std::vector<int>& pred_y, std::vector<int>& pred_width, std::vector<int>& pred_heigth);
};

