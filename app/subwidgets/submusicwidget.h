#ifndef SUBMUSICWIDGET_H
#define SUBMUSICWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QString>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QSettings>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFileDialog>
#include <QMessageBox>
#include <QList>
#include <QtMultimedia>


class MusicInfo {
public:
    MusicInfo(const QString &title, const QString &artist, const QString &musicUrl,const QPixmap &cover, const QString &parentUrl)
        : title(title), artist(artist), musicUrl(musicUrl), cover(cover), parentUrl(parentUrl){}

    QString title;
    QString artist;
    QString musicUrl;
    QPixmap cover;
    QString parentUrl;
};


class MusicModel : public QStandardItemModel {
public:
    QModelIndex findMusicByURL(const QString &musicUrl) const {
        // 从第0行开始查找
        QModelIndexList matches = match(
            index(0, 0),            // 起始索引
            Qt::UserRole + 3,       // 查找UserRole + 4 (musicUrl) 数据
            musicUrl,               // 要匹配的 musicUrl
            1,                      // 只返回一个匹配项
            Qt::MatchExactly        // 精确匹配
            );

        if (!matches.isEmpty()) {
            return matches.first(); // 返回第一个匹配到的索引
        }

        return QModelIndex(); // 如果没有匹配到，返回无效索引
    }

    QModelIndex findMusicByParentURL(const QString &parentUrl) const {
        // 从第0行开始查找
        QModelIndexList matches = match(
            index(0, 0),            // 起始索引
            Qt::UserRole + 5,       // 查找UserRole + 4 (musicUrl) 数据
            parentUrl,               // 要匹配的 musicUrl
            1,                      // 只返回一个匹配项
            Qt::MatchExactly        // 精确匹配
            );

        if (!matches.isEmpty()) {
            return matches.first(); // 返回第一个匹配到的索引
        }

        return QModelIndex(); // 如果没有匹配到，返回无效索引
    }

    void addMusic(const MusicInfo &music) {
        QStandardItem *item = new QStandardItem;
        item->setData(music.title, Qt::UserRole + 1);
        item->setData(music.artist, Qt::UserRole + 2);
        item->setData(music.musicUrl, Qt::UserRole + 3);
        item->setData(music.cover, Qt::UserRole + 4);
        item->setData(music.parentUrl, Qt::UserRole + 5);
        appendRow(item);
    }
};


class MusicDelegate : public QStyledItemDelegate {
public:
    MusicDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QString title = index.data(Qt::UserRole + 1).toString();
        QString artist = index.data(Qt::UserRole + 2).toString();
        QPixmap cover = index.data(Qt::UserRole + 4).value<QPixmap>();

        // 检查鼠标悬停状态
        if (option.state & QStyle::State_MouseOver) {
            painter->fillRect(option.rect, option.palette.midlight());
        } else {
            painter->fillRect(option.rect, option.palette.base());
        }

        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, option.palette.highlight());
        }

        painter->save();

        // 设置边距
        const int margin = 5;

        // 绘制封面
        int coverSize = option.rect.height() - 2 * margin;
        QRect coverRect = option.rect.adjusted(margin, margin, -option.rect.width() + coverSize + margin, -margin);
        painter->drawPixmap(coverRect, cover);

        // 绘制标题和歌手
        QRect textRect = option.rect;
        textRect.setLeft(coverRect.right() + 10);
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, title + " - " + artist);

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        return QSize(200, 60); // 每一项的高度
    }
};

struct MusicItem {
    QString title;
    QString artist;
    QString filePath;
    QString coverPath;
    QString lyricsPath;  // 新增的字段，用于存储歌词文件路径
    QString parentUrl;
};

class PlaylistManager : public QObject {
    Q_OBJECT

public:
    PlaylistManager(const QString &filePath, QObject *parent = nullptr)
        : QObject(parent), filePath(filePath) {}

