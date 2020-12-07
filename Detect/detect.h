#ifndef DETECT_H
#define DETECT_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
class Detect
{

public:
    Detect();
    ~Detect();
    Mat *Images=NULL;                //array store the in and out images
    bool *Flag=NULL;                 //show if there is a fire
    VideoCapture * Video = NULL;        //to read the video
    VideoWriter *VideoSave = NULL;     //to save the video
    String * DetectMode = NULL;
    String * DetectMethod = NULL;
    String * DetectPath = NULL;
    //空指针问题要注意，今天调试失败在空指针上


    int OpenCamera();
    int OpenCvDetect();
    int CnnDetect();
    int OpenCvAndCnnDetect();
    int SaveVideo();
};

#endif // DETECT_H
