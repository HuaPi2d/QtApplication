#include "subtexttransitionwidget.h"
#include "ui_subtexttransitionwidget.h"
#include "universal/requestpro.h"

SubTextTransitionWidget::SubTextTransitionWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubTextTransitionWidget)
{
    ui->setupUi(this);
    /* 选择音色 */
    initVoiceData();

    /* 创建音频播放对象 */
    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);

    /* 总时长更新 */
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [=](int duration){
        ui->toatalTimeLabel->setText(QString("%1:%2").arg(duration/1000/60, 2, 10, QChar('0')).arg(duration/1000%60, 2, 10, QChar('0')));
        ui->currentTimeSlider->setRange(0, duration);
    });
    /* 进度条更新 */
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [=](int position){
        ui->toatalTimeLabel->setText(QString("%1:%2").arg(mediaPlayer->duration()/1000/60, 2, 10, QChar('0')).arg(mediaPlayer->duration()/1000%60, 2, 10, QChar('0')));
        ui->currentTimeSlider->setRange(0, mediaPlayer->duration());
        currentTime = position;
        ui->currentTimeLabel->setText(QString("%1:%2").arg(position/1000/60, 2, 10, QChar('0')).arg(position/1000%60, 2, 10, QChar('0')));
        ui->currentTimeSlider->setValue(position);
        /* 歌曲播放结束后切换下一首 */
        if(position == ui->currentTimeSlider->maximum() && position != 0)
        {
            currentTime = 0;
            mediaPlayer->setPosition(0);
            ui->currentTimeSlider->setSliderPosition(0);

            mediaPlayer->setSource(currentUrl);
            ui->playPushButton->setIcon(QIcon(":/icon/resources/icons/musicPlay.svg"));
        }
    });
    /* 进度条控制播放进度 */
    connect(ui->currentTimeSlider, &QSlider::sliderMoved, mediaPlayer, &QMediaPlayer::setPosition);

    loadSettings();
}

SubTextTransitionWidget::~SubTextTransitionWidget()
{
    saveSettings();
    delete ui;
}

void SubTextTransitionWidget::closeEvent(QCloseEvent *event)
{
    emit widgetClosed();
}

void SubTextTransitionWidget::on_closePushButton_clicked()
{
    this->close();
}

/* 保存相关配置 */
void SubTextTransitionWidget::saveSettings()
{
    /* 声明对象 */
    QSettings setting(qApp->applicationDirPath() + "/userSettings.ini", QSettings::IniFormat);

    /* 写入配置 */
    setting.beginGroup("testTransition");
    setting.setValue("voiceClass", ui->voiceClassComboBox->currentText());
    setting.setValue("text", ui->plainTextEdit->toPlainText());
    setting.setValue("currentUrl", currentUrl);
    setting.setValue("currentVoice", ui->voiceComboBox->currentText());
    setting.endGroup();
}

/* 加载相关配置 */
void SubTextTransitionWidget::loadSettings()
{
    /* 声明对象 */
    QSettings setting(qApp->applicationDirPath() + "/userSettings.ini", QSettings::IniFormat);

    /* 写入配置 */
    setting.beginGroup("testTransition");
    if(setting.value("voiceClass").toString() != "")
    {
        ui->voiceClassComboBox->setCurrentText(setting.value("voiceClass").toString());
        emit on_voiceClassComboBox_currentTextChanged(setting.value("voiceClass").toString());
        ui->voiceComboBox->setCurrentText(setting.value("currentVoice").toString());
    }
    else
    {
        emit on_voiceClassComboBox_currentTextChanged("通用");
    }
    ui->plainTextEdit->setPlainText(setting.value("text").toString());
    if(setting.value("currentUrl").toString() != "")
    {
        currentUrl = setting.value("currentUrl").toString();
        mediaPlayer->setSource(currentUrl);
    }
    setting.endGroup();
}

void SubTextTransitionWidget::on_voiceClassComboBox_currentTextChanged(const QString &arg1)
{
    ui->voiceComboBox->clear();
    if (voiceMap.contains(arg1)) {
        QStringList chineseNames;
        for (const auto &voice : voiceMap[arg1]) {
            chineseNames << voice.chineseName;
        }
        ui->voiceComboBox->addItems(chineseNames);
    }
}

