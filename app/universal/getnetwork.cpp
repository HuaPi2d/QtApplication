#include "getnetwork.h"

GetNetWork::GetNetWork(QObject *parent)
    : QObject{parent}
{}

void GetNetWork::getData(QUrl url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, [=](){
        if(reply->error() == QNetworkReply::NoError) // 成功返回数据
        {
            emit sendData(reply->readAll());
        }
        else  // 失败返回错误信息
        {
            emit sendError();
        }
        reply->deleteLater();
        this->deleteLater();
    });
}
