#ifndef SOCKETCOMM_H
#define SOCKETCOMM_H

#include <QObject>
#include <QSocketNotifier>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <QDebug>

class xddpComm : public QObject
{
    Q_OBJECT
public:
    explicit xddpComm(QObject *parent = nullptr);

    QSocketNotifier *m_activeNotifier;
    QSocketNotifier *m_errorNotifier;

    void sendErrFlg();
    void dataInit();

    void testInterface(QByteArray data);  //测试接口
signals:
    void sendXddpData(QByteArray data);

public slots:
    void readSocketData(int);
    void errorSocket(int);
    void writeToXddp(QByteArray data);
private:
    int fb;
};

#endif // SOCKETCOMM_H
