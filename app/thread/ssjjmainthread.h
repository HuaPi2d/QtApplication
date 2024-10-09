#ifndef SSJJMAINTHREAD_H
#define SSJJMAINTHREAD_H

#include <QThread>
#include <thread/initializegamethread.h>
#include <thread/startssjjthread.h>
#include <thread/entergamethread.h>
#include <thread/runscriptthread.h>
#include <thread/settlementthread.h>

class SSJJMainThread : public QThread
{
    Q_OBJECT
public:
    explicit SSJJMainThread(QObject *parent = nullptr);
    ~SSJJMainThread();
    QThread* currentThread;

private:
    SingleTask task;
    QString ssjjInstallPath;
    int moveSpeed;
    int singleScriptTime;
    SSJJRunState runState;
    QString nextStep;
    int loadingTimes;

    void initializeGameScreen();
    void restartSSJJ();
    void enterGame();
    void runScript();
    void settlement();

protected:
    void run() override;

public:signals:
    void sendInitializeGameParams(QString taskNmae);
    void sendRestartParams(QString ssjjInstallPath);
    void sendEnterGameParams(SingleTask task, int loadingTimes);
    void sendRunScriptParams(SingleTask task, int speed);
    void sendSettlementParams(QString taskNmae);
    void sendRemindInfo(QString remindInfo);
    void singleTaskFinished(SSJJRunState res);
    void sendFatalError();

public slots:
    void receiveTask(SingleTask m_task, QString m_ssjjInstallPath, int m_moveSpeed, int m_singleScriptTime, int m_loadingTimes);
    void stopThread();
    void receiveInitializeGameStates(SSJJRunState ssjjRunState);
    void receiveRestartSSJJStates(SSJJRunState ssjjRunState);
    void receiveEnterGameStates(SSJJRunState ssjjRunState);
    void receiveRunScriptStates(SSJJRunState ssjjRunState);
    void receiveSettlementStates(SSJJRunState ssjjRunState);
};

#endif // SSJJMAINTHREAD_H
