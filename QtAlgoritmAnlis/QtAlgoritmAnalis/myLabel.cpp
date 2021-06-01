#include "myLabel.h"


myLabel::myLabel(QWidget *parent) :QLabel(parent)
{
	this->setStyleSheet("background-color: red");
	myPixmap_bufer = nullptr;
	myPixmap_mouve = nullptr;
	add = false;
	up_or_left = false;
	down_or_right = false;
	imgIsShow = false;
	imgScaled = 100;
	_scaled = new double[11]{0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 2, 3, 5};
	activ_scaled = 4;
	drPoint = QPoint(0, 0);
	original_drPoint == QPoint(0, 0);
	imageFormat = 0;
	this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

myLabel::~myLabel()
{
}

void myLabel::toImgCoordinate(int &inOutX, int &inOutY, bool isContains)
{
	if (scaledSize.width() > this->width())
	{
		inOutX += drPoint.x();
	}
	else
	{
		inOutX -= (this->width() - scaledSize.width()) / 2;
	}
	if (inOutX >= scaledSize.width() && isContains)
		inOutX = scaledSize.width()-1;
	else if (inOutX <= 0 && isContains)
		inOutX = 0;

	if (scaledSize.height() > this->height())
	{
		inOutY += drPoint.y();
	}
	else
	{
		inOutY -= (this->height() - scaledSize.height()) / 2;
	}

	if (inOutY > scaledSize.height()&& isContains)
		inOutY = scaledSize.height()-1;
	else if (inOutY < 0 && isContains)
		inOutY = 0;
}

void myLabel::formatImage(int formatType)
{
	if (imageFormat == formatType)
	{

	}
	else
	{
		if (formatType == 0)
		{
			my_Pixmap = my_PixmapOriginal;
			my_Pixmap = my_Pixmap.scaled(scaledSize);
			delete myPixmap_bufer;
			myPixmap_bufer = new QPixmap(my_Pixmap);
		}
		else if (formatType == 1)
		{
			QImage buferImg(myPixmap_bufer->toImage());
			buferImg = buferImg.convertToFormat(QImage::Format_Grayscale8);
			delete myPixmap_bufer;
			myPixmap_bufer = new QPixmap(QPixmap::fromImage(buferImg));
			buferImg = my_Pixmap.toImage();
			buferImg = buferImg.convertToFormat(QImage::Format_Grayscale8);
			my_Pixmap = QPixmap::fromImage(buferImg);
		}
		imageFormat = formatType;
	}
}

void myLabel::update_myPixmap(const QPixmap& img)
{
	my_Pixmap = img;
	my_PixmapOriginal = img;
	originalSize = my_PixmapOriginal.size();
	scaledSize = originalSize;
	delete myPixmap_bufer;
	myPixmap_bufer = new QPixmap(my_Pixmap);
	delete myPixmap_mouve;
	myPixmap_mouve = new QPixmap(my_Pixmap.copy(drPoint.x(), drPoint.y(), this->width(), this->height()));
	setAllImgScaled();
	this->scaledPixmap();
}

QPixmap* myLabel::getBuferPixmap()
{
	if (myPixmap_bufer != nullptr)
		return myPixmap_bufer;
	else
		return &QPixmap(800, 600);
}

void myLabel::set_myPixmap(const QPixmap& img)
{
	my_Pixmap = img;
	my_PixmapOriginal = img;
	originalSize = my_PixmapOriginal.size();
	scaledSize = originalSize;
	delete myPixmap_bufer;
	myPixmap_bufer = new QPixmap(my_Pixmap);
	this->setPixmap(*myPixmap_bufer);
	activ_scaled = 4;
	this->updateGeometry();
	setAllImgScaled();
	/*double scal_NormImg{ (double)this->width() / (double)originalSize.width() };
	_scaled[11] = scal_NormImg;
	for (int i{ 0 }; i < 9; i++)
	{
		if (_scaled[i]>scal_NormImg)
		{
			max_activ_scaled = i;
			i = 10;
		}
		else
			max_activ_scaled = 10;
	}
	for (int i{ 9 }; i >= 0; i--)
	{
		if (_scaled[i] < scal_NormImg)
		{
			min_activ_scaled = i;
			i = -1;
		}
		else
			min_activ_scaled = 0;
	}*/
}

QPixmap myLabel::myPixmap() const
{
	return my_Pixmap;
}

void myLabel::mouseMoveEvent(QMouseEvent *evnt)
{
	if (evnt->x() < 0)
	{
		x_labl = 0;
		emit mouseLeft();
	}
	else if (evnt->x() > this->width())
	{
		x_labl = this->width() - 1;
		emit mouseLeft();
	}
	else
	{
		x_labl = evnt->x();
	}

	if (evnt->y() < 0)
	{
		y_labl = 0;
		emit mouseLeft();
	}
	else if (evnt->y() > this->height())
	{
		y_labl = this->height() - 1;
		emit mouseLeft();
	}
	else
	{
		y_labl = evnt->y();
	}
	//x_pixMap = x_labl + drPoint.x();
	//y_pixMap = y_labl + drPoint.y();
	x_lablG = evnt->x();
	y_lablG = evnt->y();
	emit mousePos();
}

void myLabel::mousePressEvent(QMouseEvent *evnt)
{
	first_x_labl = evnt->x();
	first_y_labl = evnt->y();
	f_x_pixMap = first_x_labl;
	f_y_pixMap = first_y_labl;
	toImgCoordinate(f_x_pixMap, f_y_pixMap);
	/*std::cout << "X " << f_x_pixMap << std::endl;
	std::cout << "Y " << f_y_pixMap << std::endl;*/
	//f_x_pixMap = f_x_labl + drPoint.x();
	//f_y_pixMap = f_y_labl + drPoint.y();
	ref_x = evnt->x();
	ref_y = evnt->y();
	add = true;
	emit mousePressed();
}

void myLabel::leaveEvent(QEvent *evnt)
{
	emit mouseLeft();
}

void myLabel::mouseReleaseEvent(QMouseEvent *evnt)
{
	s_x_pixMap = evnt->x();
	s_y_pixMap = evnt->y()-this->y();
	add = false;
	up_or_left = false;
	down_or_right = false;
	emit mouseRelease();
}

void myLabel::reset_x_y()
{
	f_x_labl = 0;
	f_y_labl = 0;
	s_x_labl = 0;
	s_y_labl = 0;
}

void myLabel::add_rect(QRect &Input, QPen newPen)
{
	if (add == true)
	{
		toImgCoordinate(x_labl, y_labl);
		Input.setRect(f_x_pixMap, f_y_pixMap, x_labl - f_x_pixMap, y_labl- f_y_pixMap);
		this->draw_rect(&QtRotateRect(Input), newPen);
		this->show_partImg(drPoint.x(), drPoint.y(), this->width(), this->height());
	}
}

void myLabel::show_roi(std::vector <QRect> &Input)
{
	delete myPixmap_bufer;
	myPixmap_bufer = new QPixmap(my_Pixmap);
	QPainter *p = new QPainter(myPixmap_bufer);
	p->setPen(QPen(Qt::red, 1, Qt::DashLine));
	for (int i{ 0 }; i < Input.size(); i++)
		p->drawRect(Input[i]);
	this->setPixmap(*myPixmap_bufer);
	delete p;
}

void myLabel::draw_rect(QtRotateRect* Input, QPen newPen)
{
	if (imgIsShow)
	{
		delete myPixmap_bufer;
		myPixmap_bufer = new QPixmap(my_Pixmap);
		imgIsShow = false;
	}
	else
	{

	}
	QPainter* p = new QPainter(myPixmap_bufer);
	p->setPen(newPen);
	p->drawRect(Input->x(), Input -> y(),Input->width(),Input->height());
	/*p->drawLine(QPoint(Input->getDownLeft_X(), Input->getDownLeft_Y()), QPoint(Input->getDownRigth_X(), Input->getDownRigth_Y()));
	p->drawLine(QPoint(Input->getDownLeft_X(), Input->getDownLeft_Y()), QPoint(Input->getUpLeft_X(), Input->getUpLeft_Y()));
	p->drawLine(QPoint(Input->getDownRigth_X(), Input->getDownRigth_Y()), QPoint(Input->getUpRigth_X(), Input->getUpRigth_Y()));*/
	delete p;
}

void myLabel::draw_text(QString text, QPoint drawPoint)
{
	if (imgIsShow)
	{
		delete myPixmap_bufer;
		myPixmap_bufer = new QPixmap(my_Pixmap);
		imgIsShow = false;
	}
	else
	{

	}
	QPainter* p = new QPainter(myPixmap_bufer);
	p->setPen(QPen(Qt::green, 2, Qt::DashLine));
	p->drawText(QPoint(drawPoint.x()+drPoint.x(), drawPoint.y() + drPoint.y()), text);
	delete p;
}

void myLabel::show_roi(std::vector<QRect> &Input, std::vector<int> rect, int circle)
{
	delete myPixmap_bufer;
	myPixmap_bufer = new QPixmap(my_Pixmap);
	QPainter *p = new QPainter(myPixmap_bufer);
	p->setPen(QPen(Qt::red, 1, Qt::DashLine));
	for (int i{ 0 }; i < rect.size(); i++)
		p->drawRect(Input[rect[i]]);
	this->setPixmap(*myPixmap_bufer);
	delete p;
}

void myLabel::set_rect(std::vector<QRect> &InOutput)
{
	//InOutput.push_back(QRect(QPoint(f_x_labl, f_y_labl), QPoint(s_x_labl, s_y_labl)));
	add = false;
}

void myLabel::resize_rect(QtRotateRect &InOutput)
{
	toImgCoordinate(x_labl, y_labl);
	toImgCoordinate(x_lablG, y_lablG,false);
	double rotAngel{ InOutput.getRotateAngel() };
	if ((this->cursor().shape() == Qt::SizeHorCursor && (((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5)) || (rotAngel >= 157.5 && rotAngel < 202.5)))
		||(this->cursor().shape() == Qt::SizeVerCursor && ((rotAngel >= 67.5 && rotAngel < 112.5) || (rotAngel >= 247.5 && rotAngel < 292.5)))
		||(this->cursor().shape() == Qt::SizeFDiagCursor && ((rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 202.5 && rotAngel < 247.5)))
		||(this->cursor().shape() == Qt::SizeBDiagCursor && ((rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 292.5 && rotAngel < 337.5))))
	{
		if (((f_x_pixMap < InOutput.getRigthX(y_labl) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
			|| (f_y_pixMap < InOutput.getRigthY(x_labl) && (rotAngel >= 67.5 && rotAngel < 112.5))
			|| (f_y_pixMap > InOutput.getRigthY(x_labl) && (rotAngel >= 247.5 && rotAngel < 292.5))
			|| (f_x_pixMap > InOutput.getRigthX(y_labl) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5)|| (rotAngel >= 202.5 && rotAngel < 247.5))))
			&& !down_or_right)
		{
			up_or_left = true;
			double dx{ 0 };
			bool dx_isSet{ false };
			if (((rotAngel >= 67.5 && rotAngel < 112.5) && y_labl < InOutput.getRigthY(x_labl))
				|| ((rotAngel >= 247.5 && rotAngel < 292.5) && y_labl > InOutput.getRigthY(x_labl)))
			{
				dx = sin(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getLeftY(x_labl));
			}
			else if ((x_labl < InOutput.getRigthX(y_labl) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
					|| (x_labl > InOutput.getRigthX(y_labl) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5))))
			{
				dx = cos(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getLeftX(y_labl));
			}
			else
			{
				InOutput.setX(InOutput.x() + InOutput.width() - 1);
				dx_isSet = true;
			}
			if (!dx_isSet)
			{
				InOutput.setX(InOutput.x() + static_cast<int>(dx),this->getScaledImgSize());
				//InOutput.setX(InOutput.x() + static_cast<int>(outForBoard_X(&InOutput, dx, true)));
				if (InOutput.width() <= 0)
				{
					InOutput.setX(InOutput.x() + InOutput.width() - 1);
				}
			}
		}
		else if (!up_or_left)
		{
			down_or_right = true;
			double dw{ 0 };
			bool widt_isSet{ false };
			if (((rotAngel >= 67.5 && rotAngel < 112.5) && y_labl > InOutput.getLeftY(x_labl))
				|| ((rotAngel >= 247.5 && rotAngel < 292.5) && y_labl < InOutput.getLeftY(x_labl)))
			{
				dw = sin(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getRigthY(x_labl));
			}
			else if ((x_labl > InOutput.getLeftX(y_labl)&& ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
				|| (x_labl < InOutput.getLeftX(y_labl) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5))))
			{
				dw = cos(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getRigthX(y_labl));
			}
			else
			{
				InOutput.setWidth(1);
				widt_isSet = true;
			}
			if (!widt_isSet)
			{
				InOutput.setWidth(InOutput.width() + static_cast<int>(dw), this->getScaledImgSize());
				//InOutput.setWidth(InOutput.width() + static_cast<int>(outForBoard_X(&InOutput, dw, false)));
				if (InOutput.width() <= 0)
				{
					InOutput.setWidth(1);
				}
			}
		}
	}
	else 
	if ((this->cursor().shape() == Qt::SizeVerCursor && (((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5)) || (rotAngel >= 157.5 && rotAngel < 202.5)))
		|| (this->cursor().shape() == Qt::SizeHorCursor && ((rotAngel >= 67.5 && rotAngel < 112.5) || (rotAngel >= 247.5 && rotAngel < 292.5)))
		|| (this->cursor().shape() == Qt::SizeBDiagCursor && ((rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 202.5 && rotAngel < 247.5)))
		|| (this->cursor().shape() == Qt::SizeFDiagCursor && ((rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 292.5 && rotAngel < 337.5))))
	{
		if (((f_y_pixMap < InOutput.getDownY(x_labl) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
			|| (f_y_pixMap > InOutput.getDownY(x_labl) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5)))
			|| ((rotAngel >= 67.5 && rotAngel < 112.5) && f_x_pixMap > InOutput.getDownX(y_labl))
			|| ((rotAngel >= 247.5 && rotAngel < 292.5) && f_x_pixMap < InOutput.getDownX(y_labl)))
			&& !down_or_right)
		{
			up_or_left = true;
			double dy_first{ 0 };
			
			bool dy_isSet{ false };
			bool dy_isMinus{ false };
			if (((rotAngel >= 67.5 && rotAngel < 112.5) && x_labl > InOutput.getDownX(y_labl))
				|| ((rotAngel >= 247.5 && rotAngel < 292.5) && x_labl < InOutput.getDownX(y_labl)))
			{
				dy_first = sin(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getUpX(y_labl));
				dy_isMinus = true;
			}
			else if ((y_labl < InOutput.getDownY(x_labl) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
				|| (y_labl > InOutput.getDownY(x_labl) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5))))
			{
				dy_first = cos(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getUpY(x_labl));
			}
			else
			{
				InOutput.setY(InOutput.y() + InOutput.height() - 1);
				dy_isSet = true;
			}
			if (!dy_isSet)
			{
				if (dy_isMinus)
				{
					InOutput.setY(InOutput.y() - static_cast<int>(dy_first), this->getScaledImgSize());
					//InOutput.setY(InOutput.y() - static_cast<int>(outForBoard_Y(&InOutput,dy_first, true, dy_isMinus)));
				}
				else
				{
					InOutput.setY(InOutput.y() + static_cast<int>(dy_first), this->getScaledImgSize());
					//InOutput.setY(InOutput.y() + static_cast<int>(outForBoard_Y(&InOutput, dy_first, true, dy_isMinus)));
				}
			}
		}
		else if (!up_or_left)
		{
			down_or_right = true;
			bool dh_isSet{ false };
			bool dh_isMinus{ false };
			double dh_first{ 0 };
			if (((rotAngel >= 67.5 && rotAngel < 112.5) && x_labl < InOutput.getUpX(y_labl))
				|| ((rotAngel >= 247.5 && rotAngel < 292.5) && x_labl > InOutput.getUpX(y_labl)))
			{
				dh_first = sin(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getDownX(y_labl));
				dh_isMinus = true;
			}
			else if ((y_labl > InOutput.getUpY(x_labl) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
				|| (y_labl < InOutput.getUpY(x_labl) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5))))
			{
				dh_first = cos(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getDownY(x_labl));
			}
			else
			{
				InOutput.setHeight(1);
				dh_isSet = true;
			}
			if (!dh_isSet)
			{
				if (dh_isMinus)
				{
					InOutput.setHeight(InOutput.height() - static_cast<int>(dh_first), this->getScaledImgSize());
					//InOutput.setHeight(InOutput.height() - static_cast<int>(outForBoard_Y(&InOutput,dh_first,dh_isMinus, false)));
				}
				else
				{
					InOutput.setHeight(InOutput.height() + static_cast<int>(dh_first), this->getScaledImgSize());
					//InOutput.setHeight(InOutput.height() + static_cast<int>(outForBoard_Y(&InOutput, dh_first, dh_isMinus, false)));
				}
			}
		}
	}
	else 
	if ((this->cursor().shape() == Qt::SizeFDiagCursor && (((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5)) || (rotAngel >= 157.5 && rotAngel < 202.5)))
		|| (this->cursor().shape() == Qt::SizeBDiagCursor && ((rotAngel >= 67.5 && rotAngel < 112.5) || (rotAngel >= 247.5 && rotAngel < 292.5)))
		|| (this->cursor().shape() == Qt::SizeVerCursor && ((rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 202.5 && rotAngel < 247.5)))
		|| (this->cursor().shape() == Qt::SizeHorCursor && ((rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 292.5 && rotAngel < 337.5))))
	{ 
		if (((f_y_pixMap < InOutput.getDownRigth_Y() && (rotAngel >= 0 && rotAngel < 90)) 
			|| (f_x_pixMap > InOutput.getDownRigth_X() && (rotAngel >= 90 && rotAngel < 180)) 
			|| (f_y_pixMap > InOutput.getDownRigth_Y() && (rotAngel >= 180 && rotAngel < 270))
			|| (f_x_pixMap < InOutput.getDownRigth_X() && (rotAngel >= 270 && rotAngel <= 360))) 
			&& !down_or_right)
		{
			up_or_left = true;
			if (((y_labl < InOutput.getDownY(x_lablG) && x_labl < InOutput.getRigthX(y_lablG)) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
				|| (f_x_pixMap > InOutput.getDownX(y_lablG) && y_labl < InOutput.getRigthY(x_lablG) && x_labl > InOutput.getDownX(y_lablG) && (rotAngel >= 67.5 && rotAngel < 112.5))
				|| (f_x_pixMap < InOutput.getDownX(y_lablG) && y_labl > InOutput.getRigthY(x_lablG) && x_labl < InOutput.getDownX(y_lablG) && (rotAngel >= 247.5 && rotAngel < 292.5))
				|| ((y_labl > InOutput.getDownY(x_lablG) && x_labl > InOutput.getRigthX(y_lablG)) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5) || (rotAngel >= 247.5 && rotAngel < 292.5))))
			{
				double dy{ 0 };
				double dx{ 0 };
				if ((rotAngel >= 315.0 && rotAngel <= 360.0) || (rotAngel >= 0.0 && rotAngel < 45.0) || (rotAngel >= 135.0 && rotAngel < 225.0))
				{
					dy = cos(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getUpLeft_Y());
					dx = cos(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getUpLeft_X());
				}
				else if ((rotAngel >= 45.0 && rotAngel < 135.0) || (rotAngel >= 225.0 && rotAngel < 315.0))
				{
					dy = -sin(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getUpLeft_X());
					dx = sin(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getUpLeft_Y());
				}
				InOutput.setY(InOutput.y() + static_cast<int>(dy), this->getScaledImgSize());
				InOutput.setX(InOutput.x() + static_cast<int>(dx), this->getScaledImgSize());
				//InOutput.setY(InOutput.y() + static_cast<int>(outForBoard_Y(&InOutput, dy, true, false)));
				//InOutput.setX(InOutput.x() + static_cast<int>(outForBoard_X(&InOutput, dx, true)));
				if (InOutput.height() <= 0)
				{
					InOutput.setY(InOutput.y() + InOutput.height() - 1);
				}
				if (InOutput.width() <= 0)
				{
					InOutput.setX(InOutput.x() + InOutput.width() - 1);
				}
			}
			else 
			if ((y_labl >= InOutput.getDownY(x_lablG)  && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)) )
				|| (f_x_pixMap > InOutput.getDownX(y_lablG) && x_labl<= InOutput.getDownX(y_lablG)+1 && (rotAngel >= 67.5 && rotAngel < 112.5))
				|| (f_x_pixMap < InOutput.getDownX(y_lablG) && x_labl >= InOutput.getDownX(y_lablG) && (rotAngel >= 247.5 && rotAngel < 292.5))
				|| (y_labl <= InOutput.getDownY(x_lablG)  && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5) || (rotAngel >= 247.5 && rotAngel < 292.5)) ))
			{
				InOutput.setY(InOutput.y() + InOutput.height() - 1);
				double dx{ 0 };
				if ((rotAngel >= 315.0 && rotAngel <= 360.0) || (rotAngel >= 0.0 && rotAngel < 45.0) || (rotAngel >= 135.0 && rotAngel < 225.0))
				{
					dx = cos(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getUpLeft_X());
				}
				else if ((rotAngel >= 45.0 && rotAngel < 135.0) || (rotAngel >= 225.0 && rotAngel < 315.0))
				{
					dx = sin(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getUpLeft_Y());
				}

				if (InOutput.getDownRigth_Y() >= InOutput.getUpLeft_Y() && InOutput.getDownRigth_X() >= InOutput.getUpLeft_X())
				{
					InOutput.setX(InOutput.x() + static_cast<int>(dx), this->getScaledImgSize());
					//InOutput.setX(InOutput.x() + static_cast<int>(outForBoard_X(&InOutput, dx, true)));
					if ((InOutput.getDownRigth_X() < InOutput.getUpLeft_X() || InOutput.width() <= 0))
					{
						InOutput.setX(InOutput.x() + InOutput.width() - 1);
					}
				}
				else 
				if (InOutput.getDownRigth_Y() >= InOutput.getUpLeft_Y() && InOutput.getDownRigth_X() < InOutput.getUpLeft_X())
				{
					InOutput.setX(InOutput.x() + static_cast<int>(dx), this->getScaledImgSize());
					//InOutput.setX(InOutput.x() + static_cast<int>(outForBoard_X(&InOutput, dx, true)));
					if (InOutput.getDownRigth_Y() < InOutput.getUpLeft_Y() || InOutput.width() <= 0)
					{
						InOutput.setX(InOutput.x() + InOutput.width() - 1);
					}
				}
				else
				if (InOutput.getDownRigth_Y() < InOutput.getUpLeft_Y() && InOutput.getDownRigth_X() < InOutput.getUpLeft_X())
				{
					InOutput.setX(InOutput.x() + static_cast<int>(dx), this->getScaledImgSize());
					//InOutput.setX(InOutput.x() + static_cast<int>(outForBoard_X(&InOutput, dx, true)));
					if (InOutput.getDownRigth_X() > InOutput.getUpLeft_X() || InOutput.width() <= 0)
					{
						InOutput.setX(InOutput.x() + InOutput.width() - 1);
					}
				}
				else
				if (InOutput.getDownRigth_Y() < InOutput.getUpLeft_Y() && InOutput.getDownRigth_X() >= InOutput.getUpLeft_X())
				{
					InOutput.setX(InOutput.x() + static_cast<int>(dx), this->getScaledImgSize());
					//InOutput.setX(InOutput.x() + static_cast<int>(outForBoard_X(&InOutput, dx, true)));
					if (InOutput.getDownRigth_Y() > InOutput.getUpLeft_Y() || InOutput.width() <= 0)
					{
						InOutput.setX(InOutput.x() + InOutput.width() - 1);
					}
				}
			}
			else 
			if (( x_labl >= InOutput.getRigthX(y_lablG) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
				|| (f_x_pixMap > InOutput.getDownX(y_lablG) && x_labl >= InOutput.getDownRigth_X() && (rotAngel >= 67.5 && rotAngel < 112.5))
				|| (f_x_pixMap < InOutput.getDownX(y_lablG) && x_labl <= InOutput.getDownRigth_X() && (rotAngel >= 247.5 && rotAngel < 292.5))
				|| ( x_labl <= InOutput.getRigthX(y_lablG) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5) || (rotAngel >= 247.5 && rotAngel < 292.5))))
			{
				InOutput.setX(InOutput.x() + InOutput.width() - 1);
				double dy{ 0 };
				bool dy_isMinus{ false };
				if ((rotAngel >= 315.0 && rotAngel <= 360.0) || (rotAngel >= 0.0 && rotAngel < 45.0) || (rotAngel >= 135.0 && rotAngel < 225.0))
				{
					dy = cos(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getUpLeft_Y());
				}
				else if ((rotAngel >= 45.0 && rotAngel < 135.0) || (rotAngel >= 225.0 && rotAngel < 315.0))
				{
					dy = -sin(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getUpLeft_X());
					dy_isMinus = true;
				}
				if (InOutput.getDownRigth_Y() >= InOutput.getUpLeft_Y() && InOutput.getDownRigth_X() >= InOutput.getUpLeft_X())
				{
					InOutput.setY(InOutput.y() + static_cast<int>(dy), this->getScaledImgSize());
					//InOutput.setY(InOutput.y() + static_cast<int>(outForBoard_Y(&InOutput, dy, true, dy_isMinus)));
					if (InOutput.getDownRigth_Y() < InOutput.getUpLeft_Y() || InOutput.height() <= 0)
					{
						InOutput.setY(InOutput.y() + InOutput.height() - 1);
					}
				}
				else
				if (InOutput.getDownRigth_Y() >= InOutput.getUpLeft_Y() && InOutput.getDownRigth_X() < InOutput.getUpLeft_X())
				{
					InOutput.setY(InOutput.y() + static_cast<int>(dy), this->getScaledImgSize());
					//InOutput.setY(InOutput.y() + static_cast<int>(outForBoard_Y(&InOutput, dy, true, dy_isMinus)));
					if (InOutput.getDownRigth_X() > InOutput.getUpLeft_X() || InOutput.height() <= 0)
					{
						InOutput.setY(InOutput.y() + InOutput.height() - 1);
					}
				}
				else
				if (InOutput.getDownRigth_Y() < InOutput.getUpLeft_Y() && InOutput.getDownRigth_X() < InOutput.getUpLeft_X())
				{
					InOutput.setY(InOutput.y() + static_cast<int>(dy), this->getScaledImgSize());
					//InOutput.setY(InOutput.y() + static_cast<int>(outForBoard_Y(&InOutput, dy, true, dy_isMinus)));
					if (InOutput.getDownRigth_Y() > InOutput.getUpLeft_Y() || InOutput.height() <= 0)
					{
						InOutput.setY(InOutput.y() + InOutput.height() - 1);
					}
				}
				else
				if (InOutput.getDownRigth_Y() < InOutput.getUpLeft_Y() && InOutput.getDownRigth_X() >= InOutput.getUpLeft_X())
				{
					InOutput.setY(InOutput.y() + static_cast<int>(dy), this->getScaledImgSize());
					//InOutput.setY(InOutput.y() + static_cast<int>(outForBoard_Y(&InOutput, dy, true, dy_isMinus)));
					if (InOutput.getDownRigth_X() < InOutput.getUpLeft_X() || InOutput.height() <= 0)
					{
						InOutput.setY(InOutput.y() + InOutput.height() - 1);
					}
				}
			}
			else
			{
				InOutput.setX(InOutput.x() + InOutput.width() - 1);
				InOutput.setY(InOutput.y() + InOutput.height() - 1);
			}
		}
		else if (!up_or_left)
		{
			down_or_right = true;
			if (((y_labl > InOutput.getUpY(x_lablG) && x_labl > InOutput.getLeftX(y_lablG)) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
				|| (f_x_pixMap < InOutput.getUpX(y_lablG) && y_labl > InOutput.getLeftY(x_lablG) && x_labl < InOutput.getUpX(y_lablG) && (rotAngel >= 67.5 && rotAngel < 112.5))
				|| (f_x_pixMap > InOutput.getUpX(y_lablG) && y_labl < InOutput.getLeftY(x_lablG) && x_labl > InOutput.getUpX(y_lablG) && (rotAngel >= 247.5 && rotAngel < 292.5))
				|| ((y_labl < InOutput.getUpY(x_lablG) && x_labl < InOutput.getLeftX(y_lablG)) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5) || (rotAngel >= 247.5 && rotAngel < 292.5))))
			{
				double dh{ 0 };
				double dw{ 0 };

				if ((rotAngel >= 315.0 && rotAngel <= 360.0) || (rotAngel >= 0.0 && rotAngel < 45.0) || (rotAngel >= 135.0 && rotAngel < 225.0))
				{
					dh = cos(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getDownY(x_labl));
					dw = cos(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getRigthX(y_labl));
				}
				else if ((rotAngel >= 45.0 && rotAngel < 135.0) || (rotAngel >= 225.0 && rotAngel < 315.0))
				{
					dh = -sin(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getDownX(y_labl));
					dw = sin(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getRigthY(x_labl));
				}
				InOutput.setHeight(InOutput.height() + static_cast<int>(dh), this->getScaledImgSize());
				InOutput.setWidth(InOutput.width() + static_cast<int>(dw), this->getScaledImgSize());
				//InOutput.setHeight(InOutput.height() + static_cast<int>(outForBoard_Y(&InOutput, dh, false, false)));
				//InOutput.setWidth(InOutput.width() + static_cast<int>(outForBoard_X(&InOutput, dw, false)));
				if (InOutput.height() <= 0)
				{
					InOutput.setHeight(1);
				}
				if (InOutput.width() <= 0)
				{
					InOutput.setWidth(1);
				}
			}
			else 
			if ((y_labl <= InOutput.getUpY(x_lablG) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
				|| (f_x_pixMap < InOutput.getUpX(y_lablG) && x_labl >= InOutput.getUpX(y_lablG) + 1 && (rotAngel >= 67.5 && rotAngel < 112.5))
				|| (f_x_pixMap > InOutput.getUpX(y_lablG) && x_labl <= InOutput.getUpX(y_lablG) && (rotAngel >= 247.5 && rotAngel < 292.5))
				|| (y_labl >= InOutput.getUpY(x_lablG) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5) || (rotAngel >= 247.5 && rotAngel < 292.5))))
			{
				InOutput.setHeight(1);
				double dw{ 0 };
				if ((rotAngel >= 315.0 && rotAngel <= 360.0) || (rotAngel >= 0.0 && rotAngel < 45.0) || (rotAngel >= 135.0 && rotAngel < 225.0))
				{
					dw = cos(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getRigthX(y_labl));
				}
				else if ((rotAngel >= 45.0 && rotAngel < 135.0) || (rotAngel >= 225.0 && rotAngel < 315.0))
				{
					dw = sin(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getRigthY(x_labl));
				}
				InOutput.setWidth(InOutput.width() + static_cast<int>(dw), this->getScaledImgSize());
				//InOutput.setWidth(InOutput.width() + static_cast<int>(outForBoard_X(&InOutput, dw, false)));
				if (InOutput.width() <= 0)
				{
					InOutput.setWidth(1);
				}
			}
			else 
			if ((x_labl <= InOutput.getLeftX(y_lablG) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
				|| (f_x_pixMap < InOutput.getUpX(y_lablG) && x_labl <= InOutput.getUpX(y_lablG) && (rotAngel >= 67.5 && rotAngel < 112.5))
				|| (f_x_pixMap > InOutput.getUpX(y_lablG) && x_labl >= InOutput.getUpX(y_lablG) && (rotAngel >= 247.5 && rotAngel < 292.5))
				|| (x_labl >= InOutput.getLeftX(y_lablG) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5) || (rotAngel >= 247.5 && rotAngel < 292.5))))
			{
				InOutput.setWidth(1);
				double dh{ 0 };
				bool dh_isMinus{ false };
				if ((rotAngel >= 315.0 && rotAngel <= 360.0) || (rotAngel >= 0.0 && rotAngel < 45.0) || (rotAngel >= 135.0 && rotAngel < 225.0))
				{
					dh = cos(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getDownY(x_labl));
				}
				else if ((rotAngel >= 45.0 && rotAngel < 135.0) || (rotAngel >= 225.0 && rotAngel < 315.0))
				{
					dh = -sin(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getDownX(y_labl));
					dh_isMinus = false;
				}
				InOutput.setHeight(InOutput.height() + static_cast<int>(dh), this->getScaledImgSize());
				//InOutput.setHeight(InOutput.height() + static_cast<int>(outForBoard_Y(&InOutput, dh, false, dh_isMinus)));
				if (InOutput.height() <= 0)
				{
					InOutput.setHeight(1);
				}
			}
			else
			{
				InOutput.setHeight(1);
				InOutput.setWidth(1);
			}
		}
	}
	else 
	if ((this->cursor().shape() == Qt::SizeBDiagCursor && (((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5)) || (rotAngel >= 157.5 && rotAngel < 202.5)))
	|| (this->cursor().shape() == Qt::SizeFDiagCursor && ((rotAngel >= 67.5 && rotAngel < 112.5) || (rotAngel >= 247.5 && rotAngel < 292.5)))
	|| (this->cursor().shape() == Qt::SizeHorCursor && ((rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 202.5 && rotAngel < 247.5)))
	|| (this->cursor().shape() == Qt::SizeVerCursor && ((rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 292.5 && rotAngel < 337.5))))

	{
		if (((f_x_pixMap > InOutput.getDownLeft_X() && (rotAngel >= 0 && rotAngel < 90))
			|| (f_y_pixMap > InOutput.getDownLeft_Y() && (rotAngel >= 90 && rotAngel < 180))
			|| (f_x_pixMap < InOutput.getDownLeft_X() && (rotAngel >= 180 && rotAngel < 270))
			|| (f_y_pixMap < InOutput.getDownLeft_Y() && (rotAngel >= 270 && rotAngel <= 360)))
			&& !down_or_right) 
		{
			up_or_left = true;
			if (((y_labl < InOutput.getDownY(x_lablG) && x_labl > InOutput.getLeftX(y_lablG)) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
				|| (f_x_pixMap > InOutput.getDownX(y_lablG) && y_labl > InOutput.getLeftY(x_lablG) && x_labl > InOutput.getDownX(y_lablG) && (rotAngel >= 67.5 && rotAngel < 112.5))
				|| (f_x_pixMap < InOutput.getDownX(y_lablG) && y_labl < InOutput.getLeftY(x_lablG) && x_labl < InOutput.getDownX(y_lablG) && (rotAngel >= 247.5 && rotAngel < 292.5))
				|| ((y_labl > InOutput.getDownY(x_lablG) && x_labl < InOutput.getLeftX(y_lablG)) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5) || (rotAngel >= 247.5 && rotAngel < 292.5))))
			{
				double dy{ 0 };
				double dw{ 0 };
				if ((rotAngel >= 315.0 && rotAngel <= 360.0) || (rotAngel >= 0.0 && rotAngel < 45.0) || (rotAngel >= 135.0 && rotAngel < 225.0))
				{
					dy = cos(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getUpRigth_Y());
					dw = cos(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getRigthX(y_labl));
				}
				else if ((rotAngel >= 45.0 && rotAngel < 135.0) || (rotAngel >= 225.0 && rotAngel < 315.0))
				{
					dy = -sin(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getUpRigth_X());
					dw = sin(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getRigthY(x_labl));
				}
				InOutput.setWidth(InOutput.width() + static_cast<int>(dw), this->getScaledImgSize());
				InOutput.setY(InOutput.y() + static_cast<int>(dy), this->getScaledImgSize());
				//InOutput.setWidth(InOutput.width() + static_cast<int>(outForBoard_X(&InOutput, dw, false)));
				//InOutput.setY(InOutput.y() + static_cast<int>(outForBoard_Y(&InOutput, dy, true, false)));
				if (InOutput.height() <= 0)
				{
					InOutput.setY(InOutput.y() + InOutput.height() - 1);
				}
				if (InOutput.width() <= 0)
				{
					InOutput.setWidth(1);
				}
			}
			else 
			if ((y_labl >= InOutput.getDownY(x_lablG) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
				|| (f_x_pixMap > InOutput.getDownX(y_lablG) && x_labl <= InOutput.getDownX(y_lablG) + 1 && (rotAngel >= 67.5 && rotAngel < 112.5))
				|| (f_x_pixMap < InOutput.getDownX(y_lablG) && x_labl >= InOutput.getDownX(y_lablG) && (rotAngel >= 247.5 && rotAngel < 292.5))
				|| (y_labl <= InOutput.getDownY(x_lablG) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5) || (rotAngel >= 247.5 && rotAngel < 292.5))))
			{
				InOutput.setY(InOutput.y() + InOutput.height() - 1);
				double dw{ 0 };
				if ((rotAngel >= 315.0 && rotAngel <= 360.0) || (rotAngel >= 0.0 && rotAngel < 45.0) || (rotAngel >= 135.0 && rotAngel < 225.0))
				{
					dw = cos(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getRigthX(y_labl));
				}
				else if ((rotAngel >= 45.0 && rotAngel < 135.0) || (rotAngel >= 225.0 && rotAngel < 315.0))
				{
					dw = sin(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getRigthY(x_labl));
				}
				InOutput.setWidth(InOutput.width() + static_cast<int>(dw), this->getScaledImgSize());
				//InOutput.setWidth(InOutput.width() + static_cast<int>(outForBoard_X(&InOutput, dw, false)));
				if (InOutput.width() <= 0)
				{
					InOutput.setWidth(1);
				}
			}
			else 
			if ((x_labl <= InOutput.getLeftX(y_lablG) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
				|| (f_x_pixMap > InOutput.getDownX(y_lablG) && y_labl <= InOutput.getRigthY(x_lablG) && (rotAngel >= 67.5 && rotAngel < 112.5))
				|| (f_x_pixMap < InOutput.getDownX(y_lablG) && x_labl <= InOutput.getUpX(y_lablG) && (rotAngel >= 247.5 && rotAngel < 292.5))
				|| (x_labl >= InOutput.getLeftX(y_lablG) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5) || (rotAngel >= 247.5 && rotAngel < 292.5))))
			{
				InOutput.setWidth(1);
				double dy{ 0 };
				if ((rotAngel >= 315.0 && rotAngel <= 360.0) || (rotAngel >= 0.0 && rotAngel < 45.0) || (rotAngel >= 135.0 && rotAngel < 225.0))
				{
					dy = cos(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getUpRigth_Y());
				}
				else if ((rotAngel >= 45.0 && rotAngel < 135.0) || (rotAngel >= 225.0 && rotAngel < 315.0))
				{
					dy = -sin(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getUpRigth_X());
				}
				if (InOutput.getDownLeft_Y() >= InOutput.getUpRigth_Y() && InOutput.getDownLeft_X() < InOutput.getUpRigth_X())
				{
					InOutput.setY(InOutput.y() + static_cast<int>(dy), this->getScaledImgSize());
					///InOutput.setY(InOutput.y() + static_cast<int>(outForBoard_Y(&InOutput, dy, true, false)));
					if (InOutput.getDownRigth_Y() < InOutput.getUpLeft_Y() || InOutput.height() <= 0)
					{
						InOutput.setY(InOutput.y() + InOutput.height() - 1);
					}
				}
				else
				if (InOutput.getDownLeft_Y() < InOutput.getUpRigth_Y() && InOutput.getDownLeft_X() < InOutput.getUpRigth_X())
				{
					InOutput.setY(InOutput.y() + static_cast<int>(dy), this->getScaledImgSize());
					//InOutput.setY(InOutput.y() + static_cast<int>(outForBoard_Y(&InOutput, dy, true, false)));
					if (InOutput.getDownRigth_X() > InOutput.getUpLeft_X() || InOutput.height() <= 0)
					{
						InOutput.setY(InOutput.y() + InOutput.height() - 1);
					}
				}
				else
				if (InOutput.getDownLeft_Y() < InOutput.getUpRigth_Y() && InOutput.getDownLeft_X() >= InOutput.getUpRigth_X())
				{
					InOutput.setY(InOutput.y() + static_cast<int>(dy), this->getScaledImgSize());
					//InOutput.setY(InOutput.y() + static_cast<int>(outForBoard_Y(&InOutput, dy, true, false)));
					if (InOutput.getDownRigth_Y() > InOutput.getUpLeft_Y() || InOutput.height() <= 0)
					{
						InOutput.setY(InOutput.y() + InOutput.height() - 1);

					}
				}
				else
				if (InOutput.getDownLeft_Y() >= InOutput.getUpRigth_Y() && InOutput.getDownLeft_X() >= InOutput.getUpRigth_X())
				{
					InOutput.setY(InOutput.y() + static_cast<int>(dy), this->getScaledImgSize());
					//InOutput.setY(InOutput.y() + static_cast<int>(outForBoard_Y(&InOutput, dy, true, false)));
					if (InOutput.getDownRigth_X() < InOutput.getUpLeft_X() || InOutput.height() <= 0)
					{
						InOutput.setY(InOutput.y() + InOutput.height() - 1);
					}
				}
			}
			else
			{
				InOutput.setHeight(1);
				InOutput.setX(InOutput.x() + InOutput.width() - 1);
			}
		}
		else if (!up_or_left)
		{
			down_or_right = true;
			if (((y_labl > InOutput.getUpY(x_lablG) && x_labl < InOutput.getRigthX(y_lablG)) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
				|| (f_x_pixMap < InOutput.getUpX(y_lablG) && y_labl < InOutput.getRigthY(x_lablG) && x_labl < InOutput.getUpX(y_lablG) && (rotAngel >= 67.5 && rotAngel < 112.5))
				|| (f_x_pixMap > InOutput.getUpX(y_lablG) && y_labl > InOutput.getRigthY(x_lablG) && x_labl > InOutput.getUpX(y_lablG) && (rotAngel >= 247.5 && rotAngel < 292.5))
				|| ((y_labl < InOutput.getUpY(x_lablG) && x_labl > InOutput.getRigthX(y_lablG)) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5) || (rotAngel >= 247.5 && rotAngel < 292.5))))
			{
				double dx{ 0 };
				double dh{ 0 };
				if ((rotAngel >= 315.0 && rotAngel <= 360.0) || (rotAngel >= 0.0 && rotAngel < 45.0) || (rotAngel >= 135.0 && rotAngel < 225.0))
				{
					dx = cos(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getDownLeft_X());
					dh = cos(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getDownY(x_labl));
				}
				else if ((rotAngel >= 45.0 && rotAngel < 135.0) || (rotAngel >= 225.0 && rotAngel < 315.0))
				{
					dx = sin(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getDownLeft_Y());
					dh = -sin(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getDownX(y_labl));
				}
				InOutput.setHeight(InOutput.height() + static_cast<int>(dh), this->getScaledImgSize());
				InOutput.setX(InOutput.x() + static_cast<int>(dx), this->getScaledImgSize());
				//InOutput.setHeight(InOutput.height() + static_cast<int>(outForBoard_Y(&InOutput, dh, false, false)));
				//InOutput.setX(InOutput.x() + static_cast<int>(outForBoard_X(&InOutput, dx, true)));
				if (InOutput.width() <= 0)
				{
					InOutput.setX(InOutput.x() + InOutput.width() - 1);
				}
				if (InOutput.height() <= 0)
				{
					InOutput.setHeight(1);
				}
			}
			else 
			if ((y_labl <= InOutput.getUpY(x_lablG) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
				|| (f_x_pixMap < InOutput.getUpX(y_lablG) && x_labl >= InOutput.getUpX(y_lablG) && (rotAngel >= 67.5 && rotAngel < 112.5))
				|| (f_x_pixMap > InOutput.getUpX(y_lablG) && x_labl <= InOutput.getUpX(y_lablG) && (rotAngel >= 247.5 && rotAngel < 292.5))
				|| (y_labl >= InOutput.getUpY(x_lablG) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5) || (rotAngel >= 247.5 && rotAngel < 292.5))))
			{
				InOutput.setHeight(1);
				double dx{ 0 };
				if ((rotAngel >= 315.0 && rotAngel <= 360.0) || (rotAngel >= 0.0 && rotAngel < 45.0) || (rotAngel >= 135.0 && rotAngel < 225.0))
				{
					dx = cos(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getDownLeft_X());
				}
				else if ((rotAngel >= 45.0 && rotAngel < 135.0) || (rotAngel >= 225.0 && rotAngel < 315.0))
				{
					dx = sin(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getUpLeft_Y());
				}
				if (InOutput.getDownLeft_Y() >= InOutput.getUpRigth_Y() && InOutput.getDownLeft_X() < InOutput.getUpRigth_X())
				{
					InOutput.setX(InOutput.x() + static_cast<int>(dx), this->getScaledImgSize());
					//InOutput.setX(InOutput.x() + static_cast<int>(outForBoard_X(&InOutput, dx, true)));
					if ((InOutput.getDownRigth_X() < InOutput.getUpLeft_X() || InOutput.width() <= 0))
					{
						InOutput.setX(InOutput.x() + InOutput.width() - 1);
					}
				}
				else
				if (InOutput.getDownLeft_Y() < InOutput.getUpRigth_Y() && InOutput.getDownLeft_X() < InOutput.getUpRigth_X())
				{
					InOutput.setX(InOutput.x() + static_cast<int>(dx), this->getScaledImgSize());
					//InOutput.setX(InOutput.x() + static_cast<int>(outForBoard_X(&InOutput, dx, true)));
					if (InOutput.getDownRigth_Y() < InOutput.getUpLeft_Y() || InOutput.width() <= 0)
					{
						InOutput.setX(InOutput.x() + InOutput.width() - 1);
					}
				}
				else
				if (InOutput.getDownLeft_Y() < InOutput.getUpRigth_Y() && InOutput.getDownLeft_X() >= InOutput.getUpRigth_X())
				{
					InOutput.setX(InOutput.x() + static_cast<int>(dx), this->getScaledImgSize());
					//InOutput.setX(InOutput.x() + static_cast<int>(outForBoard_X(&InOutput, dx, true)));
					if (InOutput.getDownRigth_X() > InOutput.getUpLeft_X() || InOutput.width() <= 0)
					{
						InOutput.setX(InOutput.x() + InOutput.width() - 1);
						InOutput.setDrowPoint(QPoint(InOutput.x() - InOutput.getTranslatePoint().x(), InOutput.y() - InOutput.getTranslatePoint().y()));
					}
				}
				else
				if (InOutput.getDownLeft_Y() >= InOutput.getUpRigth_Y() && InOutput.getDownLeft_X() >= InOutput.getUpRigth_X())
				{
					InOutput.setX(InOutput.x() + static_cast<int>(dx), this->getScaledImgSize());
					//InOutput.setX(InOutput.x() + static_cast<int>(outForBoard_X(&InOutput, dx, true)));
					if (InOutput.getDownRigth_Y() > InOutput.getUpLeft_Y() || InOutput.width() <= 0)
					{
						InOutput.setX(InOutput.x() + InOutput.width() - 1);
					}
				}
			}
			else 
			if ((x_labl >= InOutput.getRigthX(y_lablG) && ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 292.5 && rotAngel < 337.5)))
				|| (f_x_pixMap < InOutput.getUpX(y_lablG) && y_labl >= InOutput.getRigthY(x_lablG) && (rotAngel >= 67.5 && rotAngel < 112.5))
				|| (f_x_pixMap > InOutput.getUpX(y_lablG) && x_labl <= InOutput.getDownX(y_lablG) && (rotAngel >= 247.5 && rotAngel < 292.5))
				|| (x_labl <= InOutput.getRigthX(y_lablG) && ((rotAngel >= 157.5 && rotAngel < 202.5) || (rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 202.5 && rotAngel < 247.5) || (rotAngel >= 247.5 && rotAngel < 292.5))))
			{
				InOutput.setX(InOutput.x() + InOutput.width() - 1);
				double dh{ 0 };
				if ((rotAngel >= 315.0 && rotAngel <= 360.0) || (rotAngel >= 0.0 && rotAngel < 45.0) || (rotAngel >= 135.0 && rotAngel < 225.0))
				{
					dh = cos(InOutput.getRotateAngel(true)) * (y_labl - InOutput.getDownY(x_labl));
				}
				else if ((rotAngel >= 45.0 && rotAngel < 135.0) || (rotAngel >= 225.0 && rotAngel < 315.0))
				{
					dh = -sin(InOutput.getRotateAngel(true)) * (x_labl - InOutput.getDownX(y_labl));
				}
				InOutput.setHeight(InOutput.height() + static_cast<int>(dh), this->getScaledImgSize());
				//InOutput.setHeight(InOutput.height() + static_cast<int>(outForBoard_Y(&InOutput, dh, false, false)));
				if (InOutput.height() <= 0)
				{
					InOutput.setHeight(1);
				}
			}
			else
			{
			}
		}
	}
	InOutput.setTranslatePoint(QPoint(InOutput.x() + cos(InOutput.getRotateAngel(true)) * InOutput.width() / 2, InOutput.y() + sin(InOutput.getRotateAngel(true)) * InOutput.height() / 2));
}

