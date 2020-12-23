#include "detect.h"
Detect::Detect(std::string* dm , std::string*dmd ,std::string*path,int*sen ,int*thr){
    this->Images = new Mat[3];
    this->Flag = new bool;
    this->DetectMode = dm;
    this->DetectMethod = dmd;
    this->sensitivity = sen ;
    this->threhold = thr;
    this->Video = new VideoCapture;
    this->VideoSave = new VideoWriter;
    this->DetectPath = path;
}

Detect::~Detect(){

    delete(this->Images);
    delete(this->Flag);
    delete(this->Video);
    delete(this->DetectMode);
    delete(this->VideoSave);

}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Open Camera ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//OutPut :[-1:"NullPtr",-2:"Open Failed",0:"Normal"]
int Detect::OpenCamera(){

    if (Video!=NULL&&!Video->isOpened()){

        if(*DetectMode=="实时检测"){

            if (!Video->open(0)){
                return -2;
            }
        }
        else if(*DetectMode=="离线检测"){
            if (!Video->open(*DetectPath)){
                return -2;
            }
        }else{
            return -2;
        }
        return -1;
    }
    else{

    }
    return 0;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Opencv Detect ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int Detect::OpenCvDetect(Mat *img,double scale ){
    //mat to compute HSI S,I
    Mat s0 =cv::Mat(cv::Size(),CV_32FC1);
    Mat s1 =cv::Mat(cv::Size(),CV_32FC1);
    Mat s2 =cv::Mat(cv::Size(),CV_32FC1);
    Mat s3 =cv::Mat(cv::Size(),CV_32FC1);
    double avgofR = 0;
    //(2x2)kernal to dilate or erode
    Mat kernal = cv::Mat::ones(cv::Size(2,2),CV_8UC1);

    //Mat array to store R-G-B three channel
    Mat array[5];

    //split 3 channel
    split(*img,array);

    //compute mean of R channel
    avgofR = mean(array[2])[0];

    //compute S and I in HSI and compute mask of both
    array[3] = min(array[0],min(array[1],array[2]));
    array[0].convertTo(s0,CV_32FC1);
    array[1].convertTo(s1,CV_32FC1);
    array[2].convertTo(s2,CV_32FC1);
    array[3].convertTo(s3,CV_32FC1);
    s0 = s0+s1+s2+1;                    //to prevent nan
    array[4] = s0> 4.5*mean(s0)[0];     //mask of 3*I>Tthrehold (I in HSI) , here using 4.5*mean(3*I) as Ithrehold
    s3 = 1-3*s3/(s0);                   //compute S in HSI
    array[3] = (s3>=scale*mean(s3)[0]);   //mask of S>Sthrehold (S in HSI) , here using 1.3*mean(S) as Sthrehold


    //compute mask of RGB
    array[0] = array[1]>=array[0];      //mask of G>B
    array[1] = array[2]>=array[1];      //mask of R>G
    array[2] = (array[2]>=scale*avgofR)&array[1]&array[0]&array[3]|array[4];  //mask of all, here using  the I(in HSI)  solves the hight problem )


    //dilate and erode to make smooth conters
    erode(array[2],array[2],kernal,cv::Point(-1,-1),1); //first using erode clear error
    dilate(array[2],array[2],kernal,cv::Point(-1,-1),3);//second using twice dilate to make full of conters
    erode(array[2],array[2],kernal,cv::Point(-1,-1),2); //third using erode make conters smooth


    //obtain conters and handle conters
    img[0].copyTo(img[1]);
    array[2].convertTo(img[2],CV_8UC3);
    std::vector<std::vector<Point>> conters;
    findContours(array[2],conters,RETR_EXTERNAL,CHAIN_APPROX_NONE);
        for(int i=0 ;i<int(conters.size());i++){
            //delete small conters
             if(contourArea(conters[i])>2)
                   drawContours(*img,conters,i,Scalar(0,255,0),2);
        }
    return 0;
    
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Cnn Detect ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int Detect::CnnDetect(Mat *img,double scale){
    cv::Mat blob;
    std::vector<cv::Mat> output;
    cv::Mat row;
    int flag = 0;
    blob = cv::dnn::blobFromImage(img[0],1/255.0,cv::Size(416,416),cv::Scalar(0,0,0),true,false);

    YoloNet->setInput(blob);
    YoloNet->forward(output,getOutputsNames(*YoloNet));

    img[0].copyTo(img[1]);
    for(int num=0 ; num <int(output.size());num++)
    {
        for (int i = 0; i < output[num].rows; i++)
        {
            row = output[num].row(i);
            assert(!row.empty());
            //std::cout<<row.at<float>(0)<<std::endl;
            if ( row.at<float>(5) > scale/5.0)
            {
                flag = 1;
                cv::rectangle(img[0],cv::Point((row.at<float>(0)+row.at<float>(2)/2.0)*img->cols,(row.at<float>(1)+row.at<float>(3)/2.0)*img->rows),
                cv::Point((row.at<float>(0)-row.at<float>(2)/2.0)*img->cols,(row.at<float>(1)-row.at<float>(3)/2.0)*img->rows),cv::Scalar(123,111,130),2);
                cv::putText(img[0],cv::format("Fire %f",row.at<float>(5)),cv::Point((row.at<float>(0)-row.at<float>(2)/2.0)*img->cols,(row.at<float>(1)-row.at<float>(3)/2.0)*img->rows)
                ,cv::FONT_ITALIC,0.75,cv::Scalar(255,0,0),2);
            }
        }
    }
    return flag;
}

//*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~OpenCV Cnn Detect~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int Detect::OpenCvAndCnnDetect(Mat *img,double scale){
    std::cout << "Both";
    return 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Save Video As  Mp4~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//OutPut :[-1:"NullPtr",-2:"Open Failed",0:"Normal"]
int Detect::SaveVideo(std::string path){
    //store the video as .mp4
    int myFourCC = VideoWriter::fourcc('m', 'p', '4', 'v');      
    

    if(VideoSave!=NULL&&!VideoSave->isOpened()){
        //get the frame size
        Size size = Size(Video->get(CAP_PROP_FRAME_WIDTH),Video->get(CAP_PROP_FRAME_HEIGHT));

        if(!VideoSave->open(path,myFourCC,30,size,true)){
            return -2;
        }
        
    }
    else{
        return -1;
    }
    return 0;

}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Get Net Output Name~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//Output : [vector<string> names]
//obtain the name of the output layer,to ge the output of special layer
std::vector<std::string> getOutputsNames( cv::dnn::Net& net)
{
    static std::vector<std::string> names;
    if (names.empty())
    {
        //Get the indices of the output layers, i.e. the layers with unconnected outputs
        std::vector<int> outLayers = net.getUnconnectedOutLayers();

        //get the names of all the layers in the network
        std::vector<std::string> layersNames = net.getLayerNames();

        // Get the names of the output layers in names
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
            names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}



