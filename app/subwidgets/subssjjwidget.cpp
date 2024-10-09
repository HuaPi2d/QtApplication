#include "subssjjwidget.h"
#include "ui_subssjjwidget.h"
#include "thread/startssjjthread.h"
#include "thread/initializegamethread.h"
#include "universal/programController.h"
#include "universal/findPicPro.h"
#include "QHotkey/qhotkey.h"

#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QIntValidator>
#include <QScrollBar>

// 获取生死狙击安装路径
QString getSSJJInstallPath() {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Wooduan\\SSJJ-4399", QSettings::NativeFormat);
    QString ssjjPath = settings.value("InstallPath").toString();
    return ssjjPath;
}

SubSSJJWidget::SubSSJJWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubSSJJWidget)
{
    ui->setupUi(this);
    widgetList.append(ui->scriptWidget);
    widgetList.append(ui->settingWidget);
    currentWidget = ui->scriptWidget;
    updateScreen();

    /* 脚本框设置 */
    ui->remindTextEdit->setReadOnly(true);
    ui->taskTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->testPushButton->setVisible(false);
    writeRemindInfo("<p>欢迎使用<b>生死狙击脚本</b>工具</p><p><span style=\"font-size: 13px; color: red;\">使用时注意本栏信息</span></p><p>请开启<span style=\"font-size: 13px; color: red;\"><b>自动登录</b></span></p>");
    ui->launcherPathLineEdit->setReadOnly(true);
    ui->LDScriptPathLineEdit->setReadOnly(true);
    QIntValidator *validator = new QIntValidator(0, 999, this);
    ui->moveSpeedLineEdit->setValidator(validator);
    ui->LDRunTimesLineEdit->setValidator(validator);
    QIntValidator *loadingTimeValidator = new QIntValidator(0, 99, this);
    ui->loadingTimeLineEdit->setValidator(loadingTimeValidator);
    QIntValidator *validatorScriptTime = new QIntValidator(0, 20, this);
    ui->singleScriptTimeLineEdit->setValidator(validatorScriptTime);

    /* 热键设置 */
    QHotkey* F10 = new QHotkey(QKeySequence("F10"),true);
    QObject::connect(F10, &QHotkey::activated, ui->startPushButton, &QPushButton::click);
    QHotkey* F11 = new QHotkey(QKeySequence("F11"),true);
    QObject::connect(F11, &QHotkey::activated, ui->endPushButton, &QPushButton::click);
    runningState = 0;

    /* 清楚表格 */
    connect(ui->taskTableWidget, &QTableWidget::itemClicked, this, &SubSSJJWidget::clearRow);

    loadSettings();

    writeRemindInfo("<p><span style=\"color: red;\">请确保在每次启动游戏时不会弹出以下图片</p><br>");
    writeRemindInfo("<img src=\"tips/ensure_ratio.png\" width=\"250\" alt=\"提示图片\"><br>");
}

SubSSJJWidget::~SubSSJJWidget()
{
    saveSettings();
    delete ui;
}

void SubSSJJWidget::closeEvent(QCloseEvent *event)
{
    emit widgetClosed();
}

void SubSSJJWidget::saveSettings()
{
    /* 声明对象 */
    QSettings setting(qApp->applicationDirPath() + "/userSettings.ini", QSettings::IniFormat);

    /* 写入配置 */
    setting.beginGroup("SSJJ_Script");
    setting.setValue("ssjjInstallPath", ssjjInstallPath);
    setting.setValue("moveSpeed", ui->moveSpeedLineEdit->text());
    setting.setValue("LDRunTimes", ui->LDRunTimesLineEdit->text());
    setting.setValue("LDScriptPath", ui->LDScriptPathLineEdit->text());
    setting.setValue("singleScriptTime", ui->singleScriptTimeLineEdit->text());
    setting.setValue("loadingTime", ui->loadingTimeLineEdit->text());
    setting.endGroup();
}

