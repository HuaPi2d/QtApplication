#include "downloadlistwidget.h"
#include "ui_downloadlistwidget.h"

DownloadListWidget::DownloadListWidget(QWidget *parent)
    : QWidget(parent, Qt::Popup)
    , ui(new Ui::DownloadListWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    layout = new QVBoxLayout(this);
    ui->scrollAreaWidgetContents->setLayout(layout);
    // 使内容自适应 QScrollArea 的宽度
    ui->scrollAreaWidgetContents->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 禁用水平滚动条
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->scrollArea->setWidgetResizable(true);
}

DownloadListWidget::~DownloadListWidget()
{
    delete ui;
}
