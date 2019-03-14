#include <QApplication>
#include <schedule.h>
#include <QThread>

#include "serialComm.h"
#include "xddpComm.h"
#include "api/myhelper.h"

/*加载qss文件*/
void setQssContent()
{
    QFile file(":style.qss");
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("Can't open the style sheet file.");
        return;
    }

    QString qss = file.readAll();
    QString paletteColor = qss.mid(20, 7);
    qApp->setPalette(QPalette(QColor(paletteColor)));
    qApp->setStyleSheet(qss);
    file.close();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    myHelper::setTextCode("UTF-8");
    myHelper::setFont("WenQuanYi Micro Hei");

    setQssContent();

    schedule w;

    SerialComm serial_comm;
    QThread serial_thread;
    serial_comm.moveToThread(&serial_thread);
    serial_thread.start();

    QObject::connect(&w,SIGNAL(toSerialData(QByteArray)),&serial_comm,SLOT(writeToSerial(QByteArray)));
    QObject::connect(&serial_comm,SIGNAL(sendSerialData(QByteArray)),&w,SLOT(handleSerialData(QByteArray)));

    xddpComm xddp_comm;
    QThread xddp_thread;
    xddp_comm.moveToThread(&xddp_thread);
    xddp_thread.start();
    QObject::connect(&w,SIGNAL(toXddpData(QByteArray)),&xddp_comm,SLOT(writeToXddp(QByteArray)));
    QObject::connect(&xddp_comm,SIGNAL(sendXddpData(QByteArray)),&w,SLOT(handleXddpData(QByteArray)));

    return a.exec();
}
