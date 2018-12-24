#include "myMessageBox.h"
#include "ui_myMessageBox.h"

myMessageBox *myMessageBox::_instance = 0;

myMessageBox::myMessageBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myMessageBox)
{
    ui->setupUi(this);
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    setGeometry(300,200,400,220);
}

myMessageBox::~myMessageBox()
{
    delete ui;
}

void myMessageBox::setMessage(const QString msg, int type)
{
    ui->btnCancel->setVisible(true);
    ui->btnOk->setVisible(true);
    if (type == BoxInfo) {
        ui->labIcoMain->setStyleSheet("border-image: url(:/image/msg_info.png);");
        ui->btnCancel->setVisible(false);
        ui->btnOk->setVisible(false);
        ui->lab_Title->setText("提示");
        ui->labInfo->setText(msg);
        this->show();
        myHelper::sleep(1500);
        this->hide();
        return;
    } else if (type == BoxQuesion) {
        ui->labIcoMain->setStyleSheet("border-image: url(:/image/msg_question.png);");
        ui->lab_Title->setText("询问");
    } else if (type == BoxError) {
        ui->labIcoMain->setStyleSheet("border-image: url(:/image/msg_error.png);");
        ui->btnCancel->setVisible(false);
        ui->lab_Title->setText("错误");
    }

    ui->labInfo->setText(msg);
    this->setWindowTitle(ui->lab_Title->text());

    this->show();
}

QString myMessageBox::getMessage()
{
    return ui->labInfo->text();
}
