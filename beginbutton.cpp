#include "beginbutton.h"

BeginButton::BeginButton(QWidget *parent) : QPushButton(parent)
{
    sensitivity = new int;
    threhold = new int;
}

void BeginButton::SetDetectParam(QAbstractButton *button){
    if(button->text()=="实时检测"||button->text()=="离线检测"){
        *this->DetectMode = button->text().toStdString();
    }
    else if(button->text()=="Opencv"||button->text()=="Yolo"||button->text()=="Yolo Opencv"){
        *this->DetectMethod = button->text().toStdString();
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Check The Mode And Method~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//output : [-1:"Param is not enough",0:"Ok"]
int BeginButton::checkParam(){
    if(*this->DetectMode==""){
        QMessageBox::warning(this,"Error","检测模式是必须选择的，请选择一种检测模式！！");
        return -1;
    }

    if(*this->DetectMethod==""){
        QMessageBox::warning(this,"Error","检测方式是必须选择的，请选择一种检测方式！！");
        return -1;
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
        }

    else{

        this->setText("开始检测");
    }

}

void BeginButton::DoDetect(){
    if(*this->DetectMode == "实时检测"){
        this->OpenCamera();
    }else if(*this->DetectMode == "离线检测"){
        *this->DetectPath = QFileDialog::getOpenFileName(nullptr,"打开文件","/","Files(*.avi *.mp4)").toStdString();
        this->OpenCamera();
    }else{
    }

}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Worker Class~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

