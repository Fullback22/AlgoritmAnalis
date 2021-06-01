#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "Charactristic.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>


class Analis
{
	std::ofstream realAndPredictObj;
	std::ofstream probability;
	std::ofstream accuracyParams;
	std::ofstream out_end;
	std::string f_name;
	int n_frame;
	//std::vector<Charactristic> Object_P;
	//std::vector<Charactristic> Object_T;
	int true_pos;
	int false_pos;
	int false_neg;
	double IoU;

public:
	Analis(std::string filenameTruAndPredObj, std::string filenameProbability, std::string filenameAccuracy);
	Analis(std::string DirToSave);
	~Analis();
	//int write_true(int *Point1, int *Point2);
	//int write_true(std::string windowName);
	//int write_true(std::string fileName,int frame);
	//int write_predict(int point_x, int point_y, int wigth, int higth);
	//int write_frame(int frame, int detect);
	//int write_end(int frame);
	void comparison(std::string fileName);
	void setRealAndPredictObj(int const NumberFrame,const std::vector<int> *real_x, const std::vector<int> *real_y, const std::vector<int> *real_width, const std::vector<int> *real_height,
		const std::vector<int> *predict_x, const std::vector<int> *predict_y, const std::vector<int> *predict_width, const std::vector<int> *predict_heigth);
	void comparisonInAllFrams(std::string fileName, std::string fileNameOut, int const frameIn, cv::Mat* const InputImg);
};

double round(double InputNumber, int const accuracy);