void SubTextTransitionWidget::initVoiceData()
{
    voiceMap["通用"] = {{"晓冉(资讯女声)", "zhiqian", "aliLongTextSpeech"}, {"艾琪(温柔女声)", "aiqi", "aliTextSpeech"}, {"爱婷(电台女声)", "aiting", "aliLongTextSpeech"}, {"爱媛(知心姐姐)", "aiyuan", "aliLongTextSpeech"}, {"艾达(标准男声)", "aida", "aliTextSpeech"}, {"思琪(温柔女声)", "siqi", "aliTextSpeech"}, {"晓宁(温暖男声)", "kenny", "aliTextSpeech"}, {"志祥(磁性男声)", "zhixiang", "aliLongTextSpeech"}, {"志初(舌尖男声)", "zhichu", "aliLongTextSpeech"}, {"思佳(标准女声)", "sijia", "aliTextSpeech"}, {"桂安(标准女声)", "guijie", "aliTextSpeech"}};
    voiceMap["客服"] = {{"艾悦(温柔女声)", "aiyue", "aliTextSpeech"}, {"志佳(甜美女声)", "zhijia", "aliLongTextSpeech"}, {"爱婷(电台女声)", "aiting", "aliLongTextSpeech"}, {"肆月(温柔女声)", "siyue", "aliTextSpeech"}, {"晓美(甜美女声)", "xiaomei", "aliTextSpeech"}, {"爱玉(自然女声)", "aiyu", "aliTextSpeech"}, {"爱帆(情感女声)", "aifan", "aliLongTextSpeech"}};
    voiceMap["阅读"] = {{"乔薇(亲和女声)", "qiaowei", "aliTextSpeech"}, {"艾祥(磁性男声)", "aixiang", "aliLongTextSpeech"}, {"爱媛(知心姐姐)", "aiyuan", "aliLongTextSpeech"}, {"艾成(标准男声)", "aicheng", "aliTextSpeech"}, {"志茹(新闻女声)", "zhiru", "aliLongTextSpeech"}, {"艾娜(浙普女声)", "aina", "aliTextSpeech"}, {"爱雅(严厉女声)", "aiya", "aliTextSpeech"}, {"艾霞(亲和女声)", "aixia", "aliLongTextSpeech"}};
    voiceMap["新闻"] = {{"艾晓(资讯女声)", "aixiao", "aliLongTextSpeech"}, {"艾德(新闻男声)", "aide", "aliLongTextSpeech"}, {"史提(沉稳男声)", "stanley", "aliTextSpeech"}, {"依娜(浙普女声)", "yina", "aliTextSpeech"}, {"爱茹(新闻女声)", "airu", "aliLongTextSpeech"}};
    voiceMap["儿童"] = {{"小豆(治愈童声)", "jielidou", "aliTextSpeech"}, {"洁莉(萝莉女声)", "xiaobei", "aliTextSpeech"}};
    voiceMap["粤语"] = {{"桃子(粤语女声)", "taozi", "aliTextSpeech"}, {"珊珊(粤语女声)", "shanshan", "aliTextSpeech"}};
    voiceMap["外语"] = {{"肉丝(美式女声)", "ava", "aliTextSpeech"}, {"安迪(美音男声)", "andy", "aliTextSpeech"}, {"露娜(英音女声)", "luna", "aliTextSpeech"}, {"威廉姆(英音男声)", "william", "aliTextSpeech"}};
}

void SubTextTransitionWidget::on_requestPushButton_clicked()
{
    currentName = ui->voiceComboBox->currentText();
    currentClass = ui->voiceClassComboBox->currentText();
    if (!currentName.isEmpty() && voiceMap.contains(currentClass)) {
        const auto &voiceList = voiceMap[currentClass];
        for (const auto &voice : voiceList) {
            if (voice.chineseName == currentName) {
                currentVoice = voice.englishName;
                currentServer = voice.server;
                currentText = ui->plainTextEdit->toPlainText();
                break;
            }
        }
        sendRequest();
    }
}

