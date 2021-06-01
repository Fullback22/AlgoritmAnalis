#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "json.hpp"

#ifdef ANALISACCURACY_DLL_EXPORTS
#define ANALISACCURACY_DLL_API __declspec(dllexport)
#else
#define ANALISACCURACY_DLL_API __declspec(dllimport)
#endif

using json = nlohmann::json;
int frame{ 0 };
int truePos{ 0 };
int falsePos{ 0 };
int falseNegativ{ 0 };
float coefficientMaxDiagonal{ 1.5 };
float coefficientMinDiagonal{ 0.5 };
std::string saveDirictory;
std::ofstream probability;
std::ofstream outAccuracy;

extern "C" ANALISACCURACY_DLL_API int setRealAndPredictObj(std::vector<int> * real_x, std::vector<int> * real_y, std::vector<int> * real_width, std::vector<int> * real_height, std::vector<int> * pred_x, std::vector<int> * pred_y, std::vector<int> * pred_width, std::vector<int> * pred_height);
extern "C" ANALISACCURACY_DLL_API int process(std::string saveDir);
extern "C" ANALISACCURACY_DLL_API int setSaveDirictory(std::string saveDir);
extern "C" ANALISACCURACY_DLL_API int setAnalisParams(std::string paramsFileName_);
extern "C" ANALISACCURACY_DLL_API int reset();