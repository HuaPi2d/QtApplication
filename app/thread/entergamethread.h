#ifndef ENTERGAMETHREAD_H
#define ENTERGAMETHREAD_H

#include <QThread>
#include "ssjjCore/ssjjCoreFun.h"

class EnterGameThread : public QThread
{
    Q_OBJECT
public:
    explicit EnterGameThread(QObject *parent = nullptr);

private:
    SingleTask task;
    int loadingTimes;

public slots:
    void receiveParams(SingleTask m_task, int m_loadingTimes);

protected:
    void run() override;

public: signals:
    void sendStates(SSJJRunState ssjjRunState);
};

#endif // ENTERGAMETHREAD_H
