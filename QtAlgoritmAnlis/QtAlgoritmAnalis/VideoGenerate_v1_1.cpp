#include "VideoGenerate_v1_1.h"

cv::Rect VideoGenerate_v1_1::findBoundingBox(cv::Point* pointArray, size_t arraySize)
{
    int maxX{ 0 };
    int maxY{ 0 };

    int minX{ 0 };
    int minY{ 0 };
    if (arraySize > 0)
    {
        minX = (pointArray + 1)->x;
        minY = (pointArray + 1)->y;
    }
    for (size_t i{ 0 }; i < arraySize; ++i)
    {
        if ((pointArray + i)->x > maxX)
            maxX = (pointArray + i)->x;
        if ((pointArray + i)->y > maxY)
            maxY = (pointArray + i)->y;
        if ((pointArray + i)->x < minX)
            minX = (pointArray + i)->x;
        if ((pointArray + i)->y < minY)
            minY = (pointArray + i)->y;
    }
    return cv::Rect(cv::Point(minX, minY), cv::Point(maxX, maxY));
}

void VideoGenerate_v1_1::setObjectColor()
{
    objectColor.resize(quantityObject);
    if (SKO <= 0)
    {
        SKO = 1;
        noiseON = false;
    }
    for (size_t i{ 0 }; i < objectColor.size(); ++i)
    {
        int overflow{ 0 };
        if (medium * SNR[i]  >= 255)
        {
            overflow = 1;
        }
        if (medium / SNR[i]  <= 0)
        {
            overflow += 10;
        }
        if (overflow == 0)
        {
            if (isPositivContrast[i])
            {
                objectColor[i] = medium * SNR[i];
            }
            else
            {
                objectColor[i] = medium / SNR[i];
            }
        }
        else if (overflow == 1)
        {
            objectColor[i] = medium / SNR[i];
        }
        else if (overflow == 10)
        {
            objectColor[i] = medium * SNR[i];
        }
        else
        {
            if (isPositivContrast[i])
            {
                objectColor[i] = 255;
            }
            else
            {
                objectColor[i] = 0;
            }
        }
    }
}

