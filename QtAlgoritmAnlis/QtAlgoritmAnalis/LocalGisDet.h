#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "iostream"

class LocalGisDet
{
	cv::Mat coreMorfologis;
	cv::Size rectSize;
	std::vector<cv::Rect> detectObj;
	int thresh;
	int medianBlurSize;
	int firstThresholdSize;
	int secondThresholdSize;
	int gausianBlurSize;
	int morfologiSize;
	int firstThresholdType;
	int secondThresholdType;

public:
	LocalGisDet();
	LocalGisDet( int thresh, int rect, cv::Size core);
	~LocalGisDet();
	void work(cv::Mat &inputImg);
	void draw(cv::Mat &inOutputImg);
	void clean();
	int getDetectedObj();
	int getObjX(int N);
	int getObjY(int N);
	int getObjHigth(int N);
	int getObjWigth(int N);
	void setParams(int thresh, int medianBlurSize, int firstThresholdSize, int secondThresholdSize, int gausianBlurSize, int morfologiSize, int firstThresholdType, int secondThresholdType, int sizeRectObj);
	void getPredictCoordinate(std::vector<int>& pred_x, std::vector<int>& pred_y, std::vector<int>& pred_width, std::vector<int>& pred_heigth);
};