void SubSSJJWidget::loadSettings()
{
    /* 声明对象 */
    QSettings setting(qApp->applicationDirPath() + "/userSettings.ini", QSettings::IniFormat);

    /* 写入配置 */
    setting.beginGroup("SSJJ_Script");
    if(setting.value("ssjjInstallPath").toString() != "")
    {
        ssjjInstallPath = setting.value("ssjjInstallPath").toString();
        writeRemindInfo("<p>生死狙击程序安装路径:</p><b>" + ssjjInstallPath + "</b><br><br>");
    }
    else
    {
        ssjjInstallPath = getRegDitValue("\\HKEY_CURRENT_USER\\Software\\Wooduan\\SSJJ-4399", "InstallPath") + "\\WDlauncher.exe";
        if( ssjjInstallPath == ""){
            writeRemindInfo("<p><span style=\"font-size: 13px; color: red;\">生死狙击程序安装路径读取失败，请手动添加<b>WDlauncher.exe</b>的路径</span></p><br>");
            writeRemindInfo("<br><img src=\"tips/choose_launcher_tip.png\" width=\"250\" alt=\"提示图片\"><br>");
        }
        else{
            writeRemindInfo("<p>生死狙击程序安装路径:<b>" + ssjjInstallPath + "</b></p><br>");
        }
    }
    if(setting.value("moveSpeed").toString() != ""){
        ui->moveSpeedLineEdit->setText(setting.value("moveSpeed").toString());
    }
    else{
        ui->moveSpeedLineEdit->setText("100");
    }
    ui->launcherPathLineEdit->setText(ssjjInstallPath);
    if(setting.value("LDRunTimes").toString() != ""){
        ui->LDRunTimesLineEdit->setText(setting.value("LDRunTimes").toString());
    }
    else{
        ui->LDRunTimesLineEdit->setText("100");
    }
    ui->LDScriptPathLineEdit->setText(setting.value("LDScriptPath").toString());
    if(setting.value("singleScriptTime").toString() != ""){
        ui->singleScriptTimeLineEdit->setText(setting.value("singleScriptTime").toString());
    }
    else{
        ui->singleScriptTimeLineEdit->setText("5");
    }
    if(setting.value("loadingTime").toString() != ""){
        ui->loadingTimeLineEdit->setText(setting.value("loadingTime").toString());
    }
    else{
        ui->loadingTimeLineEdit->setText("18");
    }
    setting.endGroup();
}

void SubSSJJWidget::on_testPushButton_clicked()
{
    StartSSJJThread* startSSJJThread = new StartSSJJThread();
    connect(this, &SubSSJJWidget::sendRestartParams, startSSJJThread, &StartSSJJThread::receiveParams);
    connect(startSSJJThread, &StartSSJJThread::sendStates, this, [=](SSJJRunState res){
        ui->remindTextEdit->insertHtml(res.remindText);
        if(res.errorType == "FatalError"){
            return;
        }
        else if(res.errorType == "Error"){
            ui->remindTextEdit->insertHtml("<p>正在尝试重新启动</p>");
            return;
        }
    });
    emit sendRestartParams(ssjjInstallPath);
    startSSJJThread->start();
}

void SubSSJJWidget::on_closePushButton_clicked()
{
    this->close();
}