void myLabel::muve_roiRect(QtRotateRect &InOutput)
{
	toImgCoordinate(x_labl, y_labl);
	int step_X{ x_labl - f_x_pixMap };
	int step_Y{ y_labl - f_y_pixMap };

	if (trunc(InOutput.getMin_X()) + step_X < 1)
	{
		step_X = 0;
		//InOutput.setX(0);
		emit mouseLeftMouveRoi(0);
	}
	else if (trunc(InOutput.getMax_X()) + step_X > this->scaledSize.width() - 2)
	{
		step_X = 0;
		/*int bufer(InOutput.width());
		InOutput.setX(scaledSize.width() - InOutput.width() - 1);
		InOutput.setWidth(bufer);*/
		emit mouseLeftMouveRoi(1);
	}

	if (trunc(InOutput.getMin_Y()) + step_Y < 1)
	{
		step_Y = 0;
		//InOutput.setY(0);
		emit mouseLeftMouveRoi(2);
	}
	else if (trunc(InOutput.getMax_Y()) + step_Y > this->scaledSize.height() - 1)
	{
		step_Y = 0;
		/*int bufer(InOutput.height());
		InOutput.setY(scaledSize.height() - InOutput.height() - 1);
		InOutput.setHeight(bufer);*/
		emit mouseLeftMouveRoi(3);
	}
	InOutput.translate(step_X, step_Y);
	InOutput.setTranslatePoint(QPoint(InOutput.x() + cos(InOutput.getRotateAngel(true)) * InOutput.width() / 2, InOutput.y() + cos(InOutput.getRotateAngel(true)) * InOutput.height() / 2));

	InOutput.translateTranslatePoint(step_X, step_Y);
	//InOutput.setDrowPoint(QPoint((InOutput.x() - InOutput.getTranslatePoint().x()), (InOutput.y() - InOutput.getTranslatePoint().y())));
	f_x_pixMap = x_labl;
	f_y_pixMap = y_labl;
}

