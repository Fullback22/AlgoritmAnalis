#pragma once
#include <iostream>

#include <qrect.h>
#include <cmath>
double const pi = 3.14159265359;
class QtRotateRect : public QRect
{
	double rotationAngle;
	QPoint drawPoint;
	QPoint translatePoint;
	double upLeftAngel_X;
	double upLeftAngel_Y;
	double upRigAngel_X;
	double upRigAngel_Y;
	double downLeftAngel_X;
	double downLeftAngel_Y;
	double downRigAngel_X;
	double downRigAngel_Y;
	
public:
	QtRotateRect(QRect parent = QRect(0, 0, 0, 0), double rotateAngl = 0.0);
	QtRotateRect(QRect parent, double rotateAngel, QPoint centerPoint);
	QtRotateRect(int upLeft_x, int upLeft_Y, int width, int heigth, double ratAngel);
	~QtRotateRect();
	void setRotateAngel(double newAngel, QPoint* center_start=nullptr);
	int setRotateAngel(double newAngel, QSize *limitSize, QPoint* center_start = nullptr);
	int resetAngel(QSize* limitSize);
	double getRotateAngel(bool inRad=false);
	void setDrowPoint(QPoint drPoint);
	QPoint getDrawPoint();
	void setTranslatePoint(QPoint transltPont);
	void translateTranslatePoint(int dx, int dy);
	QPoint getTranslatePoint();
	bool contains(QPoint point);
	bool contains(int x, int y);
	int getUpLeft_X();
	int getUpLeft_Y();
	int getDownLeft_X();
	int getDownLeft_Y();
	int getUpRigth_X();
	int getUpRigth_Y();
	int getDownRigth_X();
	int getDownRigth_Y();
	int getMax_X();
	int getMin_X();
	int getMax_Y();
	int getMin_Y();
	int getTopAngel();
	int getLowAngel();
	int getLeftAngel();
	int getRigthAngel();
	int setX(int newX, QSize* limitSize);
	void setX(int newX);
	int setY(int newY, QSize* limitSize);
	void setY(int newY);
	int setWidth(int newWidth, QSize* limitSize);
	void setWidth(int newWidth);
	int setHeight(int newHeigth, QSize* limitSize);
	void setHeight(int newHeigth);
	int getUpY(int x,int y=0);
	int getDownY(int x);
	int getUpX(int y,int x=0);
	int getDownX(int y);
	int getLeftX(int y);
	int getRigthX(int y);
	int getRigthY(int x);
	int getLeftY(int x);
	QPointF getEdgePoint(int typePoint);
	QRect getRotateRectSize();
	QPoint getUpLeftPoint();
};