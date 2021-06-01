#include "Form.h"

Form::Form(QWidget* parent)
	: QWidget(parent),
	fd(nullptr),
	f_close(false)
{
	ui.setupUi(this);
	connect(ui.comboBox_workType, SIGNAL(activated(int)), this, SLOT(slot_activ()));
	//connect(ui.lineEdit_saveDirName, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(slot_clean_L2()));
	//connect(ui.lineEdit_coordinateCeil, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(slot_clean_L1()));
	connect(ui.PB_setSaveDirName, SIGNAL(clicked()), this, SLOT(slot_setSaveDir()));
	connect(ui.PB_setNameFileWithCoordinate, SIGNAL(clicked()), this, SLOT(slot_setFileWithCoordinate()));
	connect(ui.PB_setDllName, SIGNAL(clicked()), this, SLOT(slot_setDllFile()));
	connect(ui.PB_OK, SIGNAL(clicked()), this, SLOT(slot_OK()));
	connect(ui.PB_setAnalisParams, SIGNAL(clicked()), this, SLOT(slot_setSettingsFile()));
	connect(ui.RB_bultinParams, SIGNAL(clicked()), this, SLOT(slot_chageParametrType()));
	connect(ui.RB_loadParams, SIGNAL(clicked()), this, SLOT(slot_chageParametrType()));
	setAttribute(Qt::WA_DeleteOnClose);
}

Form::~Form()
{
}
void Form::slot_OK()
{
	bool err{ false };
	bool load{ false };	
	if (ui.comboBox_workType->currentIndex() == 1)
		load = true;
	if (load)
	{
		QString str{ ui.lineEdit_coordinateCeil->text() };
		std::ifstream bufer;
		bufer.open(str.toStdString());
		if (!bufer.is_open())
		{
			QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("File with object coordinats is missing"));
			err = true;
		}
		bufer.close();
	}
	if (ui.RB_loadParams->isChecked())
	{
		QString str{ ui.lineEdit_dllName->text() };
		std::wstring bufer_{ str.toStdWString() };
		HMODULE hDLL = LoadLibrary(((LPWSTR)bufer_.c_str()));
		if (!hDLL)
		{
			QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Dll file is missing"));
			err = true;
		}
		std::ifstream bufer;
		str = ui.lineEdit_settigsForParams->text();
		bufer.open(str.toStdString());
		if (!bufer.is_open() && str!="")
		{
			QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("File with settings is missing"));
			err = true;
		}
		bufer.close();
	}
	if (!err)
	{
		f_close = true;
		emit getDataForm(ui.lineEdit_coordinateCeil->text(), ui.lineEdit_saveDirName->text(), ui.lineEdit_dllName->text(), ui.lineEdit_settigsForParams->text(), load, ui.RB_loadParams->isChecked());
	}
		
}

void Form::slot_activ()
{
	if (ui.comboBox_workType->currentIndex() == 1)
	{
		ui.lineEdit_coordinateCeil->setDisabled(false);
		ui.PB_setNameFileWithCoordinate->setEnabled(true);
	}
	else
	{
		ui.lineEdit_coordinateCeil->setDisabled(true);
		ui.PB_setNameFileWithCoordinate->setEnabled(false);
	}
}
//void Form::slot_clean_L1()
//{
//	if (!cleanLine1)
//	{
//		ui.lineEdit_coordinateCeil->clear();
//		cleanLine1 = true;
//	}
//}
//
//void Form::slot_clean_L2()
//{
//	if (!cleanLine2)
//	{
//		ui.lineEdit_saveDirName->clear();
//		cleanLine2 = true;
//	}
//}

void Form::slot_setFileWithCoordinate()
{
	fd = new QFileDialog();
	ui.lineEdit_coordinateCeil->setText(fd->getOpenFileName(this, "File", "D:/", tr("Tekst files (*.txt )")));
	delete fd;
	fd = nullptr;
}

void Form::slot_setSaveDir()
{
	fd = new QFileDialog();
	ui.lineEdit_saveDirName->setText(fd->getExistingDirectory(this, "File", "D:/"));
	delete fd;
	fd = nullptr;
}

void Form::slot_setDllFile()
{
	fd = new QFileDialog();
	ui.lineEdit_dllName->setText(fd->getOpenFileName(this, "DLL", "D:/", tr("Tekst files (*.dll )")));
	delete fd;
	fd = nullptr;
}

void Form::slot_setSettingsFile()
{
	fd = new QFileDialog();
	ui.lineEdit_settigsForParams->setText(fd->getOpenFileName(this, "Settings", "D:/", tr("Tekst files (*.json *.txt )")));
	delete fd;
	fd = nullptr;
}

void Form::slot_chageParametrType()
{
	if (ui.RB_bultinParams->isChecked())
	{
		ui.lineEdit_dllName->setEnabled(false);
		ui.PB_setDllName->setEnabled(false);
		ui.lineEdit_settigsForParams->setEnabled(false);
		ui.PB_setAnalisParams->setEnabled(false);
	}
	else if (ui.RB_loadParams->isChecked())
	{
		ui.lineEdit_dllName->setEnabled(true);
		ui.PB_setDllName->setEnabled(true);
		ui.lineEdit_settigsForParams->setEnabled(true);
		ui.PB_setAnalisParams->setEnabled(true);
	}
}

void Form::closeEvent(QCloseEvent* event)
{
	if(!f_close)
	{
		emit getDataForm(" ", " ", " ", " ", false, false);
	}	
	else
	{
	}
}