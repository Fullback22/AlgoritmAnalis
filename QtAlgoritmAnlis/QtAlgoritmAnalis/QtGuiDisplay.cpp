#include "QtGuiDisplay.h"

QtGuiDisplay::QtGuiDisplay(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	activ_scaled = 100.0;
	event_img = false;
	change_roi = false;
	activatedRoi = true;
	changeActivArea = false;
	activ_roi = -2;
	lastActiv = -2;
	setObj = false;
	isProcessingActiv = false;
	changesProcesedArears = true;
	ui.horSB_forTempImg->hide();
	ui.verSB_forTempImg->hide();
	this->updateGeometry();
	connect(ui.horSB_forTempImg, SIGNAL(valueChanged(int)), this, SLOT(slot_mouvePixmap()));
	connect(ui.verSB_forTempImg, SIGNAL(valueChanged(int)), this, SLOT(slot_mouvePixmap()));
	connect(ui.pushButt_ZoomIncress, SIGNAL(clicked()), this, SLOT(slot_ZoomImg_In()));
	connect(ui.pushButt_ZoomDeduce, SIGNAL(clicked()), this, SLOT(slot_ZoomImg_Out()));
	connect(ui.pushButt_AllLabl, SIGNAL(clicked()), this, SLOT(slot_ZoomImg_AllLabl()));
	//connect(ui.pushButton_DirToSave, SIGNAL(clicked()), this, SLOT(slot_SetDirToSave()));
	//connect(ui.pushButt_SavImg_2, SIGNAL(clicked()), this, SLOT(slot_saveImg()));
	connect(ui.pushButt_Skrean, SIGNAL(clicked()), this, SLOT(slot_saveImg()));

	connect(ui.label_for_TempImg, SIGNAL(mousePos()), this, SLOT(slot_mouseCurrentPos()));
	connect(ui.label_for_TempImg, SIGNAL(mousePressed()), this, SLOT(slot_mousePressed()));
	connect(ui.label_for_TempImg, SIGNAL(mouseRelease()), this, SLOT(slot_mouseRelease()));
	connect(ui.label_for_TempImg, SIGNAL(mouseLeft()), this, SLOT(slot_mouseLeft()));
	connect(ui.label_for_TempImg, SIGNAL(mouseLeftMouveRoi(int)), this, SLOT(slot_mouseLeft(int)));
	connect(ui.PB_writeVideo, SIGNAL(clicked()), this, SLOT(slot_startWriteVideo()));
	//connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_changeProcssActiv(int)));

}

QtGuiDisplay::~QtGuiDisplay()
{
}

void QtGuiDisplay::setSizeScrollBar()
{
	//if (!tempImg.empty())
	//{
		if (ui.label_for_TempImg->myPixmap().width() > ui.label_for_TempImg->width())
		{
			ui.horSB_forTempImg->show();
			ui.horSB_forTempImg->setRange(0, std::abs(ui.label_for_TempImg->getScaledImgSize()->width() - ui.label_for_TempImg->width()));
		}
		else
		{
			ui.horSB_forTempImg->hide();
		}
		if (ui.label_for_TempImg->myPixmap().height() > ui.label_for_TempImg->height())
		{
			ui.verSB_forTempImg->show();
			ui.verSB_forTempImg->setRange(0, std::abs(ui.label_for_TempImg->getScaledImgSize()->height() - ui.label_for_TempImg->height()));
		}
		else
		{
			ui.verSB_forTempImg->hide();
		}
	//}
}

