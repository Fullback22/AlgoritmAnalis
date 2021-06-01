#pragma once

#include <Windows.h>
#include <string>
#include <tchar.h>
#include <iostream>
#include <QWidget>
#include "ui_QtGUIVideoGenerate.h"
#include "qfiledialog.h"
#include "qmessagebox.h"

#include "VideoGenerate_v1_1.h"

class QtGUIVideoGenerate : public QWidget
{
	Q_OBJECT

public:
	QtGUIVideoGenerate(QWidget *parent = Q_NULLPTR);
	~QtGUIVideoGenerate();
	QString jsonName;
	QString dirName;
	QString dllName;
	QFileDialog *fd;
	bool isBuiltedModel;

	HMODULE hDLL;
	int(*dllSetParams)(std::string fileName);
	void(*dllVideoGenerate)(std::string saveFileName);

private:
	Ui::QtGUIVideoGenerate ui;
public slots:
	void slot_setjsonFile();
	void slot_setDirForSave();
	void slot_setDllFile();
	void slot_videoGenerate();
	void slot_changeModel();
signals:
	void dataToAlgoritmAnalis(QString saveDir);
};
