#include "submusicwidget.h"
#include "otherwidgets/deletemusicdialog.h"
#include "ui_deletemusicdialog.h"
#include "ui_submusicwidget.h"
#include "universal/getnetwork.h"
#include "universal/rematcher.h"
#include "universal/htmlParserPro.h"
#include <algorithm>  // 包含std::shuffle
#include <random>     // 包含std::random_device 和 std::mt19937


SubMusicWidget::SubMusicWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubMusicWidget)
{
    ui->setupUi(this);
    /* 创建音频播放对象 */
    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);

    /* 为输入框添加图标 */
    ui->musicSearchLineEdit->addAction(ui->music_search_action, QLineEdit::LeadingPosition);

    /* 本地音乐加载 */
    localMusicList = new PlaylistManager("pornMovieList.json", this);
    localMusicList->load();
    netMusicList = new PlaylistManager("hotGirlPic.json", this);
    netMusicList->load();

    /* 设置自定义列表样式 */
    model->setParent(this);
    ui->musicSearchResultListView->setModel(model);
    delegate->setParent(this);
    ui->musicSearchResultListView->setItemDelegate(delegate);
    localMusicModel->setParent(this);
    ui->localSongListView->setModel(localMusicModel);
    localMusicDelegate->setParent(this);
    ui->localSongListView->setItemDelegate(localMusicDelegate);

    /* 设定播放参数 */
    playMode = "round";
    ui->playModePushButton->setIcon(QIcon(":/icon/resources/icons/IconamoonPlaylistRepeatList.svg"));
    ui->playModePushButton->setIconSize(QSize(24, 24));

    /* 搜索音乐按钮按下或输入框按下Enter键 */
    connect(ui->searchMusicPushButton, &QPushButton::clicked, this, &SubMusicWidget::searchMusic);
    connect(ui->musicSearchLineEdit, &QLineEdit::returnPressed, ui->searchMusicPushButton, &QPushButton::click);
    /* 播放模式切换 */
    connect(ui->playModePushButton, &QPushButton::clicked, this, &SubMusicWidget::playModeChange);
    /* 界面关闭 */
    connect(ui->closePushButton, &QPushButton::clicked, this, [=](){
        emit widgetClosed();
    });
    /* 切换搜索和本地界面 */
    connect(ui->localMusicPushButton, &QPushButton::clicked, this, [=](){
        ui->searchWidget->hide();
        ui->downloadPushButton->hide();
        ui->localSongWidget->show();
        ui->openLocalDirPushButton->show();
        updateLocalMusicList();
        ui->localSongListView->setCurrentIndex(localMusicModel->findMusicByURL(currentSong.filePath));
    });
    connect(ui->findMuiscPushButton, &QPushButton::clicked, this, [=](){
        ui->searchWidget->show();
        ui->downloadPushButton->show();
        ui->localSongWidget->hide();
        ui->openLocalDirPushButton->hide();
    });
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
            playNextMusic();
    });
    /* 进度条控制播放进度 */
    connect(ui->currentTimeSlider, &QSlider::sliderMoved, mediaPlayer, &QMediaPlayer::setPosition);
    /* 切换音乐防呆 */
    connect(this, &SubMusicWidget::changeMusicProcessBegin, this, [=](){
        ui->previousMusicPushButton->setDisabled(true);
        ui->nextMusicPushButton->setDisabled(true);
    });
    connect(this, &SubMusicWidget::changeMusicProcessFinish, this, [=](){
        ui->previousMusicPushButton->setDisabled(false);
        ui->nextMusicPushButton->setDisabled(false);
    });

    /* 本地音乐更改 */
    connect(ui->localSongListView, &LocalMusicListView::deleteMusic, this, [=](QModelIndex index){
        MusicItem chosenItem;
        chosenItem.title = localMusicModel->itemFromIndex(index)->data(Qt::UserRole + 1).toString();
        chosenItem.artist = localMusicModel->itemFromIndex(index)->data(Qt::UserRole + 2).toString();
        chosenItem.filePath = localMusicModel->itemFromIndex(index)->data(Qt::UserRole + 3).toString();
        DeleteMusicDialog *deleteMusicDialog = new DeleteMusicDialog(this);
        /* 强制对话 */
        deleteMusicDialog->setModal(true);
        deleteMusicDialog->ui->remindTextLabel->setText("是否删除" + chosenItem.title + "-" + chosenItem.artist + "?");
        if (deleteMusicDialog->exec() == QDialog::Accepted)
        {
            if(deleteMusicDialog->ui->checkBox->isChecked())
            {
                if(currentSong.filePath != chosenItem.filePath)
                {
                    if(QFile::remove(chosenItem.filePath))
                    {
                        emit sendStateInfo("歌曲已成功删除");
                    }
                    else
                    {
                        emit sendStateInfo("文件删除失败");
                        return;
                    }
                }
                else
                {
                    emit sendStateInfo("文件已打开，删除失败");
                }
            }
            /* 从列表中删除 */
            localMusicList->removeMusicItem(chosenItem.filePath);
            updateLocalMusicList();
            createPlayList();
        }
    });
    connect(ui->localSongListView, &LocalMusicListView::modifyMusic, this, [=](QModelIndex index){
        MusicItem chosenItem;
        chosenItem.title = localMusicModel->itemFromIndex(index)->data(Qt::UserRole + 1).toString();
        chosenItem.artist = localMusicModel->itemFromIndex(index)->data(Qt::UserRole + 2).toString();

    });

    /* 加载配置 */
    ui->musicPicLabel->setPixmap(mainPixmap);
    loadSettings();
}