void SubTextTransitionWidget::sendRequest()
{
    RequestPro *request = new RequestPro();

    QJsonObject headers;
    headers["cookie"] = "track_id=0f0dffa12c725f6fdfb0bc8a038d8bf6be6bde73a3efc0c72013b41a38ca04c6a%3A2%3A%7Bi%3A0%3Bs%3A8%3A%22track_id%22%3Bi%3A1%3Bs%3A55%3A%22d6b7c4607f0f7271eacfe0992042266166e6f493cb9871.58018126%22%3B%7D; user_source_permanent=bab96838d26ee98a984610b6519e4f794c2212e1808d561e1f14462485cac180a%3A2%3A%7Bi%3A0%3Bs%3A21%3A%22user_source_permanent%22%3Bi%3A1%3Bs%3A5%3A%2223678%22%3B%7D; __root_domain_v=.houzi8.com; _qddaz=QD.293026411924530; _c_WBKFRo=ndC7SnQ1n9kCevZ1zKyXdqheyR5vjXn61m5l3hJu; YPSSESSION=80nvo8jg2k5tupo5tosjs1rs81; _identity=994576b9f14f338a0295b8cf1ed8667882b22a09cd2da6e4b86eb6686b1894f5a%3A2%3A%7Bi%3A0%3Bs%3A9%3A%22_identity%22%3Bi%3A1%3Bs%3A52%3A%22%5B1591104%2C%22tgARHyOGqqRgMRyvnOlNCY7Gx5nch0_j%22%2C2592000%5D%22%3B%7D; hunl_453eaf268fdd2d940a5999f2912f0eef=925952a8e1638dd5d327dc5e27fa57ab13e32a5aaa8fb173fffb16bfd4d2579aa%3A2%3A%7Bi%3A0%3Bs%3A37%3A%22hunl_453eaf268fdd2d940a5999f2912f0eef%22%3Bi%3A1%3Bs%3A104%3A%22YToyOntzOjU6InZhbHVlIjtzOjEzOiI2NmU2ZjRjODU0ODI0IjtzOjQ6InRpbWUiO3M6MTk6IjIwMjQtMDktMTUgMjI6NTI6NTYiO30%3D%22%3B%7D; user_source=938d6df6cf78cfa92ec9f3560e481be78db15c03543dab8010399687e82068b9a%3A2%3A%7Bi%3A0%3Bs%3A11%3A%22user_source%22%3Bi%3A1%3Bi%3A1%3B%7D; todayViewMark=869ad589fae6c88a6771e92dcd4c6244ecb5a7f270c1b784ad38801ba6646923a%3A2%3A%7Bi%3A0%3Bs%3A13%3A%22todayViewMark%22%3Bi%3A1%3Bi%3A1%3B%7D; Hm_lvt_bb1a215c22b753f9361786dbc9433727=1726411924,1726576598,1726647579; Hm_lpvt_bb1a215c22b753f9361786dbc9433727=1726647579; HMACCOUNT=AC798A82DFB188B6";
    headers["accept-language"] = "zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6";
    headers["accept"] = "application/json, text/plain, */*";
    headers["origin"] = "https://houzi8.com";
    headers["priority"] = "u=1, i";
    headers["referer"] = "https://houzi8.com/AIVoice/editor";
    headers["user-agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Safari/537.36 Edg/128.0.0.0";

    QJsonObject params;
    params["text"] = currentText;
    params["vcn"] = currentVoice;
    params["fun"] = currentServer;
    params["speed"] = "0";
    params["country"] = "zh";

    QUrl url("https://api.houzi8.com/weapi/tts/index-v2");
    QString requestType = "POST";

    QObject::connect(request, &RequestPro::requestCompleted, [=](const QJsonObject &response) {
        qDebug() << response;
        ui->requestPushButton->setText("生成语音");
        mediaPlayer->setSource(response["data"]["audio_url"].toString());
        currentUrl = response["data"]["audio_url"].toString();
        playMusic();
    });

    QObject::connect(request, &RequestPro::requestError, [this](const QString &error) {
        ui->requestPushButton->setText("生成语音");
        emit sendStateInfo("生成失败，可尝试切换音源");
    });

    request->sendRequest(headers, params, url, requestType);
    ui->requestPushButton->setText("生成中...");
}


void SubTextTransitionWidget::on_playPushButton_clicked()
{
    switch (mediaPlayer->playbackState()) {
    case QMediaPlayer::PlaybackState::PlayingState:
        {
            mediaPlayer->pause();
            changePlayPushButtonStyle();
            break;
        }
    default:
        {
            int time = currentTime;
            playMusic();
            mediaPlayer->setPosition(time);
            changePlayPushButtonStyle();
            break;
        }
    }
}

/* 改变按钮提示和样式等 */
void SubTextTransitionWidget::changePlayPushButtonStyle()
{
    switch (mediaPlayer->playbackState()) {
    case QMediaPlayer::PlaybackState::PlayingState:
    {
        ui->playPushButton->setIcon(QIcon(":/icon/resources/icons/pause.svg"));
        ui->playPushButton->setToolTip("暂停");
        break;
    }
    case QMediaPlayer::PlaybackState::StoppedState:
    {
        ui->playPushButton->setIcon(QIcon(":/icon/resources/icons/pause.svg"));
        ui->playPushButton->setToolTip("暂停");
        break;
    }
    default:
    {
        ui->playPushButton->setIcon(QIcon(":/icon/resources/icons/musicPlay.svg"));
        ui->playPushButton->setToolTip("播放");
        break;
    }
    }
}

/* 播放当前歌曲 */
void SubTextTransitionWidget::playMusic()
{
    if(mediaPlayer->playbackState() == QMediaPlayer::PlaybackState::PlayingState)
    {
        mediaPlayer->stop();
    }
    mediaPlayer->play();
    changePlayPushButtonStyle();
}

void SubTextTransitionWidget::on_downloadPushButton_clicked()
{
    // 打开文件保存对话框，获取用户选择的保存路径
    QString savePath = QFileDialog::getSaveFileName(this, "Save Audio", "textTransitionResult", "Audio Files (*.mp3 *.wav)");

    // 创建下载任务
    SingleDownloadFrame *singleDownloadFrame = new SingleDownloadFrame(ui->voiceClassComboBox->currentText() + ui->voiceComboBox->currentText(), currentUrl, savePath, "audio");

    /* 开始下载 */
    emit addDownloadTask(singleDownloadFrame);
}

