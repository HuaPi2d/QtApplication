#ifndef RUNSCRIPTTHREAD_H
#define RUNSCRIPTTHREAD_H

#include <QThread>
#include "ssjjCore/ssjjCoreFun.h"

class RunScriptThread : public QThread
{
    Q_OBJECT
public:
    explicit RunScriptThread(QObject *parent = nullptr);

private:
    SingleTask task;
    int speed;

public slots:
    void receiveParams(SingleTask m_task, int m_speed);

protected:
    void run() override;

public: signals:
    void sendStates(SSJJRunState ssjjRunState);
};

#endif // RUNSCRIPTTHREAD_H
