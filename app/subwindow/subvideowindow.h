#ifndef SUBVIDEOWINDOW_H
#define SUBVIDEOWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>

class SubVideoWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit SubVideoWindow(QWidget *parent = nullptr);

signals:
    void snedStateInfo(QString state);
    void windowDestroyed(QMdiSubWindow *window);
};

#endif // SUBVIDEOWINDOW_H
