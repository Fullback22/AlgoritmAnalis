#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "qpixmap.h"
#include "QtProcessedArea.h"

class ProcessedObj
{
protected:
	QString programName;
	QString fileName;
	QString dirName;
	//QPixmap mainImgPixmap;
	//cv::Mat mainImgMat; 
	//bool brightnesCorrectSet;
	//std::vector<QRect> rect_roi;
	std::vector<QtProcessedArea> processAreas;
	//std::vector<QPixmap> testImgPixmap;
	//std::vector<cv::Mat> testImgMat;
	bool imgIsNull;
public:
	ProcessedObj();
	ProcessedObj(QString inputFileName, QString inputDirName, cv::Mat inputMat, QPixmap inputPixmap);
	ProcessedObj(QString inputFileName, QString inputDirName, QString inputProgName, cv::Mat inputMat, QPixmap inputPixmap, bool imgIsAbsnd);
	ProcessedObj(const ProcessedObj &dep);
	~ProcessedObj();
	//cv::Mat getROI(QRect ROICoordinate);
	QString getFileName();
	void SetFileName(QString inputFileName);
	QString getDirName();
	void SetDirName(QString inputDirName);
	//QPixmap getPixmap();
	//QPixmap getTestPixmap(int number);
	//cv::Mat getTestMat(int number);
	//cv::Mat getMat();
	//void updateObj(QPixmap inputPixmap);
	void SetObjParams(QString inputFileName, QString inputDirName, cv::Mat inputMat, QPixmap inputPixmap, bool imgIsNull_);
	void setProgramName(QString newName);
	void deleteProcesArea(int areaNumber);
	/*void setBrightnesCorrectArea(QRect *area);
	void deletBrightnesCorrectArea();*/
	//void addTestImg(QString nameImg);
	//int getTestVecSize();
	std::vector<QtProcessedArea>* getProcesArears();
	/*bool brightnesCorrectAreaIsSet();*/
	/*QRect* getBrightnesCorrectArea();*/
	QString getProgramName();
	bool imgIsLoaded();
};