void QtGuiDisplay::slot_mouvePixmap()
{
	if (activ)
		ui.label_for_TempImg->show_partImg(ui.horSB_forTempImg->value(), ui.verSB_forTempImg->value(), ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
}

void QtGuiDisplay::slot_mouseCurrentPos()
{
	if (changesProcesedArears )
	{
		if (activProcessedObj.getProcesArears()->size() > 0)
		{
			if (!change_roi)
			{
				activ_roi = -2;
				int min_roi{ -1 };
				for (int i{ 0 }; i < activProcessedObj.getProcesArears()->size(); i++)
				{
					if (activProcessedObj.getProcesArears()[0][i].isActiv() && activProcessedObj.getProcesArears()[0][i].isDraw())
					{
						int x, y;
						ui.label_for_TempImg->getPointInImg(x, y);
						if (activProcessedObj.getProcesArears()[0][i].getScalRect()->contains(x, y) && activProcessedObj.getProcesArears()[0][i].getAreaType() == 0)  //check whether the pointer islocated in activ ROI
						{//selecting a activ roi
							//if (min_roi == -1)
							//{
							min_roi = i;
							//}
							/*else if (activProcessedObj->getProcesArears()[0][i].getArea(true) < (activProcessedObj->getProcesArears()[0][min_roi].getArea(true)))
							{
								min_roi = i;
							}*/
							activ_roi = min_roi;
							myCursor.setShape(Qt::SizeAllCursor);
							this->setCursor(myCursor);
						}
						else
							if (!activProcessedObj.getProcesArears()[0][i].getScalRect()->contains(x, y))
							{
								//QtRotateRect secondRect(QRect(0,0,0,0),0);
								double rotAngel{ activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel() };
								//QtRotateRect secondRect{ QRect(activProcessedObj->getProcesArears()[0][i].getScalRect()->x() - 7,activProcessedObj->getProcesArears()[0][i].getScalRect()->y() - 7,
								//	activProcessedObj->getProcesArears()[0][i].getScalRect()->width() + 14,activProcessedObj->getProcesArears()[0][i].getScalRect()->height() + 14),activProcessedObj->getProcesArears()[0][i].getScalRect()->getRotateAngel() };
								double diag{ sqrt(2 * 7 * 7) };
								if (((QtRotateRect(activProcessedObj.getProcesArears()[0][i].getScalRect()->getUpLeft_X() + 7 * sin(activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel(true)), activProcessedObj.getProcesArears()[0][i].getScalRect()->getUpLeft_Y() - 7 * cos(activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel(true)), activProcessedObj.getProcesArears()[0][i].getScalRect()->width(), 8, activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel()).contains(x, y) ||
									QtRotateRect(activProcessedObj.getProcesArears()[0][i].getScalRect()->getDownLeft_X(), activProcessedObj.getProcesArears()[0][i].getScalRect()->getDownLeft_Y(), activProcessedObj.getProcesArears()[0][i].getScalRect()->width(), 8, activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel()).contains(x, y)) && activProcessedObj.getProcesArears()[0][i].getAreaType() == 0))
								{
									//if (min_roi == -1)
									//{
									min_roi = i;
									//}
									//else if (activProcessedObj->getProcesArears()[0][i].getArea(true) < (activProcessedObj->getProcesArears()[0][min_roi].getArea(true)))
									//{
									//	min_roi = i;
									//}
									activ_roi = min_roi;
									if (((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5)) || (rotAngel >= 157.5 && rotAngel < 202.5))
										myCursor.setShape(Qt::SizeVerCursor);
									else if ((rotAngel >= 67.5 && rotAngel < 112.5) || (rotAngel >= 247.5 && rotAngel < 292.5))
										myCursor.setShape(Qt::SizeHorCursor);
									else if ((rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 202.5 && rotAngel < 247.5))
										myCursor.setShape(Qt::SizeBDiagCursor);
									else if ((rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 292.5 && rotAngel < 337.5))
										myCursor.setShape(Qt::SizeFDiagCursor);
									this->setCursor(myCursor);
								}
								else if (((QtRotateRect(activProcessedObj.getProcesArears()[0][i].getScalRect()->getUpLeft_X() - 7 * sin((90 - activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel()) * 3.14 / 180), activProcessedObj.getProcesArears()[0][i].getScalRect()->getUpLeft_Y() - 7 * cos((90 - activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel()) * 3.14 / 180), 8, activProcessedObj.getProcesArears()[0][i].getScalRect()->height(), activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel()).contains(x, y) ||
									QtRotateRect(activProcessedObj.getProcesArears()[0][i].getScalRect()->getUpRigth_X(), activProcessedObj.getProcesArears()[0][i].getScalRect()->getUpRigth_Y(), 8, activProcessedObj.getProcesArears()[0][i].getScalRect()->height(), activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel()).contains(x, y)) && activProcessedObj.getProcesArears()[0][i].getAreaType() == 0))
								{
									//if (min_roi == -1)
									//{
									//	min_roi = i;
									//}
									//else if (activProcessedObj->getProcesArears()[0][i].getArea(true) < (activProcessedObj->getProcesArears()[0][min_roi].getArea(true)))
									//{
									min_roi = i;
									//}
									activ_roi = min_roi;
									if ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 157.5 && rotAngel < 202.5))
										myCursor.setShape(Qt::SizeHorCursor);
									else if ((rotAngel >= 67.5 && rotAngel < 112.5) || (rotAngel >= 247.5 && rotAngel < 292.5))
										myCursor.setShape(Qt::SizeVerCursor);
									else if ((rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 202.5 && rotAngel < 247.5))
										myCursor.setShape(Qt::SizeFDiagCursor);
									else if ((rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 292.5 && rotAngel < 337.5))
										myCursor.setShape(Qt::SizeBDiagCursor);
									this->setCursor(myCursor);
								}
								else if (((QtRotateRect(activProcessedObj.getProcesArears()[0][i].getScalRect()->getUpLeft_X() - diag * cos((45 + activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel()) * 3.14 / 180), activProcessedObj.getProcesArears()[0][i].getScalRect()->getUpLeft_Y() - diag * sin((45 + activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel()) * 3.14 / 180), 9, 9, activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel()).contains(x, y) ||
									QtRotateRect(activProcessedObj.getProcesArears()[0][i].getScalRect()->getDownRigth_X() - 1, activProcessedObj.getProcesArears()[0][i].getScalRect()->getDownRigth_Y() - 1, 9, 9, activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel()).contains(x, y)) && activProcessedObj.getProcesArears()[0][i].getAreaType() == 0))
								{
									//if (min_roi == -1)
									//{
									//	min_roi = i;
									//}
									//else if (activProcessedObj->getProcesArears()[0][i].getArea(true) < (activProcessedObj->getProcesArears()[0][min_roi].getArea(true)))
									//{
									min_roi = i;
									//}
									activ_roi = min_roi;
									if ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 157.5 && rotAngel < 202.5))
										myCursor.setShape(Qt::SizeFDiagCursor);
									else if ((rotAngel >= 67.5 && rotAngel < 112.5) || (rotAngel >= 247.5 && rotAngel < 292.5))
										myCursor.setShape(Qt::SizeBDiagCursor);
									else if ((rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 202.5 && rotAngel < 247.5))
										myCursor.setShape(Qt::SizeVerCursor);
									else if ((rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 292.5 && rotAngel < 337.5))
										myCursor.setShape(Qt::SizeHorCursor);
									this->setCursor(myCursor);
								}
								else if (((QtRotateRect(activProcessedObj.getProcesArears()[0][i].getScalRect()->getUpRigth_X() + 6 * sin(activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel(true)), activProcessedObj.getProcesArears()[0][i].getScalRect()->getUpRigth_Y() - 8 * cos(activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel(true)), 8, 9, activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel()).contains(x, y) ||
									QtRotateRect(activProcessedObj.getProcesArears()[0][i].getScalRect()->getDownLeft_X() - 7 * cos(activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel(true)), activProcessedObj.getProcesArears()[0][i].getScalRect()->getDownLeft_Y() - 7 * sin(activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel(true)), 8, 8, activProcessedObj.getProcesArears()[0][i].getScalRect()->getRotateAngel()).contains(x, y)) && activProcessedObj.getProcesArears()[0][i].getAreaType() == 0))
								{
									//if (min_roi == -1)
									//{
									//	min_roi = i;
									//}
									//else if (activProcessedObj->getProcesArears()[0][i].getArea(true) < (activProcessedObj->getProcesArears()[0][min_roi].getArea(true)))
									//{
									min_roi = i;
									//}
									activ_roi = min_roi;
									if ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 157.5 && rotAngel < 202.5))
										myCursor.setShape(Qt::SizeBDiagCursor);
									else if ((rotAngel >= 67.5 && rotAngel < 112.5) || (rotAngel >= 247.5 && rotAngel < 292.5))
										myCursor.setShape(Qt::SizeFDiagCursor);
									else if ((rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 202.5 && rotAngel < 247.5))
										myCursor.setShape(Qt::SizeHorCursor);
									else if ((rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 292.5 && rotAngel < 337.5))
										myCursor.setShape(Qt::SizeVerCursor);
									this->setCursor(myCursor);
								}
							}
					}
				}
				if (activ_roi == -2)
				{
					myCursor.setShape(Qt::ArrowCursor);
					this->setCursor(myCursor);
				}
			}
			else
			{
				if (this->myCursor.shape() == Qt::SizeAllCursor)
				{
					if (activProcessedObj.getProcesArears()[0][activ_roi].getAreaType() == 0)
						ui.label_for_TempImg->muve_roiRect(*(activProcessedObj.getProcesArears()[0][activ_roi].getScalRect()));
				}
				else
					if (this->myCursor.shape() == Qt::SizeHorCursor || this->myCursor.shape() == Qt::SizeVerCursor ||
						this->myCursor.shape() == Qt::SizeBDiagCursor || this->myCursor.shape() == Qt::SizeFDiagCursor)
					{
						if (activProcessedObj.getProcesArears()[0][activ_roi].getAreaType() == 0)
						{
							ui.label_for_TempImg->resize_rect(*(activProcessedObj.getProcesArears()[0][activ_roi].getScalRect()));
							//ui.label_Scale->setText(QString::number(activProcessedObj->getProcesArears()[0][activ_roi].getScalRect()->getMin_Y()));
						}
					}
				processedAreaScale(activProcessedObj.getProcesArears()[0][activ_roi], true);
				//activProcessedObj->getProcesArears()[0][activ_roi].createMaster(&(activProcessedObj->getROI(activProcessedObj->getProcesArears()[0][activ_roi].getRect()->getRotateRectSize())));
				draw_proceseArears();
				int x, y;
				ui.label_for_TempImg->getDrPoint(x, y);
				ui.label_for_TempImg->show_partImg(x, y, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
				///
				///
				///////////funk(QtAreaToProcessed((activProcessedObj->getProcesArears()[0][activ_roi]));
				///
				///
			}
		}
	}
	if (event_img)
	{
		int dr_x, dr_y;
		ui.label_for_TempImg->moveImg(dr_x, dr_y);
		ui.label_for_TempImg->show_partImg(dr_x, dr_y, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
		ui.horSB_forTempImg->setSliderPosition(dr_x);
		ui.verSB_forTempImg->setSliderPosition(dr_y);
	}
}

