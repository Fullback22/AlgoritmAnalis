#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "iostream"

using namespace std;
using namespace cv;

class GlobslGisDet
{
	Mat coreMorfologis;
	Size rectSize;
	vector<Rect> detectObj;
	int thresh;
	int medianBlurSize;
	int gausianBlurSize;
	int morfologiSize;
public:
	GlobslGisDet();
	GlobslGisDet(int thresh, int rect, Size core);
	~GlobslGisDet();
	void work(Mat inputImg);
	void draw(Mat &inOutputImg);
	int getDetectedObj();
	int getObjX(int N);
	int getObjY(int N);
	int getObjHigth(int N);
	int getObjWigth(int N);
	void setParams(int thresh, int medianBlurSize, int gausianBlurSize, int morfologiSize, int sizeRectObj);
};

