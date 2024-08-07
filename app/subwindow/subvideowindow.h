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
};

#endif // SUBVIDEOWINDOW_H
