#ifndef INITIALIZEGAMETHREAD_H
#define INITIALIZEGAMETHREAD_H

#include <QThread>

#include "ssjjCore/ssjjCoreFun.h"

class InitializeGameThread : public QThread
{
    Q_OBJECT
public:
    explicit InitializeGameThread(QObject *parent = nullptr);

private:
    QString taskName;

public slots:
    void receiveParams(QString m_taskName);

protected:
    void run() override;

public: signals:
    void sendStates(SSJJRunState ssjjRunState);
};

#endif // INITIALIZEGAMETHREAD_H
