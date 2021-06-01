#pragma once

#include <QWidget>
#include <Windows.h>
#include <qmessagebox.h>
#include "ui_Form.h"
#include <fstream>
#include <iostream>
#include <qfiledialog.h>
#include <qevent.h>

class Form : public QWidget
{
	Q_OBJECT
	Ui::Form ui;
	QFileDialog *fd;
	QString dllFileName;
	void closeEvent(QCloseEvent* event);
	bool f_close;
public:
	Form(QWidget *parent = Q_NULLPTR);
	~Form();
	/*bool cleanLine1;
	bool cleanLine2;*/
	
signals:
	void getDataForm(QString fileWithCoordinate, QString dirForResWork, QString filewithParams, QString fileWithSettings, bool loadFileWithCoordinate, bool loadFileWithParams);
private slots:
	void slot_OK();
	void slot_activ();
	void slot_setFileWithCoordinate();
	void slot_setSaveDir();
	void slot_setDllFile();
	void slot_setSettingsFile();
	void slot_chageParametrType();
	//void slot_clean_L1();
	//void slot_clean_L2();
};