SubMusicWidget::~SubMusicWidget()
{
    localMusicList->save();
    netMusicList->save();
    saveSettings();
    delete ui;
}

void SubMusicWidget::closeEvent(QCloseEvent *event)
{
    emit widgetClosed();
}

/* 点击搜索 */
void SubMusicWidget::searchMusic()
{
    if(ui->musicSourceComboBox->currentIndex() == 0){
        engineGeQuBao();
    }
}

/* 调用歌曲宝搜索引擎 */
void SubMusicWidget::engineGeQuBao()
{
    /* 声明对象 */
    GetNetWork *getSong = new GetNetWork(this);

    /* 请求网址 */
    QString urlPrifix = "https://www.gequbao.com/s/";
    QString urlSearchContent = ui->musicSearchLineEdit->text();

    /* 请求成功更新搜索结果 */
    connect(getSong, &GetNetWork::sendData, this, [=](QByteArray htmlContent){
        QString htmlString = QString::fromUtf8(htmlContent);

        model->clear();
        netMusicList->clear();

        xmlDocPtr doc = get_html_doc(htmlString.toStdString());

        // QRegularExpressionMatchIterator i = findFitStruct(R"(<a href=\"(.*?)\" target=\"_blank\" class=\"music-link\">\n                                <span class=\"text-primary font-weight-bolder music-title\">\n                                    (.*?)\n                                </span>\n                                <i class=\"text-muted\">-</i>\n                                <small class=\"text-jade font-weight-bolder\">\n                                    (.*?)\n)", htmlContent);
        // QPixmap cover(":/pic/defaultPic/resources/pic/defaultPic/defaultMusicPic.svg");
        // while (i.hasNext()) {
        //     QRegularExpressionMatch match = i.next();
        //     QString url = "https://www.gequbao.com" + match.captured(1);
        //     QString songTitle = match.captured(2).trimmed();
        //     QString artist = match.captured(3).trimmed();
        //     model->addMusic(MusicInfo(songTitle, artist, "", cover, url));
        //     netMusicList->addMusicItem(songTitle, artist, "", ":/pic/defaultPic/resources/pic/defaultPic/defaultMusicPic.svg", "geQuBao", url);
        // }
    });

    /* 请求失败状态栏显示 */
    connect(getSong, &GetNetWork::sendError, this, [=](){
        emit sendStateInfo("歌曲宝搜索超时，可尝试换源");
    });

    /* 发送请求 */
    getSong->getData(QUrl(urlPrifix + urlSearchContent));
}

/* 播放模式改变 */
void SubMusicWidget::playModeChange()
{
    if(playMode == "round")
    {
        playMode = "random";
        setPlayMode();
    }
    else if(playMode == "random")
    {
        playMode = "single";
        setPlayMode();
    }
    else
    {
        playMode = "round";
        setPlayMode();
    }
}

