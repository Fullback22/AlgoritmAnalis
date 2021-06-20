#include "QtAlgoritmAnalis.h"

QtAlgoritmAnalis::QtAlgoritmAnalis(QWidget *parent)
    : QWidget(parent),
	myform(nullptr),
	newVideoForm(nullptr),
	typeDet(0),
	typeWork(0),
	clean_line1(false),
	clean_line2(false),
	analis(false),
	loadFileWithCoordinate(false),
	loadFileWithParams(false),
	detLocalGis(nullptr),
	detSubBacground(nullptr),
	detTemp(nullptr),
	detVevlet(nullptr),
	analisDet(nullptr),
	video_1(nullptr),
	threshe(0),
	threshe_doub(0),
	bin_1(3),
	bin_2(3),
	gausBlur(1),
	morfol_1(1),
	medianBlur_1(1),
	rectSize(3),
	bin_1_type(0),
	bin_2_type(0),
	tempType(0),
	learSpeed(0),
	fraktal(false),
	stop(false),
	pause(false),
	frame(0),
	nextFrame(true),
	writeVideo(false),
	startVideWrite(false),
	loadVideo(true)
{
	ui.setupUi(this);
	ui.horizontalSlider_thresh->setMaximum(16);
	ui.horizontalSlider_thresh->setMinimum(2);
	ui.horizontalSlider_thresh->setTickPosition(QSlider::TicksBelow);
	ui.horizontalSlider_thresh->setTickInterval(2);
	//ui.horizontalSlider_thresh->setValue(2);

	ui.horizontalSlider_medianBlur_1->setMinimum(1);
	ui.horizontalSlider_medianBlur_1->setMaximum(33);
	ui.horizontalSlider_medianBlur_1->setTickPosition(QSlider::TicksBelow);
	ui.horizontalSlider_medianBlur_1->setTickInterval(5);

	ui.horizontalSlider_GausBlur_1->setMinimum(1);
	ui.horizontalSlider_GausBlur_1->setMaximum(33);
	ui.horizontalSlider_GausBlur_1->setTickPosition(QSlider::TicksBelow);
	ui.horizontalSlider_GausBlur_1->setTickInterval(5);

	ui.horizontalSlider_Bin_1->setMinimum(3);
	ui.horizontalSlider_Bin_1->setMaximum(101);
	ui.horizontalSlider_Bin_1->setTickPosition(QSlider::TicksBelow);
	ui.horizontalSlider_Bin_1->setTickInterval(2);

	ui.horizontalSlider_Bin_2->setMinimum(3);
	ui.horizontalSlider_Bin_2->setMaximum(45);
	ui.horizontalSlider_Bin_2->setTickPosition(QSlider::TicksBelow);
	ui.horizontalSlider_Bin_2->setTickInterval(2);

	ui.horizontalSlider_Morfolo_1->setMinimum(3);
	ui.horizontalSlider_Morfolo_1->setMaximum(33);
	ui.horizontalSlider_Morfolo_1->setTickPosition(QSlider::TicksBelow);
	ui.horizontalSlider_Morfolo_1->setTickInterval(5);

	ui.horizontalSlider_rectSize->setMinimum(4);
	ui.horizontalSlider_rectSize->setMaximum(800);
	ui.horizontalSlider_rectSize->setTickPosition(QSlider::TicksBelow);
	ui.horizontalSlider_rectSize->setTickInterval(80);
	
	ui.horizontalSlider_learnSpeed->setMinimum(-100);
	ui.horizontalSlider_learnSpeed->setMaximum(+100);
	ui.horizontalSlider_learnSpeed->setTickPosition(QSlider::TicksBelow);
	ui.horizontalSlider_learnSpeed->setTickInterval(20);

	ui.lineEdit_Bin_1->setReadOnly(true);
	ui.lineEdit_Bin_2->setReadOnly(true);
	ui.lineEdit_GausBlur_1->setReadOnly(true);
	ui.lineEdit_medianBlur_1->setReadOnly(true);
	ui.lineEdit_Morfolo_1->setReadOnly(true);
	ui.lineEdit_thresh->setReadOnly(true);
	ui.lineEdit__rectSize->setReadOnly(true);
	ui.lineEdit_learnSpeed->setReadOnly(true);
	
	//ui.widgetDisplayVideo->setChangesProcessedArears(false);
	video_1 = new VideoCapture;
	
	connect(ui.lineEdit_videoName, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(slot_cleanVideoNameLE()));
	connect(ui.comboBox_processingType, SIGNAL(activated(int)), this, SLOT(slot_activParams()));
	connect(ui.lineEdit_masterFileName, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(slot_cleanFileNameLE()));
	connect(ui.comboBox_workType, SIGNAL(activated(int)), this, SLOT(slot_analis()));
	connect(ui.pushButton_toVideo, SIGNAL(clicked()), this, SLOT(slot_openVideo()));
	connect(ui.pushButton_toMasterFile, SIGNAL(clicked()), this, SLOT(slot_openTempImg()));
	connect(ui.pushButton_toDllFile, SIGNAL(clicked()), this, SLOT(slot_loadDll()));
	connect(ui.PB_ok, SIGNAL(clicked()), this, SLOT(slot_OK()));
	connect(ui.PB_start, SIGNAL(clicked()), this, SLOT(slot_workBeginDete()));
	connect(ui.pushButton_Pause, SIGNAL(clicked()), this, SLOT(slot_pause()));
	connect(ui.pushButton_Pause, SIGNAL(clicked()), this, SLOT(slot_workDete()));
	connect(ui.pushButton_Stop, SIGNAL(clicked()), this, SLOT(slot_stop()));
	connect(ui.comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_setParams()));
	connect(ui.horizontalSlider_thresh, SIGNAL(valueChanged(int)), this, SLOT(slot_setParams()));
	connect(ui.horizontalSlider_medianBlur_1, SIGNAL(valueChanged(int)), this, SLOT(slot_setParams()));
	connect(ui.horizontalSlider_Bin_1, SIGNAL(valueChanged(int)), this, SLOT(slot_setParams()));
	connect(ui.horizontalSlider_Bin_2, SIGNAL(valueChanged(int)), this, SLOT(slot_setParams()));
	connect(ui.horizontalSlider_GausBlur_1, SIGNAL(valueChanged(int)), this, SLOT(slot_setParams()));
	connect(ui.horizontalSlider_Morfolo_1, SIGNAL(valueChanged(int)), this, SLOT(slot_setParams()));
	connect(ui.horizontalSlider_rectSize, SIGNAL(valueChanged(int)), this, SLOT(slot_setParams()));
	connect(ui.horizontalSlider_learnSpeed, SIGNAL(valueChanged(int)), this, SLOT(slot_setParams()));
	connect(ui.checkBox_bin_1, SIGNAL(clicked()), this, SLOT(slot_setParams()));
	connect(ui.checkBox_bin_2, SIGNAL(clicked()), this, SLOT(slot_setParams()));
	connect(ui.checkBox_fractal, SIGNAL(clicked()), this, SLOT(slot_setParams()));
	connect(ui.PB_createNewVideo, SIGNAL(clicked()), this, SLOT(slot_createNewVideo()));
	connect(ui.PB_addRect, SIGNAL(clicked()), this, SLOT(slot_addRect()));
	connect(ui.PB_setFileWithAlgoritmParams, SIGNAL(clicked()), this, SLOT(slot_setFileWithParamsForAlgoritm()));
	connect(ui.PB_delRect, SIGNAL(clicked()), this, SLOT(slot_deletRect()));
	connect(ui.PB_nextFrame, SIGNAL(clicked()), this, SLOT(slot_nextFrame()));
	connect(ui.widgetDisplayVideo, SIGNAL(clic_startWrite()), this, SLOT(slot_startStopWriteVideo()));
}

