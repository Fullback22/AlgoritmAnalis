#pragma once

#include <QObject>
#include <qrect.h>
#include "QtRotateRect.h"

#include "qimage.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

class QtProcessedArea : public QObject
{
	Q_OBJECT
	int processedAreaType; ///0-BrightnesCorrect, 1-outlineArea, 2-color, 3-edgePixel, 4-positionAdjustment, 5-width, 6-diametr, 7-edge, 8-pitch, 9-HighSpeed,10-OCR
	int areaType;///0-rectangel,1-circle,2-enter
	QtRotateRect rect;
	QtRotateRect scalRect;
	bool activ;
	bool draw;
	QString systemName;
	QString userName;
	int doubleTreshF;
	int doubleTreshS;
	bool singlTresActiv;

public:
	QtProcessedArea(QObject *parent=Q_NULLPTR );
	QtProcessedArea(int processedType, int areaType, QtRotateRect newRect, QObject* parent = Q_NULLPTR);

	QtProcessedArea(const QtProcessedArea& drop);
	~QtProcessedArea();
	void createMaster(cv::Mat const *inputImg);
	void setRect(QtRotateRect* newRect);
	void setScalRect(QtRotateRect* newRect);

	void setAreaType(int newType);
	void setProcessedType(int newType);
	void setActiv(bool isActiv);
	void setDraw(bool isDraw);
	void setSystemName(QString newName);
	void setActivTres(bool isSingel);
	void setDoubelTreshF(int min);
	void setDoubelTreshS(int max);
	bool singelIsActivTresh();
	int getDoubelTresF();
	int getDoubelTresS();
	QString getSystemName();
	QString getProcesName();
	void setUserName(QString newName);
	QString getUserName();
	bool isActiv();
	bool isDraw();
	int getAreaType();
	int getProcesseedType();
	double getArea(bool scaled=false);
	QtRotateRect* getRect();
	QtRotateRect* getScalRect();

	QtProcessedArea& operator=(const QtProcessedArea& drop);
	QRect getOriginalLimitRect();
	QRect getScaledLimitRect();
};
