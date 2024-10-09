#ifndef RANDOMLEARNINGWINDOW_H
#define RANDOMLEARNINGWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>

#include "subwidgets/randomlearningwidget.h"

class RandomLearningWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit RandomLearningWindow(QWidget *parent = nullptr);

signals:
    void sendStateInfo(QString state);
    void windowDestroyed(QMdiSubWindow *window);
    void downloadFinished();
};

#endif // RANDOMLEARNINGWINDOW_H