void QtAlgoritmAnalis::readCoordinateFromFile(std::vector<int>& x_, std::vector<int>& y_, std::vector<int>& width_, std::vector<int>& height_)
{
	std::ifstream fileTrue{fileNameWithCoordinate.toStdString()};
	std::string true_rect{ 0 };
	bool end_frame{false};
	for (int i{ 1 }; end_frame==false; i++)
	{
		std::getline(fileTrue, true_rect);
		std::istringstream iss{ true_rect };
		int frame_{ 0 }, r_x{ 0 }, r_y{ 0 }, r_w{ 0 }, r_h{ 0 };
		iss >> frame_>>r_x>>r_y>>r_w>>r_h;
		if (frame_ == frame)
		{
			if (r_w == 0 && r_h == 0)
			{
			}
			else
			{
				x_.push_back(r_x);
				y_.push_back(r_y);
				width_.push_back(r_w);
				height_.push_back(r_h);
			}
		}		
		if (frame_ > frame || fileTrue.eof())
		{
			end_frame = true;
		}
	}
}

void QtAlgoritmAnalis::writeCoordinateRealObjectToFile(std::vector<int>& x, std::vector<int>& y, std::vector<int>& width, std::vector<int>& height)
{
	std::ofstream realOdjectCoordinate(dirNameResWork.toStdString() + "/realObj.txt", std::ios::app);
	if (realOdjectCoordinate.is_open())
	{
		for (size_t i{ 0 }; i < x.size(); ++i)
		{
			realOdjectCoordinate << frame<<"\t"<<x[i]<<"\t"<<y[i]<<"\t"<<width[i]<<"\t"<<height[i]<< std::endl;
		}
	}
	realOdjectCoordinate.close();
}

void QtAlgoritmAnalis::estimateAlgoritmCoordinateDifinedOperator(std::vector<int> *predict_x, std::vector<int>* predict_y, std::vector<int>* predict_width, std::vector<int>* predict_height)
{
	if (!pause && nextFrame)
	{
		nextFrame = false;
		pause = true;
	}
	else if (nextFrame && pause)
	{
		std::vector<int> real_x;
		std::vector<int> real_y;
		std::vector<int> real_width;
		std::vector<int> real_height;
		ui.widgetDisplayVideo->setObjectCoordinates(real_x, real_y, real_width, real_height);
		writeCoordinateRealObjectToFile(real_x, real_y, real_width, real_height);
		if (loadFileWithParams)
		{
			dllAnalis_setAnalisSettings(name_fileSettingsForLoadParams.toStdString());
			dllAnalis_setRealAndPredictObj(&real_x, &real_y, &real_width, &real_height, predict_x, predict_y, predict_width, predict_height);
		}
		else
		{
			analisDet->setRealAndPredictObj(frame, &real_x, &real_y, &real_width, &real_height, predict_x, predict_y, predict_width, predict_height);
		}
		pause = false;
	}
}

