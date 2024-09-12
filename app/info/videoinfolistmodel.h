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

    Q_INVOKABLE void itemClicked(int index);

public: signals:
    Q_INVOKABLE void scrollToBottom();

private:
    QVector<VideoInfo> videoInfoList;

private: Q_PROPERTY(int state READ getState WRITE setState NOTIFY stateChanged FINAL)

signals:
    void finishReceiveData();

    void stateChanged();

public:
    // 状态 0:显示 1:加载
    int state;

    void clearModel(){
        // 通知视图即将重置模型数据
        beginResetModel();

        // 清空 QVector
        videoInfoList.clear();

        // 通知视图模型数据已经完成重置
        endResetModel();
    }

    void addVideoInfo(const VideoInfo &videoInfo);
    int getState() const;
    void setState(int newState);
};

#endif // VIDEOINFOLISTMODEL_H
