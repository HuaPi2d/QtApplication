#ifndef STARTSSJJTHREAD_H
#define STARTSSJJTHREAD_H

#include <QThread>
#include <QErrorMessage>

#include "ssjjCore/ssjjCoreFun.h"

class StartSSJJThread : public QThread
{
    Q_OBJECT
public:
    explicit StartSSJJThread(QObject *parent = nullptr);

private:
    QString ssjjInstallPath;

public slots:
    void receiveParams(QString m_ssjjInstallPath);

protected:
    void run() override;

public: signals:
    void sendStates(SSJJRunState ssjjRunState);
};

#endif // STARTSSJJTHREAD_H