void QtAlgoritmAnalis::estimateAlgoritmCoordinateLoadFromFile(std::vector<int>* predict_x, std::vector<int>* predict_y, std::vector<int>* predict_width, std::vector<int>* predict_height)
{
	std::vector<int> real_x;
	std::vector<int> real_y;
	std::vector<int> real_width;
	std::vector<int> real_height;
	readCoordinateFromFile(real_x, real_y, real_width, real_height);//из файла
	if (loadFileWithParams)
	{
		dllAnalis_setAnalisSettings(name_fileSettingsForLoadParams.toStdString());
		dllAnalis_setRealAndPredictObj(&real_x, &real_y, &real_width, &real_height, predict_x, predict_y, predict_width, predict_height);
	}
	else
	{
		analisDet->setRealAndPredictObj(frame, &real_x, &real_y, &real_width, &real_height, predict_x, predict_y, predict_width, predict_height);
	}
}

void QtAlgoritmAnalis::writeAlgoritmWork()
{
	QImage buferPixmap(ui.widgetDisplayVideo->getProcessingImg());
	cv::Mat frameOut(buferPixmap.height(), buferPixmap.width(), CV_8UC4, buferPixmap.bits(), buferPixmap.bytesPerLine());
	cvtColor(frameOut, frameOut, COLOR_BGR2GRAY);
	if (!startVideWrite)
	{
		time_t now = time(0);
		tm* dt = localtime(&now);
		QString name{ "recording/rec" + QString::number(dt->tm_mday) + QString::number(dt->tm_mon) + QString::number(dt->tm_year) + QString::number(dt->tm_hour) + QString::number(dt->tm_min) + QString::number(dt->tm_sec) + ".avi" };
		outVideo = cv::VideoWriter::VideoWriter(cv::String(name.toStdString()), cv::VideoWriter::fourcc('D', 'I', 'V', 'X'), 23, frameOut.size(), false);
		startVideWrite = true;
	}
	outVideo.write(frameOut);
}

void QtAlgoritmAnalis::slot_OK()//установка пути к видео и эталонну(dll файлу)
{
	bool err{ false };
	fileNameVid = ui.lineEdit_videoName->text();
	VideoCapture video_1(fileNameVid.toStdString());
	typeDet = ui.comboBox_processingType->currentIndex();
	tempImg = ui.lineEdit_masterFileName->text();
	name_dllFileWithAlgoritm = ui.lineEdit_masterFileName->text();
	Mat test;
	video_1.read(test);
	if (test.empty())//проверка существования данного видео
	{
		QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Video is missing"));
		err = true;
	}
	test = imread(tempImg.toStdString());
	if ((typeDet == 0) && test.empty())//проверка существования данной картинки
	{
		QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Images is missing"));
		err = true;
	}
	typeWork = ui.comboBox_workType->currentIndex();
	if (typeDet == 4 && !hDLL_detectAlgoritm)
	{
		QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Dll file is missing"));
		err = true;
	}
	if (!err)
		ui.PB_start->setEnabled(true);
}

void QtAlgoritmAnalis::slot_analis()//проверка будет ли производится анализ работы
{
	if (ui.comboBox_workType->currentIndex() == 1)//если да, то открывем второе окно
	{
		myform = new Form();
		myform->show();
		connect(myform, SIGNAL(getDataForm(QString, QString, QString, QString, bool, bool)), this, SLOT(slot_dataFromForm(QString, QString, QString, QString, bool, bool)));//взаимодействие со второй формой
		analis = true;
		this->setEnabled(false);
	}
	else
		analis = false;
}

void QtAlgoritmAnalis::slot_cleanVideoNameLE()
{
	if (!clean_line1)
	{
		ui.lineEdit_videoName->clear();
		clean_line1 = true;
	}
}

void QtAlgoritmAnalis::slot_cleanFileNameLE()
{
	if (!clean_line2)
	{
		ui.lineEdit_masterFileName->clear();
		clean_line2 = true;
	}
}

void QtAlgoritmAnalis::slot_dataFromForm(QString fileWithCoordinate, QString dirToSave, QString dllWithLoadParams,QString settingsForLoadParams, bool isLoadCoordinate, bool isLoadParams) //приём данных со второй формы
{
	fileNameWithCoordinate = fileWithCoordinate;
	dirNameResWork = dirToSave;
	name_dllFileWithParams = dllWithLoadParams;
	name_fileSettingsForLoadParams = settingsForLoadParams;
	loadFileWithCoordinate = isLoadCoordinate;
	loadFileWithParams = isLoadParams;
	this->setEnabled(true);
	myform->close();
}

