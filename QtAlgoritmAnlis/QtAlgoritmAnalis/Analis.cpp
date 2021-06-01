#include "Analis.h"

//void myMouseCallback_1(int event, int x, int y, int flag, void* param)
//{
//	static int i{ 0 };
//	static int first_point[2]{0, 0};
//	int second_point[2]{0, 0};
//	WriteToFile* wr;
//	wr = (WriteToFile*)param;
//	if (i == 0 && event == 1)
//	{
//		first_point[0] = x;
//		first_point[1] = y;
//		++i;
//		event = 0;
//	}
//	if (i == 1 && event == 1)
//	{
//		second_point[0] = x;
//		second_point[1] = y;
//		i++;
//		event = 0;
//	}
//	if (i == 2)
//	{
//		wr->write_true(first_point, second_point);
//		i = 0;
//	}
//}

Analis::Analis(std::string filename_1, std::string filename_2, std::string filename_3):
	n_frame(0),
	true_pos(0),
	false_pos(0),
	false_neg(0),
	IoU(0)
{
	if (realAndPredictObj.is_open() == false)
		realAndPredictObj.open(filename_1, std::ios::app);
	if (probability.is_open() == false)
		probability.open(filename_2, std::ios::app);
	probability << "frame\tRelOb\tDetObj\tTrueDet\tFalseDet\tNotDet" << std::endl;
	if (accuracyParams.is_open() == false)
		accuracyParams.open(filename_3, std::ios::app);
	//accuracyParams << "frame\tSKO_X\tSKO_Y\tIoU" << std::endl;
	accuracyParams << "frame\tIoU" << std::endl;
}

Analis::Analis(std::string dirToSave) :
	n_frame(0),
	true_pos(0),
	false_pos(0),
	false_neg(0),
	IoU(0)
{
	if (realAndPredictObj.is_open() == false)
		realAndPredictObj.open(dirToSave + "/realAndPredictObj.txt", std::ios::app);
	if (probability.is_open() == false)
		probability.open(dirToSave + "/probability.txt", std::ios::app);
	probability << "frame\tRelOb\tDetObj\tTrueDet\tFalseDet\tNotDet" << std::endl;
	if (accuracyParams.is_open() == false)
		accuracyParams.open(dirToSave + "/accuracyParams.txt", std::ios::app);
	//accuracyParams << "frame\tSKO_X\tSKO_Y\tIoU" << std::endl;
	accuracyParams << "frame\tIoU" << std::endl;
}

Analis::~Analis()
{

}

//int WriteToFile::write_true( int *Point1, int *Point2 )
//{
//	realAndPredictObj <<"T "<< *(Point1) << "\t" << *(Point1 + 1) << "\t" << *(Point2) << "\t" << *(Point2 + 1)<<"\t";
//	Object_T.push_back(Charactristic(*(Point1), *(Point1 + 1), *(Point2), *(Point2 + 1)));
//	return 0;
//}
//
//int WriteToFile::write_true(std::string fileName,int frame_)
//{
//	std::ifstream fileTrue{fileName};
//	std::string true_rect{ 0 };
//	bool end_frame{false};
//	for (int i{ 1 }; end_frame==false; i++)
//	{
//		std::getline(fileTrue, true_rect);
//		std::istringstream iss{ true_rect };
//		int frame{ 0 }, px_1{ 0 }, py_1{ 0 }, px_2{ 0 }, py_2{ 0 };
//		iss >> frame>>px_1>>py_1>>px_2>>py_2;
//		if (frame == frame_)
//		{
//			if (px_1 == px_2 && py_1 == py_2)
//			{
//			}
//			else
//			{
//				realAndPredictObj << "T " << px_1 << "\t" << py_1 << "\t" << px_2 << "\t" << py_2 << "\t";
//				Object_T.push_back(Charactristic(px_1, py_1, px_2, py_2));
//			}
//		}		
//
//		if (frame > frame_ || fileTrue.eof())
//		{
//			end_frame = true;
//		}
//			
//	}
//	return 0;
//}
//
//int WriteToFile::write_frame(int frame, int detect)
//{
//	//n_frame = frame;
//	//realAndPredictObj << frame << "\t" << detect << "\t";
//	return 0;
//}

