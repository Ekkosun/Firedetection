#include "detect.h"

Detect::Detect(){
    this->Images = new Mat[4];
    this->Flag = new bool;
    this->DetectMode = new String;
    this->DetectMethod = new String;
    this->Video = new VideoCapture;
    this->VideoSave = new VideoWriter;
    this->DetectPath = new String;
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
int Detect::OpenCvDetect(){
    
    Mat BinarryImage;
    return 0;
    
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Cnn Detect ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int Detect::CnnDetect(){
    std::cout << "Cnn";
    return 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~OpenCV Cnn Detect~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int Detect::OpenCvAndCnnDetect(){
    std::cout << "Both";
    return 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Save Video As  Mp4~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//OutPut :[-1:"NullPtr",-2:"Open Failed",0:"Normal"]s
int Detect::SaveVideo(){
    //store the video as .mp4
    int myFourCC = VideoWriter::fourcc('m', 'p', '4', 'v');      
    

    if(VideoSave!=NULL&&!VideoSave->isOpened()){
        //get the frame size
        Size size = Size(Video->get(CAP_PROP_FRAME_WIDTH),Video->get(CAP_PROP_FRAME_HEIGHT));

        if(!VideoSave->open("new.mp4",myFourCC,25,size,true)){
            return -2;
        }
        
    }
    else{
        return -1;
    }
    return 0;

}






/*int main(){
    
    Detect a;
    a.OpenCamera();
    a.Video->read(a.Images[0]);
    a.SaveVideo();
    while (1)
    {
        a.Video->read(a.Images[0]);
        a.VideoSave->write(a.Images[0]);
        imshow("Test-A",a.Images[0]);
        imshow("Test-R",a.Images[0]);
        imshow("Test_G",a.Images[0]);
        imshow("Test-B",a.Images[0]);
        if(waitKey(20)==27){
            break;
        }
    }
    a.Video->release();
    a.VideoSave->release();
    return 0;

}*/