void myLabel::muve_roiCircle(QPoint& centerPoint,const int& radius)
{
	toImgCoordinate(x_labl, y_labl);
	int step_X{ x_labl - f_x_pixMap };
	int step_Y{ y_labl - f_y_pixMap };

	if (centerPoint.x() - radius + step_X < 0)
	{
		step_X = 0;
		centerPoint.setX(radius);
		emit mouseLeftMouveRoi(0);
	}
	else if (centerPoint.x() + radius + step_X > this->scaledSize.width() - 1)
	{
		step_X = 0;
		centerPoint.setX(scaledSize.width() - radius - 1);
		emit mouseLeftMouveRoi(1);
	}

	if (centerPoint.y() -radius + step_Y < 0)
	{
		step_Y = 0;
		centerPoint.setY(radius);
		emit mouseLeftMouveRoi(2);
	}
	else if (centerPoint.y() + radius + step_Y > this->scaledSize.height() - 1)
	{
		step_Y = 0;
		centerPoint.setY(scaledSize.height() -radius - 1);
	}
	centerPoint.setX(centerPoint.x() + step_X);
	centerPoint.setY(centerPoint.y() + step_Y);
	f_x_pixMap = x_labl;
	f_y_pixMap = y_labl;
}

void myLabel::show_partImg(int dx, int dy, int width, int height)
{
	this->repaint();
	if (dx <= 0)
		dx = 0;
	else if (dx >= (scaledSize.width() - width))
		dx = scaledSize.width() - width;
	if (dy <= 0)
		dy = 0;
	else if (dy >= (scaledSize.height() - height))
		dy = scaledSize.height() - height;

	drPoint.setX(dx);
	drPoint.setY(dy);

	delete myPixmap_mouve;
	if (myPixmap_bufer == nullptr)
	{
		myPixmap_mouve = new QPixmap(my_Pixmap.copy(dx, dy, width, height));
	}
	else
	{
		myPixmap_mouve = new QPixmap(myPixmap_bufer->copy(dx, dy, width, height));
	}

	this->setPixmap(*myPixmap_mouve);
	imgIsShow = true;
}

