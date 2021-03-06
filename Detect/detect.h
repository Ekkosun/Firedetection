#ifndef DETECT_H
#define DETECT_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/dnn.hpp"
#include <iostream>
using namespace cv;
extern cv::dnn::Net* YoloNet ;
class Detect
{

public:
    Detect(std::string* , std::string*,std::string*,int* ,int*);
    ~Detect();
    Mat *Images=NULL;                //array store the in and out images
    bool *Flag=NULL;                 //show if there is a fire
    VideoCapture * Video = NULL;        //to read the video
    VideoWriter *VideoSave = NULL;     //to save the video
    std::string* DetectMode = NULL;
    std::string* DetectMethod = NULL;
    std::string* DetectPath = NULL;
    int *sensitivity =NULL;
    int *threhold= NULL;
    //空指针问题要注意，今天调试失败在空指针上

    int OpenCamera();
    static int OpenCvDetect(Mat *,double );
    static int CnnDetect(Mat *,double);
    static int OpenCvAndCnnDetect(Mat *,double );
    int SaveVideo(std::string);
};

#endif // DETECT_H
std::vector<std::string> getOutputsNames( cv::dnn::Net& net);
