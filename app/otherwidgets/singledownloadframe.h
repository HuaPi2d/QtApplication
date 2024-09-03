#ifndef SINGLEDOWNLOADFRAME_H
#define SINGLEDOWNLOADFRAME_H

#include "qnamespace.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QUrl>
#include <QDebug>

namespace Ui {
class SingleDownloadFrame;
}

class SingleDownloadFrame : public QWidget
{
    Q_OBJECT
//
public:
    explicit SingleDownloadFrame(QString name,  QUrl url, QString filePath, QString type, QWidget *parent = nullptr);
    ~SingleDownloadFrame();
    void startDownload();
    void onReadyRead();
    void onDownloadFinished();

public: signals:
    void sendStateInfo(QString state);
    void downloadFinished();

private:
    Ui::SingleDownloadFrame *ui;
    QString name;
    QUrl url;
    QString filePath;
    QNetworkAccessManager *networkManager;
    QFile localFile;
};

#endif // SINGLEDOWNLOADFRAME_H
