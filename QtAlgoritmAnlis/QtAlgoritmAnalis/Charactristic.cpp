#include "Charactristic.h"


Charactristic::Charactristic(int Point1_X, int Point1_Y, int Point2_X,int Point2_Y)
{
	int wigth{Point2_X - Point1_X};
	int higth{Point2_Y - Point1_Y};
  	Sqear = wigth*higth;
	diagonal = sqrt(pow((double)wigth, 2) + pow((double)higth, 2));
	center_point_X = Point1_X + wigth / 2;
	center_point_Y = Point1_Y + higth / 2;
}


Charactristic::~Charactristic()
{
}
double Charactristic::get_diagonal()
{
	return diagonal;
}
double Charactristic::get_center_X()
{
	return center_point_X;
}
double Charactristic::get_center_Y()
{
	return center_point_Y;
}
double Charactristic::get_sqear()
{
	return Sqear;
}
int Charactristic::comparison(Charactristic &True_rectangel, double &IoU_out,double &SKO_X_out, double &SKO_Y_out)
{
	IoU_out = True_rectangel.get_sqear() / Sqear;
	SKO_X_out = sqrt(pow(center_point_X - True_rectangel.get_center_X(), 2));
	SKO_Y_out = sqrt(pow(center_point_Y - True_rectangel.get_center_Y(), 2));
	if (1.5 < IoU_out < 0.5 && (SKO_X_out <= True_rectangel.get_diagonal() / 2.5 && SKO_Y_out <= True_rectangel.get_diagonal() / 2.5))
		return 1;
	else
 		return 0;
}