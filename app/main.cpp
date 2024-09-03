#include "mainwindow.h"
#include "qqml.h"

#include <QApplication>
#include <QQuickWidget>
#include "info/videoinfo.h"
#include "info/videoinfolistmodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qmlRegisterType<VideoInfo>("QMLVideoInfo", 1, 0, "VideoInfo");
    qmlRegisterType<VideoInfoListModel>("QMLVideoInfoListModel",1,0,"VideoInfoListModel");
    MainWindow w;
    w.show();
    return a.exec();
}
