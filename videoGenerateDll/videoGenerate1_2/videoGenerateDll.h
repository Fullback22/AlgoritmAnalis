#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>
#include "json.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#ifdef VIDEOGENERATEDLL_EXPORTS
#define VIDEOGENERATEDLL_API __declspec(dllexport)
#else
#define VIDEOGENERATEDLL_API __declspec(dllimport)
#endif

using json = nlohmann::json;

int quantityObject{0};
std::vector<int> objectH;
std::vector<int> objectW;
std::vector<int> maxObjectW;
std::vector<int> maxObjectH;
std::vector<int> minObjectW;
std::vector<int> minObjectH;
std::vector<int> velW;
std::vector<int> velH;
std::vector<float> startAngel;
std::vector<float> velAngel;
std::vector<float> SNR;
std::vector<int> velX;
std::vector<int> velY;
std::vector<int> firstObjectFrame;
std::vector<int> lastObjectFrame;
std::vector<int> objectX;
std::vector<int> objectY;
std::vector<int> isPositivContrast;
std::vector<int> objectColor;
float SKO{0};
int medium{0};
int frameW{0};
int frameH{0};
int numFrame{0};
int FPS{0};

//std::string saveFileName;
std::string paramsFileName;

bool videoIsStarted{false};
cv::Mat background;
bool noiseON{true};
std::vector<int> objectDiagonal;

int random(int upLim = 100, int downLim = 0);
void setObjectColor();
void generateBakcground();
bool updateVideoParams();
cv::Rect findBoundingBox(cv::Point *pointArray, size_t arraySize);

extern "C" VIDEOGENERATEDLL_API int setVideoParams(std::string paramsFileName_);
extern "C" VIDEOGENERATEDLL_API int videoGeneratr(std::string saveFileName_);