void QtAlgoritmAnalis::slot_setParams()//передает численные значения параметров работы алгоритмов(палзунки) в соответствующие (ползункам) переменные
{

	threshe = ui.horizontalSlider_thresh->value();//порог обнаружения
	ui.lineEdit_thresh->setText(QVariant(threshe).toString());

	bin_1 = ui.horizontalSlider_Bin_1->value();//сторона квадрата первой функции бинаризации
	ui.lineEdit_Bin_1->setText(QVariant(bin_1).toString());

	bin_2 = ui.horizontalSlider_Bin_2->value();//сторона квадрата второй функции бинаризации
	ui.lineEdit_Bin_2->setText(QVariant(bin_2).toString());

	gausBlur = ui.horizontalSlider_GausBlur_1->value();//сторона квадрата фильтра гаусса
	ui.lineEdit_GausBlur_1->setText(QVariant(gausBlur).toString());

	medianBlur_1 = ui.horizontalSlider_medianBlur_1->value();//сторона квадрата медианного фильтра
	ui.lineEdit_medianBlur_1->setText(QVariant(medianBlur_1).toString());

	morfol_1 = ui.horizontalSlider_Morfolo_1->value();//сторона квадрата фильтра морфологических преобразований
	ui.lineEdit_Morfolo_1->setText(QVariant(morfol_1).toString());

	rectSize = ui.horizontalSlider_rectSize->value();//сторона квадрата облости подсчёта пикселей
	ui.lineEdit__rectSize->setText(QVariant(rectSize).toString());

	learSpeed = ui.horizontalSlider_learnSpeed->value();//скорость обучения
	learSpeed /= 100; // делим на 100 тк её диапазон -1 и 1 
	ui.lineEdit_learnSpeed->setText(QVariant(learSpeed).toString());

	if (ui.comboBox_processingType->currentIndex() != 3)//расчёт max значения порго обнаружения(площадь квадрата области подсчёта)
	{
		if (rectSize < 4)
			rectSize = 4;
		ui.horizontalSlider_thresh->setMaximum(rectSize * rectSize);
		ui.horizontalSlider_thresh->setMinimum(2);
		ui.horizontalSlider_thresh->setTickInterval(rectSize * rectSize / 5);
	}

	bin_1_type = ui.checkBox_bin_1->isChecked();//тип первой функции бинаризации (иныерсия)
	bin_2_type = ui.checkBox_bin_2->isChecked();//тип второй функции бинаризации (иныерсия)
	fraktal = ui.checkBox_fractal->isChecked();//фрактальный фильр

	tempType = ui.comboBox_3->currentIndex();//тип корреляционного обнаружителя
}

void QtAlgoritmAnalis::slot_workBeginDete()//нажатие кнопки start, инициализация параметров, создание объектов обнаружителей и анализатора
{
	ui.PB_start->setEnabled(false);
	ui.PB_ok->setEnabled(false);
	ui.pushButton_Stop->setEnabled(true);
	ui.pushButton_Pause->setEnabled(true);
	stop = false;
	pause = false;
	nextFrame = true;
	frame = 0;
	ui.comboBox_processingType->setEnabled(false);
	video_1->open(fileNameVid.toStdString());
	
	switch (ui.comboBox_processingType->currentIndex())
	{
	case 0:
		detTemp = new TempDetect();
		break;
	case 1:
		detLocalGis = new LocalGisDet();
		break;
	case 2:
		detSubBacground = new SubtractBacground();
		break;
	case 3:
		detVevlet = new Vevlet_obnoruth();
		break;
	case 4:
		dllAlgoritm_Work = (int(*)(Mat InputImg))GetProcAddress(hDLL_detectAlgoritm, "work");
		dllAlgoritm_Draw = (int(*)(Mat & InOutputImg))GetProcAddress(hDLL_detectAlgoritm, "draw");
		dllAlgotitm_GetDetObj = (int(*)(void))GetProcAddress(hDLL_detectAlgoritm, "getDetectedObj");
		dllAlgoritm_SetParams = (int(*)(std::string))GetProcAddress(hDLL_detectAlgoritm, "setParams");
		dllAlgoritm_GetPredictCoordinate = (int(*)(std::vector<int>&x, std::vector<int>&y, std::vector<int>&width, std::vector<int>&height))GetProcAddress(hDLL_detectAlgoritm, "getPredictCoordinate");
		dllAlgoritm_SetDefault=(int(*)(void))GetProcAddress(hDLL_detectAlgoritm, "setDefault");
		break;
	default:
		break;
	}
	if (ui.comboBox_workType->currentIndex() == 1)//если выбран режим анализ данных
	{
		analis = true;
		
		if (!loadFileWithCoordinate)
		{
			ui.PB_addRect->setEnabled(true);
			ui.PB_delRect->setEnabled(true);
			ui.PB_nextFrame->setEnabled(true);
			ui.pushButton_Pause->setEnabled(false);
		}
		else
		{

		}
		if (loadFileWithParams)
		{
			std::wstring bufer{ name_dllFileWithParams.toStdWString() };
			hDLL_analisAlgoritm = LoadLibrary(((LPWSTR)bufer.c_str()));
			dllAnalis_setRealAndPredictObj = (int(*)(std::vector<int> *real_x, std::vector<int> *real_y, std::vector<int> *real_width, std::vector<int> *real_height, std::vector<int> *pred_x, std::vector<int> *pred_y, std::vector<int> *pred_width, std::vector<int> *pred_height))GetProcAddress(hDLL_analisAlgoritm, "setRealAndPredictObj");
			dllAnalis_process = (int(*)(std::string saveDir))GetProcAddress(hDLL_analisAlgoritm, "process");
			dllAnalis_setDirForSave = (int(*)(std::string saveDir))GetProcAddress(hDLL_analisAlgoritm, "setSaveDirictory");
			dllAnalis_setAnalisSettings = (int(*)(std::string paramsFileName_))GetProcAddress(hDLL_analisAlgoritm, "setAnalisParams");
			dllAnalis_reset = (int(*)())GetProcAddress(hDLL_analisAlgoritm, "reset");
			dllAnalis_setDirForSave(dirNameResWork.toStdString());
			dllAnalis_reset();
		}
		else
		{
			analisDet = new Analis(dirNameResWork.toStdString());
		}
	}
	this->slot_workDete();
}

