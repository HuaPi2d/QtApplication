#ifndef DELETEMUSICDIALOG_H
#define DELETEMUSICDIALOG_H

#include <QDialog>

namespace Ui {
class DeleteMusicDialog;
}

class DeleteMusicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteMusicDialog(QWidget *parent = nullptr);
    ~DeleteMusicDialog();

public:
    Ui::DeleteMusicDialog *ui;
};

#endif // DELETEMUSICDIALOG_H
