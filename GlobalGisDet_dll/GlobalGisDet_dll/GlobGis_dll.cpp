#include "stdafx.h"
#include "GlobGis_dll.h"

int work(cv::Mat inputImg)
{
	cv::Mat out;
	inputImg.copyTo(out);
	detectObj.clear();
	detectObj.push_back(cv::Rect(0, 0, 0, 0));
	medianBlur(out, out, medianBlurSize);
	threshold(out, out, 200, 255, cv::THRESH_OTSU); //бинаризаци€
	GaussianBlur(out, out, cv::Size(gausianBlurSize, gausianBlurSize), 0);
	morphologyEx(out, out, 1, coreMorfologis);
	threshold(out, out, 200, 255, cv::THRESH_OTSU);
	for (int i{ 0 }; inputImg.cols - rectSize.height > i && i + rectSize.height < inputImg.cols; i += rectSize.height / 2)//обход выходного изображени€ окном размерами rectSize ’ rectSize
	{
		for (int j{ 0 }; inputImg.rows - rectSize.width > j && j + rectSize.width < inputImg.rows; j += rectSize.width / 2)
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
	for (int k{ 0 }; k<detectObj.size(); ++k) //ещЄ одна проверка на объединение
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
	return 0;
}

int draw(cv::Mat &inOutputImg)
{
	for (int k{ 0 }; k<detectObj.size(); ++k)
		rectangle(inOutputImg, detectObj[k], 1);
	return 0;
}
int getDetectedObj()
{
	return detectObj.size();
}

int getObjX(int N)
{
	return detectObj[N].x;
}

int getPredictCoordinate(std::vector<int>& x, std::vector<int>& y, std::vector<int>& width, std::vector<int>& height)
{
	for (size_t i{ 0 }; i < detectObj.size(); ++i)
	{
		x.push_back(detectObj[i].x);
		y.push_back(detectObj[i].y);
		width.push_back(detectObj[i].width);
		height.push_back(detectObj[i].height);
	}
	return 0;
}

int getObjY(int N)
{
	return detectObj[N].y;
}

int getObjHigth(int N)
{
	return detectObj[N].height;
}

int getObjWigth(int N)
{
	return detectObj[N].width;
}

void setParams(int thresh_in, int medianBlurSize_in, int gausianBlurSize_in, int morfologiSize_in, int sizeRectObj_in)
{
	if (thresh_in<0)
		thresh = 0;
	else
		thresh = thresh_in;

	if (medianBlurSize_in > 31)
		medianBlurSize = 31;
	else if (medianBlurSize_in <= 0)
		medianBlurSize = 1;
	else if (medianBlurSize_in % 2 == 0)
		medianBlurSize = medianBlurSize_in++;
	else
		medianBlurSize = medianBlurSize_in;

	if (gausianBlurSize_in > 31)
		gausianBlurSize = 31;
	else if (gausianBlurSize_in <= 0)
		gausianBlurSize = 1;
	else if (gausianBlurSize_in % 2 == 0)
		gausianBlurSize = gausianBlurSize_in++;
	else
		gausianBlurSize = gausianBlurSize_in;

	if (morfologiSize_in > 31)
		morfologiSize = 31;
	else if (morfologiSize_in <= 0)
		morfologiSize = 1;
	else if (morfologiSize_in % 2 == 0)
		morfologiSize = morfologiSize_in++;
	else
		coreMorfologis = cv::Mat(cv::Size(morfologiSize, morfologiSize), CV_8U);

	if (sizeRectObj_in <= 2)
		rectSize = cv::Size(3, 3);
	else
		rectSize = cv::Size(sizeRectObj_in, sizeRectObj_in);
}

int setParams(std::string fileName)
{
	json jsonConf;

	std::ifstream paramFile(fileName);
	if (!paramFile.is_open()) {

	}
	try {
		jsonConf = json::parse(paramFile);
	}
	catch (...) {
		paramFile.close();
	}//try...
	try {
		int buferThresh{ 0 };
		int buferMedianBlur{ 0 };
		int buferGausianBlur{ 0 };
		int buferMorfologi{ 0 };
		int buferSizeRectObj{ 0 };
		json channelJson = jsonConf.at("ALGORITM_PARAMS");
		buferThresh = channelJson.at("thresh").get<int>();
		buferMedianBlur = channelJson.at("medianBlurSize").get<int>();
		buferGausianBlur = channelJson.at("gausianBlurSize").get<int>();
		buferMorfologi = channelJson.at("morfologiSize").get<int>();
		buferSizeRectObj = channelJson.at("sizeRectObj").get<int>();
		setParams(buferThresh, buferMedianBlur, buferGausianBlur, buferMorfologi, buferSizeRectObj);
	}
	catch (...) {
		paramFile.close();
	}//try...
	paramFile.close();
	return 0;
}

int activParams(bool &thresh_enbl, bool &medianFilter_enbl, bool &bin_1_enbl, bool &gausFilter_enbl,
	bool &marfolFilter, bool &rectSize_enl, int &thresh_size, bool &tempImage_in)
{
	thresh_enbl = true;
	medianFilter_enbl = true;
	bin_1_enbl = false;
	gausFilter_enbl = true;
	marfolFilter = true;
	rectSize_enl = true;
	thresh_size = -1;
	tempImage_in = false;
	return 0;
}