#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QtDebug>
#include <QPainter>
#include <QImage>
#include <iostream>
#ifndef BEGINBUTTON_H
#define BEGINBUTTON_H

class BeginButton : public QPushButton
{
    Q_OBJECT
public:
    explicit BeginButton(QWidget *parent = nullptr);
    void SetDetectParam(QAbstractButton* button);
    void BeginDetect();
signals:
    void Begin(std::string* , std::string*,int* ,int*);
    void End();
private:
    int *sensitivity =NULL;
    int *threhold= NULL;
    std::string* detectMode = NULL;
    std::string* detectMethod = NULL;
    int checkParam();
};

#endif // BEGINBUTTON_H
