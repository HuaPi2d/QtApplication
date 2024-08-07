#ifndef GETNETWORK_H
#define GETNETWORK_H

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>


class GetNetWork : public QObject
{
    Q_OBJECT
public:
    explicit GetNetWork(QObject *parent = nullptr);
    void getData(QUrl url);

signals:
    void sendData(QByteArray data);
    void sendError();
};

#endif // GETNETWORK_H
