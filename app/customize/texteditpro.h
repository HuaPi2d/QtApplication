#ifndef TEXTEDITPRO_H
#define TEXTEDITPRO_H

#include <QObject>
#include <QTextEdit>
#include <QKeyEvent>

class TextEditPro : public QTextEdit {
    Q_OBJECT

public:
    TextEditPro(QWidget *parent = nullptr) : QTextEdit(parent) {}

protected:
    // 重载 keyPressEvent 函数，监听键盘事件
    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_B && event->modifiers() & Qt::ControlModifier) {
            // 获取当前光标
            QTextCursor cursor = this->textCursor();
            if (cursor.hasSelection()) {
                // 获取当前字符格式
                QTextCharFormat format = cursor.charFormat();
                // 切换字体加粗样式
                if (format.fontWeight() == QFont::Bold) {
                    format.setFontWeight(QFont::Normal);
                } else {
                    format.setFontWeight(QFont::Bold);
                }
                // 设置选中区域的字符格式
                cursor.mergeCharFormat(format);
            }
            return; // 防止事件继续传递
        }
        // 传递其他按键事件
        QTextEdit::keyPressEvent(event);
    }
};

#endif // TEXTEDITPRO_H