void myLabel::show_partImg()
{
	this->repaint();
	delete myPixmap_mouve;
	if (myPixmap_bufer == nullptr)
	{
		myPixmap_mouve = new QPixmap(my_Pixmap.copy(drPoint.x(), drPoint.y(), this->width(), this->height()));
	}
	else
	{
		myPixmap_mouve = new QPixmap(myPixmap_bufer->copy(drPoint.x(), drPoint.y(), this->width(), this->height()));
	}
	this->setPixmap(*myPixmap_mouve);
	imgIsShow = true;
}

double myLabel::scaledPixmap(int scaled, int &dx, int &dy)
{
	bool ver_center{ false };
	bool hor_center{ false };
	if (scaledSize.width() < this->width())
		hor_center = true;
	if (scaledSize.height() < this->height())
		ver_center=true;
	Qt::AspectRatioMode _aspectRotMod;
	if (scaled == 0 && activ_scaled == 0)
	{
		activ_scaled = 0;
		scaledSize.setWidth(this->width());
		scaledSize.setHeight(this->height());
		_aspectRotMod = Qt::KeepAspectRatio;
	}
	else
	{
		if (activ_scaled != 0)
		{

			if (scaled != 0)
			{
				this->toCenterPoint(drPoint);
				original_drPoint.setX(drPoint.x() / _scaled[activ_scaled]);
				original_drPoint.setY(drPoint.y() / _scaled[activ_scaled]);
				if (scaled > 0 && activ_scaled < 10)
				{
					activ_scaled++;
				}
				else if (scaled < 0 && activ_scaled>1)
				{
					activ_scaled--;
				}
				scaledSize.setWidth(originalSize.width() * _scaled[activ_scaled]);
				scaledSize.setHeight(originalSize.height() * _scaled[activ_scaled]);
				_aspectRotMod = Qt::IgnoreAspectRatio;
			}
			else
			{
				activ_scaled = 0;
				scaledSize.setWidth(this->width());
				scaledSize.setHeight(this->height());
				_aspectRotMod = Qt::KeepAspectRatio;
			}
		}
		else
		{
			this->toCenterPoint(drPoint);
			original_drPoint.setX(drPoint.x() / (static_cast<double>(scaledSize.width())/originalSize.width()));
			original_drPoint.setY(drPoint.y() / (static_cast<double>(scaledSize.height()) / originalSize.height()));
			if (scaled > 0)
				activ_scaled = max_activ_scaled;
			else
				activ_scaled = min_activ_scaled;
			scaledSize.setWidth(originalSize.width() * _scaled[activ_scaled]);
			scaledSize.setHeight(originalSize.height() * _scaled[activ_scaled]);
			/*original_drPoint.setX(scaledSize.width() * _scaled[activ_scaled]);
			original_drPoint.setY(scaledSize.height() * _scaled[activ_scaled]);*/
			_aspectRotMod = Qt::IgnoreAspectRatio;
		}
	}
	if (myPixmap_bufer != nullptr)
	{
		QPixmap scaledPixmap;
		scaledPixmap = my_PixmapOriginal.scaled(scaledSize, _aspectRotMod);
		delete myPixmap_bufer;
		myPixmap_bufer = new QPixmap(scaledPixmap);
		my_Pixmap = scaledPixmap;
		int buferFormat{ imageFormat };
		imageFormat = 0;
		formatImage(buferFormat);
		scaledSize.setHeight(scaledPixmap.height());
		scaledSize.setWidth(scaledPixmap.width());
		if (activ_scaled != 0)
		{
			if (!hor_center)
				dx = original_drPoint.x() * _scaled[activ_scaled];
			else
				dx = scaledSize.width() / 2;
			if (!ver_center)
				dy = original_drPoint.y() * _scaled[activ_scaled];
			else
				dy = scaledSize.height()/2;
			this->toUpLeftpoint(dx, dy);
		}
		else
		{

		}
	}
	
	if (scaled != 0)
	{
		return _scaled[activ_scaled] * 100;
	}
	else
	{
		return _scaled[11] * 100;
	}
}

