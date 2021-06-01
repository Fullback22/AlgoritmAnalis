#pragma once

#include <tchar.h>
#include <Windows.h>
#include <QtWidgets/QWidget>
#include "ui_QtAlgoritmAnalis.h"
#include <qfiledialog.h>
#include <qerrormessage.h>
#include <qaction.h>
#include <qmessagebox.h>
#include "qlabel.h"
#include "LocalGisDet.h"
#include <iostream>
#include "Form.h"
#include "Analis.h"
#include "SubtractBacground.h"
#include "TempDetect.h"
#include "Vevlet_obnoruth.h"
#include "GlobslGisDet.h"
#include "ContourDet.h"
#include "SVMandHOG.h"
#include "QtGUIVideoGenerate.h"
#include <ctime>
#include <fstream>

class QtAlgoritmAnalis : public QWidget
{
    Q_OBJECT
	Form* myform;
	QtGUIVideoGenerate* newVideoForm;
	QString fileNameVid;
	QString fileNameWithCoordinate;
	QString dirNameResWork;
	QString tempImg;
	QString name_dllFileWithAlgoritm;
	QString name_fileParamsForAlgoritm;
	QString name_dllFileWithParams;
	QString name_fileSettingsForLoadParams;
	int typeDet;
	int typeWork;
	bool clean_line1;
	bool clean_line2;
	bool analis;
	bool loadFileWithCoordinate;
	bool loadFileWithParams;
	LocalGisDet* detLocalGis;
	SubtractBacground* detSubBacground;
	TempDetect* detTemp;
	Vevlet_obnoruth* detVevlet;
	QFileDialog fd;
	Analis* analisDet;
	VideoCapture* video_1;
	int threshe;
	double threshe_doub;
	int bin_1;
	int bin_2;
	int gausBlur;
	int morfol_1;
	int medianBlur_1;
	int rectSize;
	int bin_1_type;
	int bin_2_type;
	int tempType;
	double learSpeed;
	bool fraktal;
	bool stop;
	bool pause;
	int frame;
	bool nextFrame;
	bool writeVideo;
	bool startVideWrite;
	bool loadVideo;
	cv::VideoWriter outVideo;

	HMODULE hDLL_detectAlgoritm;
	int(*dllAlgoritm_Work)(Mat InputImg);
	int(*dllAlgoritm_Draw)(Mat& InOutputImg);
	int(*dllAlgotitm_GetDetObj)(void);
	int(*dllAlgoritm_SetParams)(std::string fileName);
	int(*dllAlgoritm_GetPredictCoordinate)(std::vector<int>& x, std::vector<int>& y, std::vector<int>& width, std::vector<int>& height);	
	int(*dllAlgoritm_SetDefault)(void);
	
	HMODULE hDLL_analisAlgoritm;
	int(*dllAnalis_setRealAndPredictObj)(std::vector<int>* real_x, std::vector<int>* real_y, std::vector<int>* real_width, std::vector<int>* real_height, std::vector<int>* pred_x, std::vector<int>* pred_y, std::vector<int>* pred_width, std::vector<int>* pred_height);
	int(*dllAnalis_process)(std::string saveDir);
	int(*dllAnalis_setDirForSave)(std::string saveDir);
	int(*dllAnalis_setAnalisSettings)(std::string fileName);
	int(*dllAnalis_reset)();

	void readCoordinateFromFile(std::vector<int> &x, std::vector<int>& y, std::vector<int>& width, std::vector<int>& height);
	void writeCoordinateRealObjectToFile(std::vector<int>& x, std::vector<int>& y, std::vector<int>& width, std::vector<int>& height);
	void estimateAlgoritmCoordinateDifinedOperator(std::vector<int>* predict_x, std::vector<int>* predict_y, std::vector<int>* predict_width, std::vector<int>* predict_height);
	void estimateAlgoritmCoordinateLoadFromFile(std::vector<int>* predict_x, std::vector<int>* predict_y, std::vector<int>* predict_width, std::vector<int>* predict_height);
	void writeAlgoritmWork();
public:
    QtAlgoritmAnalis(QWidget *parent = Q_NULLPTR);
private slots:
	void slot_OK();
	void slot_analis();
	void slot_cleanVideoNameLE();
	void slot_cleanFileNameLE();
	void slot_setParams();
	void slot_workDete();
	void slot_workBeginDete();
	void slot_activParams();
	void closeEvent(QCloseEvent*);
	void slot_openVideo();
	void slot_openTempImg();
	void slot_pause();
	void slot_stop();
	void slot_loadDll();
	void slot_createNewVideo();
	void slot_dataFromVideoGenerate(QString videoName);
	void slot_addRect();
	void slot_deletRect();
	void slot_nextFrame();
	void slot_setFileWithParamsForAlgoritm();
	void slot_startStopWriteVideo();
private:
    Ui::QtAlgoritmAnalisClass ui;
public slots:
	void slot_dataFromForm(QString fileWithCoordinate, QString dirToSave, QString dllWithLoadParams,QString settingsForLoadParams, bool isLoadCoordinate, bool isLoadParams);
};