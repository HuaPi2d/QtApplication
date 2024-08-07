#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtAdvancedStylesheet.h>
#include <QSignalMapper>
#include <QButtonGroup>
#include <QMdiSubWindow>

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
    void changeToVideoUI();
    void changeToMusicUI();
    void getRandomSentence();
    void copySentence();
    void changeToWorkingUI();
    void showStateInfo(QString state);
    void checkSubWindows(QMdiSubWindow *window);

private:
    Ui::MainWindow *ui;
    acss::QtAdvancedStylesheet* advancedStyleSheet;
    QString appDir;
    QString currentTheme;
    QSignalMapper *signalMapper;
    QButtonGroup *taskButtonList;

private slots:
    void onThemeActionTriggered();

public: signals:
    void isAnyChildWindow();
};
#endif // MAINWINDOW_H
