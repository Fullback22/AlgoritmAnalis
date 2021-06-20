#include "TempDetect.h"


TempDetect::TempDetect()
	:
	gausianBlurSize{1}
{
	thresher = 0;
	rectSize = cv::Size(5,5);
	medianBlurSize=3;
	firstThresholdType=1;
	tempType = 5;
}
TempDetect::TempDetect(int thresher_, cv::Mat const &tempImg_, int RectSize):
	gausianBlurSize{1}
{
	thresher = thresher_;
	tempImg = tempImg_;
	rectSize = cv::Size(RectSize,RectSize);
	medianBlurSize = 5;
	firstThresholdType = 1;
	tempType = 5;
}

TempDetect::~TempDetect()
{
}

void TempDetect::work(cv::Mat inputImg)
{
	cv::Mat preprocessingImg;
	inputImg.copyTo(preprocessingImg);
	cv::Mat out_img;
	if (gausianBlurSize >= 3)
		cv::GaussianBlur(preprocessingImg, preprocessingImg, cv::Size(gausianBlurSize,gausianBlurSize),3);
	if (medianBlurSize >= 3)
		medianBlur(preprocessingImg, preprocessingImg, medianBlurSize);
	matchTemplate(preprocessingImg, tempImg, out_img, tempType);//построение корреляционной матрицы
	for (int idx = 0; idx < out_img.rows; idx++)
	{
		for (int idy = 0; idy < out_img.cols; idy++)//исключение отрицательных значений из out_img
		{
			if (out_img.at<float>(idx, idy) < 0)
				out_img.at<float>(idx, idy) = 0;
		}
	}
	cv::Scalar std, meanImg;
	meanStdDev(out_img, meanImg, std);
	double T{ meanImg[0] + 2.5 * std[0] };//расчёт порога
	if (T > 1)
		T = 2 - T;
	//if (firstThresholdType == 1)
	//	T = abs(1 - T);
	threshold(out_img, out_img, T, 1, firstThresholdType);//бинаризация полученного изображения
	detectObj.clear();
	if (cv::countNonZero(out_img) > 0)
	{
		detectObj.push_back(cv::Rect(0, 0, 0, 0));
		for (int i{ 0 }; out_img.cols - rectSize.height > i && i + rectSize.height < out_img.cols; i += rectSize.height / 2) //проход бинаризованного ихображения скользящим окном
		{
			for (int j{ 0 }; out_img.rows - rectSize.width > j&& j + rectSize.width < out_img.rows; j += rectSize.width / 2)
			{
				cv::Rect obl(i, j, rectSize.width, rectSize.height);
				if (countNonZero(out_img(obl)) > thresher) //сравнение количества белых пикселей в окне с порогом
				{
					bool end{ false };
					for (int k{ 0 }; k<detectObj.size() && end == false; ++k) //проверка нового окна на пересечение с имеющимися
					{
						if (detectObj[k].y + detectObj[k].width  < obl.y || detectObj[k].y > obl.y + obl.width || //если выполняется условие, то пересечения нет
							detectObj[k].x > obl.x + obl.height || detectObj[k].x + detectObj[k].height< obl.x)
						{
						}
						else //если пересечение есть объединяем новое окно и старые
						{
							if (detectObj[k].x>obl.x)
								detectObj[k].x = obl.x;
							if (detectObj[k].y > obl.y)
								detectObj[k].y = obl.y;
							if (detectObj[k].x + detectObj[k].width < obl.x + obl.width)
								detectObj[k].width = obl.x + obl.width - detectObj[k].x;
							if (detectObj[k].y + detectObj[k].height < obl.y + obl.height)
								detectObj[k].height = obl.y + obl.height - detectObj[k].y;
							k = detectObj.size();
						}
						if (k == detectObj.size() - 1) //если пересечение нового окна со старыми нет, то добавляем новое окно в вектор
						{
							detectObj.push_back(obl);
							end = true;
						}
					}
				}
			}
		}
		if (detectObj.size() != 0)
		{
			if (detectObj[0].width == 0 && detectObj[0].height == 0)
				detectObj.erase(detectObj.begin());
		}
		for (int k{ 0 }; k<detectObj.size(); ++k) //ещё одна проверка на объединение
		{
			for (int l{ k + 1 }; l<detectObj.size(); ++l)
			{
				if (detectObj[k].y + detectObj[k].height  < detectObj[l].y || detectObj[k].y >  detectObj[l].y + detectObj[l].height ||
					detectObj[k].x >  detectObj[l].x + detectObj[l].width || detectObj[k].x + detectObj[k].width<  detectObj[l].x)
				{
				}
				else
				{
					if (detectObj[k].x> detectObj[l].x)
						detectObj[k].x = detectObj[l].x;
					if (detectObj[k].y > detectObj[l].y)
						detectObj[k].y = detectObj[l].y;
					if (detectObj[k].x + detectObj[k].width < detectObj[l].x + detectObj[l].width)
						detectObj[k].width = detectObj[l].x + detectObj[l].width - detectObj[k].x;
					if (detectObj[k].y + detectObj[k].height < detectObj[l].y + detectObj[l].height)
						detectObj[k].height = detectObj[l].y + detectObj[l].height - detectObj[k].y;
					detectObj.erase(detectObj.begin() + l);
					l--;
				}
			}
		}
	}
	std::vector<cv::Point> center;
	for (int k{ 0 }; k < detectObj.size(); ++k)
	{
		center.push_back(cv::Point((detectObj[k].width / 2 + detectObj[k].x) + tempImg.cols / 2 - 1, (detectObj[k].height / 2 + detectObj[k].y) + tempImg.rows / 2 - 1));//вычисление центра обнаруженной области в системе координат исходного изображения
	}
	drawObj.clear();
	for (int k{ 0 }; k < detectObj.size(); ++k)
	{
		drawObj.push_back(cv::Rect(cv::Point(center[k].x - tempImg.cols / 2, center[k].y - tempImg.rows / 2), cv::Point(center[k].x + tempImg.cols / 2, center[k].y + tempImg.rows / 2))); //вычисление координат прямоугольников выделяющих обнаруженные цели
	}
}
void TempDetect::draw(cv::Mat &inOutputImg)
{
	for (int k{ 0 }; k < drawObj.size(); ++k)
		rectangle(inOutputImg, drawObj[k], 255, 2);
}
int TempDetect::getDetectedObj()
{
	return drawObj.size();
}
int TempDetect::getObjX(int N)
{
	return drawObj[N].x;
}
int TempDetect::getObjY(int N)
{
	return drawObj[N].y;
}
int TempDetect::getObjHigth(int N)
{
	return drawObj[N].height;
}
int TempDetect::getObjWigth(int N)
{
	return drawObj[N].width;
}
void TempDetect::setParams(int medianBlurSize_, int gausianBlurSize_, int firstThresholdType_, int rectSize_, int TempType_, int thres_)
{
	if (medianBlurSize_ > 33)
		medianBlurSize_ = 33;
	else if (medianBlurSize_ < 1)
		medianBlurSize_ = 3;
	else if (medianBlurSize_ % 2 == 0)
		++medianBlurSize_;
	medianBlurSize = medianBlurSize_;

	if (gausianBlurSize_ > 33)
		gausianBlurSize_ = 33;
	else if (gausianBlurSize_ < 1)
		gausianBlurSize_ = 3;
	else if (gausianBlurSize_ % 2 == 0)
		++gausianBlurSize_;
	gausianBlurSize = gausianBlurSize_;

	if (firstThresholdType_ > 1 || firstThresholdType_ < 0)
		firstThresholdType = 1;
	else
		firstThresholdType = firstThresholdType_;

	if (rectSize_ < 0)
		rectSize = cv::Size(3, 3);
	else
		rectSize = cv::Size(rectSize_, rectSize_);
	
	if (TempType_>5 || TempType_ < 0)
		tempType = 5;
	else
		tempType = TempType_;

	if (thres_ < 0)
		thresher = 1;
	else
		thresher = thres_;
}
void TempDetect::setTempImg(cv::Mat &tempImg_)
{
	tempImg = tempImg_;
}

void TempDetect::getPredictCoordinate(std::vector<int>& pred_x, std::vector<int>& pred_y, std::vector<int>& pred_width, std::vector<int>& pred_heigth)
{
	for (size_t i{ 0 }; i < drawObj.size(); ++i)
	{
		pred_x.push_back(drawObj[i].x);
		pred_y.push_back(drawObj[i].y);
		pred_width.push_back(drawObj[i].width);
		pred_heigth.push_back(drawObj[i].height);
	}
}
