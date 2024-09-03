#include "submusicwindow.h"
#include "subwidgets/submusicwidget.h"

#include <QTimer>
#include <QEventLoop>

SubMusicWindow::SubMusicWindow(QWidget *parent)
    : QMdiSubWindow{parent}
{
    SubMusicWidget *subMusicWidget = new SubMusicWidget(this);
    this->setWidget(subMusicWidget);
    this->setWindowFlags(Qt::FramelessWindowHint);

    connect(subMusicWidget, &SubMusicWidget::widgetClosed, this, [=](){
        emit windowDestroyed(this);
        this->deleteLater();
    });
    connect(subMusicWidget, &SubMusicWidget::sendStateInfo, this, &SubMusicWindow::sendStateInfo);
    connect(subMusicWidget, &SubMusicWidget::addDownloadTask, this, &SubMusicWindow::addDownloadTask);
}
