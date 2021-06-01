#include "SubtractBacground.h"


SubtractBacground::SubtractBacground()
{
	coreMorfologis = cv::Mat(3, 3, CV_8U);
	rectSize=cv::Size(3,3);
	thresh=0;
	medianBlurSize = 3;
	thresholdSize = 3;
	morfologiSize = 3;
	thresholdType = 1;
	learnSpeed = 0;
}

SubtractBacground::SubtractBacground(int thresh_, int rect, cv::Size coreSize)
{
	coreMorfologis = cv::Mat(coreSize, CV_8U);
	rectSize = cv::Size(rect, rect);
	thresh = thresh_;
	medianBlurSize = 5;
	thresholdSize = 3;
	thresholdType = 0;
	learnSpeed = 0;
}

SubtractBacground::~SubtractBacground()
{
}

void SubtractBacground::work(cv::Mat& inputImg)
{
	cv::Mat out;
	inputImg.copyTo(out);
	if (medianBlurSize >= 3)
		medianBlur(out, out, medianBlurSize);  //медианный фильтр
	if (coreMorfologis.cols >= 3)
		morphologyEx(out, out, 1, coreMorfologis);//морфологические преобразования
	pBackSub->apply(inputImg, out, learnSpeed);//непосредственно вычитание фона и его обнавление
	adaptiveThreshold(out, out, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, thresholdType, thresholdSize, 3);//бинаризация
	detectObj.clear();
	detectObj.push_back(cv::Rect(0, 0, 0, 0));
	for (int i{ 0 }; inputImg.cols - rectSize.height > i && i + rectSize.height < inputImg.cols; i += rectSize.height / 2) //проход бинаризованного ихображения скользящим окном
	{
		for (int j{ 0 }; inputImg.rows - rectSize.width > j && j + rectSize.width < inputImg.rows; j += rectSize.width / 2)
		{
			cv::Rect obl(i, j, rectSize.width, rectSize.height); 
			if (cv::countNonZero(out(obl)) > thresh) //сравнение количества белых пикселей в окне с порогом
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
	if (detectObj.size()!=0)
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

void SubtractBacground::draw(cv::Mat &inOutputImg)
{
	for (int k{ 0 }; k < detectObj.size(); ++k)
		rectangle(inOutputImg, detectObj[k], 255, 2);
}
void SubtractBacground::clean()
{
	detectObj.clear();
}
int SubtractBacground::getDetectedObj()
{
	return detectObj.size();
}

int SubtractBacground::getObjX(int N)
{
	return detectObj[N].x;
}

int SubtractBacground::getObjY(int N)
{
	return detectObj[N].y;
}

int SubtractBacground::getObjHigth(int N)
{
	return detectObj[N].height;
}

int SubtractBacground::getObjWigth(int N)
{
	return detectObj[N].width;

}
void SubtractBacground::setParams(int thresh_in, int medianBlurSize_in, int ThresholdSize_in, int morfologiSize_in, int ThresholdType_in, double learnSpeed_in,int sizeRectObj)
{
	thresh = thresh_in;

	if (medianBlurSize_in > 33)
		medianBlurSize = 33;
	else if (medianBlurSize_in < 1)
		medianBlurSize = 3;
	else if (medianBlurSize_in % 2 == 0)
		medianBlurSize = ++medianBlurSize_in;
	else
		medianBlurSize = medianBlurSize_in;

	if (ThresholdSize_in > 101)
		thresholdSize = 101;
	else if (ThresholdSize_in <= 1)
		thresholdSize = 3;
	else if (ThresholdSize_in % 2 == 0)
		thresholdSize = ++ThresholdSize_in;
	else 

	if (learnSpeed_in > 1 || learnSpeed_in<-1)
		learnSpeed = 0;
	else
		learnSpeed = learnSpeed_in;

	if (morfologiSize_in > 33)
		morfologiSize = 33;
	else if (morfologiSize_in < 1)
		morfologiSize = 3;
	else if (morfologiSize_in % 2 == 0)
		morfologiSize = ++morfologiSize_in;
	else
		coreMorfologis = cv::Mat(cv::Size(morfologiSize, morfologiSize), CV_8U);

	if (ThresholdType_in > 1 || ThresholdType_in < 0)
		thresholdType = 0;
	else
		thresholdType = ThresholdType_in;
	
	if (sizeRectObj <= 0)
		rectSize = cv::Size(3, 3);
	else
		rectSize = cv::Size(sizeRectObj, sizeRectObj);
}

void SubtractBacground::getPredictCoordinate(std::vector<int>& pred_x, std::vector<int>& pred_y, std::vector<int>& pred_width, std::vector<int>& pred_heigth)
{
	for (size_t i{ 0 }; i < detectObj.size(); ++i)
	{
		pred_x.push_back(detectObj[i].x);
		pred_y.push_back(detectObj[i].y);
		pred_width.push_back(detectObj[i].width);
		pred_heigth.push_back(detectObj[i].height);
	}
}
