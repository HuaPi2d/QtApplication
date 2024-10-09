#include "randomlearningwidget.h"
#include "ui_randomlearningwidget.h"

RandomLearningWidget::RandomLearningWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RandomLearningWidget)
{
    ui->setupUi(this);

    // 设置小数位数为 0，只能输入整数
    ui->doubleSpinBox->setDecimals(0);

    // 设置最小值和最大值
    ui->doubleSpinBox->setMinimum(10);  // 设置下限为 1
    ui->doubleSpinBox->setMaximum(30); // 设置上限为 100
}

RandomLearningWidget::~RandomLearningWidget()
{
    delete ui;
}

void RandomLearningWidget::closeEvent(QCloseEvent *event)
{
    emit widgetClosed();
}

void RandomLearningWidget::on_closePushButton_clicked()
{
    this->close();
}


void RandomLearningWidget::on_openSourceFirstPushButton_clicked()
{

}


void RandomLearningWidget::on_fontComboBox_currentFontChanged(const QFont &f)
{
    QFont font = f;
    font.setPointSize(ui->doubleSpinBox->value());
    ui->textEdit->setFont(font);
}


void RandomLearningWidget::on_doubleSpinBox_valueChanged(double arg1)
{
    QFont font = ui->textEdit->font();
    font.setPointSize(arg1);
    ui->textEdit->setFont(font);
}

