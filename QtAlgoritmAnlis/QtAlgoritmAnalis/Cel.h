#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "iostream"
#include <math.h>
#include <vector>
#include <iterator>
#include "Oblast.h"


class Cel
{
protected:
	cv::Mat cel_img;
	int x;
	int y;
	int wigth;
	int length;
	int m;
	int k;	
public:
	Cel();
	Cel(Oblast*,cv::Mat*);
	~Cel();
	void set_cel(Oblast *, cv::Mat* );
	void set_cel(int, int, cv::Mat&);
	void set_cel(Oblast *, cv::Mat&);
	void draw(cv::Mat &);
	int get_wigth();
	int get_length();
	int get_x();
	int get_y();
	int get_end_x();
	int get_end_y();
	int get_m();
	int get_k();
	void detect();
	void lose_cel(bool);
	cv::Mat* get_img();
};

