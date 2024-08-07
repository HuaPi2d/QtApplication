#ifndef SUBMUSICWIDGET_H
#define SUBMUSICWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QString>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QStyledItemDelegate>
#include <QPainter>

class MusicInfo {
public:
    MusicInfo(const QString &title, const QString &artist, const QPixmap &cover, const QString &musicUrl)
        : title(title), artist(artist), cover(cover), musicUrl(musicUrl){}

    QString title;
    QString artist;
    QPixmap cover;
    QString musicUrl;
};


class MusicModel : public QStandardItemModel {
public:
    void addMusic(const MusicInfo &music) {
        QStandardItem *item = new QStandardItem;
        item->setData(music.title, Qt::UserRole + 1);
        item->setData(music.artist, Qt::UserRole + 2);
        item->setData(music.cover, Qt::UserRole + 3);
        item->setData(music.musicUrl, Qt::UserRole + 4);
        appendRow(item);
    }
};


class MusicDelegate : public QStyledItemDelegate {
public:
    MusicDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QString title = index.data(Qt::UserRole + 1).toString();
        QString artist = index.data(Qt::UserRole + 2).toString();
        QPixmap cover = index.data(Qt::UserRole + 3).value<QPixmap>();

        // 检查鼠标悬停状态
        if (option.state & QStyle::State_MouseOver) {
            painter->fillRect(option.rect, option.palette.highlight());
        } else {
            painter->fillRect(option.rect, option.palette.base());
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
    void searchResultClicked(const QModelIndex &index);
    void playModeChange();
    void saveSettings();
    void loadSettings();
    void setPlayMode();
    void setMainPixmap();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::SubMusicWidget *ui;
    MusicModel *model = new MusicModel;
    MusicDelegate *delegate = new MusicDelegate;
    QPixmap mainPixmap;
    QString playMode;

public: signals:
    void widgetClosed();
    void sendStateInfo(QString state);
};

#endif // SUBMUSICWIDGET_H
