#include "ssjjmainthread.h"

#include <QTimer>

SSJJMainThread::SSJJMainThread(QObject *parent)
    : QThread{parent}
{
    nextStep = "initializeGameScreen";
    currentThread = new QThread();
}

SSJJMainThread::~SSJJMainThread()
{}

void SSJJMainThread::initializeGameScreen()
{
    /* 初始化游戏画面 */
    InitializeGameThread* initializeGameThread = new InitializeGameThread(this);
    currentThread = initializeGameThread;
    connect(this, &SSJJMainThread::sendInitializeGameParams, initializeGameThread, &InitializeGameThread::receiveParams);
    connect(initializeGameThread, &InitializeGameThread::sendStates, this, &SSJJMainThread::receiveInitializeGameStates);

    emit sendInitializeGameParams(task.taskName);

    initializeGameThread->start();
    initializeGameThread->wait();
}

void SSJJMainThread::restartSSJJ()
{
    StartSSJJThread* startSSJJThread = new StartSSJJThread(this);
    currentThread = startSSJJThread;
    connect(this, &SSJJMainThread::sendRestartParams, startSSJJThread, &StartSSJJThread::receiveParams);
    connect(startSSJJThread, &StartSSJJThread::sendStates, this, &SSJJMainThread::receiveRestartSSJJStates);

    emit sendRestartParams(ssjjInstallPath);

    startSSJJThread->start();
    startSSJJThread->wait();
}

void SSJJMainThread::enterGame()
{
    EnterGameThread* enterGameThread = new EnterGameThread(this);
    currentThread = enterGameThread;
    connect(this, &SSJJMainThread::sendEnterGameParams, enterGameThread, &EnterGameThread::receiveParams);
    connect(enterGameThread, &EnterGameThread::sendStates, this, &SSJJMainThread::receiveEnterGameStates);

    emit sendEnterGameParams(task, loadingTimes);

    enterGameThread->start();
    enterGameThread->wait();
}

void SSJJMainThread::runScript()
{
    RunScriptThread* runScriptThread = new RunScriptThread(this);
    currentThread = runScriptThread;
    connect(this, &SSJJMainThread::sendRunScriptParams, runScriptThread, &RunScriptThread::receiveParams);
    connect(runScriptThread, &RunScriptThread::sendStates, this, &SSJJMainThread::receiveRunScriptStates);

    emit sendRunScriptParams(task, moveSpeed);

    // 设置时间限制
    QTimer::singleShot(singleScriptTime * 60 * 1000, runScriptThread, &RunScriptThread::terminate);

    runScriptThread->start();
    runScriptThread->wait();
}

void SSJJMainThread::settlement()
{
    SettlementThread* settlementThread = new SettlementThread(this);
    currentThread = settlementThread;
    connect(this, &SSJJMainThread::sendSettlementParams, settlementThread, &SettlementThread::receiveParams);
    connect(settlementThread, &SettlementThread::sendStates, this, &SSJJMainThread::receiveSettlementStates);

    emit sendSettlementParams(task.taskName);

    settlementThread->start();
    settlementThread->wait();
}

void SSJJMainThread::run()
{
    /* 开始运行 */
    if(task.taskName != ""){
        while(true){
            if(nextStep == "initializeGameScreen"){
                this->initializeGameScreen();
            }
            else if(nextStep == "enterGame"){
                this->enterGame();
            }
            else if(nextStep == "restartSSJJ"){
                this->restartSSJJ();
            }
            else if(nextStep == "fatalError"){
                emit sendFatalError();
                break;
            }
            else if(nextStep == "runScriptFile"){
                this->runScript();
            }
            else if(nextStep == "settlement"){
                this->settlement();
            }
        }
    }
}

void SSJJMainThread::receiveTask(SingleTask m_task, QString m_ssjjInstallPath, int m_moveSpeed, int m_singleScriptTime, int m_loadingTimes)
{
    task = m_task;
    ssjjInstallPath = m_ssjjInstallPath;
    moveSpeed = m_moveSpeed;
    singleScriptTime = m_singleScriptTime;
    loadingTimes = m_loadingTimes;
}

void SSJJMainThread::stopThread()
{
    if(currentThread != new QThread()){
        currentThread->terminate();
    }
}

void SSJJMainThread::receiveInitializeGameStates(SSJJRunState ssjjRunState)
{
    emit sendRemindInfo(ssjjRunState.remindText);
    if(ssjjRunState.errorType == "NoError"){
        nextStep = "enterGame";
    }
    else if(ssjjRunState.errorType == "Error"){
        nextStep = "restartSSJJ";
    }
}

void SSJJMainThread::receiveRestartSSJJStates(SSJJRunState ssjjRunState)
{
    emit sendRemindInfo(ssjjRunState.remindText);
    if(ssjjRunState.errorType == "FatalError"){
        nextStep = "fatalError";
    }
    else if(ssjjRunState.errorType == "Error"){
        nextStep = "restartSSJJ";
    }
    else if(ssjjRunState.errorType == "NoError"){
        nextStep = "initializeGameScreen";
    }
}

void SSJJMainThread::receiveEnterGameStates(SSJJRunState ssjjRunState)
{
    emit sendRemindInfo(ssjjRunState.remindText);
    if(ssjjRunState.errorType == "NoError"){
        nextStep = "runScriptFile";
    }
    else if(ssjjRunState.errorType == "Error"){
        nextStep = "restartSSJJ";
    }
    else if(ssjjRunState.errorType == "Remind"){
        nextStep = "enterGame";
    }
}

void SSJJMainThread::receiveRunScriptStates(SSJJRunState ssjjRunState)
{
    emit sendRemindInfo(ssjjRunState.remindText);
    if(ssjjRunState.errorType == "NoError"){
        nextStep = "settlement";
    }
}

void SSJJMainThread::receiveSettlementStates(SSJJRunState ssjjRunState)
{
    emit sendRemindInfo(ssjjRunState.remindText);
    QThread::msleep(100);
    if(ssjjRunState.errorType == "Success"){
        emit singleTaskFinished(ssjjRunState);
        nextStep = "enterGame";
    }
    else if(ssjjRunState.errorType == "Error"){
        nextStep = "restartSSJJ";
    }
}
