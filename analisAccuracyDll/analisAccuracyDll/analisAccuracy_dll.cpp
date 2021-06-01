#include "pch.h"
#include "analisAccuracy_dll.h"

ANALISACCURACY_DLL_API int setRealAndPredictObj(std::vector<int>* real_x, std::vector<int>* real_y, std::vector<int>* real_width, std::vector<int>* real_height, std::vector<int>* pred_x, std::vector<int>* pred_y, std::vector<int>* pred_width, std::vector<int>* pred_height)
{
    //probability << "dsfjhsdhgbjhfhgjhsd";
    std::vector<int> bufer_r_x(*real_x);
    std::vector<int> bufer_r_y(*real_y);
    std::vector<int> bufer_r_w(*real_width);
    std::vector<int> bufer_r_h(*real_height);
    std::vector<int> bufer_p_x(*pred_x);
    std::vector<int> bufer_p_y(*pred_y);
    std::vector<int> bufer_p_w(*pred_width);
    std::vector<int> bufer_p_h(*pred_height);
    int truePos_{ 0 };
    int falseNegativ_{ 0 };
    for (size_t i{ 0 }; i < bufer_r_x.size(); ++i)
    {
        cv::Rect realRect(bufer_r_x[i], bufer_r_y[i], bufer_r_w[i], bufer_r_h[i]);
        float realRectDiagonal(std::sqrt(std::pow(bufer_r_w[i], 2) + std::pow(bufer_r_h[i], 2)));
        bool f_falseNegativ{ true };
        for (size_t j{ 0 }; j < bufer_p_x.size(); ++j)
        {
            float predRectDiagonal(std::sqrt(std::pow(bufer_p_w[j], 2) + std::pow(bufer_p_h[j], 2)));
            cv::Point predictCenterPoint(bufer_p_x[j] + bufer_p_w[j] / 2, bufer_p_y[j] + bufer_p_h[j] / 2);
            if (realRect.contains(predictCenterPoint) && realRectDiagonal * coefficientMinDiagonal < predRectDiagonal && realRectDiagonal * coefficientMaxDiagonal > predRectDiagonal)
            {
                ++truePos_;
                bufer_p_x.erase(bufer_p_x.begin() + j);
                bufer_p_y.erase(bufer_p_y.begin() + j);
                bufer_p_w.erase(bufer_p_w.begin() + j);
                bufer_p_h.erase(bufer_p_h.begin() + j);
                f_falseNegativ = false;
                --j;
            }
        }
        bufer_r_x.erase(bufer_r_x.begin() + i);
        bufer_r_y.erase(bufer_r_y.begin() + i);
        bufer_r_w.erase(bufer_r_w.begin() + i);
        bufer_r_h.erase(bufer_r_h.begin() + i);
        --i;
        if (f_falseNegativ)
        {
            ++falseNegativ_;
        }
    }
    falsePos += bufer_p_x.size();
    truePos += truePos_;
    falseNegativ += falseNegativ_;
    probability << frame << "\t" << real_x->size() << "\t" << pred_x->size() << "\t" << truePos_ << "\t" << bufer_p_x.size() << "\t" << falseNegativ_ << std::endl;
    ++frame;
    return 0;
}

ANALISACCURACY_DLL_API int process(std::string saveDir)
{   
    if (outAccuracy.is_open() == false)
        outAccuracy.open(saveDir + "accuracy.txt", std::ios::app);
    outAccuracy << "accuracy" << std::endl;
    float accuracy{ static_cast<float>(truePos) / static_cast<float>(truePos + falsePos + falseNegativ) };
    outAccuracy << accuracy;
    probability.close();
    outAccuracy.close();
}

ANALISACCURACY_DLL_API int setSaveDirictory(std::string saveDir)
{
    saveDirictory = saveDir;
    if (probability.is_open() == false)
        probability.open(saveDir + "probability.txt", std::ios::app);
    probability << "frame\tRelOb\tDetObj\tTrueDet\tFalseDet\tNotDet" << std::endl;
    return 0;
}

ANALISACCURACY_DLL_API int setAnalisParams(std::string paramsFileName_)
{
    json jsonConf;
    std::ifstream paramFile(paramsFileName_);
    try {
        jsonConf = json::parse(paramFile);
    }
    catch (...) {
        //std::cout << "ERROR: JSON" << std::endl;
        paramFile.close();
        return -1;
    }//try...
    try {
        json channelJson = jsonConf.at("ANALIS_SETTINGS");
        coefficientMinDiagonal = channelJson.at("coefficientMinDiagonal").get<float>();
        coefficientMaxDiagonal = channelJson.at("coefficientMaxDiagonal").get<float>();
    }
    catch (...) {
        //std::cout << "ERROR: JSON" << std::endl;
        paramFile.close();
        return -1;
    }//try...
    paramFile.close();
    return 0;
}

ANALISACCURACY_DLL_API int reset()
{
    frame = 0;
    truePos= 0;
    falsePos = 0;
    falseNegativ = 0;
    coefficientMaxDiagonal = 1.5;
    coefficientMinDiagonal = 0.5;
    return 0;
}
