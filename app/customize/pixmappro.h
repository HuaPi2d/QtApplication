#ifndef PIXMAPPRO_H
#define PIXMAPPRO_H

#include <QObject>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>


class PixmapPro : public QObject
{
    Q_OBJECT
public:
    explicit PixmapPro(QString filePath, QObject *parent = nullptr);

public:
    QPixmap pixmap;

    QPixmap getPixmap() const;

signals:
    void pixmapChanged();
};

#endif // PIXMAPPRO_H