/* 设定播放模式 */
void SubMusicWidget::setPlayMode()
{
    if(playMode == "round")
    {
        ui->playModePushButton->setIcon(QIcon(":/icon/resources/icons/IconamoonPlaylistRepeatList.svg"));
        ui->playModePushButton->setIconSize(QSize(24, 24));
    }
    else if(playMode == "random")
    {
        ui->playModePushButton->setIcon(QIcon(":/icon/resources/icons/IconamoonPlaylistShuffle.svg"));
        ui->playModePushButton->setIconSize(QSize(24, 24));
    }
    else if(playMode == "single")
    {
        ui->playModePushButton->setIcon(QIcon(":/icon/resources/icons/IconamoonPlaylistRepeatSong.svg"));
        ui->playModePushButton->setIconSize(QSize(24, 24));
    }
    else
    {
        playMode = "round";
        ui->playModePushButton->setIcon(QIcon(":/icon/resources/icons/IconamoonPlaylistRepeatList.svg"));
        ui->playModePushButton->setIconSize(QSize(24, 24));
    }
    createPlayList();
}

/* 保存相关配置 */
void SubMusicWidget::saveSettings()
{
    /* 声明对象 */
    QSettings setting(qApp->applicationDirPath() + "/userSettings.ini", QSettings::IniFormat);

    /* 写入配置 */
    setting.beginGroup("music");
    setting.setValue("playMode", playMode);
    setting.setValue("currentSongName", currentSong.title);
    setting.setValue("currentSongArtist", currentSong.artist);
    setting.setValue("currentSourcePath", currentSong.filePath);
    setting.setValue("currentSongCoverPath", currentSong.coverPath);
    setting.setValue("currentSongLyricsPath", currentSong.lyricsPath);
    setting.setValue("currentTime", ui->currentTimeSlider->value());
    setting.setValue("currentSongTotalTime", ui->currentTimeSlider->maximum());
    setting.endGroup();
}

/* 加载相关配置 */
void SubMusicWidget::loadSettings()
{
    /* 声明对象 */
    QSettings setting(qApp->applicationDirPath() + "/userSettings.ini", QSettings::IniFormat);

    /* 写入配置 */
    setting.beginGroup("music");
    if(setting.value("playMode").toString() == "")
        playMode = "round";
    else
        playMode = setting.value("playMode").toString();
    setPlayMode();
    currentSong.title = setting.value("currentSongName").toString();
    currentSong.artist = setting.value("currentSongArtist").toString();
    currentSong.filePath = setting.value("currentSourcePath").toString();
    if (setting.value("currentSongCoverPath").toString() == "")
        currentSong.coverPath = ":/pic/defaultPic/resources/pic/defaultPic/defaultMusicPic.svg";
    else
        currentSong.coverPath = setting.value("currentSongCoverPath").toString();
    setMainPixmap();
    currentSong.lyricsPath = setting.value("currentSongLyricsPath").toString();
    mediaPlayer->setSource(currentSong.filePath);
    ui->musicNameLabel->setText(currentSong.title);
    ui->ArtistLabel->setText(currentSong.artist);
    ui->currentTimeSlider->setRange(0, setting.value("currentSongTotalTime").toInt());
    ui->currentTimeSlider->setValue(setting.value("currentTime").toInt());
    mediaPlayer->setPosition(setting.value("currentTime").toInt());
    currentTime = setting.value("currentTime").toInt();
    setting.endGroup();

    /* 其余配置 */
    ui->searchWidget->show();
    ui->downloadPushButton->show();
    ui->localSongWidget->hide();
    ui->openLocalDirPushButton->hide();

    /* 加载上次搜索结果 */
    for(MusicItem musicItem: netMusicList->getPlaylist())
    {
        QPixmap cover(musicItem.coverPath);
        model->addMusic(MusicInfo(musicItem.title, musicItem.artist, musicItem.filePath, cover, musicItem.parentUrl));
    }

    /* 创建localMusicList */
    updateLocalMusicList();

    /* 创建播放列表 */
    createPlayList();
}

