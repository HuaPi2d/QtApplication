#include "subvideowidget.h"
#include "ui_subvideowidget.h"

#include <QSettings>

SubVideoWidget::SubVideoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubVideoWidget)
{
    ui->setupUi(this);

    loadSettings();

    connect(ui->closePushButton, &QPushButton::clicked, this, [=](){
        emit widgetClosed();
    });
}

SubVideoWidget::~SubVideoWidget()
{
    saveSettings();
    delete ui;
}

void SubVideoWidget::saveSettings()
{
    /* 声明对象 */
    QSettings setting(qApp->applicationDirPath() + "/userSettings.ini", QSettings::IniFormat);

    /* 写入配置 */
    setting.beginGroup("video");
    setting.setValue("website", ui->website_comboBox->currentIndex()); // 解析视频网站
    setting.setValue("videoUrl", ui->video_url->text());               // 解析视频网址
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
    setting.endGroup();
}

void SubVideoWidget::closeEvent(QCloseEvent *event)
{
    emit widgetClosed();
}