// 添加提示信息
void SubSSJJWidget::writeRemindInfo(QString htmlString)
{
    // 获取当前光标位置
    QTextCursor cursor = ui->remindTextEdit->textCursor();

    // 将光标移动到文档末尾
    cursor.movePosition(QTextCursor::End);

    cursor.insertHtml(htmlString);

    // 滚动到 QTextEdit 的末尾
    QScrollBar *scrollBar = ui->remindTextEdit->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

/* 转到设置界面 */
void SubSSJJWidget::on_settingPushButton_clicked()
{
    currentWidget = ui->settingWidget;
    updateScreen();
}

/* 转到脚本界面 */
void SubSSJJWidget::on_scriptPushButton_clicked()
{
    currentWidget = ui->scriptWidget;
    updateScreen();
}

/* 界面转换的更新 */
void SubSSJJWidget::updateScreen()
{
    for(QWidget* m_Widget: widgetList){
        if(m_Widget == currentWidget){
            m_Widget->show();
        }
        else{
            m_Widget->hide();
        }
    }
}

/* 选择启动器路径 */
void SubSSJJWidget::on_chooseLauncherPathPushButton_clicked()
{
    // 设置文件过滤器，只允许选择 .exe 文件
    QString filter = "Executable Files (*.exe)";

    // 打开文件选择对话框
    QString fileName = QFileDialog::getOpenFileName(this, "请选择 WDlauncher.exe",  "", filter);

    // 检查用户是否选择了文件
    if (!fileName.isEmpty()) {
        // 检查选择的文件是否是 WDlauncher.exe
        QFileInfo fileInfo(fileName);
        // qDebug() << fileInfo.fileName();
        if (fileInfo.fileName().contains("WDlauncher.exe")) {
            // 如果是 WDlauncher.exe，执行进一步操作
            QMessageBox::information(this, "成功", "您已成功选择 WDlauncher.exe 文件！");
            ui->launcherPathLineEdit->setText(fileName);
            ssjjInstallPath = fileName;
        } else {
            // 如果不是 WDlauncher.exe，弹出警告提示
            QMessageBox::warning(this, "错误", "请选择 WDlauncher.exe 文件！");
        }
    }
}

/* 判断当前脚本界面是否选择了关卡 */
bool isAnyRadioButtonChecked(QWidget *widget) {
    // 获取所有子对象
    QList<QRadioButton *> radioButtons = widget->findChildren<QRadioButton *>();

    // 遍历所有 QRadioButton，判断是否有被选中的
    for (QRadioButton *radioButton : radioButtons) {
        if (radioButton->isChecked()) {
            return true;  // 找到被选中的按钮
        }
    }

    return false;  // 没有按钮被选中
}

/* 添加脚本任务 */
void SubSSJJWidget::on_addTaskPushButton_clicked()
{
    if(isAnyRadioButtonChecked(ui->tabWidget->currentWidget())){
        ui->taskTableWidget->setRowCount(ui->taskTableWidget->rowCount() + 1);
        currentRow = ui->taskTableWidget->rowCount();
    }
    else{
        writeRemindInfo("<p>请<span style=\"color:red;\"><b>先选择关卡</b></span>后再添加任务</p><br>");
        return;
    }

    if(ui->tabWidget->currentWidget() == ui->LDTab){
        if(ui->LDRunTimesLineEdit->text() == ""){
            writeRemindInfo("<p><span style=\"color:red;\"><b>关卡运行次数</b></span>未指定</p><br>");
            ui->taskTableWidget->removeRow(currentRow - 1);
            return;
        }

        ui->taskTableWidget->setItem(currentRow - 1, 1, new QTableWidgetItem("无"));
        ui->taskTableWidget->setItem(currentRow - 1, 2, new QTableWidgetItem(ui->LDRunTimesLineEdit->text()));
        ui->taskTableWidget->setItem(currentRow - 1, 3, new QTableWidgetItem("0"));
        if(ui->LDScriptPathLineEdit->text() != ""){
            ui->taskTableWidget->setItem(currentRow - 1, 4, new QTableWidgetItem(ui->LDScriptPathLineEdit->text()));
        }
        else{
            ui->taskTableWidget->setItem(currentRow - 1, 4, new QTableWidgetItem("未选择"));
        }

        if(ui->teamPropRadioButton->isChecked() == true){
            ui->taskTableWidget->setItem(currentRow - 1, 0, new QTableWidgetItem("团队道具赛"));
        }
    }
    else if(ui->tabWidget->currentWidget() == ui->ZXTab){

    }
}

/* 选择脚本 */
void SubSSJJWidget::on_chooseLDScriptPathPushButton_clicked()
{
    // 设置文件过滤器，只允许选择 .exe 文件
    QString filter = "Script File (*.scp)";

    // 打开文件选择对话框
    QString fileName = QFileDialog::getOpenFileName(this, "请选择乱斗脚本文件(LD结尾)",  "", filter);

    // 检查用户是否选择了文件
    if (!fileName.isEmpty()) {
        // 检查选择的文件是否是 乱斗脚本
        QFileInfo fileInfo(fileName);
        if (fileInfo.fileName().contains("LD")) {
            // 如果是 乱斗脚本，执行进一步操作
            ui->LDScriptPathLineEdit->setText(fileName);
        } else {
            // 如果不是 乱斗脚本，弹出警告提示
            QMessageBox::information(this, "提示", "选择的可能不是乱斗脚本");
            ui->LDScriptPathLineEdit->setText(fileName);
        }
    }
    else{
        ui->LDScriptPathLineEdit->setText("");
    }
}

/* 开始运行脚本按钮按下 */
void SubSSJJWidget::on_startPushButton_clicked()
{
    // 重置任务次数
    for(int i = 0; i < ui->taskTableWidget->rowCount(); i++){
        ui->taskTableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(0)));
    }

    if(runningState == 1){
        writeRemindInfo("<p><span style=\"color:red;\"><b>脚本正在运行中，请勿重复点击</b></span></p><br>");
        return;
    }
    runningState = 1;
    writeRemindInfo("<p><span style=\"color:lightgreen;\"><b>脚本开始运行</b></span></p><br>");

    // 脚本逻辑
    /* 创建脚本线程 */
    ssjjMainThread = new SSJJMainThread(this);
    /* 接收提示信息 */
    connect(ssjjMainThread, &SSJJMainThread::sendRemindInfo, this, &SubSSJJWidget::receiveRemindInfo);
    /* 发送单个任务 */
    connect(this, &SubSSJJWidget::sendSingleTask, ssjjMainThread, &SSJJMainThread::receiveTask);
    /* 接收任务执行结果 */
    connect(ssjjMainThread, &SSJJMainThread::singleTaskFinished, this, &SubSSJJWidget::sendNextTask);
    connect(ssjjMainThread, &SSJJMainThread::sendFatalError, this, &SubSSJJWidget::receiveFatalError);
    /* 发送任务 */
    sendNextTask(SSJJRunState());
    /* 开始执行 */
    ssjjMainThread->start();
}