void VideoGenerate_v1_1::generateBakcground()
{
    if (SKO <= 0)
    {
        SKO = 1;
        noiseON = false;
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::normal_distribution<float> dist(medium, SKO);
    for (int i{ 0 }; i < frameH * frameW; ++i)
    {
        if (noiseON)
        {
            float val = dist(gen);
            if (val > 255.0)
            {
                val = 255.0;
            }
            else if (val < 0.0)
            {
                val = 0.0;
            }
            background.data[i] = static_cast<unsigned char>(val);
        }
        else
        {
            background.data[i] = static_cast<unsigned char>(medium);
        }
    }
}

VideoGenerate_v1_1::VideoGenerate_v1_1(std::string saveFileName_, std::string paramsFileName_):
	quantityObject(0),
	objectH(),
	objectW(),
	SNR(),
	velX(),
	velY(),
	firstObjectFrame(),
    lastObjectFrame(),
    objectX(),
    objectY(),
	isPositivContrast(),
	objectColor(),
    isObject(),
	SKO(0),
	medium(0),
	frameW(0),
	frameH(0),
	numFrame(0),
	FPS(0),
    gausianSize(0),
	saveFileName(saveFileName_),
	paramsFileName(paramsFileName_),
    videoIsStarted(false),
    background(0,0,CV_8U),
    noiseON(true),
    objectDiagonal()
{
}

void VideoGenerate_v1_1::setSaveFile(std::string saveFileName_)
{
	saveFileName = saveFileName_;
}

void VideoGenerate_v1_1::setParamsFile(std::string paramsFileName_)
{
	paramsFileName = paramsFileName_;
}

bool VideoGenerate_v1_1::setVideoParams(std::string paramsFileName_)
{
    if (paramsFileName_ != "")
    {
        paramsFileName = paramsFileName_;
    }
    if (videoIsStarted)
    {
        std::cout << "ERROR: the video is already being recorded" << std::endl;
        return false;
    }
    else
    {
        json jsonConf;

        std::ifstream paramFile(paramsFileName);
        if (!paramFile.is_open()) {
            std::cout << "ERROR: File not opened" << std::endl;
            return false;
        }
        std::cout << "OK: File opened" << std::endl;

        try {
            jsonConf = json::parse(paramFile);
        }
        catch (...) {
            std::cout << "ERROR: JSON" << std::endl;
            paramFile.close();
            return false;
        }//try...
        try {
            json channelJson = jsonConf.at("VIDEO_PARAMS");
            quantityObject = channelJson.at("quantity").get<int>();
            objectH = channelJson.at("objectH").get<std::vector<int>>();
            objectW = channelJson.at("objectW").get<std::vector<int>>();
            SNR = channelJson.at("signalToNoiseRatio").get<std::vector<float>>();
            velX = channelJson.at("velX").get<std::vector<int>>();
            velY = channelJson.at("velY").get<std::vector<int>>();
            firstObjectFrame = channelJson.at("firstFrame").get<std::vector<int>>();
            lastObjectFrame = channelJson.at("lastFrame").get<std::vector<int>>();
            objectX = channelJson.at("startX").get<std::vector<int>>();
            objectY = channelJson.at("startY").get<std::vector<int>>();
            isPositivContrast = channelJson.at("positiveContrast").get<std::vector<int>>();
            isObject = channelJson.at("isObject").get<std::vector<int>>();
            SKO = channelJson.at("sko").get<float>();
            medium = channelJson.at("medium").get<int>();
            frameW = channelJson.at("frameW").get<int>();
            frameH = channelJson.at("frameH").get<int>();
            numFrame = channelJson.at("numFrames").get<int>();
            FPS = channelJson.at("fps").get<float>();
            gausianSize= channelJson.at("gausian").get<int>();
        }
        catch (...) {
            std::cout << "ERROR: JSON" << std::endl;
            paramFile.close();
            return false;
        }//try...
        setObjectColor();
        objectDiagonal.resize(quantityObject);
        for (size_t i{ 0 }; i < quantityObject; ++i)
        {
            objectDiagonal[i] = static_cast<int>(pow(objectH[i] + objectH[i] + objectW[i] * objectW[i], 0.5));
            if (objectDiagonal[i] > objectX[i])
            {
                objectX[i] = objectDiagonal[i];
            }
            if (objectDiagonal[i] > objectY[i])
            {
                objectX[i] = objectDiagonal[i];
            }
        }
        if (gausianSize % 2 == 0)
            ++gausianSize;
        background = cv::Mat(frameH, frameW, CV_8U);
        paramFile.close();
        return true;
    }
}

bool VideoGenerate_v1_1::updateVideoParams()
{
    json jsonConf;
    std::vector<int> startX;
    std::vector<int> startY;
    std::vector<int> oldVelX{ velX };
    std::vector<int> oldVelY{ velY };
    std::vector<int> updateStartCoordinate{ 0 };
    int oldQuantity{ quantityObject };
    std::ifstream paramFile(paramsFileName);
    if (!paramFile.is_open()) {
        std::cout << "ERROR: File not opened" << std::endl;
        return false;
    }
    std::cout << "OK: File opened" << std::endl;

    try {
        jsonConf = json::parse(paramFile);
    }
    catch (...) {
        std::cout << "ERROR: JSON" << std::endl;
        paramFile.close();
        return false;
    }//try...
    try {
        json channelJson = jsonConf.at("VIDEO_PARAMS");
        quantityObject = channelJson.at("quantity").get<int>();
        updateStartCoordinate = channelJson.at("updateStartCoordinate").get<std::vector<int>>();
        objectH = channelJson.at("objectH").get<std::vector<int>>();
        objectW = channelJson.at("objectW").get<std::vector<int>>();
        SNR = channelJson.at("signalToNoiseRatio").get<std::vector<float>>();
        velX = channelJson.at("velX").get<std::vector<int>>();
        velY = channelJson.at("velY").get<std::vector<int>>();
        firstObjectFrame = channelJson.at("firstFrame").get<std::vector<int>>();
        lastObjectFrame = channelJson.at("lastFrame").get<std::vector<int>>();
        startX = channelJson.at("startX").get<std::vector<int>>();
        startY = channelJson.at("startY").get<std::vector<int>>();
        isPositivContrast = channelJson.at("positiveContrast").get<std::vector<int>>();
        SKO = channelJson.at("sko").get<float>();
        medium = channelJson.at("medium").get<int>();
        gausianSize = channelJson.at("gausian").get<int>();
    }
    catch (...) {
        std::cout << "ERROR: JSON" << std::endl;
        paramFile.close();
        return false;
    }//try...
    if (gausianSize % 2 == 0)
        ++gausianSize;
    setObjectColor();
    objectDiagonal.resize(quantityObject);
    for (size_t i{0}; i < quantityObject; ++i)
    {
        if (oldQuantity >= i)
        {
            if (updateStartCoordinate[i] == 1)
            {
                objectX[i] = startX[i];
                objectY[i] = startY[i];
            }
        }
        else
        {
            objectX[i] = startX[i];
            objectY[i] = startY[i];
        }
    }
    for (size_t i{ 0 }; i < quantityObject; ++i)
    {
        if (oldQuantity >= i)
        {
            if((oldVelX[i]>0&&velX[i]>0)|| (oldVelX[i] < 0 && velX[i] < 0))
            { }
            else
            {
                velX[i] = -velX[i];
            }
            if ((oldVelY[i] > 0 && velY[i] > 0) || (oldVelY[i] < 0 && velY[i] < 0))
            {
            }
            else
            {
                velY[i] = -velY[i];
            }
        }
    }
    for (size_t i{ 0 }; i < quantityObject; ++i)
    {
        objectDiagonal[i] = static_cast<int>(pow(objectH[i] + objectH[i] + objectW[i] * objectW[i], 0.5));
        if (objectDiagonal[i] > objectX[i])
        {
            objectX[i] = objectDiagonal[i];
        }
        if (objectDiagonal[i] > objectY[i])
        {
            objectX[i] = objectDiagonal[i];
        }
    }
    paramFile.close();
    return true;
}

void VideoGenerate_v1_1::videoGeneratr(std::string saveFileName_)
{
    videoIsStarted = true;
    if (saveFileName_ != "")
    {
        saveFileName = saveFileName_;
    }
    cv::VideoWriter outVideo(cv::String(saveFileName), cv::VideoWriter::fourcc('Y', 'V', '1', '2'), FPS, cv::Size(frameW, frameH), true);
    std::size_t found = saveFileName.find_last_of("/\\");
    std::ofstream objectCoordinate(saveFileName.substr(0, found) +"/realObjectCoordinate_gen.txt", std::ios_base::out | std::ios_base::trunc);
    for (size_t i{ 0 }; i < numFrame; ++i)
    {
        updateVideoParams();
        generateBakcground();
        for (size_t j{ 0 }; j < quantityObject; ++j)
        {
            cv::RotatedRect drawRect(cv::Point(objectX[j], objectY[j]), cv::Size(objectW[j], objectH[j]), 0.0);
            cv::Point2f vertices2f[4];
            drawRect.points(vertices2f);
            cv::Point vertices[4];
            for (int q{ 0 }; q < 4; ++q)
            {
                vertices[q] = vertices2f[q];
            }
            cv::fillConvexPoly(background, vertices, 4, objectColor[j], 8);
            cv::Rect bounerBox(findBoundingBox(vertices, 4));
            if (isObject[j] == 1)
                objectCoordinate << i+1 <<"\t" << bounerBox.x << "\t" << bounerBox.y << "\t" << bounerBox.width << "\t" << bounerBox.height << "\t" << bounerBox.height * bounerBox.width << std::endl;
            objectX[j] += velX[j];
            if ((objectX[j] - objectDiagonal[j] <= 0) || (objectX[j] + objectDiagonal[j] >= frameW))
            {
                velX[j] = -velX[j];
            }
            objectY[j] += velY[j];
            if ((objectY[j] - objectDiagonal[j] <= 0) || (objectY[j] + objectDiagonal[j] >= frameH))
            {
                velY[j] = -velY[j];
            }
        }
        cv::Mat outFrame;
        if (gausianSize >= 3)
            cv::GaussianBlur(background, background, cv::Size(gausianSize, gausianSize), 0);
        cv::cvtColor(background, outFrame, cv::COLOR_GRAY2BGR);
        outVideo.write(outFrame);

        cv::imshow("Video generate 1.1", outFrame);
        cv::waitKey(1);
    }
    objectCoordinate.close();
    cv::destroyAllWindows();
    outVideo.release();
    videoIsStarted = false;
}
