#include "entergamethread.h"

EnterGameThread::EnterGameThread(QObject *parent)
    : QThread{parent}
{}

void EnterGameThread::receiveParams(SingleTask m_task, int m_loadingTimes)
{
    task = m_task;
    loadingTimes = m_loadingTimes;
}

void EnterGameThread::run()
{
    SSJJRunState res = enterGame(task, loadingTimes);
    emit sendStates(res);
    QThread::msleep(1000);
    this->deleteLater();
}
