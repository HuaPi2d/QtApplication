#ifndef LOCALMUSICLISTVIEW_H
#define LOCALMUSICLISTVIEW_H

#include <QListView>
#include <QStandardItemModel>
#include <QMouseEvent>
#include <QMenu>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QAction>

class LocalMusicListView : public QListView {
    Q_OBJECT

public:
    LocalMusicListView(QWidget *parent = nullptr) : QListView(parent) {}

public: signals:
    void deleteMusic(QModelIndex index);
    void modifyMusic(QModelIndex index);

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::RightButton) {
            QModelIndex index = indexAt(event->pos());
            if (index.isValid()) {
                QMenu menu(this);
                QAction *deleteAction = new QAction(this);
                QAction *modifyAction = new QAction(this);
                deleteAction->setText("删除");
                deleteAction->setIcon(QIcon(":/icon/resources/icons/delete.svg"));
                menu.addAction(deleteAction);
                modifyAction->setText("修改歌曲信息");
                modifyAction->setIcon(QIcon(":/icon/resources/icons/PhNotePencilBold.svg"));
                menu.addAction(modifyAction);
                connect(deleteAction, &QAction::triggered, this, [=](){
                    emit deleteMusic(index);
                });
                connect(modifyAction, &QAction::triggered, this, [=](){
                    emit modifyMusic(index);
                });
                menu.exec(event->globalPos());
                //showFloatingWindow(event->globalPos(), index);
            }
        }
        QListView::mousePressEvent(event);
    }
};

#endif // LOCALMUSICLISTVIEW_H
