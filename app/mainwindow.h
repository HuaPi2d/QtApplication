#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtAdvancedStylesheet.h>
#include <QSignalMapper>
#include <QButtonGroup>
#include <QMdiSubWindow>
#include "otherwidgets/downloadlistwidget.h"
#include "otherwidgets/singledownloadframe.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void saveSettings();
    void loadSettings();
    void updateTheme();
    void loadDefaultSettings();
    void getRandomSentence();
    void copySentence();
    void changeToWorkingUI();
    void showStateInfo(QString state);
    void checkSubWindows(QMdiSubWindow *window);
    void createMusicSubWindow();
    void createVideoSubWindow();
    void createTextTransitionSubWindow();
    void createRandomLearningSubWindow();
    void createSSJJSubWindow();
    void addDownloadTask(SingleDownloadFrame *singleDownloadFrame);

private:
    Ui::MainWindow *ui;
    acss::QtAdvancedStylesheet* advancedStyleSheet;
    QString appDir;
    QString currentTheme;
    QSignalMapper *signalMapper;
    QButtonGroup *taskButtonList;
    QWidgetList *subWindowList;
    DownloadListWidget *downloadListWidget;

private slots:
    void onThemeActionTriggered();

public: signals:
    void isAnyChildWindow();
    void updateQuickWidgetColor(QColor color);
};
#endif // MAINWINDOW_H
