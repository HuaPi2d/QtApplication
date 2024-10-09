#include "runscriptthread.h"

RunScriptThread::RunScriptThread(QObject *parent)
    : QThread{parent}
{}

void RunScriptThread::receiveParams(SingleTask m_task, int m_speed)
{
    task = m_task;
    speed = m_speed;
}

void RunScriptThread::run()
{
    SSJJRunState res = runScript(task, speed);
    emit sendStates(res);
    QThread::msleep(1000);
    this->deleteLater();
}
