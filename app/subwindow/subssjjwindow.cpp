#include "subssjjwindow.h"

SubSSJJWindow::SubSSJJWindow(QWidget *parent)
    : QMdiSubWindow{parent}
{
    SubSSJJWidget *subSSJJWidget = new SubSSJJWidget(this);
    this->setWidget(subSSJJWidget);
    this->setWindowFlags(Qt::FramelessWindowHint);

    connect(subSSJJWidget, &SubSSJJWidget::widgetClosed, this, [=](){
        emit windowDestroyed(this);
        this->deleteLater();
    });
    connect(subSSJJWidget, &SubSSJJWidget::sendStateInfo, this, &SubSSJJWindow::sendStateInfo);
}