void QtAlgoritmAnalis::slot_workDete()  //работа обнаружителей
{
	Mat isxod;
	switch (ui.comboBox_processingType->currentIndex())
	{
	case 0: // порядок работы корреляционного обнаружителя
	{
		Mat temp;
		temp = imread(tempImg.toStdString()); //загрузка эталонна
		cvtColor(temp, temp, COLOR_BGR2GRAY);
		detTemp->setTempImg(temp);
		for (; !stop;) //непосредственно обработка видео, если нажата stop цикл заканчивается
		{
			if (!pause)
			{
				if (loadVideo)
					video_1->read(isxod);
				else
					isxod = imread(ui.lineEdit_videoName->text().toStdString());
				if (isxod.empty())//если видео закончилось цикл останавливается
					break;
				cvtColor(isxod, isxod, COLOR_BGR2GRAY);
				detTemp->setParams(medianBlur_1, gausBlur, bin_1_type, rectSize, tempType, threshe);
				detTemp->work(isxod);
				detTemp->draw(isxod);
				ui.widgetDisplayVideo->setNewImage(isxod);
				if (writeVideo)
				{
					writeAlgoritmWork();
				}
				frame++;
			}
			if (loadVideo)
				ui.widgetDisplayVideo->printFrameNuber(frame);
			ui.widgetDisplayVideo->printDetectObjNuber(detTemp->getDetectedObj());
			ui.widgetDisplayVideo->updateImg();

			if (analis) //анализ работы алгоритма
			{
				std::vector<int> predict_x;
				std::vector<int> predict_y;
				std::vector<int> predict_width;
				std::vector<int> predict_height;
				detTemp->getPredictCoordinate(predict_x, predict_y, predict_width, predict_height);
				if (loadFileWithCoordinate && !pause)//выбор способа определения цели в кадре
				{
					estimateAlgoritmCoordinateLoadFromFile(&predict_x, &predict_y, &predict_width, &predict_height);

				}
				else if (!loadFileWithCoordinate)
				{
					estimateAlgoritmCoordinateDifinedOperator(&predict_x, &predict_y, &predict_width, &predict_height);
				}
			}
			if (waitKey(50) >= 0)
				break;
		}
		break;
	}
	case 1: //порядок работы алгоритма локальных гистограмм
	{
		for (; !stop;)//непосредственно обработка видео, если нажата stop цикл заканчивается
		{
			if (!pause)
			{
				if(loadVideo)
					video_1->read(isxod);
				else
					isxod = imread(ui.lineEdit_videoName->text().toStdString());
				if (isxod.empty())//если видео закончилось цикл останавливается
					break;
				cvtColor(isxod, isxod, COLOR_BGR2GRAY);
				detLocalGis->setParams(threshe, medianBlur_1, bin_1, bin_2, gausBlur, morfol_1, bin_1_type, bin_2_type, rectSize);
				detLocalGis->work(isxod);
				detLocalGis->draw(isxod);
				ui.widgetDisplayVideo->setNewImage(isxod);
				if (writeVideo)
				{
					writeAlgoritmWork();
				}
				frame++;
			}
			if (loadVideo)
				ui.widgetDisplayVideo->printFrameNuber(frame);
			ui.widgetDisplayVideo->printDetectObjNuber(detLocalGis->getDetectedObj());
			ui.widgetDisplayVideo->updateImg();

			if (analis)//анализ работы алгоритма
			{
				std::vector<int> predict_x;
				std::vector<int> predict_y;
				std::vector<int> predict_width;
				std::vector<int> predict_height;
				detLocalGis->getPredictCoordinate(predict_x, predict_y, predict_width, predict_height);
				if (loadFileWithCoordinate && !pause)//выбор способа определения цели в кадре
				{	
					estimateAlgoritmCoordinateLoadFromFile(&predict_x, &predict_y, &predict_width, &predict_height);
				}
				else if (!loadFileWithCoordinate)
				{
					estimateAlgoritmCoordinateDifinedOperator(&predict_x, &predict_y, &predict_width, &predict_height);
				}
			}
			if (waitKey(50) >= 0)
				break;
		}
		break;
	}
	case 2: //порядок работы алгоритма вычитания фона
	{
		for (; !stop;) // непосредственно обработка видео, если нажата stop цикл заканчивается
		{
			if (!pause)
			{
				if(loadVideo)
					video_1->read(isxod);
				else
					isxod = imread(ui.lineEdit_videoName->text().toStdString());
				if (isxod.empty())//если видео закончилось цикл останавливается
					break;
				cvtColor(isxod, isxod, COLOR_BGR2GRAY);
				detSubBacground->setParams(threshe, medianBlur_1, bin_1, morfol_1, bin_1_type, learSpeed, rectSize);
				detSubBacground->work(isxod);
				detSubBacground->draw(isxod);
				ui.widgetDisplayVideo->setNewImage(isxod);
				if (writeVideo)
				{
					writeAlgoritmWork();
				}
				frame++;
			}
			if (loadVideo)
				ui.widgetDisplayVideo->printFrameNuber(frame);
			ui.widgetDisplayVideo->printDetectObjNuber(detSubBacground->getDetectedObj());
			ui.widgetDisplayVideo->updateImg();

			if (analis)//анализ работы алгоритма
			{
				std::vector<int> predict_x;
				std::vector<int> predict_y;
				std::vector<int> predict_width;
				std::vector<int> predict_height;
				detSubBacground->getPredictCoordinate(predict_x, predict_y, predict_width, predict_height);
				if (loadFileWithCoordinate && !pause)//выбор способа определения цели в кадре
				{
					estimateAlgoritmCoordinateLoadFromFile(&predict_x, &predict_y, &predict_width, &predict_height);
				}
				else if (!loadFileWithCoordinate)
				{
					estimateAlgoritmCoordinateDifinedOperator(&predict_x, &predict_y, &predict_width, &predict_height);
				}
			}
			if (waitKey(50) >= 0)
				break;
		}
		break;
	}
	case 3: //порядок работы алгоритма обнаружения целей при априорной неопределённости
	{
		for (; !stop;)// непосредственно обработка видео, если нажата stop цикл заканчивается
		{
			if (!pause)
			{
				if(loadVideo)
					video_1->read(isxod);
				else
					isxod = imread(ui.lineEdit_videoName->text().toStdString());
				if (isxod.empty())//если видео закончилось цикл останавливается
					break;
				cvtColor(isxod, isxod, COLOR_BGR2GRAY);
				detVevlet->setParams(threshe, medianBlur_1, gausBlur, fraktal);
				detVevlet->set_img(&isxod);
				detVevlet->poisk();
				detVevlet->fractal_filter();
				detVevlet->korrelycion_pois();
				detVevlet->set_temp_obl();
				detVevlet->draw_cel(isxod);
				ui.widgetDisplayVideo->setNewImage(isxod);
				if (writeVideo)
				{
					writeAlgoritmWork();
				}
				frame++;
			}
			if (loadVideo)
				ui.widgetDisplayVideo->printFrameNuber(frame);
			ui.widgetDisplayVideo->printDetectObjNuber(detVevlet->find_object());
			ui.widgetDisplayVideo->updateImg();

			if (analis)//анализ работы алгоритма
			{
				std::vector<int> predict_x;
				std::vector<int> predict_y;
				std::vector<int> predict_width;
				std::vector<int> predict_height;
				detVevlet->getPredictCoordinate(predict_x, predict_y, predict_width, predict_height);
				if (loadFileWithCoordinate && !pause)//выбор способа определения цели в кадре
				{		
					estimateAlgoritmCoordinateLoadFromFile(&predict_x, &predict_y, &predict_width, &predict_height);
				}
				else if (!loadFileWithCoordinate)
				{
					estimateAlgoritmCoordinateDifinedOperator(&predict_x, &predict_y, &predict_width, &predict_height);
				}
			}
			if (waitKey(50) >= 0)
				break;
		}
		break;
	}
	case 4: //порядок работы с dll файлом
	{
		for (; !stop;)//непосредственно обработка видео, если нажата stop цикл заканчивается
		{
			if (!pause)
			{
				if(loadVideo)
					video_1->read(isxod);
				else
					isxod = imread(ui.lineEdit_videoName->text().toStdString());
				if (isxod.empty())//если видео закончилось цикл останавливается
					break;
				cvtColor(isxod, isxod, COLOR_BGR2GRAY);
				dllAlgoritm_SetParams(name_fileParamsForAlgoritm.toStdString());
				dllAlgoritm_Work(isxod);
				dllAlgoritm_Draw(isxod);
				ui.widgetDisplayVideo->getProcessingImg();
				ui.widgetDisplayVideo->setNewImage(isxod);
				if (writeVideo)
				{
					writeAlgoritmWork();
				}
				frame++;
			}
			if (loadVideo)
				ui.widgetDisplayVideo->printFrameNuber(frame);
			ui.widgetDisplayVideo->printDetectObjNuber(dllAlgotitm_GetDetObj());
			ui.widgetDisplayVideo->updateImg();

			if (analis)//анализ работы алгоритма
			{
				std::vector<int> predict_x;
				std::vector<int> predict_y;
				std::vector<int> predict_width;
				std::vector<int> predict_height;
				dllAlgoritm_GetPredictCoordinate(predict_x, predict_y, predict_width, predict_height);
				if (loadFileWithCoordinate && !pause)//выбор способа определения цели в кадре
				{
					estimateAlgoritmCoordinateLoadFromFile(&predict_x, &predict_y, &predict_width, &predict_height);
				}
				
				else if (!loadFileWithCoordinate)
				{
					estimateAlgoritmCoordinateDifinedOperator(&predict_x, &predict_y, &predict_width, &predict_height);
				}
			}
			if (waitKey(50) >= 0)
				break;
		}
		break;
	}
	}

	if (isxod.empty() && !stop)//если видео закончилось, выполняется метод slot_stop
	{
		this->slot_stop();
	}

	if (stop)//если нажата stop, то удаляются все объекты обнаружителей, объект каласса анализатора работы
	{
		if (analis)
		{
			if (loadFileWithParams)
			{
				dllAnalis_process(dirNameResWork.toStdString());
			}
			else
			{
				analisDet->comparison(dirNameResWork.toStdString() + "/comp.txt");//если выполнялся анализ, то расчитываются текущие результаты
				delete analisDet;
				analisDet = nullptr;
			}
			analis = false;
		}
		switch (ui.comboBox_processingType->currentIndex())
		{
		case 0:
			delete detTemp;
			detTemp = nullptr;
			break;
		case 1:
			delete detLocalGis;
			detLocalGis = nullptr;
			break;
		case 2:
			delete detSubBacground;
			detSubBacground = nullptr;
			break;
		case 3:
			delete detVevlet;
			detVevlet = nullptr;
			break;
		case 4:
			dllAlgoritm_SetDefault();
			break;
		default:
			break;
		}
	}
}

