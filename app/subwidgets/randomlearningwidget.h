#ifndef RANDOMLEARNINGWIDGET_H
#define RANDOMLEARNINGWIDGET_H

#include <QWidget>
#include <QFont>

namespace Ui {
class RandomLearningWidget;
}

class RandomLearningWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RandomLearningWidget(QWidget *parent = nullptr);
    ~RandomLearningWidget();

public: signals:
    void widgetClosed();
    void sendStateInfo(QString state);

private slots:
    void on_closePushButton_clicked();

    void on_openSourceFirstPushButton_clicked();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_doubleSpinBox_valueChanged(double arg1);

private:
    Ui::RandomLearningWidget *ui;

    void closeEvent(QCloseEvent *event);
};

#endif // RANDOMLEARNINGWIDGET_H