void myLabel::scaledPixmap()
{
	Qt::AspectRatioMode _aspectRotMod;

	this->toCenterPoint(drPoint);
	original_drPoint.setX(drPoint.x() / _scaled[activ_scaled]);
	original_drPoint.setY(drPoint.y() / _scaled[activ_scaled]);
	this->toUpLeftpoint(drPoint);
	double sdf{ _scaled[activ_scaled] };
	if (activ_scaled == 0)
	{
		scaledSize.setWidth(width());
		scaledSize.setHeight(height());
	}
	else
	{
		scaledSize.setWidth(originalSize.width() * _scaled[activ_scaled]);
		scaledSize.setHeight(originalSize.height() * _scaled[activ_scaled]);
	}
	_aspectRotMod = Qt::IgnoreAspectRatio;
	if (myPixmap_bufer != nullptr)
	{
		QPixmap scaledPixmap;
		scaledPixmap = my_PixmapOriginal.scaled(scaledSize, _aspectRotMod);
		delete myPixmap_bufer;
		myPixmap_bufer = new QPixmap(scaledPixmap);
		my_Pixmap = scaledPixmap;
		int buferFormat{ imageFormat };
		imageFormat = 0;
		formatImage(buferFormat);
		scaledSize.setHeight(scaledPixmap.height());
		scaledSize.setWidth(scaledPixmap.width());
	}
}

