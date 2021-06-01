#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "iostream"
#include "fstream"
#include <vector>
#include "json.hpp"

#ifdef GLOBALGIS_DLL_EXPORTS
#define GLOBALGIS_DLL_API __declspec(dllexport)
#else
#define GLOBALGIS_DLL_API __declspec(dllimport)
#endif

using json = nlohmann::json;

cv::Mat coreMorfologis;
cv::Size rectSize;
std::vector<cv::Rect> detectObj;
int thresh;
int medianBlurSize;
int gausianBlurSize;
int morfologiSize;

void setParams(int thresh, int medianBlurSize, int gausianBlurSize, int morfologiSize, int sizeRectObj);

extern "C" GLOBALGIS_DLL_API int work(cv::Mat inputImg);
extern "C" GLOBALGIS_DLL_API int draw(cv::Mat &inOutputImg);
extern "C" GLOBALGIS_DLL_API int getDetectedObj();
extern "C" GLOBALGIS_DLL_API int getObjX(int N);
extern "C" GLOBALGIS_DLL_API int getPredictCoordinate(std::vector<int>&x, std::vector<int>&y, std::vector<int>&width, std::vector<int>&height);
extern "C" GLOBALGIS_DLL_API int getObjY(int N);
extern "C" GLOBALGIS_DLL_API int getObjHigth(int N);
extern "C" GLOBALGIS_DLL_API int getObjWigth(int N);
extern "C" GLOBALGIS_DLL_API int setParams(std::string fileName);
extern "C" GLOBALGIS_DLL_API int activParams(bool &thresh_enbl, bool &medianFilter_enbl, bool &bin_1_enbl, bool &gausFilter_enbl,
												bool &marfolFilter, bool &rectSize_enl, int &thresh, bool &tempImg);