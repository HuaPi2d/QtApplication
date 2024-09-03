#include "info/videoinfo.h"

int VideoInfo::getPlay() const
{
    return play;
}

void VideoInfo::setPlay(int newPlay)
{
    if (play == newPlay)
        return;
    play = newPlay;
    emit playChanged();
}

QString VideoInfo::getPic() const
{
    return pic;
}

void VideoInfo::setPic(const QString &newPic)
{
    if (pic == newPic)
        return;
    pic = newPic;
    emit picChanged();
}

QString VideoInfo::getArcurl() const
{
    return arcurl;
}

void VideoInfo::setArcurl(const QString &newArcurl)
{
    if (arcurl == newArcurl)
        return;
    arcurl = newArcurl;
    emit arcurlChanged();
}
