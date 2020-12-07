#ifndef BEGINBUTTON_H
#define BEGINBUTTON_H

#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QtDebug>
#include <QPainter>
#include <QImage>
#include "detect.h"

class BeginButton : public QPushButton ,public Detect
{
    Q_OBJECT
public:
    explicit BeginButton(QWidget *parent = nullptr);
    void SetDetectParam(QAbstractButton* button);
    void BeginDetect();
    void DoDetect();
signals:
    void Quit();
private:
    int *sensitivity =NULL;
    int *threhold= NULL;
    int checkParam();
};

#endif // BEGINBUTTON_H