void myLabel::moveImg(int &out_dx, int &out_dy)
{
	int dx{first_x_labl-x_labl};
	first_x_labl = x_labl;
	int dy{ first_y_labl - y_labl };
	first_y_labl = y_labl;
	out_dx = drPoint.x() + dx;
	out_dy = drPoint.y() + dy;
}

void myLabel::getDrPoint(int &x, int &y)
{
	x = drPoint.x();
	y = drPoint.y();
}

void  myLabel::getMaxMinScal(int &max, int &min)
{
	max = max_activ_scaled;
	min = min_activ_scaled;
}

void myLabel::toCenterPoint(QPoint &upLeftPoint)
{
	//if(this->width()<scaledSize.width())
		upLeftPoint.setX(upLeftPoint.x() + (this->width()-1) / 2);
	/*else
		upLeftPoint.setX( (this->width() - 1) / 2);*/
	//if(this->height()<scaledSize.height())
		upLeftPoint.setY(upLeftPoint.y() + (this->height()-1) / 2);
	/*else
		upLeftPoint.setX((this->height() - 1) / 2);*/
}

void myLabel::toUpLeftpoint(QPoint &centerPoint)
{
	centerPoint.setY(centerPoint.y() - (this->height() - 1) / 2);
	/*if (centerPoint.y() < 0)
		centerPoint.setY(0);*/
	centerPoint.setX(centerPoint.x() - (this->width() - 1) / 2);
	/*if (centerPoint.x() < 0)
		centerPoint.setY(0);*/
}

