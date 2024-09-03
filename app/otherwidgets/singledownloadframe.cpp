#include "singledownloadframe.h"
#include "ui_singledownloadframe.h"

SingleDownloadFrame::SingleDownloadFrame(QString name,  QUrl url, QString filePath, QString type, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SingleDownloadFrame)
{
    ui->setupUi(this);
    this->url = url;
    this->name = name;
    this->filePath = filePath;
    ui->urlLabel->setScrollingText(name + "-源:" + url.toString());
    ui->urlLabel->setFixedWidth(200);
    if(type == "audio")
    {
        ui->iconLabel->setPixmap(QPixmap(":/icon/resources/icons/audio.svg"));
    }
}

SingleDownloadFrame::~SingleDownloadFrame()
{
    delete ui;
}

void SingleDownloadFrame::startDownload()
{
    // 初始化 QNetworkAccessManager
    networkManager = new QNetworkAccessManager(this);

    // 创建请求
    qDebug() << url;
    QNetworkRequest request(url);

    // 发送 GET 请求
    QNetworkReply *reply = networkManager->get(request);

    // 连接信号与槽
    connect(reply, &QNetworkReply::finished, [=](){
        if(reply->error() == QNetworkReply::NoError) // 成功返回数据
        {
            localFile.setFileName(filePath);
            if (!localFile.open(QIODevice::WriteOnly)) {
                sendStateInfo(filePath + "打开失败");
                return;
            }
            localFile.write(reply->readAll());
            localFile.close();
            emit downloadFinished();
        }
        else  // 失败返回错误信息
        {
            emit sendStateInfo(name + "下载失败");
        }
    });

    connect(reply, &QNetworkReply::downloadProgress, this, [=](qint64 bytesReceived, qint64 bytesTotal){
        emit sendStateInfo(QString::number(static_cast<int>(bytesReceived)));
        if (bytesTotal > 0) {
            ui->downoadProgressBar->setMaximum(static_cast<int>(bytesTotal));
            ui->downoadProgressBar->setValue(static_cast<int>(bytesReceived));
            ui->bytesLabel->setText(QString("%1MB/%2MB").arg(bytesReceived/1024.0/1024, 0, 'g', 3).arg(bytesTotal/1024.0/1024, 0, 'g', 3));
        }
    });
}
