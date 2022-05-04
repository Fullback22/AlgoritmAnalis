#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>
#include "json.hpp"
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using json = nlohmann::json;

class VideoGenerate_v1_1
{
    int quantityObject;
    std::vector<int> objectH;
    std::vector<int> objectW;
    std::vector<float> SNR;
    std::vector<int> velX;
    std::vector<int> velY;
    std::vector<int> firstObjectFrame;
    std::vector<int> lastObjectFrame;
    std::vector<int> objectX;
    std::vector<int> objectY;
    std::vector<int> isPositivContrast;
    std::vector<int> objectColor;
    std::vector<int> isObject;
    float SKO;
    int medium;
    int frameW;
    int frameH;
    int numFrame;
    int FPS;
    int gausianSize;

    std::string saveFileName;
    std::string paramsFileName;

    bool videoIsStarted;
    cv::Mat background;
    bool noiseON;
    std::vector<int> objectDiagonal;
    void setObjectColor();
    void generateBakcground();
    cv::Rect findBoundingBox(cv::Point* pointArray, size_t arraySize);
public:
    VideoGenerate_v1_1(std::string saveFileName_, std::string paramsFileName_);
    void setSaveFile(std::string saveFileName_);
    void setParamsFile(std::string paramsFileName_);
    bool setVideoParams(std::string paramsFileName_="");
    bool updateVideoParams();
    void videoGeneratr(std::string saveFileName_ = "");
    
};

