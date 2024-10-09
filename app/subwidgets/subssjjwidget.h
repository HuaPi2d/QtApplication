#ifndef SUBSSJJWIDGET_H
#define SUBSSJJWIDGET_H

#include <QWidget>
#include <QTableWidgetItem>

#include "thread/ssjjmainthread.h"

namespace Ui {
class SubSSJJWidget;
}

class SubSSJJWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubSSJJWidget(QWidget *parent = nullptr);
    ~SubSSJJWidget();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::SubSSJJWidget *ui;

    QString ssjjInstallPath;
    QVector<QWidget*> widgetList;
    QWidget* currentWidget;
    int currentRow;
    int runningState;
    SingleTask currentTask;
    SSJJMainThread* ssjjMainThread;

private: signals:
    void widgetClosed();
    void sendStateInfo(QString state);
    void sendRestartParams(QString ssjjInstallPath);
    void sendInitializeGameParams(QString taskName);
    void sendSingleTask(SingleTask singleTask, QString ssjjInstallPath, int moveSpeed, int singleScriptTime, int loadingTimes);


private slots:
    void saveSettings();
    void loadSettings();
    void on_testPushButton_clicked();
    void on_closePushButton_clicked();
    void writeRemindInfo(QString htmlString);
    void on_settingPushButton_clicked();
    void on_scriptPushButton_clicked();
    void updateScreen();
    void on_chooseLauncherPathPushButton_clicked();
    void on_addTaskPushButton_clicked();
    void on_chooseLDScriptPathPushButton_clicked();
    void on_startPushButton_clicked();
    void on_endPushButton_clicked();
    void getSingleTask();
    void receiveRemindInfo(QString remindInfo);
    void sendNextTask(SSJJRunState res);
    void receiveFatalError();
    void clearRow(QTableWidgetItem *item);
};

#endif // SUBSSJJWIDGET_H
