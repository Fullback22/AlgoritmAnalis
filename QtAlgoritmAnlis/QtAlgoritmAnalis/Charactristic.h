#pragma once
#include <math.h>

class Charactristic
{
	double diagonal;
	double Sqear;
	double center_point_X;
	double center_point_Y;
public:
	Charactristic(int Point1_X, int Point1_Y, int Point2_X, int Point2_Y);
	~Charactristic();
	int comparison(Charactristic &obj, double &IoU_out, double &SKO_X_out, double &SKO_Y_out);
	double get_diagonal();
	double get_center_X();
	double get_center_Y();
	double get_sqear();
};