/* 更新封面图片内容 */
void SubMusicWidget::setMainPixmap()
{
    emit sendStateInfo(currentSong.coverPath);
    if(currentSong.coverPath.contains("http"))
    {
        /* 图片地址 */
        QUrl picUrl = currentSong.coverPath;

        /* 获取图片 */
        GetNetWork *getPic = new GetNetWork(this);

        /* 获取成功更新图标 */
        connect(getPic, &GetNetWork::sendData, this, [=](QByteArray picData){
            mainPixmap.loadFromData(picData);
            QPixmap scalePixmap = mainPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->musicPicLabel->setPixmap(scalePixmap);
        });

        /* 获取失败设置默认图标 */
        connect(getPic, &GetNetWork::sendError, this, [=](){
            mainPixmap.load(":/pic/defaultPic/resources/pic/defaultPic/geQuBao.png");
        });

        /* 发送请求 */
        getPic->getData(picUrl);
    }
    else if(currentSong.coverPath.contains("music_cover3"))
    {
        mainPixmap.load(":/pic/defaultPic/resources/pic/defaultPic/geQuBao.png");
        QPixmap scalePixmap = mainPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->musicPicLabel->setPixmap(scalePixmap);
    }
    else if(QFile::exists(currentSong.coverPath))
    {
        mainPixmap.load(currentSong.coverPath);
        QPixmap scalePixmap = mainPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->musicPicLabel->setPixmap(scalePixmap);
    }
    else
    {
        mainPixmap.load(":/pic/defaultPic/resources/pic/defaultPic/defaultMusicPic.svg");
        QPixmap scalePixmap = mainPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->musicPicLabel->setPixmap(scalePixmap);
    }
}

/* 更新本地音乐列表 */
void SubMusicWidget::updateLocalMusicList()
{
    localMusicModel->clear();
    for(MusicItem musicItem: this->localMusicList->getPlaylist())
    {
        if(QFile::exists(musicItem.filePath) == false){
            localMusicList->removeMusicItem(musicItem.filePath);
            // qDebug() << musicItem.filePath << "不存在的";
        }

        QPixmap cover;
        if(musicItem.coverPath == "")
            cover.load(":/pic/defaultPic/resources/pic/defaultPic/defaultMusicPic.svg");
        else
            cover.load(musicItem.coverPath);
        localMusicModel->addMusic(MusicInfo(musicItem.title, musicItem.artist, musicItem.filePath, cover, ""));
    }
}

/* 更新播放列表 */
void SubMusicWidget::createPlayList()
{
    currentList.clear();
    if(playMode == "single")
        currentList.append(currentSong);
    else
    {
        /* 判断当前播放的是本地歌曲还是网络歌曲 */
        if(isLocal() == true)
            for(MusicItem musicItem: this->localMusicList->getPlaylist())
                currentList.append(musicItem);
        else
            for(MusicItem musicItem: this->netMusicList->getPlaylist())
                currentList.append(musicItem);

        /* 若为随机播放则打乱播放列表顺序 */
        if(playMode == "random")
        {
            std::random_device rd;
            std::mt19937 g(rd());

            // 使用std::shuffle来打乱QList
            std::shuffle(currentList.begin(), currentList.end(), g);
        }
    }
}

/* 改变按钮提示和样式等 */
void SubMusicWidget::changePlayPushButtonStyle()
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
void SubMusicWidget::playMusic()
{

    if(isLocal() == false)
    {
        ui->downloadPushButton->setEnabled(true);
    }
    else
    {
        ui->downloadPushButton->setEnabled(false);
    }
    if(mediaPlayer->playbackState() == QMediaPlayer::PlaybackState::PlayingState)
    {
        mediaPlayer->stop();
    }
    mediaPlayer->setSource(currentSong.filePath);
    mediaPlayer->play();
    changePlayPushButtonStyle();
}

/* 打开本地文件夹 */
void SubMusicWidget::on_openLocalDirPushButton_clicked()
{
    auto musicDirString = QFileDialog::getExistingDirectory(this, "选择音乐所在目录", "D:\\qt-project\\request\\app\\music");
    QDir musicDir(musicDirString);
    auto musicList = musicDir.entryList(QStringList() << "*.mp3" << "*.m4a" << "*.wav");
    MusicItem musicItem;
    for(QString music : musicList)
    {
        musicItem.title = music;
        localMusicList->addMusicItem(music, "", musicDirString + '/' + music, "", "", "");
    }
    updateLocalMusicList();
}

