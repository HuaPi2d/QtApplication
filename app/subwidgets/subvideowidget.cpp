#include "subvideowidget.h"
#include "ui_subvideowidget.h"
#include "universal/requestpro.h"
#include "info/videoinfolistmodel.h"

#include <QSettings>

SubVideoWidget::SubVideoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubVideoWidget)
{
    ui->setupUi(this);

    loadSettings();

    /* 视频搜索功能实现 */
    ui->videoSearchContentLineEdit->addAction(ui->searchVideoAction, QLineEdit::LeadingPosition);

    /* 搜索音乐按钮按下或输入框按下Enter键 */
    connect(ui->searchVideoPushButton, &QPushButton::clicked, this, &SubVideoWidget::searchVideo);
    connect(ui->videoSearchContentLineEdit, &QLineEdit::returnPressed, ui->searchVideoPushButton, &QPushButton::click);

    connect(ui->closePushButton, &QPushButton::clicked, this, [=](){
        emit widgetClosed();
    });
}

SubVideoWidget::~SubVideoWidget()
{
    saveSettings();
    delete ui;
}

void SubVideoWidget::closeEvent(QCloseEvent *event)
{
    emit widgetClosed();
}

void SubVideoWidget::saveSettings()
{
    /* 声明对象 */
    QSettings setting(qApp->applicationDirPath() + "/userSettings.ini", QSettings::IniFormat);

    /* 写入配置 */
    setting.beginGroup("video");
    setting.setValue("website", ui->website_comboBox->currentIndex()); // 解析视频网站
    setting.setValue("videoUrl", ui->video_url->text());               // 解析视频网址
    setting.setValue("videoSearchEngine", ui->searchEngineComboBox->currentIndex());
    setting.endGroup();
}

void SubVideoWidget::loadSettings()
{
    /* 声明对象 */
    QSettings setting(qApp->applicationDirPath() + "/userSettings.ini", QSettings::IniFormat);

    /* 读取配置 */
    setting.beginGroup("video");
    ui->website_comboBox->setCurrentIndex(setting.value("website").toInt());
    ui->video_url->setText(setting.value("videoUrl").toString());
    ui->searchEngineComboBox->setCurrentIndex(setting.value("videoSearchEngine").toInt());
    setting.endGroup();
}

/* 搜索视频 */
void SubVideoWidget::searchVideo()
{
    if(ui->videoSearchContentLineEdit->text() != "")
    {
        if(ui->searchEngineComboBox->currentText() == "bilibili")
        {
            searchByBiliBili();
        }
    }

}

/* bilibili搜索引擎 */
void SubVideoWidget::searchByBiliBili()
{
    RequestPro *request = new RequestPro();

    QJsonObject headers;
    headers["accept"] = "application/json, text/plain, */*";
    headers["accept-language"] = "zh-CN,zh;q=0.9";
    headers["cookie"] = "buvid3=B08EB628-B1FC-4655-9F80-10229B78DD0918180infoc";
    headers["origin"] = "https://search.bilibili.com";
    headers["referer"] = "https://search.bilibili.com/all?vt=10985409&keyword=ffmpeg&from_source=webtop_search&spm_id_from=333.1007&search_source=5&page=2&o=24";
    headers["sec-ch-ua"] = "\"Not)A;Brand\";v=\"99\", \"Microsoft Edge\";v=\"127\", \"Chromium\";v=\"127\"";
    headers["sec-ch-ua-mobile"] = "?0";
    headers["sec-ch-ua-platform"] = "\"Windows\"";
    headers["user-agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/127.0.0.0 Safari/537.36 Edg/127.0.0.0";

    QJsonObject params;
    params["search_type"] = "video";
    params["page"] = "1";
    params["keyword"] = ui->videoSearchContentLineEdit->text();

    QUrl url("https://api.bilibili.com/x/web-interface/wbi/search/type");
    QString requestType = "GET";

    QObject::connect(request, &RequestPro::requestCompleted, [=](const QJsonObject &response) {
        /* 加载搜索框，并寻找根节点 */
        ui->searchResultQuickWidget->setSource(QUrl(QStringLiteral("qrc:/QML/resources/QML/SingleVideo.qml")));
        QQuickItem *rootObject = ui->searchResultQuickWidget->rootObject();
        VideoInfoListModel *videoInfoListModel = rootObject->findChild<VideoInfoListModel *>("videoInfoListModel");

        /* 清除上次搜索结果 */
        videoInfoListModel->clearModel();

        QJsonArray resultList = response["data"]["result"].toArray();

        VideoInfo videoInfo;

        for(QJsonValueRef videoInfoRaw: resultList)
        {
            QJsonObject videoInfoJsonObject = videoInfoRaw.toObject();

            videoInfo.arcurl = videoInfoJsonObject["arcurl"].toString();
            if(videoInfoJsonObject["pic"].toString().contains("https:") != true){
                videoInfo.pic = "https:" + videoInfoJsonObject["pic"].toString();
            }
            else{
                videoInfo.pic = videoInfoJsonObject["pic"].toString();
            }
            videoInfo.author = videoInfoJsonObject["author"].toString();
            videoInfo.duration = videoInfoJsonObject["duration"].toString();
            videoInfo.play = videoInfoJsonObject["play"].toInt();
            videoInfo.pubdate = videoInfoJsonObject["pubdate"].toInt();
            videoInfo.title = videoInfoJsonObject["title"].toString();

            videoInfoListModel->addVideoInfo(videoInfo);
        }

        emit videoInfoListModel->finishReceiveData();
    });

    QObject::connect(request, &RequestPro::requestError, [this](const QString &error) {
        emit sendStateInfo("搜索失败，可尝试换源");
    });

    request->sendRequest(headers, params, url, requestType);
}

void SubVideoWidget::updateQuickWidgetColor(QColor color)
{
    ui->searchResultQuickWidget->setClearColor(color);
    ui->searchResultQuickWidget->rootContext()->setContextProperty("widgetColor", color);
}