void SubSSJJWidget::on_endPushButton_clicked()
{
    if(runningState == 0){
        writeRemindInfo("<p><span style=\"color:red;\"><b>当前没有脚本运行</b></span></p><br>");
        return;
    }

    // 强制停止逻辑
    if(ssjjMainThread->isFinished() == false){
        ssjjMainThread->stopThread();
        ssjjMainThread->terminate();
    }

    runningState = 0;
    writeRemindInfo("<p><span style=\"color:lightgreen;\"><b>脚本运行已结束</b></span></p><br>");
}

void SubSSJJWidget::getSingleTask()
{
    for(int i = 0; i < ui->taskTableWidget->rowCount(); i++){
        if(ui->taskTableWidget->item(i, 3)->text().toInt() < ui->taskTableWidget->item(i, 2)->text().toInt()){
            currentTask.taskName = ui->taskTableWidget->item(i, 0)->text();
            currentTask.difficulty = ui->taskTableWidget->item(i, 1)->text();
            currentTask.script = ui->taskTableWidget->item(i, 4)->text();
            return;
        }
    }

    // 运行结束
    runningState = 0;
}

void SubSSJJWidget::receiveRemindInfo(QString remindInfo)
{
    writeRemindInfo(remindInfo);
}

/* 发送下一个任务 */
void SubSSJJWidget::sendNextTask(SSJJRunState res)
{
    /* 执行成功更新表格 */
    if(res.errorType == "Success"){
        for(int i = 0; i < ui->taskTableWidget->rowCount(); i++){
            if(ui->taskTableWidget->item(i, 3)->text().toInt() < ui->taskTableWidget->item(i, 2)->text().toInt()){
                ui->taskTableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(ui->taskTableWidget->item(i, 3)->text().toInt() + 1)));
            }
        }
    }

    getSingleTask();

    if(runningState == 1){
        emit sendSingleTask(currentTask, ssjjInstallPath, ui->moveSpeedLineEdit->text().toInt(), ui->singleScriptTimeLineEdit->text().toInt(), ui->loadingTimeLineEdit->text().toInt());
    }
    else{
        // 强制停止逻辑
        if(ssjjMainThread->isFinished() == false){
            if(ssjjMainThread->currentThread != new QThread()){
                ssjjMainThread->stopThread();
            }
            ssjjMainThread->terminate();
        }
        writeRemindInfo("<p><span style=\"color:lightgreen;\"><b>全部任务已执行完毕</b></span></p><br>");
        writeRemindInfo("<p><span style=\"color:lightgreen;\"><b>脚本运行已结束</b></span></p><br>");
    }
}

void SubSSJJWidget::receiveFatalError()
{
    on_endPushButton_clicked();
}

void SubSSJJWidget::clearRow(QTableWidgetItem *item)
{
    int row = item->row();  // 获取当前点击的行
    ui->taskTableWidget->removeRow(row);  // 删除该行
}
