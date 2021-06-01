#include "ContourDet.h"


ContourDet::ContourDet()
{
	thresh = 0;
	medianBlurSize = 3;
	gausianBlurSize = 3;
}

ContourDet::~ContourDet()
{
}
void ContourDet::work(Mat &inputImg)
{
	Mat out;
	inputImg.copyTo(out);
	detectObj.clear();
	detectObj.push_back(Rect(0, 0, 0, 0));
	medianBlur(out, out, medianBlurSize);
	GaussianBlur(out, out, Size(gausianBlurSize, gausianBlurSize), 0);
	Canny(out, out, 100, 200); //выделение контуров
	vector<vector<Point>> contours;// вектор для хранения контуров
	findContours(out, contours, 0, 1); //поиск контуров
	for (int i{ 0 }; i<contours.size(); i++)
	{
		if (matchShapes(searchObj[0], contours[i], CV_CONTOURS_MATCH_I1, 0) < thresh) //сравнение моментов эталонного контура и обнаруженных и сравненеи полученного 
			detectObj.push_back(boundingRect(contours[i]));					  //значения с парогом. Добавление в обнаруженные объекты	
	}

	if (detectObj.size() != 0)//удаление первого пустого элемента в detectObj
	{
		if (detectObj[0].height == 0 && detectObj[0].width == 0)
			detectObj.erase(detectObj.begin());
	}
	for (int i{ 0 }; i < 2; i++)
	{
		for (int k{ 0 }; k < detectObj.size(); ++k) //проверка на объединение выполняется 2 раза
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
}

void ContourDet::setObj(Mat inputImg)//выделение эталонного контура
{
	Mat out;
	Canny(inputImg, out, 100, 250);
	findContours(out, searchObj, 0, 1);
}

void ContourDet::draw(Mat &inOutputImg)
{
	for (int k{ 0 }; k<detectObj.size(); ++k)
		rectangle(inOutputImg, detectObj[k], 1);
}

int ContourDet::getDetectedObj()
{
	return detectObj.size();
}

int ContourDet::getObjX(int N)
{
	return detectObj[N].x;
}

int ContourDet::getObjY(int N)
{
	return detectObj[N].y;
}

int ContourDet::getObjHigth(int N)
{
	return detectObj[N].height;
}

int ContourDet::getObjWigth(int N)
{
	return detectObj[N].width;
}

void ContourDet::setParams(int thresh_in, int medianBlurSize_in, int gausianBlurSize_in)
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
		medianBlurSize = ++medianBlurSize_in;
	else
		medianBlurSize = medianBlurSize_in;

	if (gausianBlurSize_in > 31)
		gausianBlurSize = 31;
	else if (gausianBlurSize_in <= 0)
		gausianBlurSize = 1;
	else if (gausianBlurSize_in % 2 == 0)
		gausianBlurSize = ++gausianBlurSize_in;
	else
		gausianBlurSize = gausianBlurSize_in;

}
