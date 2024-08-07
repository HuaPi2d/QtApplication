#include "submusicwidget.h"
#include "ui_submusicwidget.h"
#include "universal/getnetwork.h"
#include "universal/rematcher.h"

#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QSettings>

SubMusicWidget::SubMusicWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubMusicWidget)
{
    ui->setupUi(this);

    /* 为输入框添加图标 */
    ui->musicSearchLineEdit->addAction(ui->music_search_action, QLineEdit::LeadingPosition);

    /* 设置自定义列表参数 */
    model->setParent(this);
    ui->musicSearchResultListView->setModel(model);
    delegate->setParent(this);
    ui->musicSearchResultListView->setItemDelegate(delegate);

    /* 设定播放参数 */
    playMode = "round";
    ui->playModePushButton->setIcon(QIcon(":/icon/resources/icons/IconamoonPlaylistRepeatList.svg"));
    ui->playModePushButton->setIconSize(QSize(24, 24));

    /* 设置图标 */
    mainPixmap.load(":/pic/defaultPic/resources/pic/defaultPic/defaultMusicPic.svg");
    setMainPixmap();

    /* 加载配置 */
    loadSettings();

    /* 绑定事件 */
    connect(ui->searchMusicPushButton, &QPushButton::clicked, this, &SubMusicWidget::searchMusic);
    connect(ui->musicSearchLineEdit, &QLineEdit::returnPressed, ui->searchMusicPushButton, &QPushButton::click); // 绑定QlineEdit与QPushbutton事件
    connect(ui->musicSearchResultListView, &QListView::clicked, this, &SubMusicWidget::searchResultClicked);  // 点击音乐列表内容
    connect(ui->playModePushButton, &QPushButton::clicked, this, &SubMusicWidget::playModeChange);  // 播放模式切换
    connect(ui->closePushButton, &QPushButton::clicked, this, [=](){
        emit widgetClosed();
    });
}

SubMusicWidget::~SubMusicWidget()
{
    saveSettings();
    delete ui;
}

void SubMusicWidget::searchMusic()
{
    if(ui->musicSourceComboBox->currentIndex() == 0){
        engineGeQuBao();
    }
}

void SubMusicWidget::engineGeQuBao()
{
    /* 声明对象 */
    GetNetWork *getSong = new GetNetWork(this);

    /* 请求网址 */
    QString urlPrifix = "https://www.gequbao.com/s/";
    QString urlSearchContent = ui->musicSearchLineEdit->text();

    /* 请求成功更新搜索结果 */
    connect(getSong, &GetNetWork::sendData, this, [=](QByteArray htmlContent){
        model->clear();
        QRegularExpressionMatchIterator i = findFitStruct(R"(<a href=\"(.*?)\"\n                               class=\"text-primary font-weight-bold\"\n                               target=\"_blank\">(.*?)\n                                                            </a>\n                        </div>\n                        <div class=\"text-success col-4 col-content\">\n                            (.*?)\n                        </div>)", htmlContent);
        QPixmap cover(":/pic/defaultPic/resources/pic/defaultPic/defaultMusicPic.svg");
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            QString url = "https://www.gequbao.com" + match.captured(1);
            QString songTitle = match.captured(2).trimmed();
            QString artist = match.captured(3).trimmed();
            model->addMusic(MusicInfo(songTitle, artist, cover, url));
        }
    });

    /* 请求失败状态栏显示 */
    connect(getSong, &GetNetWork::sendError, this, [=](){
        emit sendStateInfo("歌曲宝搜索超时，可尝试换源");
    });

    /* 发送请求 */
    getSong->getData(QUrl(urlPrifix + urlSearchContent));
}

void SubMusicWidget::searchResultClicked(const QModelIndex &index)
{
    ui->musicNameLabel->setText(index.data(Qt::UserRole + 1).toString());
    ui->ArtistLabel->setText(index.data(Qt::UserRole + 2).toString());
    if(index.data(Qt::UserRole + 4).toString().contains("gequbao") == true)
    {
        /* 声明对象 */
        GetNetWork *getSongDetailInfo = new GetNetWork(this);

        /* 获取网址 */
        QString songDetailInfoUrl = index.data(Qt::UserRole + 4).toString();

        /* 请求成功 */
        connect(getSongDetailInfo, &GetNetWork::sendData, this, [=](QByteArray htmlContent){
            /* 提取图片地址 */
            QRegularExpressionMatchIterator i = findFitStruct(R"(<meta property=\"og:image\" content=\"(.*?)\")", htmlContent);
            QString picUrl = i.next().captured(1);

            /* 获取图片 */
            GetNetWork *getPic = new GetNetWork(this);

            /* 获取成功更新图标 */
            connect(getPic, &GetNetWork::sendData, this, [=](QByteArray picData){
                mainPixmap.loadFromData(picData);
                setMainPixmap();
            });

            /* 获取失败设置默认图标 */
            connect(getPic, &GetNetWork::sendError, this, [=](){
                mainPixmap.load(":/pic/defaultPic/resources/pic/defaultPic/geQuBao.png");
                setMainPixmap();
            });

            /* 发送请求 */
            getPic->getData(picUrl);
        });

        /* 请求失败 */
        connect(getSongDetailInfo, &GetNetWork::sendError, this, [=](){
            emit sendStateInfo("歌曲封面加载失败");
            mainPixmap.load(":/pic/defaultPic/resources/pic/defaultPic/defaultMusicPic.svg");
            setMainPixmap();
        });

        getSongDetailInfo->getData(songDetailInfoUrl);

        /* 获取歌曲播放地址 */
        GetNetWork *getSong = new GetNetWork(this);
        QRegularExpressionMatchIterator iterator = findFitStruct(R"(https://www.gequbao.com/music/(.*?))", songDetailInfoUrl);
        QString api = "https://www.gequbao.com/api/play_url";

    }
}

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
    else if(playMode == "single")
    {
        playMode = "round";
        setPlayMode();
    }
}

void SubMusicWidget::saveSettings()
{
    /* 声明对象 */
    QSettings setting(qApp->applicationDirPath() + "/userSettings.ini", QSettings::IniFormat);

    /* 写入配置 */
    setting.beginGroup("music");
    setting.setValue("playMode", playMode);
    setting.setValue("currentSongName", ui->musicNameLabel->text());
    setting.setValue("currnetArtist", ui->ArtistLabel->text());
    setting.endGroup();
}

void SubMusicWidget::loadSettings()
{
    /* 声明对象 */
    QSettings setting(qApp->applicationDirPath() + "/userSettings.ini", QSettings::IniFormat);

    /* 写入配置 */
    setting.beginGroup("music");
    playMode = setting.value("playMode").toString();
    setPlayMode();
    ui->musicNameLabel->setText(setting.value("currentSongName").toString());
    ui->ArtistLabel->setText(setting.value("currnetArtist").toString());
    setting.endGroup();
}

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
}

void SubMusicWidget::setMainPixmap()
{
    QPixmap scalePixmap = mainPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->musicPicLabel->setPixmap(scalePixmap);
}

void SubMusicWidget::closeEvent(QCloseEvent *event)
{
    emit widgetClosed();
}
