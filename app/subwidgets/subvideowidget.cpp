#include "subvideowidget.h"
#include "ui_subvideowidget.h"
#include "universal/requestpro.h"
#include "universal/stringConvert.h"

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
        currentPage = 1;

        /* 加载搜索框，并寻找根节点 */
        ui->searchResultQuickWidget->setSource(QUrl(QStringLiteral("qrc:/QML/resources/QML/FlickableSingleVideo.qml")));
        QQuickItem *rootObject = ui->searchResultQuickWidget->rootObject();
        videoInfoListModel = rootObject->findChild<VideoInfoListModel *>("videoInfoListModel");

        connect(videoInfoListModel, &VideoInfoListModel::scrollToBottom, this, &SubVideoWidget::scrollToBottom);

        videoInfoListModel->state = 1;

        /* 清除上次搜索结果 */
        videoInfoListModel->clearModel();

        // 记住当前搜索信息，方便后续动态加载相同内容
        currentWeb = ui->searchEngineComboBox->currentText();
        currentSearchContent = ui->videoSearchContentLineEdit->text();

        if(currentWeb == "bilibili")
        {
            searchByBiliBili();
        }
        else if(currentWeb == "风花雪月")
        {
            searchByMoGu();
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
    params["page"] = QString::number(currentPage);
    params["keyword"] = currentSearchContent;

    QUrl url("https://api.bilibili.com/x/web-interface/wbi/search/type");
    QString requestType = "GET";

    QObject::connect(request, &RequestPro::requestCompleted, [=](const QJsonObject &response) {
        // 搜索成功，页数增加
        currentPage += 1;

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
            videoInfo.duration = convertTimeFormat(videoInfoJsonObject["duration"].toString());
            // videoInfo.duration = videoInfoJsonObject["duration"].toString();
            if(videoInfoJsonObject["play"].toInt() < 10000){
                videoInfo.play = QString::number(videoInfoJsonObject["play"].toInt());
            }
            else if(videoInfoJsonObject["play"].toInt() < 10000000){
                videoInfo.play = QString("%1万").arg(videoInfoJsonObject["play"].toInt()/10000.0, 0, 'g', 3);
            }
            else{
                videoInfo.play = QString("%1万").arg(videoInfoJsonObject["play"].toInt()/10000, 0, 10);
            }
            videoInfo.pubdate = QDateTime::fromSecsSinceEpoch(videoInfoJsonObject["pubdate"].toInt()).toString("yyyy-MM-dd");
            videoInfo.title = videoInfoJsonObject["title"].toString();

            videoInfoListModel->addVideoInfo(videoInfo);
        }

        emit videoInfoListModel->finishReceiveData();

        videoInfoListModel->state = 0;
    });

    QObject::connect(request, &RequestPro::requestError, [this](const QString &error) {
        emit sendStateInfo("搜索失败，可尝试换源");
        videoInfoListModel->state = 0;
    });

    request->sendRequest(headers, params, url, requestType);
}

void SubVideoWidget::searchByMoGu()
{
    RequestPro *request = new RequestPro();

    QJsonObject headers;
    headers["User-Agent"] = "okhttp/3.12.0";
    headers["Connection"] = "Keep-Alive";
    headers["Accept"] = "application/json";

    QJsonObject params;
    params["limit"] = "20";
    params["page"] = QString::number(currentPage);
    params["wd"] = currentSearchContent;

    QUrl url("https://api.koudailc.net/api/vod/clever");
    QString requestType = "GET";

    QObject::connect(request, &RequestPro::requestCompleted, [=](const QJsonObject &response) {
        // 搜索成功，页数增加
        currentPage += 1;

        QJsonArray resultList = response["data"]["list"].toArray();

        VideoInfo videoInfo;

        for(QJsonValueRef videoInfoRaw: resultList)
        {
            QJsonObject videoInfoJsonObject = videoInfoRaw.toObject();

            videoInfo.arcurl = videoInfoJsonObject["id"].toString();
            videoInfo.pic = videoInfoJsonObject["cover"].toString();
            videoInfo.author = videoInfoJsonObject["users"].toArray()[0].toObject()["name"].toString();
            videoInfo.duration = convertTimeFormat(videoInfoJsonObject["duration"].toString());
            if(videoInfoJsonObject["views"].toInt() < 10000){
                videoInfo.play = QString::number(videoInfoJsonObject["views"].toInt());
            }
            else if(videoInfoJsonObject["views"].toInt() < 10000000){
                videoInfo.play = QString("%1万").arg(videoInfoJsonObject["views"].toInt()/10000.0, 0, 'g', 3);
            }
            else{
                videoInfo.play = QString("%1万").arg(videoInfoJsonObject["views"].toInt()/10000, 0, 10);
            }
            videoInfo.pubdate = "";
            videoInfo.title = videoInfoJsonObject["title"].toString();

            videoInfoListModel->addVideoInfo(videoInfo);
        }

        emit videoInfoListModel->finishReceiveData();

        videoInfoListModel->state = 0;
    });

    QObject::connect(request, &RequestPro::requestError, [this](const QString &error) {
        emit sendStateInfo("搜索失败，可尝试换源");
        videoInfoListModel->state = 0;
    });

    request->sendRequest(headers, params, url, requestType);
}

void SubVideoWidget::updateQuickWidgetColor(QColor color)
{
    ui->searchResultQuickWidget->setClearColor(color);
    ui->searchResultQuickWidget->rootContext()->setContextProperty("widgetColor", color);
}

void SubVideoWidget::scrollToBottom()
{
    videoInfoListModel->state = 1;

    if(currentWeb == "bilibili")
    {
        searchByBiliBili();
    }
    else if(currentWeb == "风花雪月")
    {
        searchByMoGu();
    }
}

