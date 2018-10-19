#ifndef MYMESSAGEBOX_H
#define MYMESSAGEBOX_H

#include <QMutex>
#include <QWidget>
#include "api/myhelper.h"

/*弹出框type*/
enum windowTYpe
{
    BoxInfo = 0,      //提示弹出框
    BoxError,         //错误弹出框
    BoxQuesion        //问题弹出框
};

namespace Ui {
class myMessageBox;
}

class myMessageBox : public QWidget
{
    Q_OBJECT

public:
    static myMessageBox *getInstance()
    {
        static QMutex mutex;
        if (!_instance)
        {
            QMutexLocker locker(&mutex);
            if (!_instance)
            {
                _instance = new myMessageBox();
            }
        }
        return _instance;
    }

    void setMessage(const QString msg, int type);
    QString getMessage();
private:
    Ui::myMessageBox *ui;

    explicit myMessageBox(QWidget *parent = 0);
    ~myMessageBox();

    static myMessageBox *_instance;
};

#endif // MYMESSAGEBOX_H