void QtAlgoritmAnalis::slot_activParams() //активация требуемых параметров настройки алгоритмов
{
	if (ui.comboBox_processingType->currentIndex() == 0 )//для корреляционного обнаружителя, контурного обнаружитея
	{
		ui.lineEdit_masterFileName->setDisabled(false);//активация строки для записи пути к эталону
		ui.pushButton_toMasterFile->setDisabled(false);
	}
	else
	{
		ui.lineEdit_masterFileName->setDisabled(true);
		ui.pushButton_toMasterFile->setDisabled(true);
	}
	switch (ui.comboBox_processingType->currentIndex())
	{
	case 0: //для корреляционного обнаружителя
		ui.horizontalSlider_thresh->setEnabled(true);
		ui.horizontalSlider_medianBlur_1->setEnabled(true);
		ui.horizontalSlider_GausBlur_1->setEnabled(true);
		ui.horizontalSlider_Bin_1->setEnabled(false);
		ui.horizontalSlider_Bin_2->setEnabled(false);
		ui.horizontalSlider_Morfolo_1->setEnabled(false);
		ui.horizontalSlider_rectSize->setEnabled(true);
		ui.horizontalSlider_learnSpeed->setEnabled(false);
		ui.checkBox_bin_1->setEnabled(true);
		ui.checkBox_bin_2->setEnabled(false);
		ui.comboBox_3->setEnabled(true);
		ui.checkBox_fractal->setEnabled(false);
		break;
	case 1://алгоритма локальных гистограмм
		ui.horizontalSlider_thresh->setEnabled(true);
		ui.horizontalSlider_medianBlur_1->setEnabled(true);
		ui.horizontalSlider_GausBlur_1->setEnabled(true);
		ui.horizontalSlider_Bin_1->setEnabled(true);
		ui.horizontalSlider_Bin_2->setEnabled(true);
		ui.horizontalSlider_Morfolo_1->setEnabled(true);
		ui.horizontalSlider_rectSize->setEnabled(true);
		ui.horizontalSlider_learnSpeed->setEnabled(false);
		ui.checkBox_bin_1->setEnabled(true);
		ui.checkBox_bin_2->setEnabled(true);
		ui.comboBox_3->setEnabled(false);
		ui.checkBox_fractal->setEnabled(false);
		break;
	case 2://алгоритма вычитания фона
		ui.horizontalSlider_thresh->setEnabled(true);
		ui.horizontalSlider_medianBlur_1->setEnabled(true);
		ui.horizontalSlider_GausBlur_1->setEnabled(true);
		ui.horizontalSlider_Bin_1->setEnabled(true);
		ui.horizontalSlider_Bin_2->setEnabled(false);
		ui.horizontalSlider_Morfolo_1->setEnabled(true);
		ui.horizontalSlider_rectSize->setEnabled(true);
		ui.horizontalSlider_learnSpeed->setEnabled(true);
		ui.checkBox_bin_1->setEnabled(true);
		ui.checkBox_bin_2->setEnabled(false);
		ui.comboBox_3->setEnabled(false);
		ui.checkBox_fractal->setEnabled(false);
		break;
	case 3://алгоритма обнаружения в условиях априорной неопределённости
		ui.horizontalSlider_thresh->setEnabled(true);
		ui.horizontalSlider_medianBlur_1->setEnabled(true);
		ui.horizontalSlider_GausBlur_1->setEnabled(true);
		ui.horizontalSlider_Bin_1->setEnabled(false);
		ui.horizontalSlider_Bin_2->setEnabled(false);
		ui.horizontalSlider_Morfolo_1->setEnabled(false);
		ui.horizontalSlider_rectSize->setEnabled(false);
		ui.horizontalSlider_learnSpeed->setEnabled(false);
		ui.checkBox_bin_1->setEnabled(false);
		ui.checkBox_bin_2->setEnabled(false);
		ui.comboBox_3->setEnabled(false);
		ui.checkBox_fractal->setEnabled(true);
		ui.horizontalSlider_thresh->setMaximum(20);
		threshe = ui.horizontalSlider_thresh->value();
		ui.horizontalSlider_thresh->setTickInterval(4);
		ui.lineEdit_thresh->setText(QVariant(threshe).toString());
		break;
	default:
		break;
	}
	if (ui.comboBox_processingType->currentIndex() == 4) //для dll файлов
	{
		ui.lineEdit_dllFile->setEnabled(true);
		ui.pushButton_toDllFile->setEnabled(true);
		ui.stackedWidget_algoritmParams->setCurrentIndex(1);
	}
	else
	{
		ui.lineEdit_dllFile->setEnabled(false);
		ui.pushButton_toDllFile->setEnabled(false);
		ui.stackedWidget_algoritmParams->setCurrentIndex(0);
	}
}