/* 点击本地音乐 */
void SubMusicWidget::on_localSongListView_clicked(const QModelIndex &index)
{
    MusicItem musicItem = localMusicList->getPlaylist().at(index.row());
    currentSong = musicItem;
    ui->musicNameLabel->setText(musicItem.title);
    ui->ArtistLabel->setText(musicItem.artist);
    setMainPixmap();
    if(musicItem.coverPath == "")
        mainPixmap.load(":/pic/defaultPic/resources/pic/defaultPic/defaultMusicPic.svg");
    else
        mainPixmap.load(musicItem.coverPath);
    /* 创建播放列表 */
    createPlayList();
    /* 播放音乐 */
    playMusic();
}

/* 点击搜索结果 */
void SubMusicWidget::on_musicSearchResultListView_clicked(const QModelIndex &index)
{

    playIndexNetMusic(index);
    /* 创建播放列表 */
    createPlayList();
}

/* 主控按钮 */
void SubMusicWidget::on_playPushButton_clicked()
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

/* 下一首音乐按钮点击 */
void SubMusicWidget::on_nextMusicPushButton_clicked()
{
    playNextMusic();
}

/* 上一首音乐按钮点击 */
void SubMusicWidget::on_previousMusicPushButton_clicked()
{
    playLastMusic();
}

/* 播放网络歌曲 */
void SubMusicWidget::playIndexNetMusic(const QModelIndex &index)
{
    emit changeMusicProcessBegin();
    mediaPlayer->pause();
    ui->musicNameLabel->setText(index.data(Qt::UserRole + 1).toString());
    currentSong.title = index.data(Qt::UserRole + 1).toString();
    ui->ArtistLabel->setText(index.data(Qt::UserRole + 2).toString());
    currentSong.artist = index.data(Qt::UserRole + 2).toString();

    if(index.data(Qt::UserRole + 5).toString().contains("gequbao") == true)
    {
        /* 声明对象 */
        GetNetWork *getSongDetailInfo = new GetNetWork(this);

        /* 获取网址 */
        QString songDetailInfoUrl = index.data(Qt::UserRole + 5).toString();
        currentSong.parentUrl = songDetailInfoUrl;
        currentSong.filePath = songDetailInfoUrl;
        currentSong.lyricsPath = "geQuBao";
        currentSong.coverPath = ":/pic/defaultPic/resources/pic/defaultPic/defaultMusicPic.svg";

        /* 请求成功 */
        connect(getSongDetailInfo, &GetNetWork::sendData, this, [=](QByteArray htmlContent){
            /* 提取图片地址 */
            QRegularExpressionMatchIterator i = findFitStruct(R"(<meta property=\"og:image\" content=\"(.*?)\")", htmlContent);
            QString picUrl = i.next().captured(1);
            currentSong.coverPath = picUrl;

            /* 更新图片 */
            setMainPixmap();
        });

        /* 请求失败 */
        connect(getSongDetailInfo, &GetNetWork::sendError, this, [=](){
            emit sendStateInfo("歌曲封面加载失败");
            currentSong.coverPath = ":/pic/defaultPic/resources/pic/defaultPic/defaultMusicPic.svg";
            setMainPixmap();
        });

        emit sendStateInfo(songDetailInfoUrl);
        getSongDetailInfo->getData(songDetailInfoUrl);

        /* 获取歌曲播放地址 */
        GetNetWork *getSong = new GetNetWork(this);
        QRegularExpression regex("https://www\\.gequbao\\.com/music/(\\d+)");
        QRegularExpressionMatch match = regex.match(songDetailInfoUrl);
        QUrl api("https://www.gequbao.com/api/play_url");
        QUrlQuery query;
        query.addQueryItem("id", match.captured(1));
        query.addQueryItem("json", "1");
        api.setQuery(query);

        /* 请求成功 */
        connect(getSong, &GetNetWork::sendData, this, [=](QByteArray byteArrayData){
            QJsonObject jsonReply = QJsonDocument::fromJson(byteArrayData).object();
            QUrl songUrl = jsonReply.find("data")->toObject().find("url")->toString();
            currentSong.filePath = songUrl.toString();
            emit sendStateInfo(api.toString());

            playMusic();

            emit changeMusicProcessFinish();
        });

        /* 请求失败 */
        connect(getSong, &GetNetWork::sendError, this, [=](){
            emit sendStateInfo("获取歌曲播放链接失败");
            emit changeMusicProcessFinish();
        });

        getSong->getData(api);
        emit sendStateInfo(api.toString());
    }
}

