#ifndef VIDEOINFOLISTMODEL_H
#define VIDEOINFOLISTMODEL_H

#include <QAbstractListModel>
#include "info/videoinfo.h"


class VideoInfoListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum RoleName{
        arcurl = Qt::DisplayRole + 1,
        pic,
        play,
        duration,
        title,
        pubdate,
        author
    };

    explicit VideoInfoListModel(QObject *parent = nullptr);

    static VideoInfoListModel * getInstance();

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int,QByteArray> roleNames() const override;

private:
    QVector<VideoInfo> videoInfoList;

private: signals:
    void finishReceiveData();

public:
    void clearModel(){
        // 通知视图即将重置模型数据
        beginResetModel();

        // 清空 QVector
        videoInfoList.clear();

        // 通知视图模型数据已经完成重置
        endResetModel();
    }

    void addVideoInfo(const VideoInfo &videoInfo);
};

#endif // VIDEOINFOLISTMODEL_H