void QtAlgoritmAnalis::closeEvent(QCloseEvent* event)//закрытие программы
{
	destroyAllWindows();
	exit(1);
}

void QtAlgoritmAnalis::slot_openVideo()//обзор для поиска видео
{
	ui.lineEdit_videoName->clear();
	if (loadVideo)
		ui.lineEdit_videoName->setText(fd.getOpenFileName(this, "Vide", "D:/", tr("Video files (*.mp4 *.avi)")));
	else
		ui.lineEdit_videoName->setText(fd.getOpenFileName(this, "Vide", "D:/", tr("Images files (*.png *.jpg)")));
}

void QtAlgoritmAnalis::slot_openTempImg()//обзор для поиска эталона
{
	ui.lineEdit_masterFileName->clear();
	ui.lineEdit_masterFileName->setText(fd.getOpenFileName(this, "Vide", "D:/", tr("Images files (*.png *.jpg)")));
}

void QtAlgoritmAnalis::slot_pause()//активация/дезактивация паузы
{
	if (!pause)
	{
		pause = true;
	}
	else
	{
		pause = false;
		//this->slot_workDete();
	}
}

void QtAlgoritmAnalis::slot_stop()//стоп- прекращение обработки данного видео
{
	stop = true;
	if (pause)
	{
		this->slot_workDete();//если видео на паузе, то завершается его обработка
	}
	if (writeVideo)
	{
		outVideo.release();
		startVideWrite = false;
		writeVideo = false;
	}
	ui.PB_ok->setEnabled(true);
	ui.comboBox_processingType->setEditable(true);
	ui.pushButton_Stop->setEnabled(false);
	ui.pushButton_Pause->setEnabled(false);
	ui.comboBox_processingType->setEnabled(true);
	ui.PB_addRect->setEnabled(false);
	ui.PB_delRect->setEnabled(false);
	ui.PB_nextFrame->setEnabled(false);
}

