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
};

#endif // SUBMUSICWINDOW_H
