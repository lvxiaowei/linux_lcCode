#ifndef ALARMBOX_H
#define ALARMBOX_H

#include <QDialog>

namespace Ui {
class AlarmBox;
}

class AlarmBox : public QDialog
{
    Q_OBJECT

public:
    explicit AlarmBox(QWidget *parent = 0);
    ~AlarmBox();

    void adAlarm(QString str);
    void rmAlarm(QString str);
private:
    Ui::AlarmBox *ui;

    QStringList m_lstAlarm;

    void updateAlarm();
};

#endif // ALARMBOX_H
