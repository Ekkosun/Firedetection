#ifndef UI_H
#define UI_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QFileDialog>
#include <QThread>
#include <QTimer>
#include "beginbutton.h"
#include "detect.h"


//UI
class UI : public QWidget
{
    Q_OBJECT
public:
    UI(QWidget *parent = nullptr);
    ~UI();



    //begin button
    BeginButton *buttonOfBegin= NULL;

    //slide bar1-2 to set sensitivity and threhold
    QSlider* sliderOfSensitivity= NULL;
    QSlider* sliderOfThrehold= NULL;

    //label
    QLabel* labelOfSensitivity= NULL;
    QLabel* labelOfThrehold= NULL;

    //radio button
    QButtonGroup* buttonGroupOfDetectMode= NULL;
    QRadioButton* radioButtonOfOnlineDetect= NULL;
    QRadioButton* radioButtonOfOfflineDetect= NULL;
    QButtonGroup* buttonGroupOfDetectAlgorithm= NULL;
    QRadioButton* radioButtonOfOpencv= NULL;
    QRadioButton* radioButtonOfYolo= NULL;
    QRadioButton* radioButtonOfBoth= NULL;

    //label to show image
    QLabel* imageLabel0 = NULL;
    //timer to read frame
    QTimer* readAndWriteTimer = NULL;

    Detect* detect = NULL ;

    void beginInitial(std::string* , std::string*,std::string*,int* ,int*);
    void detectOneFrame();
    void endDetect();
    void imshow(Mat* , int );
};

#endif // UI_H
