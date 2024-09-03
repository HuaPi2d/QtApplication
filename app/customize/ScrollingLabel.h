#ifndef SCROLLINGLABEL_H
#define SCROLLINGLABEL_H

#include <QApplication>
#include <QLabel>
#include <QTimer>
#include <QHBoxLayout>

class ScrollingLabel : public QLabel {
    Q_OBJECT

public:
    ScrollingLabel(QWidget *parent = nullptr) : QLabel(parent), currentIndex(0) {
        setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        startTimer(100); // 每100毫秒触发一次定时器
    }

    void setScrollingText(const QString &text) {
        fullText = text;
        if (fontMetrics().horizontalAdvance(fullText) <= width()) {
            // 如果文本长度不超过标签的宽度，直接显示
            setText(fullText);
            currentIndex = 0;
        } else {
            // 如果文本长度超过标签的宽度，设置滚动效果
            setText(fullText.left(width() / fontMetrics().averageCharWidth()));
        }
    }

protected:
    void timerEvent(QTimerEvent *event) override {
        if (fontMetrics().horizontalAdvance(fullText) > width()) {
            currentIndex = (currentIndex + 1) % fullText.length();
            QString visibleText = fullText.mid(currentIndex) + " " + fullText.left(currentIndex);
            setText(visibleText.left(width() / fontMetrics().averageCharWidth()));
        }
    }

private:
    QString fullText;
    int currentIndex;
};

#endif // SCROLLINGLABEL_H
