#ifndef VIDEOINFO_H
#define VIDEOINFO_H

#include <QObject>
#include <QtQml>

class VideoInfo : public QObject
{
    Q_OBJECT

    QML_ELEMENT
public:
    // 默认构造函数
    VideoInfo() = default;

    // 拷贝构造函数
    VideoInfo(const VideoInfo &other)
        : arcurl(other.arcurl),
        pic(other.pic),
        play(other.play),
        duration(other.duration),
        title(other.title),
        pubdate(other.pubdate),
        author(other.author)
    {
        // 如果有需要深拷贝的指针成员或动态分配的资源，在这里处理
    }

    // 拷贝赋值运算符
    VideoInfo& operator=(const VideoInfo &other)
    {
        if (this != &other) {
            arcurl = other.arcurl;
            pic = other.pic;
            play = other.play;
            duration = other.duration;
            title = other.title;
            pubdate = other.pubdate;
            author = other.author;
            // 复制其他成员变量
        }
        return *this;
    }

    // 数据成员
    QString arcurl;
    QString pic;
    int play;
    QString duration;
    QString title;
    int pubdate;
    QString author;
    QString getArcurl() const;
    void setArcurl(const QString &newArcurl);
    int getPlay() const;
    void setPlay(int newPlay);

    QString getPic() const;
    void setPic(const QString &newPic);

signals:
    void arcurlChanged();
    void playChanged();

    void picChanged();

private:
    Q_PROPERTY(QString arcurl READ getArcurl WRITE setArcurl NOTIFY arcurlChanged FINAL)
    Q_PROPERTY(int play READ getPlay WRITE setPlay NOTIFY playChanged FINAL)
    Q_PROPERTY(QString pic READ getPic WRITE setPic NOTIFY picChanged FINAL)
};

#endif // VIDEOINFO_H