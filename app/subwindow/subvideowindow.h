#ifndef SUBVIDEOWINDOW_H
#define SUBVIDEOWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include "otherwidgets/singledownloadframe.h"

class SubVideoWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit SubVideoWindow(QWidget *parent = nullptr);

signals:
    void sendStateInfo(QString state);
    void windowDestroyed(QMdiSubWindow *window);
    void updateQuickWidgetColor(QColor color);
};

#endif // SUBVIDEOWINDOW_H
