#ifndef SUBMUSICWINDOW_H
#define SUBMUSICWINDOW_H

#include <QMdiSubWindow>

class SubMusicWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit SubMusicWindow(QWidget *parent = nullptr);

signals:
    void snedStateInfo(QString state);
    void windowDestroyed(QMdiSubWindow *window);
};

#endif // SUBMUSICWINDOW_H
