#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "subwindow/subvideowindow.h"
#include "universal/getnetwork.h"
#include "subwindow/submusicwindow.h"

#include <QSettings>
#include <QDebug>
#include <QFontDialog>
#include <QClipboard>
#include <QToolBar>


#define _STR(x) #x
#define STRINGIFY(x) _STR(x)


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* 获取程序运行路径 */
    appDir = qApp->applicationDirPath();  // request/build/Desktop_Qt_6_7_0_MinGW_64_bit-Release/app/release
    QString stylesDir = STRINGIFY(STYLES_DIR);  // STYLES_DIR 在.pro文件中定义

    /* 声明样式表对象，设定父对象 */
    advancedStyleSheet = new acss::QtAdvancedStylesheet;
    advancedStyleSheet->setParent(this);

    /* 设定样式表参数 */
    advancedStyleSheet->setStylesDirPath(stylesDir);  // 主题路径 可用 appDir + "/../styles"代替 request/app/../styles == request/styles
    advancedStyleSheet->setOutputDirPath(appDir + "/output"); // request/build/Desktop_Qt_6_7_0_MinGW_64_bit-Release/app/release/ouput
    advancedStyleSheet->setCurrentStyle("qt_material");   // 设定风格
    currentTheme = "light_blue";  // 设定主题

    /* 更新界面 */
    advancedStyleSheet->updateStylesheet();  // 设定后更新主题
    qApp->setStyleSheet(advancedStyleSheet->styleSheet());

    /* 实现主题更换效果 */
    for(QAction *action : ui->menu_styles->actions()){
        connect(action, &QAction::triggered, this, &MainWindow::onThemeActionTriggered);
    }

    /* 读取用户配置 */
    loadSettings();

    /* 更新好文好句 */
    getRandomSentence();

    /* 隐藏工作区，任务栏，禁用返回按钮 */
    ui->processToolBar->hide();
    ui->mutiWindowWidget->hide();

    /* 任务簇 */
    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(activateSubWindow(QWidget*)));
    taskButtonList = new QButtonGroup(this);
    taskButtonList->setExclusive(true);

    /* 连接信号和槽 */
    connect(ui->reset_theme_action, &QAction::triggered, this, &MainWindow::loadDefaultSettings);  // 恢复默认配置
    connect(ui->video_pushButton, &QPushButton::clicked, this, &MainWindow::changeToVideoUI);
    connect(ui->video_action, &QAction::triggered, this, &MainWindow::changeToVideoUI); // 开启视频解析窗口
    connect(ui->music_pushButton, &QPushButton::clicked, this, &MainWindow::changeToMusicUI);
    connect(ui->music_action, &QAction::triggered, this, &MainWindow::changeToMusicUI); // 开启音乐盒
    connect(ui->home_action, &QAction::triggered, this, [=](){
        ui->mainWidget->show();
        ui->mutiWindowWidget->hide();
        ui->processToolBar->hide();
    });
    // connect(this, &MainWindow::isAnyChildWindow, this, [=](){
    //     if(ui->functionArea->subWindowList().count() == 1){
    //         ui->mainWidget->show();
    //         ui->mutiWindowWidget->hide();
    //         ui->processToolBar->hide();
    //     }
    // });
    connect(ui->getRandomSentencePushButton, &QPushButton::clicked, this, &MainWindow::getRandomSentence); // 获取随机好文好句
    connect(ui->copySentencePushButton, &QPushButton::clicked, this, &MainWindow::copySentence);  // 复制到剪切板
    connect(ui->processBarControlPushButton, &QPushButton::clicked, this, [=](){
        if(ui->processToolBar->isHidden() == true)
        {
            ui->processToolBar->show();
            ui->processBarControlPushButton->setIcon(QIcon(":/icon/resources/icons/rightRight.svg"));
        }
        else
        {
            ui->processToolBar->hide();
            ui->processBarControlPushButton->setIcon(QIcon(":/icon/resources/icons/leftLeft.svg"));
        }
    });
    connect(ui->functionArea, &QMdiArea::subWindowActivated, this, &MainWindow::checkSubWindows);
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::onThemeActionTriggered()
{
    auto Action = qobject_cast<QAction*>(sender());
    currentTheme = Action->text();
    updateTheme();
}

void MainWindow::saveSettings()
{
    /* 声明对象 */
    QSettings setting(appDir + "/userSettings.ini", QSettings::IniFormat);

    /* 写入配置 */
    setting.beginGroup("window");
    setting.setValue("theme", currentTheme); // 写入主题配置
    setting.setValue("width", this->width());
    setting.setValue("height", this->height());
    setting.setValue("isMaximized", this->isMaximized()); // 窗口大小
    setting.endGroup();

    setting.beginGroup("text");
    setting.setValue("randomSentence", ui->randomSentenceLabel->text());
    setting.endGroup();
}

void MainWindow::loadSettings()
{
    /* 声明对象 */
    QSettings setting(appDir + "/userSettings.ini", QSettings::IniFormat);

    /* 读取配置 */
    setting.beginGroup("window");
    currentTheme = setting.value("theme").toString();
    updateTheme(); // 读取主题设置
    this->resize(QSize(setting.value("width").toInt(), setting.value("height").toInt()));
    if(setting.value("isMaximized") == "true")
    {
        this->showMaximized();
    }  // 改变窗口大小
    setting.endGroup();

    setting.beginGroup("text");
    ui->randomSentenceLabel->setText(setting.value("randomSentence").toString());
    setting.endGroup();
}

void MainWindow::loadDefaultSettings()
{
    /* 读取默认配置 */
    currentTheme = "light_blue";
    updateTheme(); // 读取主题设置
}

