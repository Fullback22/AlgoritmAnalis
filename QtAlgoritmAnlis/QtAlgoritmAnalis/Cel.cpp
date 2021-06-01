#include "Cel.h"

Cel::Cel()
{
	x = 0;
	y = 0;
	wigth = 0;
	length = 0;
	m = 0;
	k = 0;
}
Cel::Cel(Oblast* isxod,cv::Mat *isxod_img)
{
	cel_img = *(isxod_img);
	x = isxod->get_first_col();
	y = isxod->get_first_row();
	//wigth = isxod->get_end_col() - x;
	//length = isxod->get_end_row() - y;
	wigth = isxod_img->cols;
	length = isxod_img->rows;
	m = 1;
	k = 0;
}


Cel::~Cel()
{
}

void Cel::set_cel(Oblast* isxod,cv::Mat *isxod_img)
{
	x = isxod->get_first_col();
	y=isxod->get_first_row();
	wigth=isxod->get_end_col()-x;
	length=isxod->get_end_row()-y;
	cv::Rect oblst(x, y, wigth, length);
	cel_img = *(isxod_img);
}
void Cel::set_cel(int x_in, int y_in, cv::Mat&isxod)
{
	y = y_in;// -length / 8;
	x = x_in;// -wigth / 8;
	cv::Rect obl(x, y, wigth, length);
	cel_img = isxod(obl);
}
//void Cel::set_cel(Oblast* isxod, Mat&isxod_img)
//{
//	x = isxod->get_first_col();
//	y = isxod->get_first_row();
//	wigth = isxod->get_end_col() - x;
//	length = isxod->get_end_row() - y;
//	Rect oblst(x, y, wigth, length);
//	cel_img = isxod_img(oblst);
//}
void Cel::draw(cv::Mat &in_out)
{
	cv::rectangle(in_out, cv::Rect(x,y,wigth,length), 255, 2);
}
int Cel::get_length()
{
	return length;
}
int Cel::get_wigth()
{
	return wigth;
}
int Cel::get_x()
{
	return x;
}
int Cel::get_y()
{
	return y;
}
int Cel::get_end_x()
{
	return x + wigth;
}
int Cel::get_end_y()
{
	return y + length;
}
cv::Mat* Cel::get_img()
{
	return &cel_img;
}
void Cel::detect()
{
	if (m != 3)
		m++;
}
int Cel::get_m()
{
	return m;
}
int Cel::get_k()
{
	return k;
}
void Cel::lose_cel(bool not_lose)
{
	if (not_lose == true)
		k=0;
	else
		++k;
}