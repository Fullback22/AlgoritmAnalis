#include "SVMandHOG.h"


//SVMandHOG::SVMandHOG()
//{
//}
//
//SVMandHOG::SVMandHOG(string fileName)
//{
//	my_SVM.load(fileName.data()); ///загружает классификатор
//	sizeRect = 3;
//	medianBlurSize = 3;
//	gausianBlurSize = 3;
//}
//SVMandHOG::~SVMandHOG()
//{
//}
//
//void SVMandHOG::kaskad(Mat &in_images, int step, int size_rect, int size_hog)
//{
//	detectObjFilter.clear();
//	out_images.clear();
//	detectObjFilter.push_back(Rect(0, 0, 0, 0));
//	for (int i{ 0 }; i < (in_images.rows - size_rect); i += step)//обход изображения скользящим окном размерами size_rect Х size_rect
//	{
//		for (int j{ 0 }; j <= (in_images.cols - size_rect); j += step)
//		{
//			Rect obl(j, i, size_rect, size_rect);
//			Mat img_obl = in_images(obl);//выделяется окно
//			vector<float> test;
//			hog.compute(img_obl, test, Size(size_hog, size_hog));//вычисление ход дискриптора
//			Mat img_in_hog(1, test.size(), CV_32FC1);
//			for (int q{ 0 }; q < test.size(); ++q)
//			{
//				img_in_hog.at<float>(0, q) = test[q];//загрузка дискриптора в Mat
//			}
//
//			if (my_SVM.predict(img_in_hog) == 1)
//			{
//				bool end{ false };
//				for (int k{ 0 }; k<detectObjFilter.size() && end == false; ++k) // проверка: является ли новая область пересекающийся с выделеными ранее
//				{
//
//					if (detectObjFilter[k].y + detectObjFilter[k].height  < obl.y || detectObjFilter[k].y > obl.y + obl.height ||  //проверка на пересекаемость. Если не пересекается с текущей проверяем со следующей
//						detectObjFilter[k].x > obl.x + obl.width || detectObjFilter[k].x + detectObjFilter[k].width< obl.x)
//					{
//					}
//					else //если пересекается, то происходит объединение областей
//					{
//						if (detectObjFilter[k].x>obl.x)
//							detectObjFilter[k].x = obl.x;
//						if (detectObjFilter[k].y > obl.y)
//							detectObjFilter[k].y = obl.y;
//						if (detectObjFilter[k].x + detectObjFilter[k].width < obl.x + obl.width)
//							detectObjFilter[k].width = obl.x + obl.width - detectObjFilter[k].x;
//						if (detectObjFilter[k].y + detectObjFilter[k].height < obl.y + obl.height)
//							detectObjFilter[k].height = obl.y + obl.height - detectObjFilter[k].y;
//						k = detectObjFilter.size();
//					}
//					if (k == detectObjFilter.size() - 1)
//					{
//						detectObjFilter.push_back(obl); //если новая область не пересекается ни с одной  из выделенных ранее, то добавлем её в вектор
//						end = true;
//					}
//				}
//			}
//		}
//	}
//
//	if (detectObjFilter.size() != 0)
//	{
//		if (detectObjFilter[0].height == 0 && detectObjFilter[0].height == 0)
//			detectObjFilter.erase(detectObjFilter.begin());
//	}
//	for (int k{ 0 }; k<detectObjFilter.size(); ++k) //ещё одна проверка на объединение
//	{
//		for (int l{ k + 1 }; l<detectObjFilter.size(); ++l)
//		{
//			if (detectObjFilter[k].y + detectObjFilter[k].height  < detectObjFilter[l].y || detectObjFilter[k].y >  detectObjFilter[l].y + detectObjFilter[l].height ||
//				detectObjFilter[k].x >  detectObjFilter[l].x + detectObjFilter[l].width || detectObjFilter[k].x + detectObjFilter[k].width<  detectObjFilter[l].x)
//			{
//			}
//			else
//			{
//				if (detectObjFilter[k].x> detectObjFilter[l].x)
//					detectObjFilter[k].x = detectObjFilter[l].x;
//				if (detectObjFilter[k].y > detectObjFilter[l].y)
//					detectObjFilter[k].y = detectObjFilter[l].y;
//				if (detectObjFilter[k].x + detectObjFilter[k].width < detectObjFilter[l].x + detectObjFilter[l].width)
//					detectObjFilter[k].width = detectObjFilter[l].x + detectObjFilter[l].width - detectObjFilter[k].x;
//				if (detectObjFilter[k].y + detectObjFilter[k].height < detectObjFilter[l].y + detectObjFilter[l].height)
//					detectObjFilter[k].height = detectObjFilter[l].y + detectObjFilter[l].height - detectObjFilter[k].y;
//				detectObjFilter.erase(detectObjFilter.begin() + l);
//				l--;
//			}
//		}
//	}
//	for (int k{ 0 }; k<detectObjFilter.size(); ++k)
//	{
//		out_images.push_back(in_images(detectObjFilter[k]));//облоси где найдена цель загружаются в out_images
//	}
//}
//
//void SVMandHOG::kaskad(Mat &in_images)
//{
//	float const *start_support_vector = my_SVM.get_support_vector(0);
//	vector<float> support_vector;
//	int support_vector_size{ my_SVM.get_var_count() };
//	Mat sup_vec(1, support_vector_size,CV_32F);
//	for (int i{ 0 }; i<support_vector_size; ++i)
//	{
//		//support_vector.push_back(*(start_support_vector + i));
//		sup_vec.at<float>(0, i) = *(start_support_vector + i);
//	}
//	
//	hog.setSVMDetector(sup_vec);
//	detectObj.clear();
//	detectObjFilter.clear();
//	hog.detectMultiScale(in_images, detectObj, 0, Size(10, 10), Size(100, 100), 1.05, 2);
//	size_t i, j;
//	for (i = 0; i < detectObj.size(); i++)
//	{
//		Rect r = detectObj[i];
//		for (j = 0; j < detectObj.size(); j++)
//		if (j != i && (r & detectObj[j]) == r)
//			break;
//		if (j == detectObj.size())
//			detectObjFilter.push_back(r);
//	}
//	for (int i{ 0 }; i < 2; i++)
//	{
//		for (int k{ 0 }; k < detectObjFilter.size(); ++k) //проверка на объединение выполняется 2 раза
//		{
//			for (int l{ k + 1 }; l<detectObjFilter.size(); ++l)
//			{
//				if (detectObjFilter[k].y + detectObjFilter[k].height  < detectObjFilter[l].y || detectObjFilter[k].y >  detectObjFilter[l].y + detectObjFilter[l].height ||
//					detectObjFilter[k].x >  detectObjFilter[l].x + detectObjFilter[l].width || detectObjFilter[k].x + detectObjFilter[k].width<  detectObjFilter[l].x)
//				{
//				}
//				else
//				{
//					if (detectObjFilter[k].x> detectObjFilter[l].x)
//						detectObjFilter[k].x = detectObjFilter[l].x;
//					if (detectObjFilter[k].y > detectObjFilter[l].y)
//						detectObjFilter[k].y = detectObjFilter[l].y;
//					if (detectObjFilter[k].x + detectObjFilter[k].width < detectObjFilter[l].x + detectObjFilter[l].width)
//						detectObjFilter[k].width = detectObjFilter[l].x + detectObjFilter[l].width - detectObjFilter[k].x;
//					if (detectObjFilter[k].y + detectObjFilter[k].height < detectObjFilter[l].y + detectObjFilter[l].height)
//						detectObjFilter[k].height = detectObjFilter[l].y + detectObjFilter[l].height - detectObjFilter[k].y;
//					detectObjFilter.erase(detectObjFilter.begin() + l);
//					l--;
//				}
//			}
//		}
//	}
//	for (int i{ 0 };i<detectObjFilter.size();i++)
//	{
//		detectObjFilter[i].x += cvRound(detectObjFilter[i].width*0.1);
//		detectObjFilter[i].width = cvRound(detectObjFilter[i].width * 1);
//		detectObjFilter[i].y += cvRound(detectObjFilter[i].height*0.07);
//		detectObjFilter[i].height = cvRound(detectObjFilter[i].height*0.5);
//	}
//	cout << 1;
//}
//
//void SVMandHOG::kaskad_SVM(Mat &in_images)
//{
//	detectObjFilter.clear();
//	out_images.clear();
//	detectObjFilter.push_back(Rect(0, 0, 0, 0));
//	for (int i{ 0 }; i < (in_images.rows - sizeRect); i += sizeRect / 2)//обход изображения скользящим окном размерами size_rect Х size_rect
//	{
//		for (int j{ 0 }; j <= (in_images.cols - sizeRect); j += sizeRect / 2)
//		{
//			Rect obl(j, i, sizeRect, sizeRect);
//			Mat img_obl = in_images(obl);//выделяется окно
//			Mat img_in_hog(1, img_obl.cols*img_obl.rows, CV_32FC1);
//			int g{ 0 };
//			for (int i{ 0 }; i < img_obl.rows; ++i)
//			{
//				for (int k{ 0 }; k < img_obl.cols; k++)
//				{
//					img_in_hog.at<float>(0, g) = img_obl.at<uchar>(i, k);
//					g++;
//				}
//
//			}
//
//			if (my_SVM.predict(img_in_hog) == 1)
//			{
//				bool end{ false };
//				for (int k{ 0 }; k<detectObjFilter.size() && end == false; ++k) // проверка: является ли новая область пересекающийся с выделеными ранее
//				{
//
//					if (detectObjFilter[k].y + detectObjFilter[k].height  < obl.y || detectObjFilter[k].y > obl.y + obl.height ||  //проверка на пересекаемость. Если не пересекается с текущей проверяем со следующей
//						detectObjFilter[k].x > obl.x + obl.width || detectObjFilter[k].x + detectObjFilter[k].width< obl.x)
//					{
//					}
//					else //если пересекается, то происходит объединение областей
//					{
//						if (detectObjFilter[k].x > obl.x)
//							detectObjFilter[k].x = obl.x;
//						if (detectObjFilter[k].y > obl.y)
//							detectObjFilter[k].y = obl.y;
//						if (detectObjFilter[k].x + detectObjFilter[k].width < obl.x + obl.width)
//							detectObjFilter[k].width = obl.x + obl.width - detectObjFilter[k].x;
//						if (detectObjFilter[k].y + detectObjFilter[k].height < obl.y + obl.height)
//							detectObjFilter[k].height = obl.y + obl.height - detectObjFilter[k].y;
//						k = detectObjFilter.size();
//					}
//					if (k == detectObjFilter.size() - 1)
//					{
//						detectObjFilter.push_back(obl); //если новая область не пересекается ни с одной  из выделенных ранее, то добавлем её в вектор
//						end = true;
//					}
//				}
//			}
//		}
//	}
//
//	if (detectObjFilter.size() != 0)
//	{
//		if (detectObjFilter[0].height == 0 && detectObjFilter[0].height == 0)
//			detectObjFilter.erase(detectObjFilter.begin());
//	}
//	for (int k{ 0 }; k<detectObjFilter.size(); ++k) //ещё одна проверка на объединение
//	{
//		for (int l{ k + 1 }; l<detectObjFilter.size(); ++l)
//		{
//			if (detectObjFilter[k].y + detectObjFilter[k].height  < detectObjFilter[l].y || detectObjFilter[k].y >  detectObjFilter[l].y + detectObjFilter[l].height ||
//				detectObjFilter[k].x >  detectObjFilter[l].x + detectObjFilter[l].width || detectObjFilter[k].x + detectObjFilter[k].width<  detectObjFilter[l].x)
//			{
//			}
//			else
//			{
//				if (detectObjFilter[k].x> detectObjFilter[l].x)
//					detectObjFilter[k].x = detectObjFilter[l].x;
//				if (detectObjFilter[k].y > detectObjFilter[l].y)
//					detectObjFilter[k].y = detectObjFilter[l].y;
//				if (detectObjFilter[k].x + detectObjFilter[k].width < detectObjFilter[l].x + detectObjFilter[l].width)
//					detectObjFilter[k].width = detectObjFilter[l].x + detectObjFilter[l].width - detectObjFilter[k].x;
//				if (detectObjFilter[k].y + detectObjFilter[k].height < detectObjFilter[l].y + detectObjFilter[l].height)
//					detectObjFilter[k].height = detectObjFilter[l].y + detectObjFilter[l].height - detectObjFilter[k].y;
//				detectObjFilter.erase(detectObjFilter.begin() + l);
//				l--;
//			}
//		}
//	}
//	for (int k{ 0 }; k<detectObjFilter.size(); ++k)
//	{
//		out_images.push_back(in_images(detectObjFilter[k]));//облоси где найдена цель загружаются в out_images
//	}
//}
//
//void SVMandHOG::draw(Mat &inOutputImg)
//{
//	size_t i;
//	for (i = 0; i < detectObjFilter.size(); i++)
//	{
//		rectangle(inOutputImg, detectObjFilter[i], cv::Scalar(0, 255, 0), 1);
//	}
//}
//
//int SVMandHOG::getDetectedObj()
//{
//	return detectObjFilter.size();
//}
//int SVMandHOG::getObjX(int N)
//{
//	return detectObjFilter[N].x;
//}
//int SVMandHOG::getObjY(int N)
//{
//	return detectObjFilter[N].y;
//}
//int SVMandHOG::getObjHigth(int N)
//{
//	return detectObjFilter[N].height;
//}
//int SVMandHOG::getObjWigth(int N)
//{
//	return detectObjFilter[N].width;
//}
//
//void SVMandHOG::setParams(int medianBlurSize_in, int gausianBlurSize_in, int sizeRect_in)
//{
//	if (medianBlurSize_in > 31)
//		medianBlurSize = 31;
//	else if (medianBlurSize_in <= 0)
//		medianBlurSize = 1;
//	else if (medianBlurSize_in % 2 == 0)
//		medianBlurSize = medianBlurSize_in++;
//	else
//		medianBlurSize = medianBlurSize_in;
//
//	if (gausianBlurSize_in > 31)
//		gausianBlurSize = 31;
//	else if (gausianBlurSize_in <= 0)
//		gausianBlurSize = 1;
//	else if (gausianBlurSize_in % 2 == 0)
//		gausianBlurSize = gausianBlurSize_in++;
//	else
//		gausianBlurSize = gausianBlurSize_in;
//
//	if (sizeRect_in <= 3)
//		sizeRect = 3;
//	else
//		sizeRect = sizeRect_in;
//}