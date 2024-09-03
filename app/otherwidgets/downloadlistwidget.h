#ifndef DOWNLOADLISTWIDGET_H
#define DOWNLOADLISTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

namespace Ui {
class DownloadListWidget;
}

class DownloadListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadListWidget(QWidget *parent = nullptr);
    ~DownloadListWidget();

public:
    Ui::DownloadListWidget *ui;
    QVBoxLayout *layout;

protected:
    void focusOutEvent(QFocusEvent *event) override {
        QWidget::focusOutEvent(event);
        // 当失去焦点时隐藏窗口
        hide();
    }
};

#endif // DOWNLOADLISTWIDGET_H