    bool load() {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Could not open file for reading:" << file.errorString();
            return false;
        }

        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull() || !doc.isObject()) {
            qWarning() << "Failed to parse JSON document";
            return false;
        }

        QJsonArray jsonArray = doc.object().value("playlist").toArray();
        for (const QJsonValue &value : jsonArray) {
            QJsonObject obj = value.toObject();
            MusicItem item;
            item.title = obj.value("title").toString();
            item.artist = obj.value("artist").toString();
            item.filePath = obj.value("filePath").toString();
            item.coverPath = obj.value("coverPath").toString();
            item.lyricsPath = obj.value("lyricsPath").toString(); // 读取歌词路径
            item.parentUrl = obj.value("parentUrl").toString();
            playlist.append(item);
        }

        return true;
    }

    bool save() const {
        QJsonArray jsonArray;
        for (const MusicItem &item : playlist) {
            QJsonObject obj;
            obj["title"] = item.title;
            obj["artist"] = item.artist;
            obj["filePath"] = item.filePath;
            obj["coverPath"] = item.coverPath;
            obj["lyricsPath"] = item.lyricsPath; // 保存歌词路径
            obj["parentUrl"] = item.parentUrl;
            jsonArray.append(obj);
        }

        QJsonObject jsonObj;
        jsonObj["playlist"] = jsonArray;
        QJsonDocument doc(jsonObj);

        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "Could not open file for writing:" << file.errorString();
            return false;
        }

        file.write(doc.toJson());
        file.close();

        return true;
    }

    void addMusicItem(const QString &title, const QString &artist, const QString &filePath, const QString &coverPath, const QString &lyricsPath, const QString &parentUrl) {
        if(checkMusicItem(title, artist, filePath)) {
            return;
        }
        MusicItem item;
        item.title = title;
        item.artist = artist;
        item.filePath = filePath;
        item.coverPath = coverPath;
        item.lyricsPath = lyricsPath; // 添加歌词路径
        item.parentUrl = parentUrl;
        playlist.append(item);
    }

    void removeMusicItem(const QString &filePath) {
        auto it = std::remove_if(playlist.begin(), playlist.end(), [filePath](const MusicItem &item) {
            return item.filePath == filePath;
        });
        if (it != playlist.end()) {
            playlist.erase(it, playlist.end());
        }
    }

    bool checkMusicItem(const QString &title, const QString &artist, const QString &filePath) {
        auto it = std::find_if(playlist.begin(), playlist.end(), [title, artist, filePath](const MusicItem &item) {
            return item.title == title && item.artist == artist && item.filePath == filePath;
        });
        return it != playlist.end();
    }

    void clear() {
        playlist.clear();
    }

    QVector<MusicItem> getPlaylist() const {
        return playlist;
    }

private:
    QString filePath;
    QVector<MusicItem> playlist; // 使用QVector代替QList
};

namespace Ui {
class SubMusicWidget;
}

class SubMusicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubMusicWidget(QWidget *parent = nullptr);
    ~SubMusicWidget();
    void searchMusic();
    void engineGeQuBao();
    void playModeChange();
    void saveSettings();
    void loadSettings();
    void setPlayMode();
    void setMainPixmap();
    void updateLocalMusicList();
    void createPlayList();
    void changePlayPushButtonStyle();
    void playMusic();
    void playIndexNetMusic(const QModelIndex &index);
    void playNextMusic();
    void playLastMusic();
    bool isLocal();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::SubMusicWidget *ui;
    MusicModel *model = new MusicModel;
    MusicModel *localMusicModel = new MusicModel;
    MusicDelegate *delegate = new MusicDelegate;
    MusicDelegate *localMusicDelegate = new MusicDelegate;
    QPixmap mainPixmap;
    QString playMode;
    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
    PlaylistManager *localMusicList;
    PlaylistManager *netMusicList;
    QVector<MusicItem> currentList;
    MusicItem currentSong;
    int currentTime;


public: signals:
    void widgetClosed();
    void sendStateInfo(QString state);
    void changeMusicProcessBegin();
    void changeMusicProcessFinish();

private slots:
    void on_openLocalDirPushButton_clicked();
    void on_localSongListView_clicked(const QModelIndex &index);
    void on_musicSearchResultListView_clicked(const QModelIndex &index);
    void on_playPushButton_clicked();
    void on_nextMusicPushButton_clicked();
    void on_previousMusicPushButton_clicked();
    void on_downloadPushButton_clicked();
};

#endif // SUBMUSICWIDGET_H
