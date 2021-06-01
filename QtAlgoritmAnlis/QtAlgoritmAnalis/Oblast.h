#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "iostream"
#include <math.h>
#include <vector>
#include <iterator>


class Oblast
{
private:
	std::vector <int> first_gr;
	std::vector <short int> second_gr;
	int first_row;
	int end_row;
	int first_col;
	int end_col;
	int chislo_elementov;
public:
	Oblast();
	~Oblast();
	void set_oblast(int const *, int const *, int , int const , int const* , bool);
	int get_first_gr(int);
	int get_second_gr(int);
	void reset_obl_new(int const *, int const *, int const, int const, int const, int const *, bool);
	void set_first_row(int);
	void set_end_row(int);
	void set_first_col(int);
	void set_end_col(int);
	int get_end_row();
	int get_first_row();
	int get_end_col();
	int get_first_col();
	int get_razmer(); 
	void draw_obl(cv::Mat);
	void normalizade_row();
	void normalizade_col();
	double fraktal(cv::Mat, int);
};