void QtGuiDisplay::slot_mousePressed()
{
	if (activ_roi != -2 && changesProcesedArears)
	{
		event_img = false;
		if (this->myCursor.shape() == Qt::SizeAllCursor|| this->myCursor.shape() == Qt::SizeHorCursor || this->myCursor.shape() == Qt::SizeVerCursor ||
			this->myCursor.shape() == Qt::SizeBDiagCursor || this->myCursor.shape() == Qt::SizeFDiagCursor)
		{
			change_roi = true;
			lastActiv = activ_roi;
		}
		//event_roi = true;
	}
	else if(changeActivArea)
	{
		event_img = true;
		int min_roi{ -1 };
		for (int i{ 0 }; i < activProcessedObj.getProcesArears()->size(); i++)
		{
			if (activProcessedObj.getProcesArears()[0][i].isDraw())
			{
				int x, y;
				ui.label_for_TempImg->getPointInImg(x, y);
				if ((activProcessedObj.getProcesArears()[0][i].getScalRect()->contains(x, y) && activProcessedObj.getProcesArears()[0][i].getAreaType() == 0))  //check whether the pointer islocated in activ ROI
				{//selecting a activ roi
					if (min_roi == -1)
					{
						min_roi = i;
					}
					else if (activProcessedObj.getProcesArears()[0][i].getArea(true) < (activProcessedObj.getProcesArears()[0][min_roi].getArea(true)))
					{
						min_roi = i;
					}
				}
				else if (activProcessedObj.getProcesArears()[0][i].getAreaType() == 2 && min_roi == -1)
				{
					min_roi = i;
				}
			}
		}
		for (int i{ 0 }; i < activProcessedObj.getProcesArears()->size(); i++)
		{
			if (min_roi != -1)
			{
				if (i == min_roi)
				{
					emit changeActivProcesArea(i);
					activProcessedObj.getProcesArears()[0][i].setActiv(true);
					if (changesProcesedArears)
					{
						myCursor.setShape(Qt::SizeAllCursor);
						this->setCursor(myCursor);
						change_roi = true;
					}
					activ_roi = i;
					event_img = false;
				}
				else
				{
					activProcessedObj.getProcesArears()[0][i].setActiv(false);
				}
			}
		}
		//draw_proceseArears();
		//ui.label_for_TempImg->show_partImg();
	}
	else
	{
		event_img = true;
	}
}

