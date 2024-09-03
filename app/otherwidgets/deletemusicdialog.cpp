#include "deletemusicdialog.h"
#include "ui_deletemusicdialog.h"

#include <QPushButton>

DeleteMusicDialog::DeleteMusicDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeleteMusicDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("删除本地歌曲");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
}

DeleteMusicDialog::~DeleteMusicDialog()
{
    delete ui;
}