void MainWindow::changeToVideoUI()
{
    /* 隐藏主界面，显示功能界面 */
    if(ui->mainWidget->isHidden() == false)
    {
        changeToWorkingUI();
    }

    /* 刷新视频界面 */
    SubVideoWindow *subVideoWindow = new SubVideoWindow(this);
    qDebug() << subVideoWindow;
    subVideoWindow->setParent(this);
    ui->functionArea->addSubWindow(subVideoWindow);
    subVideoWindow->showMaximized();

    /* 创建任务栏按钮 */
    QPushButton *taskButton = new QPushButton(subVideoWindow);
    taskButton->setIcon(QIcon(":/icon/resources/icons/MaterialSymbolsMediaLinkOutline.svg"));
    taskButton->setCheckable(true);
    taskButton->setChecked(true);
    taskButton->setStyleSheet("QPushButton {border: none; background-color:rgb(231, 234, 248);}QPushButton:hover {background-color:rgb(249, 239, 241);}QPushButton:checked {background-color:grey;}");
    ui->processToolBar->addWidget(taskButton);
    this->taskButtonList->addButton(taskButton);

    /* 将任务栏按钮和子窗口连接起来 */
    connect(taskButton, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
    signalMapper->setMapping(taskButton, subVideoWindow);

    connect(taskButton, &QPushButton::clicked, this, [=](){
        subVideoWindow->showMaximized();
    });

    /* 连接信号和槽 */
    connect(subVideoWindow, &SubVideoWindow::destroyed, this, &MainWindow::isAnyChildWindow); // 窗口销毁时判断是否回到主界面
    connect(subVideoWindow, &SubVideoWindow::destroyed, [this, taskButton]() {
        taskButton->deleteLater();
        qDebug() << taskButtonList->buttons();
    });  // 窗口销毁时，销毁任务栏按钮
    connect(subVideoWindow, &SubVideoWindow::snedStateInfo, this, &MainWindow::showStateInfo);
}

void MainWindow::updateTheme()
{
    this->advancedStyleSheet->setCurrentTheme(currentTheme);
    this->advancedStyleSheet->updateStylesheet();
    qApp->setStyleSheet(advancedStyleSheet->styleSheet());
}

void MainWindow::changeToMusicUI()
{
    /* 隐藏主界面，显示功能界面 */
    if(ui->mainWidget->isHidden() == false)
    {
        changeToWorkingUI();
    }

    /* 创建音乐子界面对象 */
    SubMusicWindow *subMusicWindow = new SubMusicWindow(this);
    qDebug() << subMusicWindow;
    subMusicWindow->setParent(this);
    ui->functionArea->addSubWindow(subMusicWindow);
    subMusicWindow->showMaximized();

    /* 创建任务栏按钮 */
    QPushButton *taskButton = new QPushButton(subMusicWindow);
    taskButton->setIcon(QIcon(":/icon/resources/icons/MaterialSymbolsGenresOutline.svg"));
    taskButton->setCheckable(true);
    taskButton->setChecked(true);
    taskButton->setStyleSheet("QPushButton {border: none; background-color:rgb(231, 234, 248);}QPushButton:hover {background-color:rgb(249, 239, 241);}QPushButton:checked {background-color:grey;}");
    ui->processToolBar->addWidget(taskButton);
    this->taskButtonList->addButton(taskButton);

    /* 将任务栏按钮和子窗口连接起来 */
    connect(taskButton, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
    signalMapper->setMapping(taskButton, subMusicWindow);

    connect(taskButton, &QPushButton::clicked, this, [=](){
        subMusicWindow->showMaximized();
    });

    /* 连接信号和槽 */
    connect(subMusicWindow, &SubMusicWindow::destroyed, this, &MainWindow::isAnyChildWindow); // 窗口销毁时判断是否回到主界面
    connect(subMusicWindow, &SubMusicWindow::destroyed, [this, taskButton]() {
        taskButton->deleteLater();
    });  // 窗口销毁时，销毁任务栏按钮
    connect(subMusicWindow, &SubMusicWindow::snedStateInfo, this, &MainWindow::showStateInfo);
}

void MainWindow::getRandomSentence()
{
    /* 声明对象 */
    GetNetWork *getSentence = new GetNetWork(this);

    /* 连接信号与槽 */
    connect(getSentence, &GetNetWork::sendData, this, [=](QByteArray data){
        ui->randomSentenceLabel->setText(data);
    });
    connect(getSentence, &GetNetWork::sendError, this, [=](){
        showStateInfo("请求出现错误，更新失败");
    });

    /* 发送请求 */
    getSentence->getData(QUrl("https://tenapi.cn/v2/yiyan"));
}

void MainWindow::copySentence()
{
    /* 声明对象 */
    QClipboard *clipBoard = QApplication::clipboard();

    /* 写入剪切板 */
    clipBoard->setText(ui->randomSentenceLabel->text());
}

void MainWindow::changeToWorkingUI()
{
    /* 隐藏主界面，显示功能界面 */
    ui->mainWidget->hide();
    ui->mutiWindowWidget->show();

    /* 更改按钮图标 */
    if(ui->processToolBar->isHidden() == true)
    {
        ui->processBarControlPushButton->setIcon(QIcon(":/icon/resources/icons/leftLeft.svg"));
    }
    else
    {
        ui->processBarControlPushButton->setIcon(QIcon(":/icon/resources/icons/rightRight.svg"));
    }
}

void MainWindow::showStateInfo(QString state)
{
    ui->statusbar->showMessage(state, 3000);
}

void MainWindow::checkSubWindows(QMdiSubWindow *window) {
    if (window == nullptr)
    {
        ui->mainWidget->show();
        ui->processToolBar->hide();
        ui->mutiWindowWidget->hide();
    }
    else {
        window->showMaximized();
    }
}
