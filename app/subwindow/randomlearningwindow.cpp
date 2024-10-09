#include "randomlearningwindow.h"

RandomLearningWindow::RandomLearningWindow(QWidget *parent)
    : QMdiSubWindow{parent}
{
    RandomLearningWidget *randomLearningWidget = new RandomLearningWidget(this);
    this->setWidget(randomLearningWidget);
    this->setWindowFlags(Qt::FramelessWindowHint);

    connect(randomLearningWidget, &RandomLearningWidget::widgetClosed, this, [=](){
        emit windowDestroyed(this);
        this->deleteLater();
    });
    connect(randomLearningWidget, &RandomLearningWidget::sendStateInfo, this, &RandomLearningWindow::sendStateInfo);
}
