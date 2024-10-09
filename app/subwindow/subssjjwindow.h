#ifndef SUBSSJJWINDOW_H
#define SUBSSJJWINDOW_H

#include <QMdiSubWindow>
#include "subwidgets/subssjjwidget.h"

class SubSSJJWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit SubSSJJWindow(QWidget *parent = nullptr);

signals:
    void sendStateInfo(QString state);
    void windowDestroyed(QMdiSubWindow *window);
    void downloadFinished();
};

#endif // SUBSSJJWINDOW_H
