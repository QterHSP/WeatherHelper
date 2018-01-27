#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextCodec>
#include <QSharedMemory>
#include <QtSingleApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QSharedMemory shared("WeatherHelper");//随便填个名字就行
//    if (shared.attach())
//    {
//        return 0;
//    }
//    shared.create(1);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");//情况1
    QTextCodec::setCodecForLocale(codec);

    QFile file(":/qss/white.qss");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return -1;

    a.setStyleSheet(QString::fromLatin1(file.readAll()));

    MainWindow w;
    w.show();

    return a.exec();
}
