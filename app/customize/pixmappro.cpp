#include <QEventLoop>

#include "pixmappro.h"
#include "universal/getnetwork.h"

PixmapPro::PixmapPro(QString filePath, QObject *parent)
    : QObject{parent}
{
    QEventLoop loop;

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(filePath);
    QNetworkReply *reply = manager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, [=](){
        if(reply->error() == QNetworkReply::NoError) // 成功返回数据
        {
            pixmap.loadFromData(reply->readAll());
        }
        else  // 失败返回错误信息
        {
            pixmap.load(":/pic/defaultPic/resources/pic/defaultPic/defaultMusicPic.svg");
        }
        reply->deleteLater();
        this->deleteLater();
    });

    loop.exec();
}

QPixmap PixmapPro::getPixmap() const
{
    return pixmap;
}
