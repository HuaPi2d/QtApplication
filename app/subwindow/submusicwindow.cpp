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
        this->deleteLater();
    });
    connect(subMusicWidget, &SubMusicWidget::sendStateInfo, this, &SubMusicWindow::snedStateInfo);
}