//int WriteToFile::write_predict(int point1_x, int point1_y, int point2_x, int point2_y)
//{
//	realAndPredictObj << "P " << point1_x << "\t" << point1_y << "\t" << point2_x << "\t" << point2_y << "\t";
//	Object_P.push_back(Charactristic(point1_x, point1_y, point2_x, point2_y));
//	return 0;
//}

//int WriteToFile::write_end(int frame)
//{
//	//int detect_true{ 0 };
//	//int detect_false{ 0 };
//	//int real{ 0 };
//	//real = Object_T.size();
//	//int detect{ 0 };
//	//detect = Object_P.size();
//	//detect_false = detect;
// //	for (int i{ 0 }; i < Object_P.size(); i++)
//	//{
//	//	for (int j{ 0 }; j < Object_T.size(); j++)
//	//	{
//	//		double SKO_x{ 0 };
//	//		double SKO_y{ 0 };
//	//		double IoU_{ 0 };
//	//		if (Object_P[i].comparison(Object_T[j],IoU_,SKO_x,SKO_y) == 1)
//	//		{
//	//			accuracyParams << frame << "\t" << SKO_x << "\t" << SKO_y << "\t" << IoU_ << std::endl;
//	//			detect_true++;
//	//			detect_false--;
//	//			IoU += IoU_;
//	//		}
//	//	}	
//	//}
//	//int not_det{ 0 };
//	//not_det = real - detect_true;
//	//if (not_det < 0)
//	//	not_det = 0;
//	//probability << frame << "\t" << real << "\t" << detect << "\t" << detect_true << "\t" << detect_false <<"\t"<<not_det<< std::endl;
//	//realAndPredictObj << std::endl;
//	//true_pos += detect_true;
//	//false_pos += detect_false;
//	//false_neg += not_det;
//	//Object_P.clear();
//	//Object_T.clear();
//	return 0;
//}

void Analis::comparison(std::string fileName)
{
	if (out_end.is_open() == false)
		out_end.open(fileName, std::ios::app);
	//double F11{ 0 }, F10{ 0 }, F01{ 0 };
	double P{ 0 }, R{ 0 };
	double IoU_sr{ 0 };
	//int N;
	//N = true_pos + false_neg + false_pos;
	//F11 = (double)true_pos / (double)N;
	//F10 = (double)false_pos / (double)N;
	//F01 = (double)false_neg / (double)N;
	P = (double)true_pos / (double)(true_pos + false_pos);
	R = (double)true_pos / (double)(true_pos + false_neg);
	IoU_sr = (double)IoU / (double)true_pos;
	//out_end << "F11\tF10\tF01\tP\tR\tIoU" << std::endl;
	//out_end << F11 << "\t" << F10 << "\t" <<F01 << "\t"<< P << "\t" << R  << "\t"<< IoU_sr << std::endl;
	out_end << "P\tR\tIoU" << std::endl;
	out_end << P << "\t" << R  << "\t"<< IoU_sr << std::endl;
	out_end.close();
	realAndPredictObj.close();
	probability.close();
	accuracyParams.close();
}

