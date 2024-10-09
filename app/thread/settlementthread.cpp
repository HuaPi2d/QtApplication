#include "settlementthread.h"

SettlementThread::SettlementThread(QObject *parent)
    : QThread{parent}
{}

void SettlementThread::receiveParams(QString m_taskName)
{
    taskName = m_taskName;
}

void SettlementThread::run()
{
    SSJJRunState res = settlement(taskName);
    /* 返回初始化结果 */
    emit sendStates(res);
    QThread::msleep(1000);
    this->deleteLater();
}
