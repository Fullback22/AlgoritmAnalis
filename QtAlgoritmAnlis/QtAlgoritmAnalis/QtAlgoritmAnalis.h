#pragma once

#include <tchar.h>
#include <Windows.h>
#include <QtWidgets/QWidget>
#include "ui_QtAlgoritmAnalis.h"
#include <qfiledialog.h>
#include <qerrormessage.h>
#include <qaction.h>
#include <qmessagebox.h>
//#include "qlabel.h"
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
	Form* setAnalisParmsForm{ nullptr };
	QtGUIVideoGenerate* newVideoForm{ nullptr };
	QString videoFilename{};
	QString fileNameWithCoordinate;
	QString dirNameResWork;
	QString tempImg;
	QString name_dllFileWithAlgoritm;
	QString name_fileParamsForAlgoritm;
	QString name_dllFileWithParams;
	QString name_fileSettingsForLoadParams;


	/*bool clean_line1{ false };
	bool clean_line2{ false };*/
	bool analisMode{ false };
	bool loadFileWithCoordinate{ false };
	bool loadFileWithParams{ false };
	LocalGisDet* detLocalGis{ nullptr };
	SubtractBacground* detSubBacground{ nullptr };
	TempDetect* detTemp{ nullptr };
	Vevlet_obnoruth* detVevlet{ nullptr };
	Analis* analisDet{ nullptr };
	VideoCapture* analisVideo{ nullptr };
	
	int threshe{ 0 };
	double threshe_doub{ 0 };
	int bin_1{ 3 };
	int bin_2{ 3 };
	int gausBlur{ 1 };
	int morfol_1{ 1 };
	int medianBlur_1{ 1 };
	int rectSize{ 3 };
	int bin_1_type{ 0 };
	int bin_2_type{ 0 };
	int tempType{ 0 };
	double learSpeed{ 0 };
	bool fraktal{ false };

	bool stop{ false };
	bool pause{ false };
	int frameNamber{ 0 };
	bool nextFrame{ true };
	bool writeVideo{ false };
	bool startVideWrite{ false };
	bool isVideoAnalysis{ true };
	cv::VideoWriter* outVideo{ nullptr };

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

	void readCoordinateFromFile(std::vector<int>& x, std::vector<int>& y, std::vector<int>& width, std::vector<int>& height);
	void writeCoordinateRealObjectToFile(std::vector<int>& x, std::vector<int>& y, std::vector<int>& width, std::vector<int>& height);
	void estimateAlgoritmCoordinateDifinedOperator(std::vector<int>* predict_x, std::vector<int>* predict_y, std::vector<int>* predict_width, std::vector<int>* predict_height);
	void estimateAlgoritmCoordinateLoadFromFile(std::vector<int>* predict_x, std::vector<int>* predict_y, std::vector<int>* predict_width, std::vector<int>* predict_height);
	void writeAlgoritmWork();
	void setGiuParametrs();
public:
	QtAlgoritmAnalis(QWidget* parent = Q_NULLPTR);
private slots:
	void slot_OK();
	void slot_analis();
	/*void slot_cleanVideoNameLE();
	void slot_cleanFileNameLE();*/
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
	void slot_addObject();
	void slot_deletRect();
	void slot_nextFrame();
	void slot_setFileWithParamsForAlgoritm();
	void slot_startStopWriteVideo();
private:
	Ui::QtAlgoritmAnalisClass ui;
public slots:
	void slot_dataFromForm(QString fileWithCoordinate, QString dirToSave, QString dllWithLoadParams, QString settingsForLoadParams, bool isLoadCoordinate, bool isLoadParams);
};