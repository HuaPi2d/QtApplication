#ifndef SUBTEXTTRANSITIONWIDGET_H
#define SUBTEXTTRANSITIONWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSettings>
#include <QFileDialog>
#include "otherwidgets/singledownloadframe.h"

// 声音信息结构体，包含中文和英文名称
struct VoiceInfo {
    QString chineseName;
    QString englishName;
    QString server;
};

namespace Ui {
class SubTextTransitionWidget;
}

class SubTextTransitionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubTextTransitionWidget(QWidget *parent = nullptr);
    ~SubTextTransitionWidget();

public: signals:
    void widgetClosed();
    void sendStateInfo(QString state);
    void addDownloadTask(SingleDownloadFrame *singleDownloadFrame);

private slots:
    void on_closePushButton_clicked();

    void on_voiceClassComboBox_currentTextChanged(const QString &arg1);
    // 初始化声音类型和对应的中文、英文名称列表
    void initVoiceData();

    void on_requestPushButton_clicked();

    void sendRequest();

    void on_playPushButton_clicked();

    void saveSettings();
    void loadSettings();

    void on_downloadPushButton_clicked();

private:
    Ui::SubTextTransitionWidget *ui;

    void closeEvent(QCloseEvent *event);

    // 存储声音类型及其对应的名称列表
    QMap<QString, QList<VoiceInfo>> voiceMap;

    // 处理 comboBox2（中文名称）的切换，输出对应的英文名称
    void onComboBox2Changed(int index);

    void changePlayPushButtonStyle();
    void playMusic();

    QString currentName;
    QString currentServer;
    QString currentClass;
    QString currentVoice;
    QString currentText;
    QString currentUrl;
    int currentTime;

    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
};

#endif // SUBTEXTTRANSITIONWIDGET_H