void myLabel::toUpLeftpoint(int &x, int &y)
{
	y = y - (this->height() - 1) / 2;
	x = x - (this->width() - 1) / 2;
}

void myLabel::resizeEvent(QResizeEvent* event)
{
	oldSize = event->oldSize();
}

QPoint*  myLabel::getImgPoint(void)
{
	return &(QPoint(x_labl, y_labl));
}

QSize myLabel::getOldSize()
{
	return oldSize;
}

QSize* myLabel::getScaledImgSize()
{
	return &scaledSize;
}

QSize* myLabel::getOriginalImgSize()
{
	return &originalSize;
}

void myLabel::setAllImgScaled()
{
	double scal_NormImg{ (double)this->width() / (double)originalSize.width() };
	_scaled[11] = scal_NormImg;
	for (int i{ 0 }; i < 9; i++)
	{
		if (_scaled[i] > scal_NormImg)
		{
			max_activ_scaled = i;
			i = 10;
		}
		else
			max_activ_scaled = 10;
	}
	for (int i{ 9 }; i >= 0; i--)
	{
		if (_scaled[i] < scal_NormImg)
		{
			min_activ_scaled = i;
			i = -1;
		}
		else
			min_activ_scaled = 0;
	}
}

void myLabel::getPointInImg(int& x, int& y)
{
	x = x_labl;
	y = y_labl;
	toImgCoordinate(x,y);
}

QPixmap myLabel::getPixmapWithROI(std::vector<QRect> &Input) const
{
	QPixmap out{my_PixmapOriginal};
	QPainter *p = new QPainter(&out);
	p->setPen(QPen(Qt::red, 1, Qt::DashLine));
	for (int i{ 0 }; i < Input.size(); i++)
		p->drawRect(Input[i]);
	delete p;
	return out;
}

