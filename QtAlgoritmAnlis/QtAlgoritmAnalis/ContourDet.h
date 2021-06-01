#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "iostream"

using namespace std;
using namespace cv;

class ContourDet
{
	vector<Rect> detectObj;
	vector<vector<Point>> searchObj;
	int thresh;
	int medianBlurSize;
	int gausianBlurSize;
public:
	ContourDet();
	~ContourDet();
	void work(Mat &inputImg);
	void draw(Mat &inOutputImg);
	void setObj(Mat inputImg);
	int getDetectedObj();
	int getObjX(int N);
	int getObjY(int N);
	int getObjHigth(int N);
	int getObjWigth(int N);
	void setParams(int thresh, int medianBlurSize, int gausianBlurSize);
};

