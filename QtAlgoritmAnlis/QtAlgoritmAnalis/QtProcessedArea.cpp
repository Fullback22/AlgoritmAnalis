#include "QtProcessedArea.h"

QtProcessedArea::QtProcessedArea(QObject* parent)
	: QObject(parent),
	processedAreaType(0),
	areaType(0),
	rect(QtRotateRect{}),
	scalRect(QtRotateRect{}),
	activ(false),
	draw(false),
	systemName(QString{}),
	userName(QString{}),
	doubleTreshF(0),
	doubleTreshS(0),
	singlTresActiv(false)
{
	//std::cout << qw << std::endl;
}

QtProcessedArea::QtProcessedArea(int processedType, int areaType_, QtRotateRect newRect, QObject* parent)
	: QObject(parent),
	processedAreaType(processedType),
	areaType(areaType_),
	rect(newRect),
	scalRect(newRect),
	activ(true),
	draw(true),
	systemName(QString{}),
	userName(QString{}),
	doubleTreshF(0),
	doubleTreshS(0),
	singlTresActiv(false)
{
	if (processedType == 1)
	{
		/*counterProc = new CountoursProcessing();
		qw = true;
		std::cout << qw << std::endl;*/
	}
}

QtProcessedArea::QtProcessedArea(const QtProcessedArea& drop)
	:processedAreaType(drop.processedAreaType),
	areaType(drop.areaType),
	rect(drop.rect),
	scalRect(drop.scalRect),
	activ(drop.activ),
	draw(drop.draw),
	systemName(drop.systemName),
	userName(drop.userName),
	doubleTreshF(drop.doubleTreshF),
	doubleTreshS(drop.doubleTreshS),
	singlTresActiv(drop.singlTresActiv)
{
	//std::cout << qw << std::endl;
}

QtProcessedArea::~QtProcessedArea()
{
}

void QtProcessedArea::createMaster(cv::Mat const* inputImg)
{
}

void QtProcessedArea::setRect(QtRotateRect* newRect)
{
	rect = *newRect;
}

void QtProcessedArea::setScalRect(QtRotateRect* newRect)
{
	scalRect = *newRect;
}


void QtProcessedArea::setAreaType(int newType)
{
	areaType = newType;
}

void QtProcessedArea::setProcessedType(int newType)
{
	processedAreaType = newType;
}

void QtProcessedArea::setActiv(bool isActiv)
{
	activ = isActiv;
}

void QtProcessedArea::setDraw(bool isDraw)
{
	draw = isDraw;
}

void QtProcessedArea::setSystemName(QString newName)
{
	systemName = newName;
}

void QtProcessedArea::setActivTres(bool isSingel)
{
	singlTresActiv = isSingel;
}

void QtProcessedArea::setDoubelTreshF(int min)
{	
	doubleTreshF = min;
}

void QtProcessedArea::setDoubelTreshS(int max)
{

	doubleTreshS = max;
}

bool QtProcessedArea::singelIsActivTresh()
{
	return singlTresActiv;
}

int QtProcessedArea::getDoubelTresF()
{
	return doubleTreshF;
}

int QtProcessedArea::getDoubelTresS()
{
	return doubleTreshS;
}

QString QtProcessedArea::getSystemName()
{
	return systemName;
}

QString QtProcessedArea::getProcesName()
{
	if (processedAreaType == 1)
		return "Outline";
	else if (processedAreaType == 2)
		return "Color";
	else if (processedAreaType == 3)
		return "Edge Pixel";
	else if (processedAreaType == 4)
		return "Position Adjustment";
	else if (processedAreaType == 5)
		return "Width";
	else if (processedAreaType == 6)
		return "Diametr";
	else if (processedAreaType == 7)
		return "Edge";
	else if (processedAreaType == 8)
		return "Pitch";
	else if (processedAreaType == 9)
		return "High-Speed";
	else if (processedAreaType == 10)
		return "OCR";
	else
		return "";
}

void QtProcessedArea::setUserName(QString newName)
{
	userName = newName;
}

QString QtProcessedArea::getUserName()
{
	return userName;
}

bool QtProcessedArea::isActiv()
{
	return activ;
}

bool QtProcessedArea::isDraw()
{
	return draw;
}

int QtProcessedArea::getAreaType()
{
	return areaType;
}

int QtProcessedArea::getProcesseedType()
{
	return processedAreaType;
}

QtRotateRect* QtProcessedArea::getRect()
{
	return &rect;
}

QtRotateRect* QtProcessedArea::getScalRect()
{
	return &scalRect;
}

QtProcessedArea& QtProcessedArea::operator=(const QtProcessedArea& drop)
{
	processedAreaType = drop.processedAreaType;
	areaType = drop.areaType;
	rect = drop.rect;
	scalRect = drop.scalRect;
	activ = drop.activ;
	draw = drop.draw;
	systemName = drop.systemName;
	userName = drop.userName;
	doubleTreshF = drop.doubleTreshF;
	doubleTreshS = drop.doubleTreshS;
	singlTresActiv = drop.singlTresActiv;
	std::cout << "=" << std::endl;
	return *this;
}

QRect QtProcessedArea::getOriginalLimitRect()
{
	return QRect(rect.getMin_X(), rect.getMin_Y(), rect.getMax_X() - rect.getMin_X(), rect.getMax_Y() - rect.getMin_Y());
}

QRect QtProcessedArea::getScaledLimitRect()
{
	return QRect(scalRect.getMin_X(), scalRect.getMin_Y(), scalRect.getMax_X() - scalRect.getMin_X(), scalRect.getMax_Y() - scalRect.getMin_Y());
}

double QtProcessedArea::getArea(bool scaled)
{
	if (scaled)
	{
		return scalRect.height() * scalRect.width();
	}
	else
	{
		return rect.height() * rect.width();
	}
}