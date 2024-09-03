#ifndef SUBVIDEOWIDGET_H
#define SUBVIDEOWIDGET_H

#include "ui_subvideowidget.h"
#include <QWidget>
#include <QQuickWidget>
#include <QQuickItem>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QQmlContext>

namespace Ui {
class SubVideoWidget;
}

class SubVideoWidget : public QWidget
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event) override;

public:
    explicit SubVideoWidget(QWidget *parent = nullptr);
    ~SubVideoWidget();

    void saveSettings();
    void loadSettings();
    void searchVideo();
    void searchByBiliBili();

public slots:
    void updateQuickWidgetColor(QColor color);

private:
    Ui::SubVideoWidget *ui;

public: signals:
    void widgetClosed();
    void sendStateInfo(QString state);
};

#endif // SUBVIDEOWIDGET_H
