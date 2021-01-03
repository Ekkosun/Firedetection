#include "beginbutton.h"


BeginButton::BeginButton(QWidget *parent) : QPushButton(parent)
{
    sensitivity = new int;
    threhold = new int;
    detectMode = new std::string;
    detectMethod = new std::string;
    videoPath = new std::string;
}

void BeginButton::SetDetectParam(QAbstractButton *button){
    if(button->text()=="实时检测"||button->text()=="离线检测"){
        *this->detectMode = button->text().toStdString();
        //qDebug() << QString::fromStdString(*detectMode) ;
    }
    else if(button->text()=="Opencv"||button->text()=="Cnn"||button->text()=="Cnn Opencv"){
        *this->detectMethod = button->text().toStdString();
        //qDebug() << QString::fromStdString(*detectMethod) ;
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Check The Mode And Method~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//output : [-1:"Param is not enough",0:"Ok"]
int BeginButton::checkParam(){
    if(*this->detectMode==""){
        QMessageBox::warning(this,"Error","检测模式是必须选择的，请选择一种检测模式！！");
        return -1;
    }

    if(*this->detectMethod==""){
        QMessageBox::warning(this,"Error","检测方式是必须选择的，请选择一种检测方式！！");
        return -1;
    }
    if(*this->detectMode=="离线检测"){
        *this->videoPath = QFileDialog::getOpenFileName(nullptr,"请选择要检测的视频","","*.*").toStdString();
    }
    return 0;

}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Detect  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//used when detect button is clicked
//The most significant function
void BeginButton::BeginDetect(){
    if(this->text()=="开始检测"){

        if(this->checkParam()==-1)
            return ;
        this->setText("结束检测");
        emit Begin(this->detectMode,this->detectMethod,this->videoPath,this->sensitivity,this->threhold);
        }

    else{

        this->setText("开始检测");
        emit End();
    }

}





