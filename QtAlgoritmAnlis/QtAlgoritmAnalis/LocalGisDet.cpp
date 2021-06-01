#include "LocalGisDet.h"


LocalGisDet::LocalGisDet()
{
	coreMorfologis = cv::Mat(0, 0, CV_8U);
	rectSize = cv::Size(10,10);
	thresh=0;
	medianBlurSize = 3;
	firstThresholdSize = 3;
	secondThresholdSize = 3;
	gausianBlurSize = 3;
	morfologiSize = 1;
	firstThresholdType = 0;
	secondThresholdType = 1;
}

LocalGisDet::LocalGisDet( int thresh_=600, int rect=25, cv::Size core_=cv::Size(3,3))
{
	thresh = thresh_;
	rectSize = cv::Size(rect,rect);
	coreMorfologis = cv::Mat(core_, CV_8U);
	medianBlurSize = 21;
	firstThresholdSize = 3;
	secondThresholdSize = 3;
	gausianBlurSize = 5;
	firstThresholdType = 0;
	secondThresholdType =1;
}
LocalGisDet::~LocalGisDet()
{
}
void LocalGisDet::work(cv::Mat & inputImg)
{
	cv::Mat out;
	inputImg.copyTo(out);
	detectObj.clear();
	detectObj.push_back(cv::Rect(0, 0, 0, 0));
	if (medianBlurSize >= 3)
		medianBlur(out, out, medianBlurSize);
	if (firstThresholdSize >= 3)
		adaptiveThreshold(out, out, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, firstThresholdType, firstThresholdSize, 3); //бинаризаци€
	if (gausianBlurSize >= 3)
		GaussianBlur(out, out, cv::Size(gausianBlurSize, gausianBlurSize), 0);
	if (coreMorfologis.cols >= 3)
		morphologyEx(out, out, 1, coreMorfologis);
	if (secondThresholdSize >= 3)
		adaptiveThreshold(out, out, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, secondThresholdType, secondThresholdSize, 3);

	for (int i{ 0 }; inputImg.cols - rectSize.height > i && i + rectSize.height < inputImg.cols; i += rectSize.height/2)//обход выходного изображени€ окном размерами rectSize ’ rectSize
	{
		for (int j{ 0 }; inputImg.rows - rectSize.width > j && j + rectSize.width < inputImg.rows; j += rectSize.width/2)
		{
			cv::Rect obl(i, j, rectSize.height, rectSize.width); //участок в котором считаютс€ белые пиксели
			if (cv::countNonZero(out(obl)) > thresh) //провекрка количества белых пикселей с порогом
			{
				bool end{ false };
				for (int k{ 0 }; k<detectObj.size() && end == false; ++k) // проверка: €вл€етс€ ли нова€ область пересекающийс€ с выделеными ранее
				{

					if (detectObj[k].y + detectObj[k].height  < obl.y || detectObj[k].y > obl.y + obl.height ||  //проверка на пересекаемость. ≈сли не пересекаетс€ с текущей провер€ем со следующей
						detectObj[k].x > obl.x + obl.width || detectObj[k].x + detectObj[k].width< obl.x)
					{
					}
					else //если пересекаетс€, то происходит объединение областей
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
					if (k == detectObj.size() - 1)
					{
						detectObj.push_back(obl); //если у нова€ область не пересекаетс€ ни с одной выделенных ранее добавлем еЄ в вектор
						end = true;
					}
				}
			}
		}
	}
	if (detectObj.size() != 0)
	{
		if (detectObj[0].height == 0 && detectObj[0].height == 0)
			detectObj.erase(detectObj.begin());
	}
	for (int k{ 0 }; k<detectObj.size() ; ++k) //ещЄ одна проверка на объединение
	{
		for (int l{ k+1 }; l<detectObj.size(); ++l)
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
void LocalGisDet::draw(cv::Mat &inOutputImg)
{
	for (int k{ 0 }; k < detectObj.size(); ++k)
		rectangle(inOutputImg, detectObj[k], 255, 2);
}
void LocalGisDet::clean()
{
	detectObj.clear();
}
int LocalGisDet::getDetectedObj()
{
	return detectObj.size();
}
int LocalGisDet::getObjX(int N)
{
	return detectObj[N].x;
}
int LocalGisDet::getObjY(int N)
{
	return detectObj[N].y;
}
int LocalGisDet::getObjHigth(int N)
{
	return detectObj[N].height;
}
int LocalGisDet::getObjWigth(int N)
{
	return detectObj[N].width;
}
void LocalGisDet::setParams(int thresh_in, int medianBlurSize_in, int firstThresholdSize_in, int secondThresholdSize_in, int gausianBlurSize_in, int morfologiSize_in, int firstThresholdType_in, int secondThresholdType_in, int sizeRectObj_in)
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

	if (firstThresholdSize_in > 101)
		firstThresholdSize = 101;
	else if (firstThresholdSize_in < 1)
		firstThresholdSize = 3;
	else if (firstThresholdSize_in % 2 == 0)
		firstThresholdSize = ++firstThresholdSize_in;
	else
		firstThresholdSize = firstThresholdSize_in;

	if (secondThresholdSize_in > 45)
		secondThresholdSize = 45;
	else if (secondThresholdSize_in < 1)
		secondThresholdSize = 3;
	else if (secondThresholdSize_in % 2 == 0)
		secondThresholdSize = ++secondThresholdSize_in;
	else
		secondThresholdSize = secondThresholdSize_in;

	if (gausianBlurSize_in > 33)
		gausianBlurSize = 33;
	else if (gausianBlurSize_in < 1)
		gausianBlurSize = 3;
	else if (gausianBlurSize_in % 2 == 0)
		gausianBlurSize = ++gausianBlurSize_in;
	else
		gausianBlurSize = gausianBlurSize_in;

	if (morfologiSize_in > 33)
		morfologiSize = 33;
	else if (morfologiSize_in < 1)
		morfologiSize = 3;
	else if (morfologiSize_in % 2 == 0)
		morfologiSize = ++morfologiSize_in;
	else
		coreMorfologis=cv::Mat(cv::Size(morfologiSize, morfologiSize), CV_8U);
	
	if (firstThresholdType_in > 1 || firstThresholdType_in < 0)
		firstThresholdType = 1;
	else
		firstThresholdType = firstThresholdType_in;

	if (secondThresholdType_in > 1 || secondThresholdType_in < 0)
		secondThresholdType = 1;
	else
		secondThresholdType = secondThresholdType_in;
	
	if (sizeRectObj_in <= 2)
		rectSize = cv::Size(3, 3);
	else
		rectSize = cv::Size(sizeRectObj_in, sizeRectObj_in);
}

void LocalGisDet::getPredictCoordinate(std::vector<int>& pred_x, std::vector<int>& pred_y, std::vector<int>& pred_width, std::vector<int>& pred_heigth)
{
	for (size_t i{ 0 }; i < detectObj.size(); ++i)
	{
		pred_x.push_back(detectObj[i].x);
		pred_y.push_back(detectObj[i].y);
		pred_width.push_back(detectObj[i].width);
		pred_heigth.push_back(detectObj[i].height);
	}
}
