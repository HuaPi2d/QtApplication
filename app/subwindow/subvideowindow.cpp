#include "subvideowindow.h"
#include "subwidgets/subvideowidget.h"

SubVideoWindow::SubVideoWindow(QWidget *parent)
    : QMdiSubWindow{parent}
{
    SubVideoWidget *subVideoWidget = new SubVideoWidget(this);
    this->setWidget(subVideoWidget);
    this->setWindowFlags(Qt::FramelessWindowHint);

    connect(subVideoWidget, &SubVideoWidget::widgetClosed, this, [=](){
        emit windowDestroyed(this);
        this->deleteLater();
    });
    connect(subVideoWidget, &SubVideoWidget::sendStateInfo, this, &SubVideoWindow::sendStateInfo);
}
