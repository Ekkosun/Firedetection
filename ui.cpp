#include "ui.h"
#include <QDebug>
UI::UI(QWidget *parent)
    : QWidget(parent)
{

    buttonOfBegin = new BeginButton;
    //set the position of button
    buttonOfBegin->setParent(this);
    buttonOfBegin->move(1030,650);
    buttonOfBegin->resize(75,23);
    buttonOfBegin->setText("开始检测");

    //set the  style of slider
    sliderOfSensitivity = new QSlider(Qt::Horizontal,this);
    sliderOfThrehold = new QSlider(Qt::Horizontal,this);
    sliderOfSensitivity->resize(160,22);
    sliderOfThrehold->resize(160,22);
    sliderOfSensitivity->setMinimum(1);
    sliderOfSensitivity->setMaximum(100);
    sliderOfThrehold->setMinimum(1);
    sliderOfThrehold->setMaximum(100);
    //set the positon of slider
    sliderOfSensitivity->move(990,600);
    sliderOfThrehold->move(990,540);

    //set  label
    labelOfSensitivity = new QLabel("检测灵敏度:",this);
    labelOfThrehold = new QLabel("检测阈值:",this);
    //set the position of label
    labelOfSensitivity->move(1035,580);
    labelOfSensitivity->resize(80,16);
    labelOfThrehold->move(1040,520);
    labelOfThrehold->resize(80,16);

    //set radio button to choose the mode used in detect
    buttonGroupOfDetectAlgorithm = new QButtonGroup(this);
    buttonGroupOfDetectMode = new QButtonGroup(this);
    radioButtonOfBoth = new QRadioButton("Cnn Opencv",this);
    radioButtonOfOfflineDetect = new QRadioButton("离线检测",this);
    radioButtonOfOnlineDetect = new QRadioButton("实时检测",this);
    radioButtonOfOpencv = new QRadioButton("Opencv",this);
    radioButtonOfYolo = new QRadioButton("Cnn",this);
    buttonGroupOfDetectMode->addButton(radioButtonOfOnlineDetect,0);
    buttonGroupOfDetectMode->addButton(radioButtonOfOfflineDetect,1);
    buttonGroupOfDetectAlgorithm->addButton(radioButtonOfBoth,0);
    buttonGroupOfDetectAlgorithm->addButton(radioButtonOfOpencv,1);
    buttonGroupOfDetectAlgorithm->addButton(radioButtonOfYolo,2);
    radioButtonOfBoth->resize(111,41);
    radioButtonOfOpencv->resize(111,41);
    radioButtonOfYolo->resize(111,41);
    radioButtonOfOfflineDetect->resize(111,41);
    radioButtonOfOnlineDetect->resize(111,41);

    //set the position of radio button
    radioButtonOfOnlineDetect->move(990,480);
    radioButtonOfOfflineDetect->move(1080,480);
    radioButtonOfOpencv->move(990,380);
    radioButtonOfYolo->move(1080,380);
    radioButtonOfBoth->move(990,420);

    //timer
    readAndWriteTimer = new QTimer(this);

    //imageshow
    imageLabel = new QLabel[3];
    for(int i=0;i<3;i++){
        imageLabel[i].setParent(this);
        imageLabel[i].resize(300,400);
        imageLabel[i].move(300*i,0);
    }

    connect(buttonGroupOfDetectMode,static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),buttonOfBegin,&BeginButton::SetDetectParam);
    connect(buttonGroupOfDetectAlgorithm,static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),buttonOfBegin,&BeginButton::SetDetectParam);
    connect(buttonOfBegin,&BeginButton::clicked,buttonOfBegin,&BeginButton::BeginDetect);
    connect(buttonOfBegin,&BeginButton::Begin,this,&UI::beginInitial);
    connect(buttonOfBegin,&BeginButton::End,this,&UI::endDetect);
    connect(readAndWriteTimer,&QTimer::timeout,this,&UI::detectOneFrame);

}

UI::~UI()
{
}

/*--------------------------------------------------------------------------beign initial----------------------------------------------------*/
//input [detectmode , detectmethod ,videopath, sensitivity , thredhold]

void UI::beginInitial(std::string* dm , std::string*dmd,std::string* path,int* sen ,int*thr){
    if(this->detect==NULL)
    {
        this->detect = new Detect(dm,dmd,path,sen,thr);
        qDebug() << QString::fromStdString(*detect->DetectMethod) ;
    }

    //set timer every 30ms read and write one frame
    this->readAndWriteTimer->start(30);
    this->detect->OpenCamera();
}

/*--------------------------------------------------------------------------detect one frame----------------------------------------------------*/
void UI::detectOneFrame(){
    int (*detectPtr)()= NULL;
    if(*detect->DetectMethod=="Opencv"){
        detectPtr = detect->OpenCvDetect;
    }else if(*detect->DetectMethod=="Cnn"){
        detectPtr = detect->CnnDetect;
    }else{
        detectPtr = detect->OpenCvAndCnnDetect;
    }

    detect->Video->read(detect->Images[0]);
    if(detect->Images[0].empty()){
        clearLabel();
        emit buttonOfBegin->End();
        return;
    }
    detect->Images[1]=detect->Images[0];
    detect->Images[2]=detect->Images[0];
    //handle the image

    imshow(detect->Images,3);
    //连续12帧存在火焰则证明有火，



    //show the image read and handled ，Opencv: 0.原图 1.带有标记的图 2.二值化图像
    //Cnn 0.原图 1.标记图 2.热力图


}
/*--------------------------------------------------------------------------end  detecting----------------------------------------------------*/
void UI::endDetect(){
    readAndWriteTimer->stop();
    //stop video capture
    if(detect->Video->isOpened()){
        detect->Video->release();
    }
    if(buttonOfBegin->text()=="结束检测"){

        buttonOfBegin->setText("开始检测");
    }
    //clear the lable
    clearLabel();

    //
}
/*--------------------------------------------------------------------------end  detecting----------------------------------------------------*/
//input:[ptr of images , num to display]
void UI::imshow(cv::Mat*images,int num){
    cvtColor(images[0],images[0],cv::COLOR_BGR2RGB);
    for(int i = 0 ; i<num ;i++){
        QImage img = QImage((const unsigned char*)(images[i].data),images[i].cols,images[i].rows,QImage::Format_RGB888);
        imageLabel[i].setPixmap(QPixmap::fromImage(img));
    }

}
/*--------------------------------------------------------------------------clear Label----------------------------------------------------*/

void UI::clearLabel(){
    for (int i=0; i<3 ;i++ ) {
        this->imageLabel[i].clear();
    }
}
