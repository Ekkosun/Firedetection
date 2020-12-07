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
    radioButtonOfBoth = new QRadioButton("Yolo Opencv",this);
    radioButtonOfOfflineDetect = new QRadioButton("离线检测",this);
    radioButtonOfOnlineDetect = new QRadioButton("实时检测",this);
    radioButtonOfOpencv = new QRadioButton("Opencv",this);
    radioButtonOfYolo = new QRadioButton("Yolo",this);
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


    connect(buttonGroupOfDetectMode,static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),buttonOfBegin,&BeginButton::SetDetectParam);
    connect(buttonGroupOfDetectAlgorithm,static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),buttonOfBegin,&BeginButton::SetDetectParam);
    connect(buttonOfBegin,&BeginButton::clicked,buttonOfBegin,&BeginButton::BeginDetect);
}

UI::~UI()
{
    detectThread->quit();
}


