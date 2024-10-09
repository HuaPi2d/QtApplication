#ifndef SUBTEXTTRANSITIONWINDOW_H
#define SUBTEXTTRANSITIONWINDOW_H

#include <QMdiSubWindow>
#include <otherwidgets/singledownloadframe.h>
#include <subwidgets/subtexttransitionwidget.h>

class SubTextTransitionWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit SubTextTransitionWindow(QWidget *parent = nullptr);

signals:
    void sendStateInfo(QString state);
    void windowDestroyed(QMdiSubWindow *window);
    void downloadFinished();
    void addDownloadTask(SingleDownloadFrame *singleDownloadFrame);
};

#endif // SUBTEXTTRANSITIONWINDOW_H