void QtAlgoritmAnalis::slot_loadDll()
{
	ui.lineEdit_dllFile->clear();
	ui.lineEdit_dllFile->setText(fd.getOpenFileName(this, "DLL", "D:/", tr("DLL files (*.dll)")));
	name_dllFileWithAlgoritm = ui.lineEdit_dllFile->text();
	wstring bufer{ name_dllFileWithAlgoritm.toStdWString() };
	hDLL_detectAlgoritm = LoadLibrary(((LPWSTR)bufer.c_str()));
	if (!hDLL_detectAlgoritm)
	{
		QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Dll file is missing"));
	}
	else if (hDLL_detectAlgoritm)
	{
	}
	if (ui.comboBox_processingType->currentIndex() == 4)
	{

	}
}


void QtAlgoritmAnalis::slot_createNewVideo()
{
	newVideoForm = new QtGUIVideoGenerate();
	connect(newVideoForm, SIGNAL(dataToAlgoritmAnalis(QString)), this, SLOT(slot_dataFromVideoGenerate(QString)));
	newVideoForm->show();
}

void QtAlgoritmAnalis::slot_dataFromVideoGenerate(QString videoName)
{
	ui.lineEdit_videoName->setText(" ");
	ui.lineEdit_videoName->setText(videoName);
	fileNameVid = videoName;
	newVideoForm->close();
	delete newVideoForm;
	newVideoForm = nullptr;
}

void QtAlgoritmAnalis::slot_addRect()
{
	ui.widgetDisplayVideo->add_rect(1);
}

void QtAlgoritmAnalis::slot_deletRect()
{
	ui.widgetDisplayVideo->deleteActivRect();
}

void QtAlgoritmAnalis::slot_nextFrame()
{
	nextFrame = true;
}

void QtAlgoritmAnalis::slot_setFileWithParamsForAlgoritm()
{
	ui.lineEdit_algoritmParams->setText(fd.getOpenFileName(this, "Text file", "D:/", tr("Text files (*.json *.txt)")));
	std::ifstream bufer;
	name_fileParamsForAlgoritm = ui.lineEdit_algoritmParams->text();
	bufer.open(name_fileParamsForAlgoritm.toStdString());
	if (!bufer.is_open() && name_fileParamsForAlgoritm != "")
	{
		QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("File with settings is missing"));
		name_fileParamsForAlgoritm = "";
	}
	bufer.close();
}

void QtAlgoritmAnalis::slot_startStopWriteVideo()
{
	writeVideo = !writeVideo;
	if (writeVideo)
	{
	}
	else
	{
		outVideo.release();
		startVideWrite = false;
	}
}