void Analis::setRealAndPredictObj(int const frame,const std::vector<int> *real_x, const std::vector<int> *real_y, const std::vector<int> *real_width, const std::vector<int> *real_height,
	const std::vector<int> *predict_x, const std::vector<int> *predict_y, const std::vector<int> *predict_width, const std::vector<int> *predict_heigth)
{
	std::vector<int> bufer_r_x(*real_x);
	std::vector<int> bufer_r_y(*real_y);
	std::vector<int> bufer_r_w(*real_width);
	std::vector<int> bufer_r_h(*real_height);
	std::vector<int> bufer_p_x(*predict_x);
	std::vector<int> bufer_p_y(*predict_y);
	std::vector<int> bufer_p_w(*predict_width);
	std::vector<int> bufer_p_h(*predict_heigth);
	realAndPredictObj << frame << "\t" << predict_y->size() << "\t";
	for (size_t i{ 0 }; i < real_x->size(); ++i)
	{
		realAndPredictObj << "T " << bufer_r_x[i] << "\t" << bufer_r_y[i] << "\t" << bufer_r_w[i] << "\t" << bufer_r_h[i] << "\t";
	}
	for (size_t i{ 0 }; i < predict_x->size(); ++i)
	{
		realAndPredictObj << "P " << bufer_p_x[i] << "\t" << bufer_p_y[i] << "\t" << bufer_p_w[i] << "\t" << bufer_p_h[i] << "\t";
	}
	realAndPredictObj << std::endl;
	short int detect_true{ 0 };
	short int not_det{ 0 };
	for (size_t i{ 0 }; i < bufer_r_x.size(); ++i)
	{
		cv::Rect realRect(bufer_r_x[i], bufer_r_y[i], bufer_r_w[i], bufer_r_h[i]);
		float realRectDiagonal(std::sqrt(std::pow(bufer_r_w[i], 2) + std::pow(bufer_r_h[i], 2)));
		bool f_falseNegativ{ true };
		for (size_t j{ 0 }; j < bufer_p_x.size(); ++j)
		{
			float predRectDiagonal(std::sqrt(std::pow(bufer_p_w[j], 2) + std::pow(bufer_p_h[j], 2)));
			cv::Point predictCenterPoint(bufer_p_x[j] + bufer_p_w[j] / 2, bufer_p_y[j] + bufer_p_h[j] / 2);
			if (realRect.contains(predictCenterPoint) )
				//&& realRectDiagonal / 3 < predRectDiagonal && realRectDiagonal * 3 > predRectDiagonal)
			{
				++detect_true;
				float IoU_{ static_cast<float>(realRect.area()) / (bufer_p_h[j]*bufer_p_w[j]) };
				//float SKO_x = abs(bufer_r_x[i] + bufer_r_w[i] / 2 - bufer_p_x[j] + bufer_p_w[j] / 2);
				//float SKO_y = abs(bufer_r_y[i] + bufer_r_h[i] / 2 - bufer_p_y[j] + bufer_p_h[j] / 2);
				//accuracyParams << frame << "\t" << SKO_x << "\t" << SKO_y << "\t" << IoU_ << std::endl;
				accuracyParams << frame << "\t" << IoU_ << std::endl;
				IoU += IoU_;
				bufer_p_x.erase(bufer_p_x.begin() + j);
				bufer_p_y.erase(bufer_p_y.begin() + j);
				bufer_p_w.erase(bufer_p_w.begin() + j);
				bufer_p_h.erase(bufer_p_h.begin() + j);
				f_falseNegativ = false;
				--j;
			}
		}
		bufer_r_x.erase(bufer_r_x.begin() + i);
		bufer_r_y.erase(bufer_r_y.begin() + i);
		bufer_r_w.erase(bufer_r_w.begin() + i);
		bufer_r_h.erase(bufer_r_h.begin() + i);
		--i;
		if (f_falseNegativ)
		{
			++not_det;
		}
	}
	probability << frame << "\t" << real_x->size() << "\t" << predict_x->size() << "\t" << detect_true << "\t" << bufer_p_x.size() << "\t" << not_det << std::endl;
	false_pos += bufer_p_x.size();
	true_pos += detect_true;
	false_neg += not_det;
}

