#include "videoinfolistmodel.h"

VideoInfoListModel::VideoInfoListModel(QObject *parent)
    : QAbstractListModel(parent)
{}

VideoInfoListModel *VideoInfoListModel::getInstance()
{
    static VideoInfoListModel *obj = new VideoInfoListModel;
    return obj;
}

int VideoInfoListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return videoInfoList.count();

    // FIXME: Implement me!
}

QVariant VideoInfoListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == RoleName::arcurl)
        return videoInfoList[index.row()].arcurl;
    else if(role == RoleName::pic)
        return videoInfoList[index.row()].pic;
    else if(role == RoleName::play)
        return videoInfoList[index.row()].play;
    else if(role == RoleName::duration)
        return videoInfoList[index.row()].duration;
    else if(role == RoleName::title)
        return videoInfoList[index.row()].title;
    else if(role == RoleName::pubdate)
        return videoInfoList[index.row()].pubdate;
    else if(role == RoleName::author)
        return videoInfoList[index.row()].author;
    else
        return QVariant();
}

QHash<int, QByteArray> VideoInfoListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleName::arcurl, "arcurl");
    roles.insert(RoleName::pic, "pic");
    roles.insert(RoleName::play, "play");
    roles.insert(RoleName::duration, "duration");
    roles.insert(RoleName::title, "title");
    roles.insert(RoleName::pubdate, "pubdate");
    roles.insert(RoleName::author, "author");
    return roles;
}

void VideoInfoListModel::itemClicked(int index)
{
    qDebug() << index;
}

int VideoInfoListModel::getState() const
{
    return state;
}

void VideoInfoListModel::setState(int newState)
{
    if (state == newState)
        return;
    state = newState;
    emit stateChanged();
}

void VideoInfoListModel::addVideoInfo(const VideoInfo &videoInfo)
{
    // 插入前通知模型
    beginInsertRows(QModelIndex(), videoInfoList.size(), videoInfoList.size());

    // 添加新的视频信息
    videoInfoList.append(videoInfo);

    // 插入后通知模型
    endInsertRows();
}
