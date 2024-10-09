#include "startssjjthread.h"

StartSSJJThread::StartSSJJThread(QObject *parent)
    : QThread{parent}
{}

void StartSSJJThread::receiveParams(QString m_ssjjInstallPath)
{
    ssjjInstallPath = m_ssjjInstallPath;
}

void StartSSJJThread::run()
{
    SSJJRunState state = restartSSJJ(ssjjInstallPath);
    sendStates(state);
    QThread::msleep(1000);
    this->deleteLater();
}
