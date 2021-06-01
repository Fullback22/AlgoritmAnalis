#include "QtGUIVideoGenerate.h"

QtGUIVideoGenerate::QtGUIVideoGenerate(QWidget *parent)
	: QWidget(parent),
	fd(nullptr),
	isBuiltedModel(true)
{
	ui.setupUi(this);
	connect(ui.PB_jsonFile, SIGNAL(clicked()), this, SLOT(slot_setjsonFile()));
	connect(ui.PB_saveDir, SIGNAL(clicked()), this, SLOT(slot_setDirForSave()));
	connect(ui.PB_dllName, SIGNAL(clicked()), this, SLOT(slot_setDllFile()));
	connect(ui.PB_generate, SIGNAL(clicked()), this, SLOT(slot_videoGenerate()));
	connect(ui.RB_builtInMobel, SIGNAL(clicked()), this, SLOT(slot_changeModel()));
	connect(ui.RB_loadNewModel, SIGNAL(clicked()), this, SLOT(slot_changeModel()));
	setAttribute(Qt::WA_DeleteOnClose);
}

QtGUIVideoGenerate::~QtGUIVideoGenerate()
{
}

void QtGUIVideoGenerate::slot_setjsonFile()
{
	fd = new QFileDialog;
	jsonName=fd->getOpenFileName(this, "JSON", "D:/", tr("JSON files (*.json )"));
	ui.lineEdit_jsonFile->setText(jsonName);
	delete fd;
	fd = nullptr;
}

void QtGUIVideoGenerate::slot_setDirForSave()
{
	fd = new QFileDialog;
	dirName = fd->getExistingDirectory(this, "File", "D:/");
	ui.lineEdit_saveDir->setText(dirName);
	delete fd;
	fd = nullptr;
}

void QtGUIVideoGenerate::slot_setDllFile()
{
	fd = new QFileDialog;
	dllName = fd->getOpenFileName(this, "DLL", "D:/", tr("DLL libery (*.dll )"));
	ui.lineEdit_dllName->setText(dllName);
	delete fd;
	fd = nullptr;
}

void QtGUIVideoGenerate::slot_videoGenerate()
{
	json jsonConf;
	bool error{ false };
	std::ifstream paramFile(jsonName.toStdString());
	if (!paramFile.is_open()) {
		QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("JSON-file is missing"));
		error = true;
	}
	try {
		jsonConf = json::parse(paramFile);
	}
	catch (...) {
		QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("ERROR JSON"));
		paramFile.close();
		error = true;
	}
	if (isBuiltedModel)
	{
		if (!error)
		{
			VideoGenerate_v1_1* newVideo = new VideoGenerate_v1_1(dirName.toStdString() + "/video.avi", jsonName.toStdString());
			newVideo->setVideoParams();
			newVideo->videoGeneratr();
		}
	}
	else
	{
		std::wstring bufer{ dllName.toStdWString() };
		hDLL = LoadLibrary(((LPWSTR)bufer.c_str()));
		if (!hDLL)
		{
			QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Dll file is missing"));
		}
		else if(!error)
		{
			dllSetParams = (int(*)(std::string paramFileName))GetProcAddress(hDLL, "setVideoParams");
			dllVideoGenerate = (void(*)(std::string saveDirName))GetProcAddress(hDLL, "videoGeneratr");
			int a=dllSetParams(jsonName.toStdString());
			dllVideoGenerate(dirName.toStdString() + "/video.avi");
		}
	}
	if (!error)
	{
		QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("New video create"));
		emit dataToAlgoritmAnalis(dirName + "/video.avi");
	}
}

void QtGUIVideoGenerate::slot_changeModel()
{
	if (ui.RB_builtInMobel->isChecked())
	{
		isBuiltedModel = true;
		ui.PB_dllName->setEnabled(false);
		ui.lineEdit_dllName->setEnabled(false);
	}
	else
	{
		isBuiltedModel = false;
		ui.PB_dllName->setEnabled(true);
		ui.lineEdit_dllName->setEnabled(true);
	}
}