void Analis::comparisonInAllFrams(std::string fileNameTrue, std::string fileNameOut, int const frameIn, cv::Mat *const InputImg)
{
	//cv::Scalar sumAllImg(cv::sum(*InputImg));
	//std::ifstream fileTrue{ fileNameTrue };
	//std::string true_rect{ 0 };
	//bool end_frame{ false };
	//double SNR{ 0 };
	//for (int i{ 1 }; end_frame == false; i++)
	//{
	//	std::getline(fileTrue, true_rect);
	//	std::istringstream iss{ true_rect };
	//	int frame{ 0 }, px_1{ 0 }, py_1{ 0 }, px_2{ 0 }, py_2{ 0 };
	//	iss >> frame >> px_1 >> py_1 >> px_2 >> py_2;
	//	if (frame == frameIn)
	//	{
	//		if (px_1 == px_2 && py_1 == py_2)
	//		{
	//		}
	//		else
	//		{
	//			cv::Rect obl{ cv::Point(px_1,py_1),cv::Point(px_2,py_2) };
	//			cv::Scalar sumSignal{ cv::sum((*InputImg)(obl)) };
	//			double signal{ sumSignal[0] / (obl.height * obl.width) };
	//			double background{ (sumAllImg[0] - sumSignal[0]) / (InputImg->cols * InputImg->rows - obl.height * obl.width) };
	//			SNR+= signal / background;
	//			
	//		}
	//	}
	//	if (frame > frameIn || fileTrue.eof())
	//	{
	//		if (Object_T.size() == 0)
	//			SNR = 0;
	//		else
	//		{
	//			SNR /= Object_T.size();
	//			SNR = round(SNR, 3);
	//		}
	//		end_frame = true;
	//	}
	//}
	//int detect_true{ 0 };
	//int detect_false{ 0 };
	//int real{ static_cast<int>(Object_T.size()) };
	//int detect{ static_cast<int>(Object_P.size()) };
	//double medium_IoU{ 0 };
	//double medium_SKO{ 0 };
	//detect_false = detect;
	//for (int i{ 0 }; i < Object_P.size(); i++)
	//{
	//	for (int j{ 0 }; j < Object_T.size(); j++)
	//	{
	//		double SKO_x{ 0 };
	//		double SKO_y{ 0 };
	//		double IoU_{ 0 };
	//		if (Object_P[i].comparison(Object_T[j], IoU_, SKO_x, SKO_y) == 1)
	//		{
	//			detect_true++;
	//			detect_false--;
	//			medium_IoU += IoU_;
	//			medium_SKO += SKO_x + SKO_y;
	//		}
	//	}
	//}
	//if (detect_true == 0)
	//{
	//	medium_IoU = 0;
	//	medium_SKO = 0;
	//}
	//else
	//{
	//	medium_IoU /= detect_true;
	//	medium_IoU = round(medium_IoU, 3);
	//	medium_SKO /= (detect_true * 2);
	//}
	//int not_det{ 0 };
	//not_det = real - detect_true;
	//if (not_det < 0)
	//	not_det = 0;
	//double P{ 0 }, R{ 0 };
	//int N{detect_true+detect_false+not_det};
	//double F11{ static_cast<double>(detect_true) / N };
	//double F10{ static_cast<double>(detect_false) / N };
	//double F01{ static_cast<double>(not_det) / N };
	//if (N == 0)
	//{
	//	F11 = 0;
	//	F10 = 0;
	//	F01 = 0;
	//}
	//if ((detect_true + detect_false) == 0)
	//	P = 0;
	//else
	//	P = static_cast<double>(detect_true) / (detect_true + detect_false);
	//if ((detect_true + not_det) == 0)
	//	R = 0;
	//else
	//R = static_cast<double>(detect_true) / (detect_true + not_det);
	//if (out_end.is_open() == false)
	//	out_end.open(fileNameOut, std::ios::app);
	////out_end << "F11\tF10\tF01\tP\tR\tIoU\tSKO" << endl;
	//out_end << frameIn <<"\t"<< SNR << "\t" << F11 << "\t" << F10 << "\t" << F01 << "\t" << P << "\t" << R << "\t" << medium_IoU << "\t" << medium_SKO << std::endl;
	////return 0;
}

//int WriteToFile::write_true(std::string windowName)
//{
//	char *chstr = new char[windowName.length() + 1];
//	strcpy(chstr, windowName.c_str());
//	std::cout << "vdelite real celi\t";
//	cv::setMouseCallback(chstr, myMouseCallback_1, (void*)&(*this));
//	cv::waitKey();
//	delete[] chstr;
//	return 0;
//}

double round(double InputNumber, int const accuracy)
{
	InputNumber *= pow(10, accuracy + 1);
	InputNumber = static_cast<int>(round(InputNumber));
	return static_cast<double>(InputNumber / pow(10, accuracy));
}