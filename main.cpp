#include "ui.h"

#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UI w;
    w.move(20,20);
    w.resize(1200,700);
    w.show();
    return a.exec();
}