void QtGuiDisplay::slot_mouseLeft()
{
	//if (addNewBrigthnesRect || change_roi)
	if ( change_roi)
	{
		int dr_x, dr_y;
		ui.label_for_TempImg->getDrPoint(dr_x, dr_y);
		QPoint imgPoint(*(ui.label_for_TempImg->getImgPoint()));

		if (imgPoint.x() >= ui.label_for_TempImg->width() - 1)
		{
			ui.horSB_forTempImg->setSliderPosition(dr_x + 1);
		}
		else if (imgPoint.x() <= 0)
		{
			ui.horSB_forTempImg->setSliderPosition(dr_x - 1);
		}

		if (imgPoint.y() >= ui.label_for_TempImg->height() - 1)
		{
			ui.verSB_forTempImg->setSliderPosition(dr_y + 1);
		}
		else if (imgPoint.y() <= 0)
		{
			ui.verSB_forTempImg->setSliderPosition(dr_y - 1);
		}
	}
	else
	{
		myCursor.setShape(Qt::ArrowCursor);
		this->setCursor(myCursor);
	}
}

void QtGuiDisplay::slot_mouseLeft(int direct)
{
	if (change_roi)
	{
		int dr_x, dr_y;
		ui.label_for_TempImg->getDrPoint(dr_x, dr_y);
		QPoint imgPoint(*(ui.label_for_TempImg->getImgPoint()));

		if (direct==1)
		{
			ui.horSB_forTempImg->setSliderPosition(dr_x + 3);
		}
		else if (direct == 0)
		{
			ui.horSB_forTempImg->setSliderPosition(dr_x - 3);
		}

		if (direct==3)
		{
			ui.verSB_forTempImg->setSliderPosition(dr_y + 3);
		}
		else if (direct==2)
		{
			ui.verSB_forTempImg->setSliderPosition(dr_y - 3);
		}
	}
}

void QtGuiDisplay::slot_mouseRelease()
{
	/*if (addNewBrigthnesRect && activProcessedObj->brightnesCorrectAreaIsSet())
	{
		addNewBrigthnesRect = false;
		processedAreaScale(activProcessedObj->getProcesArears()[0][0],true);
		emit brightnesCorrectRectSet(true);
	}*/
	event_img = false;
	change_roi = false;
	//ui.label_forMainImg->show_roi(roi);
}

