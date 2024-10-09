#include "subtexttransitionwindow.h"

SubTextTransitionWindow::SubTextTransitionWindow(QWidget *parent)
    : QMdiSubWindow{parent}
{
    SubTextTransitionWidget *subTextTransitionWidget = new SubTextTransitionWidget(this);
    this->setWidget(subTextTransitionWidget);
    this->setWindowFlags(Qt::FramelessWindowHint);

    connect(subTextTransitionWidget, &SubTextTransitionWidget::widgetClosed, this, [=](){
        emit windowDestroyed(this);
        this->deleteLater();
    });
    connect(subTextTransitionWidget, &SubTextTransitionWidget::sendStateInfo, this, &SubTextTransitionWindow::sendStateInfo);
    connect(subTextTransitionWidget, &SubTextTransitionWidget::addDownloadTask, this, &SubTextTransitionWindow::addDownloadTask);
}
