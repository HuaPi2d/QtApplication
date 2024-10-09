#include "initializegamethread.h"

InitializeGameThread::InitializeGameThread(QObject *parent)
    : QThread{parent}
{}

void InitializeGameThread::receiveParams(QString m_taskName)
{
    taskName = m_taskName;
}

void InitializeGameThread::run()
{
    SSJJRunState res = initiallizeGameScreen(taskName);
    /* 返回初始化结果 */
    emit sendStates(res);
    QThread::msleep(1000);
    this->deleteLater();
}
