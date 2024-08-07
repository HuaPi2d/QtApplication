#ifndef SUBVIDEOWIDGET_H
#define SUBVIDEOWIDGET_H

#include <QWidget>

namespace Ui {
class SubVideoWidget;
}

class SubVideoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubVideoWidget(QWidget *parent = nullptr);
    ~SubVideoWidget();

    void saveSettings();
    void loadSettings();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::SubVideoWidget *ui;

public: signals:
    void widgetClosed();
    void sendStateInfo(QString state);
};

#endif // SUBVIDEOWIDGET_H
