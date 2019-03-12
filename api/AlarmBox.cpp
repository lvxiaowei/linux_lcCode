#include "AlarmBox.h"
#include "ui_AlarmBox.h"
#include <QScrollBar>
#include <QDebug>
AlarmBox::AlarmBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlarmBox)
{
    ui->setupUi(this);

    updateAlarm();
}

AlarmBox::~AlarmBox()
{
    delete ui;
}

/*新增告警*/
void AlarmBox:: adAlarm(QString str)
{
    m_lstAlarm.append(str);
    updateAlarm();
}

/*告警移除*/
void AlarmBox::rmAlarm(QString str)
{
    m_lstAlarm.removeOne(str);
    updateAlarm();
}

/*更新告警窗*/
void AlarmBox::updateAlarm()
{
    if(m_lstAlarm.isEmpty())
    {
        this->hide();
        return;
    }

    ui->m_AlarmContent->clear();

    ui->m_AlarmContent->setText(m_lstAlarm.join("\n"));
    ui->m_AlarmContent->moveCursor(QTextCursor::End);
    this->show();
}
