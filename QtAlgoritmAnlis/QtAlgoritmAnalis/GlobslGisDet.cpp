#include "GlobslGisDet.h"


GlobslGisDet::GlobslGisDet()
{
	coreMorfologis = Mat(0, 0, CV_8U);
	rectSize = Size(0, 0);
	thresh = 0;
	medianBlurSize = 0;
	gausianBlurSize = 0;
	morfologiSize = 0;
}

GlobslGisDet::GlobslGisDet(int thresh_ = 600, int rect = 25, Size core_ = Size(3, 3))
{
	thresh = thresh_;
	rectSize = Size(rect, rect);
	coreMorfologis = Mat(core_, CV_8U);
	medianBlurSize = 25;
	gausianBlurSize = 5;
}

GlobslGisDet::~GlobslGisDet()
{
}

void GlobslGisDet::work(Mat inputImg)
{
	Mat out;
	inputImg.copyTo(out);
	detectObj.clear();
	detectObj.push_back(Rect(0, 0, 0, 0));
	medianBlur(out, out, medianBlurSize);
	threshold(out, out, 200, 255, THRESH_OTSU); //�����������
	GaussianBlur(out, out, Size(gausianBlurSize, gausianBlurSize), 0);
	morphologyEx(out, out, 1, coreMorfologis);
	threshold(out, out, 200, 255, THRESH_OTSU);
	for (int i{ 0 }; inputImg.cols - rectSize.height > i && i + rectSize.height < inputImg.cols; i += rectSize.height / 2)//����� ��������� ����������� ����� ��������� rectSize � rectSize
	{
		for (int j{ 0 }; inputImg.rows - rectSize.width > j && j + rectSize.width < inputImg.rows; j += rectSize.width / 2)
		{
			Rect obl(i, j, rectSize.height, rectSize.width); //������� � ������� ��������� ����� �������
			if (countNonZero(out(obl)) > thresh) //��������� ���������� ����� �������� � �������
			{
				bool end{ false };
				for (int k{ 0 }; k<detectObj.size() && end == false; ++k) // ��������: �������� �� ����� ������� �������������� � ���������� �����
				{

					if (detectObj[k].y + detectObj[k].height  < obl.y || detectObj[k].y > obl.y + obl.height ||  //�������� �� ��������������. ���� �� ������������ � ������� ��������� �� ���������
						detectObj[k].x > obl.x + obl.width || detectObj[k].x + detectObj[k].width< obl.x)
					{
					}
					else //���� ������������, �� ���������� ����������� ��������
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
						detectObj.push_back(obl); //���� � ����� ������� �� ������������ �� � ����� ���������� ����� �������� � � ������
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
	for (int k{ 0 }; k<detectObj.size(); ++k) //��� ���� �������� �� �����������
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

void GlobslGisDet::draw(Mat &inOutputImg)
{
	for (int k{ 0 }; k<detectObj.size(); ++k)
		rectangle(inOutputImg, detectObj[k], 1);
}
int GlobslGisDet::getDetectedObj()
{
	return detectObj.size();
}
int GlobslGisDet::getObjX(int N)
{
	return detectObj[N].x;
}
int GlobslGisDet::getObjY(int N)
{
	return detectObj[N].y;
}
int GlobslGisDet::getObjHigth(int N)
{
	return detectObj[N].height;
}
int GlobslGisDet::getObjWigth(int N)
{
	return detectObj[N].width;
}

void GlobslGisDet::setParams(int thresh_in, int medianBlurSize_in, int gausianBlurSize_in, int morfologiSize_in, int sizeRectObj_in)//��������� ���������� ������������
{
	if (thresh_in<0)//�����
		thresh = 0;
	else
		thresh = thresh_in;

	if (medianBlurSize_in > 31)//��������� ������
		medianBlurSize = 31;
	else if (medianBlurSize_in <= 0)
		medianBlurSize = 1;
	else if (medianBlurSize_in % 2 == 0)
		medianBlurSize = ++medianBlurSize_in;
	else
		medianBlurSize = medianBlurSize_in;

	if (gausianBlurSize_in > 31)//������ ������
		gausianBlurSize = 31;
	else if (gausianBlurSize_in <= 0)
		gausianBlurSize = 1;
	else if (gausianBlurSize_in % 2 == 0)
		gausianBlurSize = ++gausianBlurSize_in;
	else
		gausianBlurSize = gausianBlurSize_in;

	if (morfologiSize_in > 31)//������ ��������������� ��������������
		morfologiSize = 31;
	else if (morfologiSize_in <= 0)
		morfologiSize = 1;
	else if (morfologiSize_in % 2 == 0)
		morfologiSize = ++morfologiSize_in;
	else
		coreMorfologis = Mat(Size(morfologiSize, morfologiSize), CV_8U);

	if (sizeRectObj_in <= 0)//������� ������� ������
		rectSize = Size(3, 3);
	else
		rectSize = Size(sizeRectObj_in, sizeRectObj_in);
}