/* 播放下一首歌曲 */
void SubMusicWidget::playNextMusic()
{
    int totalNum = currentList.count();
    int currentNum = 0;
    if(isLocal() == true)
    {
        for(MusicItem musicItem: currentList)
        {
            currentNum++;
            if(musicItem.filePath == currentSong.filePath)
                break;
        }
        currentSong = currentList.at(currentNum % totalNum);
        ui->localSongListView->setCurrentIndex(localMusicModel->findMusicByURL(currentSong.filePath));
        ui->musicNameLabel->setText(currentSong.title);
        ui->ArtistLabel->setText(currentSong.artist);
        setMainPixmap();
        playMusic();
    }
    else
    {
        for(MusicItem musicItem: currentList)
        {
            currentNum++;
            if(musicItem.parentUrl == currentSong.parentUrl)
                break;
        }
        currentSong = currentList.at(currentNum % totalNum);
        ui->musicNameLabel->setText(currentSong.title);
        ui->ArtistLabel->setText(currentSong.artist);
        setMainPixmap();
        /* 获取当前歌曲在列表中的Index */
        QModelIndex index = model->findMusicByParentURL(currentSong.parentUrl);
        ui->musicSearchResultListView->setCurrentIndex(index);
        playIndexNetMusic(index);
    }
}

/* 播放上一首歌曲 */
void SubMusicWidget::playLastMusic()
{
    int totalNum = currentList.count();
    int currentNum = -2;
    if(isLocal() == true)
    {
        for(MusicItem musicItem: currentList)
        {
            currentNum++;
            if(musicItem.filePath == currentSong.filePath)
                break;
        }
        currentSong = currentList.at((currentNum + totalNum) % totalNum);
        ui->localSongListView->setCurrentIndex(localMusicModel->findMusicByURL(currentSong.filePath));
        ui->musicNameLabel->setText(currentSong.title);
        ui->ArtistLabel->setText(currentSong.artist);
        setMainPixmap();
        playMusic();
    }
    else
    {
        for(MusicItem musicItem: currentList)
        {
            currentNum++;
            if(musicItem.parentUrl == currentSong.parentUrl)
                break;
        }
        currentSong = currentList.at((currentNum + totalNum) % totalNum);
        ui->musicNameLabel->setText(currentSong.title);
        ui->ArtistLabel->setText(currentSong.artist);
        setMainPixmap();
        /* 获取当前歌曲在列表中的Index */
        QModelIndex index = model->findMusicByParentURL(currentSong.parentUrl);
        ui->musicSearchResultListView->setCurrentIndex(index);
        playIndexNetMusic(index);
    }
}

/* 判断当前歌曲是否为本地歌曲 */
bool SubMusicWidget::isLocal()
{
    if(localMusicModel->findMusicByURL(currentSong.filePath) != QModelIndex())
        return true;
    else
        return false;
}

/* 下载 */
void SubMusicWidget::on_downloadPushButton_clicked()
{

    if(currentSong.filePath != "")
    {
        mainPixmap.save("music/cover/"+ currentSong.title + "-" + currentSong.artist + ".png");

        // emit sendDownloadInfo(currentSong.title, currentSong.filePath, "music/audio/" + currentSong.title + "-" + currentSong.artist + ".mp3", "audio");

        SingleDownloadFrame *downloadFrame = new SingleDownloadFrame(currentSong.title, currentSong.filePath, "music/audio/" + currentSong.title + "-" + currentSong.artist + ".mp3", "audio");

        /* 开始下载 */
        emit addDownloadTask(downloadFrame);

        connect(downloadFrame, &SingleDownloadFrame::downloadFinished, [=](){
            localMusicList->addMusicItem(currentSong.title, currentSong.artist, "music/audio/" + currentSong.title + "-" + currentSong.artist + ".mp3", "music/cover/"+ currentSong.title + "-" + currentSong.artist + ".png", currentSong.lyricsPath, currentSong.parentUrl);
        });
    }
}