void QtGuiDisplay::slot_ZoomImg_In()
{
	emit clic_pb();
	int dr_x, dr_y;
	activ_scaled = ui.label_for_TempImg->scaledPixmap(1, dr_x, dr_y);
	if (activ_scaled == 500)
	{
		ui.pushButt_ZoomIncress->setEnabled(false);
	}
	else
	{
		ui.pushButt_ZoomIncress->setEnabled(true);
	}
	ui.label_Scale->setText(QString::number(activ_scaled) + "%");
	ui.pushButt_ZoomDeduce->setEnabled(true);
	this->setSizeScrollBar();
	ui.label_for_TempImg->scaledPixmap();
	this->setSizeScrollBar();
	ui.horSB_forTempImg->setValue(dr_x);
	ui.verSB_forTempImg->setValue(dr_y);
	if (!ui.pushButt_AllLabl->isEnabled())
		ui.pushButt_AllLabl->setEnabled(true);
	/*int i{ 0 };
	if (!activProcessedObj->brightnesCorrectAreaIsSet())
		i = 1;*/
	/*if (activProcessedObj != nullptr)
	{*/
		for (int i{ 0 }; i < activProcessedObj.getProcesArears()->size(); ++i)
		{
			processedAreaScale(activProcessedObj.getProcesArears()[0][i]);
		}
		draw_proceseArears();
	//}
	ui.label_for_TempImg->show_partImg(dr_x, dr_y, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
}

void QtGuiDisplay::slot_ZoomImg_Out()
{
	emit clic_pb();
	ui.horSB_forTempImg->hide();
	ui.verSB_forTempImg->hide();
	int dr_x, dr_y;
	activ_scaled = ui.label_for_TempImg->scaledPixmap(-1, dr_x, dr_y);

	if (activ_scaled == 25)
	{
		ui.pushButt_ZoomDeduce->setEnabled(false);
	}
	else
	{
		ui.pushButt_ZoomDeduce->setEnabled(true);
	}
	
	ui.label_Scale->setText(QString::number(activ_scaled, 'f', 1) + "%");
	ui.pushButt_ZoomIncress->setEnabled(true);
	this->setSizeScrollBar();
	ui.label_for_TempImg->scaledPixmap();
	this->setSizeScrollBar();
	ui.horSB_forTempImg->setValue(dr_x);
	ui.verSB_forTempImg->setValue(dr_y);
	if (!ui.pushButt_AllLabl->isEnabled())
		ui.pushButt_AllLabl->setEnabled(true);
	for (int i{ 0 }; i < activProcessedObj.getProcesArears()->size(); ++i)
	{
		processedAreaScale(activProcessedObj.getProcesArears()[0][i]);
	}
	draw_proceseArears();
	ui.label_for_TempImg->show_partImg(dr_x, dr_y, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
}

void QtGuiDisplay::slot_ZoomImg_AllLabl()
{	
	emit clic_pb();
	ui.horSB_forTempImg->hide();
	ui.verSB_forTempImg->hide();
	this->updateGeometry();
	int max, min;
	activ_scaled = ui.label_for_TempImg->scaledPixmap(0, max, min);
	ui.label_Scale->setText(QString::number(activ_scaled,'f',1) + "%");
	ui.label_for_TempImg->getMaxMinScal(max, min);
	if (max<10)
		ui.pushButt_ZoomIncress->setEnabled(true);
	else
		ui.pushButt_ZoomIncress->setEnabled(false);
	if (min>0)
		ui.pushButt_ZoomDeduce->setEnabled(true);
	else
		ui.pushButt_ZoomDeduce->setEnabled(false);
	ui.pushButt_AllLabl->setEnabled(false);
	/*int i{ 0 };
	if (!activProcessedObj->brightnesCorrectAreaIsSet())
		i = 1;*/
	/*if (activProcessedObj != nullptr)
	{*/
		for (int i{ 0 }; i < activProcessedObj.getProcesArears()->size(); ++i)
		{
			processedAreaScale(activProcessedObj.getProcesArears()[0][i]);
		}
		draw_proceseArears();
	//}
	ui.label_for_TempImg->show_partImg(0, 0, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
}

void QtGuiDisplay::slot_SetDirToSave()
{
	activProcessedObj.SetDirName(QFileDialog::getExistingDirectory(this, "D:/"));
	emit clic_pb();
}

void QtGuiDisplay::slot_saveImg()
{
	time_t now = time(0);
	tm* dt = localtime(&now);
	QImage out{ ui.label_for_TempImg->getBuferPixmap()->toImage() };
	out.save("screenshots/skreen" + QString::number(dt->tm_mday) + QString::number(dt->tm_mon) + QString::number(dt->tm_year) + QString::number(dt->tm_hour) + QString::number(dt->tm_min) + QString::number(dt->tm_sec) + ".jpg");
}

void QtGuiDisplay::setActivProcessObj(ProcessedObj *activObj, bool master, int number)
{
	this->updateGeometry();
	//activProcessedObj = activObj;
	ui.label_for_TempImg->setAlignment(Qt::AlignCenter);
	/*if (master)
	{
		ui.label_for_TempImg->set_myPixmap(activObj->getPixmap());
	}
	else
	if (number < activObj->getTestVecSize())
	{
		ui.label_for_TempImg->set_myPixmap(activObj->getTestPixmap(number));
	}
	else
	{
		ui.label_for_TempImg->set_myPixmap(QPixmap("NoImg.png"));
	}*/
	this->slot_ZoomImg_AllLabl();
	if (activProcessedObj.imgIsLoaded())
	{
		ui.pushButt_ZoomDeduce->show();
		ui.pushButt_ZoomIncress->show();
		ui.pushButt_AllLabl->show();
		ui.label_Scale->show();
		//ui.pushButton_DirToSave->show();
		ui.pushButt_Skrean->show();
		/*if (activatedRoi)
			ui.comboBox->show();
		else
			ui.comboBox->hide();*/
	}
	else
	{
		ui.pushButt_ZoomDeduce->hide();
		ui.pushButt_ZoomIncress->hide();
		ui.pushButt_AllLabl->hide();
		ui.label_Scale->hide();
		//ui.comboBox->hide();
		//ui.pushButton_DirToSave->hide();
		ui.pushButt_Skrean->hide();
	}
}

void QtGuiDisplay::slot_reNameImg(QString newFileName)
{
	activProcessedObj.SetFileName(newFileName);
}

void QtGuiDisplay::slot_brighAreaDel()
{
	//activProcessedObj->deletBrightnesCorrectArea();
	//updateImg();
}

void QtGuiDisplay::setEanbleActivededRoi(bool activ)
{
	/*if (activ)
	{
		activatedRoi = true;
		ui.comboBox->show();
	}
	else
	{
		activatedRoi = false;
		ui.comboBox->hide();
	}*/
}

void QtGuiDisplay::setEnableWidtsGrouBox(bool enable)
{
	if (enable)
	{
		ui.pushButt_ZoomDeduce->show();
		ui.pushButt_ZoomIncress->show();
		ui.pushButt_AllLabl->show();
		ui.label_Scale->show();
		//ui.pushButton_DirToSave->show();
		ui.pushButt_Skrean->show();
		/*if (activatedRoi)
			ui.comboBox->show();
		else
			ui.comboBox->hide();*/
	}
	else
	{
		ui.pushButt_ZoomDeduce->hide();
		ui.pushButt_ZoomIncress->hide();
		ui.pushButt_AllLabl->hide();
		ui.label_Scale->hide();
		//ui.comboBox->hide();
		//ui.pushButton_DirToSave->hide();
		ui.pushButt_Skrean->hide();
	}
}

void QtGuiDisplay::setActiv(bool activ)
{
	this->activ = activ;
	if (activ)
	{
		ui.label_for_TempImg->setStyleSheet("background-color: red");
		//this->setActivProcessObj(activProcessedObj);
	}
	else
	{
		setEnableWidtsGrouBox(false);
		ui.label_for_TempImg->setText(" ");
		ui.label_for_TempImg->setStyleSheet("background-color: black");
		ui.horSB_forTempImg->hide();
		ui.verSB_forTempImg->hide();
		ui.label_for_TempImg->setPixmap(QPixmap());
	}
}

void QtGuiDisplay::setChangeActivArea(bool isChange)
{
	changeActivArea = isChange;
}

void QtGuiDisplay::addBrightnesCorrectRect(bool isAdd)
{
	//addNewBrigthnesRect = isAdd;
}

void QtGuiDisplay::setChangesProcessedArears(bool isChang)
{
	changesProcesedArears = isChang;
}

void QtGuiDisplay::processedAreaScale(QtProcessedArea& InOutArea, bool toOriginalScal)
{
	QtRotateRect bufer{};
	QtRotateRect buferScal{};
	int buferSize{0};
	if (toOriginalScal)
	{
		if (InOutArea.getAreaType() == 0)
		{
			bufer = *InOutArea.getScalRect();
			double mul{ static_cast<double>(ui.label_for_TempImg->getOriginalImgSize()->width()) / static_cast<double>(ui.label_for_TempImg->getScaledImgSize()->width()) };
			double point_X{ (static_cast<double>(bufer.getMin_X()) + static_cast<double>(bufer.getMax_X() - bufer.getMin_X()) / 2 + 1) * mul};
			double point_Y{ (static_cast<double>(bufer.getMin_Y()) + static_cast<double>(bufer.getMax_Y() - bufer.getMin_Y()) / 2 + 1) * mul};
			buferSize = static_cast<int>(ceil(bufer.width() * mul));
			bufer.setX(bufer.x() * mul);
			bufer.setWidth(buferSize);

			mul = static_cast<double>(ui.label_for_TempImg->getOriginalImgSize()->height()) / static_cast<double>(ui.label_for_TempImg->getScaledImgSize()->height());
			buferSize = static_cast<int>(ceil(bufer.height() * mul));
			bufer.setY(bufer.y() * mul);
			bufer.setHeight(buferSize);
			InOutArea.setRect(&QtRotateRect(QRect(bufer.x(), bufer.y(), bufer.width(), bufer.height()), InOutArea.getScalRect()->getRotateAngel(), QPoint(point_X, point_Y)));
		}
	}
	else
	{
		bufer = *InOutArea.getRect();
		buferScal = *InOutArea.getScalRect();
		if (ui.pushButt_AllLabl->isEnabled())
		{
			if (InOutArea.getAreaType() == 0)
			{
				double point_X{ (static_cast<double>(bufer.getMin_X()) + static_cast<double>(bufer.getMax_X() - bufer.getMin_X()) / 2 + 1)* activ_scaled / 100 };
				double point_Y{ (static_cast<double>(bufer.getMin_Y()) + static_cast<double>(bufer.getMax_Y() - bufer.getMin_Y()) / 2 + 1) * activ_scaled / 100};
				buferSize = static_cast<int>(ceil(bufer.width() * activ_scaled / 100));
				bufer.setX(bufer.x() * activ_scaled / 100);
				bufer.setWidth(buferSize);

				buferSize = static_cast<int>(ceil(bufer.height() * activ_scaled / 100));
				bufer.setY(bufer.y() * activ_scaled / 100);
				bufer.setHeight(buferSize);
				InOutArea.setScalRect(&QtRotateRect(QRect(bufer.x(), bufer.y(), bufer.width(), bufer.height()), InOutArea.getScalRect()->getRotateAngel(), QPoint(point_X, point_Y)));
			}
		}
		else
		{
			if (InOutArea.getAreaType() == 0)
			{
				double point_X{ (static_cast<double>(bufer.getMin_X()) + static_cast<double>(bufer.getMax_X() - bufer.getMin_X()) / 2 + 1) };
				double point_Y{ (static_cast<double>(bufer.getMin_Y()) + static_cast<double>(bufer.getMax_Y() - bufer.getMin_Y()) / 2 + 1) };

				hor_scaled = static_cast<double>(ui.label_for_TempImg->getOriginalImgSize()->width()) / static_cast<double>(ui.label_for_TempImg->getScaledImgSize()->width());
				int buferSize{ static_cast<int>(ceil(bufer.width() / hor_scaled)) };
				bufer.setX(bufer.x() / hor_scaled);
				bufer.setWidth(buferSize);

				vert_scaled = static_cast<double>(ui.label_for_TempImg->getOriginalImgSize()->height()) / static_cast<double>(ui.label_for_TempImg->getScaledImgSize()->height());
				buferSize = static_cast<int>(ceil(bufer.height() / vert_scaled));
				bufer.setY(bufer.y() / vert_scaled);
				bufer.setHeight(buferSize);

				bufer.setRotateAngel(InOutArea.getRect()->getRotateAngel(), &QPoint(point_X / hor_scaled, point_Y / vert_scaled));
				InOutArea.setScalRect(&bufer);
			}
		}
	}
}

void QtGuiDisplay::draw_proceseArears()
{
	bool noDraw{ true };
	for (int i{ 0 }; i < activProcessedObj.getProcesArears()->size(); ++i)
	{
		if (activProcessedObj.getProcesArears()[0][i].isDraw())
		{
			int penSize{ 1 };
			if (i==lastActiv)
				penSize = 2;
			QPen penBufer(Qt::red, ceil(penSize * activ_scaled / 100), Qt::DashLine);
			if ((activProcessedObj.getProcesArears()[0])[i].getProcesseedType() == 0)
				penBufer.setColor(Qt::green);
			if ((activProcessedObj.getProcesArears()[0])[i].getAreaType() == 0)
			{
				ui.label_for_TempImg->draw_rect(activProcessedObj.getProcesArears()[0][i].getScalRect(), penBufer);
				noDraw = false;
			}
		}
	}
	if (noDraw)
	{
		QRect QR(0, 0, 0, 0);
		ui.label_for_TempImg->draw_rect(&QtRotateRect(QR));
	}
}

void QtGuiDisplay::add_rect(int procesType)
{
	int w{ 0 };
	int h{ 0 };
	if (ui.label_for_TempImg->getScaledImgSize()->width() < ui.label_for_TempImg->size().width())
	{
		w = ui.label_for_TempImg->getScaledImgSize()->width() / 4;
	}
	else
	{
		w = ui.label_for_TempImg->size().width() / 4;
	}
	if (ui.label_for_TempImg->getScaledImgSize()->height() < ui.label_for_TempImg->size().height())
	{
		h = ui.label_for_TempImg->getScaledImgSize()->height() / 4;
	}
	else
	{
		h = ui.label_for_TempImg->size().height() / 4;
	}
	int x{ ui.label_for_TempImg->size().width() / 2 };
	int y{ ui.label_for_TempImg->size().height() / 2 };
	ui.label_for_TempImg->toImgCoordinate(x, y);
	activProcessedObj.getProcesArears()[0].push_back((QtProcessedArea(procesType, 0, QtRotateRect(QRect(x - w / 2, y - h / 2, w, h)))));
	processedAreaScale(activProcessedObj.getProcesArears()[0][activProcessedObj.getProcesArears()[0].size() - 1], true);
	setObj = true;
	//activProcessedObj->getProcesArears()[0][activProcessedObj->getProcesArears()[0].size() - 1].createMaster(&activProcessedObj->getMat());
	updateImg();
}

void QtGuiDisplay::deleteActivRect()
{
	if (lastActiv != -2)
	{
		activProcessedObj.deleteProcesArea(lastActiv);
		lastActiv = -2;
	}
}

void QtGuiDisplay::changeAreaType(int newType, QtProcessedArea& InOutArea)
{
	if (newType == 0)
	{
		int w{ 0 };
		int h{ 0 };
		if (ui.label_for_TempImg->getScaledImgSize()->width() < ui.label_for_TempImg->size().width())
		{
			w = ui.label_for_TempImg->getScaledImgSize()->width() / 4;
		}
		else
		{
			w = ui.label_for_TempImg->size().width() / 4;
		}
		if (ui.label_for_TempImg->getScaledImgSize()->height() < ui.label_for_TempImg->size().height())
		{
			h = ui.label_for_TempImg->getScaledImgSize()->height() / 4;
		}
		else
		{
			h = ui.label_for_TempImg->size().height() / 4;
		}
		int x{ ui.label_for_TempImg->size().width() / 2 };
		int y{ ui.label_for_TempImg->size().height() / 2 };
		ui.label_for_TempImg->toImgCoordinate(x, y);
		InOutArea.setScalRect(&QtRotateRect(QRect(x, y, w, h), InOutArea.getScalRect()->getRotateAngel()));
	}
	else if (newType == 1)
	{
		int w{ 0 };
		if (ui.label_for_TempImg->getScaledImgSize()->width() < ui.label_for_TempImg->size().width())
		{
			w = ui.label_for_TempImg->getScaledImgSize()->width() / 4;
		}
		else
		{
			w = ui.label_for_TempImg->size().width() / 4;
		}
		int x{ ui.label_for_TempImg->size().width() / 2 };
		int y{ ui.label_for_TempImg->size().height() / 2 };
		ui.label_for_TempImg->toImgCoordinate(x, y);
	}
	processedAreaScale(InOutArea, true);
}

QRect QtGuiDisplay::getLabelRect()
{
	QRect bufer;
	bufer.setX(this->x() + ui.label_for_TempImg->x());
	bufer.setY(this->y() + ui.label_for_TempImg->y());
	bufer.setWidth(ui.label_for_TempImg->rect().width());
	bufer.setHeight(ui.label_for_TempImg->rect().height());

	return bufer;
}

void QtGuiDisplay::changeImgFormat(int formatType)
{
	ui.label_for_TempImg->formatImage(formatType);
}

void QtGuiDisplay::updateImg()
{
	if (activ)
	{
		/*if(activProcessedObj!=nullptr)*/
			draw_proceseArears();
		ui.label_for_TempImg->show_partImg();
	}
}

void QtGuiDisplay::slot_changeProcssActiv(int isActiv)
{
	if (isActiv == 0)
	{
		isProcessingActiv = 0;
	}
	else
	{
		isProcessingActiv = 1;
	}
	if (activ)
	{
		updateImg();
	}
}

void QtGuiDisplay::slot_startWriteVideo()
{
	emit clic_startWrite();
}

bool QtGuiDisplay::isActiv()
{
	return activ;
}

bool QtGuiDisplay::getChageActivArea()
{
	return changeActivArea;
}

bool QtGuiDisplay::ProcessedIsActiv()
{
	return isProcessingActiv;
}

void QtGuiDisplay::setNewImage(cv::Mat const inputImg)
{
	QImage drPic(inputImg.data, inputImg.cols, inputImg.rows, inputImg.step, QImage::Format::Format_Grayscale8);
	ui.label_for_TempImg->update_myPixmap(QPixmap::fromImage(drPic));
	this->setSizeScrollBar();
}

void QtGuiDisplay::printFrameNuber(int numberFrame)
{
	ui.label_for_TempImg->draw_text("frame: " + QString::number(numberFrame), QPoint(10, 10));
}

void QtGuiDisplay::printDetectObjNuber(int numberDetectObjNuber)
{
	ui.label_for_TempImg->draw_text("detected: " + QString::number(numberDetectObjNuber), QPoint(10, 30));
}

void QtGuiDisplay::setObjectCoordinates(std::vector<int>& x_, std::vector<int>& y_, std::vector<int>& width_, std::vector<int>& height_)
{
	for (size_t i{ 0 }; i < activProcessedObj.getProcesArears()->size(); ++i)
	{
		x_.push_back(activProcessedObj.getProcesArears()[0][i].getRect()->x());
		y_.push_back(activProcessedObj.getProcesArears()[0][i].getRect()->y());
		width_.push_back(activProcessedObj.getProcesArears()[0][i].getRect()->width());
		height_.push_back(activProcessedObj.getProcesArears()[0][i].getRect()->height());
	}
}

QImage QtGuiDisplay::getProcessingImg()
{
	QImage bufer{ ui.label_for_TempImg->getBuferPixmap()->toImage() };
	//cv::Mat as(bufer.height(), bufer.width(), CV_8UC4, bufer.bits(), bufer.bytesPerLine());
	return bufer;
}

void QtGuiDisplay::resizeEvent(QResizeEvent* event)
{
	//if (qwe != 0) {
	//	ui.horSB_forTempImg->hide();
	//	ui.verSB_forTempImg->hide();
	//	int dr_x{ ui.label_for_TempImg->getOldSize().width() - ui.label_for_TempImg->size().width() };
	//	int dr_y{ ui.label_for_TempImg->getOldSize().height() - ui.label_for_TempImg->size().height() };
	//	//dr_x = 0;
	//	//dr_y = 0;
	//	ui.label_for_TempImg->show_partImg(dr_x, dr_y, ui.label_for_TempImg->size().width() - 1, ui.label_for_TempImg->size().height() - 1);
	//	ui.horSB_forTempImg->setSliderPosition(ui.horSB_forTempImg->value() + dr_x);
	//	ui.verSB_forTempImg->setSliderPosition(ui.verSB_forTempImg->value() + dr_y);
	//	ui.label_for_TempImg->getDrPoint(dr_x, dr_y);
	//	ui.label_for_TempImg->setAllImgScaled();
	//	if (!ui.pushButt_AllLabl->isEnabled())
	//		slot_ZoomImg_AllLabl();
	//	else
	//		this->setSizeScrollBar();
	//}
	//qwe = 0;
}