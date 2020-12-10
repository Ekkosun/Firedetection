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
    sliderOfSensitivity->setMaximum(500);
    //sliderOfSensitivity->setSingleStep(1);
    sliderOfThrehold->setMinimum(1);
    sliderOfThrehold->setMaximum(500);
    //sliderOfThrehold->setSingleStep(1);
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
    radioButtonOfBoth = new QRadioButton("Cnn Opencv");
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
    radioButtonOfOpencv->move(990,455);
    radioButtonOfYolo->move(1080,455);
    radioButtonOfBoth->move(990,420);

    //timer
    readAndWriteTimer = new QTimer(this);

    //imageshow
    imageLabel = new QLabel[3];
    for (int i=0 ;i<3;i++)
        imageLabel[i].setParent(this);
    imageLabel[0].resize(800,680);
    imageLabel[1].resize(300,220);
    imageLabel[2].resize(300,220);
    imageLabel[0].move(20,20);
    imageLabel[1].move(860,20);
    imageLabel[2].move(860,240);
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
    this->readAndWriteTimer->start(40);
    this->detect->OpenCamera();
}

/*--------------------------------------------------------------------------detect one frame----------------------------------------------------*/
void UI::detectOneFrame(){
    int (*detectPtr)(Mat*,double)= NULL;
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
    //handle the image
    detectPtr(detect->Images,this->sliderOfSensitivity->value()/100.0);
    for (int i =0 ;i<3 ;i++ )
        scaleAtEqualScale(i);

    //show the image read and handled ，Opencv: 0.原图 1.带有标记的图 2.二值化图像
    //Cnn 0.原图 1.标记图 2.热力图
    imshow(detect->Images,3);

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
    QImage img;
    for(int i = 0 ; i<num ;i++){
        cvtColor(images[i],images[i],cv::COLOR_BGR2RGB);

        img = QImage((const unsigned char*)(images[i].data),images[i].cols,images[i].rows,QImage::Format_RGB888);
        imageLabel[i].setPixmap(QPixmap::fromImage(img));
    }

}
/*--------------------------------------------------------------------------clear Label----------------------------------------------------*/

void UI::clearLabel(){
    for (int i=0; i<3 ;i++ ) {
        this->imageLabel[i].clear();
    }
}
/*--------------------------------------------------------------------------scale at equal scale----------------------------------------------------*/
void UI::scaleAtEqualScale(int i){
    double weight_scale , height_scale,weight,height,weightoflabel,heightoflabel;
    weightoflabel = this->imageLabel[i].width();
    heightoflabel = this->imageLabel[i].height();
    weight = detect->Images[i].cols;
    height = detect->Images[i].rows;
    weight_scale = weight/weightoflabel;
    height_scale = height/heightoflabel;
    if(weight/height_scale>=heightoflabel)
        cv::resize(detect->Images[i],detect->Images[i],cv::Size(int(weight/weight_scale),int(height/weight_scale)));
    else
        cv::resize(detect->Images[i],detect->Images[i],cv::Size(int(weight/min(weight_scale,height_scale)),int(height/min(weight_scale,height_scale))));
}
