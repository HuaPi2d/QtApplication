#ifndef SUBMUSICWINDOW_H
#define SUBMUSICWINDOW_H

#include <QMdiSubWindow>
#include <QUrl>
#include "otherwidgets/singledownloadframe.h"

class SubMusicWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit SubMusicWindow(QWidget *parent = nullptr);

signals:
    void sendStateInfo(QString state);
    void windowDestroyed(QMdiSubWindow *window);
    void downloadFinished();
    void addDownloadTask(SingleDownloadFrame *singleDownloadFrame);
};

#endif // SUBMUSICWINDOW_H
