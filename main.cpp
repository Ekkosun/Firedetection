#include "ui.h"
#include <QApplication>
cv::dnn::Net* YoloNet = new cv::dnn::Net;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UI w;
    w.move(20,20);
    w.resize(1200,700);
    w.show();
    w.loadYolo();
    return a.exec();
}