double myLabel::outForBoard_X(QtRotateRect* const InRect, double const in_X, bool const isX)
{
	double dx{ 0 };
	double dx_bufer{ 0 };
	bool reset_dx{ false };
	int X{ static_cast<int>((in_X)) };
	if (isX)
		X *= -1;
	if (InRect->getMax_X() + X > this->getScaledImgSize()->width() &&
		((InRect->getRotateAngel() >= 90 && InRect->getRotateAngel() < 270 && isX) ||
		(((InRect->getRotateAngel() >= 0 && InRect->getRotateAngel()< 90)|| (InRect->getRotateAngel() >= 270 && InRect->getRotateAngel() <= 360)) && !isX)))
	{
		dx = (cos(InRect->getRotateAngel(true)) * (this->getScaledImgSize()->width() - InRect->getMax_X()));
		if (!reset_dx)
		{
			dx_bufer = dx;
			reset_dx = true;
		}
		else if (abs(dx_bufer) < abs(dx))
			dx = dx_bufer;
	}
	if (InRect->getMax_Y() + static_cast<int>((X)) > this->getScaledImgSize()->height() && 
		((InRect->getRotateAngel() >= 180 && InRect->getRotateAngel() <= 360 && isX) || 
		(InRect->getRotateAngel() >= 0 && InRect->getRotateAngel() < 180 && !isX)))
	{
		dx = (sin(InRect->getRotateAngel(true)) * (this->getScaledImgSize()->height() - InRect->getMax_Y()));
		if (!reset_dx)
		{
			dx_bufer = dx;
			reset_dx = true;
		}
		else if (abs(dx_bufer) < abs(dx))
			dx = dx_bufer;
	}
	X *= -1;
	if (InRect->getMin_X() + static_cast<int>((X)) < 0 &&
		((((InRect->getRotateAngel() >= 270 && InRect->getRotateAngel() <= 360) || (InRect->getRotateAngel() >= 0 && InRect->getRotateAngel() < 90)) && isX) ||
		(InRect->getRotateAngel() >= 90 && InRect->getRotateAngel() < 270 && !isX)))
	{
		dx = (cos(InRect->getRotateAngel(true)) * (-InRect->getMin_X()));
		if (!reset_dx)
		{
			dx_bufer = dx;
			reset_dx = true;
		}
		else if (abs(dx_bufer) < abs(dx))
			dx = dx_bufer;
	}
	if (InRect->getMin_Y() + static_cast<int>((X)) < 0 && 
		((InRect->getRotateAngel() >= 0 && InRect->getRotateAngel() < 180 && isX) ||
		(InRect->getRotateAngel() >= 180 && InRect->getRotateAngel() <= 360 && !isX)))
	{
		dx = (sin(InRect->getRotateAngel(true)) * (-InRect->getMin_Y()));
		if (!reset_dx)
		{
			dx_bufer = dx;
			reset_dx = true;
		}
		else if (abs(dx_bufer) < abs(dx))
			dx = dx_bufer;
	}
	if (!reset_dx)
		return in_X;
	else
		return dx;
}

double myLabel::outForBoard_Y(QtRotateRect* const InRect, double const in_Y, bool const isY, bool const dy_isMinus)
{
	double dy{ 0 };
	double dy_bufer{ 0 };
	bool reset_dy{ false };
	int Y{ static_cast<int>((in_Y)) };
	if (dy_isMinus)
	{
		if (!isY)
			Y *= -1;
		if (InRect->getMax_X() + static_cast<int>((Y)) > this->getScaledImgSize()->width() &&
			((InRect->getRotateAngel() >= 0 && InRect->getRotateAngel() < 180 && isY) ||
			((InRect->getRotateAngel() >= 180 && InRect->getRotateAngel() <= 360) && !isY)))
		{
			dy = (sin(InRect->getRotateAngel(true)) * (this->getScaledImgSize()->width() - InRect->getMax_X()));
			if (!reset_dy)
			{
				dy_bufer = dy;
				reset_dy = true;
			}
			else if (abs(dy_bufer) < abs(dy))
				dy = dy_bufer;
		}
		if (InRect->getMax_Y() + static_cast<int>((Y)) > this->getScaledImgSize()->height() &&
			((InRect->getRotateAngel() >= 90 && InRect->getRotateAngel() < 270 && isY) ||
			(((InRect->getRotateAngel() >= 0 && InRect->getRotateAngel() < 90) || (InRect->getRotateAngel() >= 270 && InRect->getRotateAngel() <= 360)) && !isY)))
		{
			dy_bufer = (cos(InRect->getRotateAngel(true)) * (this->getScaledImgSize()->height() - InRect->getMax_Y()));
			if (!reset_dy)
			{
				dy_bufer = dy;
				reset_dy = true;
			}
			else if (abs(dy_bufer) < abs(dy))
				dy = dy_bufer;
		}
		Y *= -1;
		if (InRect->getMin_X() + static_cast<int>((in_Y)) < 0 &&
			((InRect->getRotateAngel() >= 180 && InRect->getRotateAngel() <= 360 && isY) ||
			((InRect->getRotateAngel() >= 0 && InRect->getRotateAngel() < 180) && !isY)))
		{
			dy = (sin(InRect->getRotateAngel(true)) * (-InRect->getMin_X()));
			if (!reset_dy)
			{
				dy_bufer = dy;
				reset_dy = true;
			}
			else if (abs(dy_bufer) < abs(dy))
				dy = dy_bufer;
		}
		if (InRect->getMin_Y() + static_cast<int>((in_Y)) < 0 &&
			((((InRect->getRotateAngel() >= 0 && InRect->getRotateAngel() < 90) || (InRect->getRotateAngel() >= 270 && InRect->getRotateAngel() <= 360)) && isY) ||
			((InRect->getRotateAngel() >= 90 && InRect->getRotateAngel() < 270) && !isY)))
		{
			dy_bufer = (cos(InRect->getRotateAngel(true)) * (-InRect->getMin_Y()));
			if (!reset_dy)
			{
				dy_bufer = dy;
				reset_dy = true;
			}
			else if (abs(dy_bufer) < abs(dy))
				dy = dy_bufer;
		}
	}
	else
	{
		if (isY)
			Y *= -1;
		if (InRect->getMax_X() + static_cast<int>((Y)) > this->getScaledImgSize()->width() &&
			((InRect->getRotateAngel() >= 0 && InRect->getRotateAngel() < 180 && isY) ||
			((InRect->getRotateAngel() >= 180 && InRect->getRotateAngel() <= 360) && !isY)))
		{
			dy_bufer = (cos(InRect->getRotateAngel(true)) * (this->getScaledImgSize()->width() - InRect->getMax_X()));
			if (!reset_dy)
			{
				dy_bufer = dy;
				reset_dy = true;
			}
			else if (abs(dy_bufer) < abs(dy))
				dy = dy_bufer;
		}
		if (InRect->getMax_Y() + static_cast<int>((Y)) > this->getScaledImgSize()->height() &&
			((InRect->getRotateAngel() >= 90 && InRect->getRotateAngel() < 270 && isY) ||
			(((InRect->getRotateAngel() >= 0 && InRect->getRotateAngel() < 90) || (InRect->getRotateAngel() >= 270 && InRect->getRotateAngel() <= 360)) && !isY)))
		{
			dy_bufer = (sin(InRect->getRotateAngel(true)) * (this->getScaledImgSize()->height() - InRect->getMax_Y()));
			if (!reset_dy)
			{
				dy_bufer = dy;
				reset_dy = true;
			}
			else if (abs(dy_bufer) < abs(dy))
				dy = dy_bufer;
		}
		Y*= -1;
		if (InRect->getMin_X() + static_cast<int>((Y)) < 0 &&
			((InRect->getRotateAngel() >= 180 && InRect->getRotateAngel() <= 360 && isY) ||
			((InRect->getRotateAngel() >= 0 && InRect->getRotateAngel() < 180) && !isY)))
		{
			dy_bufer = (cos(InRect->getRotateAngel(true)) * (-InRect->getMin_X()));
			if (!reset_dy)
			{
				dy_bufer = dy;
				reset_dy = true;
			}
			else if (abs(dy_bufer) < abs(dy))
				dy = dy_bufer;
		}
		if (InRect->getMin_Y() + static_cast<int>((Y)) < 0 &&
			((((InRect->getRotateAngel() >= 0 && InRect->getRotateAngel() < 90) || (InRect->getRotateAngel() >= 270 && InRect->getRotateAngel() <= 360)) && isY) ||
			((InRect->getRotateAngel() >= 90 && InRect->getRotateAngel() < 270) && !isY)))
		{
			dy_bufer = (sin(InRect->getRotateAngel(true)) * (-InRect->getMin_Y()));
			if (!reset_dy)
			{
				dy_bufer = dy;
				reset_dy = true;
			}
			else if (abs(dy_bufer) < abs(dy))
				dy = dy_bufer;
		}
	}
	if ((in_Y >= 0 && dy >= 0) || (in_Y < 0 && dy < 0)) {
	}
	else
	{
		dy *= -1;
	}
	if (!reset_dy)
		return in_Y;
	else
		return dy;
}

//double round(double InputNumber, int const accuracy)
//{
//	InputNumber *= pow(10, accuracy + 1);
//	InputNumber = static_cast<int>(round(InputNumber));
//	return static_cast<double>(InputNumber / pow(10, accuracy));
//}