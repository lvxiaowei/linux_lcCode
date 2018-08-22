#include "xddpComm.h"
#include <sys/socket.h>
#include <sys/un.h>
#include<QFile>
#include <unistd.h>

xddpComm::xddpComm(QObject *parent) : QObject(parent)
{
    dataInit();
}

void xddpComm::sendErrFlg()
{
    qDebug()<<"get error message!"<< fb;
}

void xddpComm::dataInit()
{
    fb = open("/dev/rtp1",O_RDWR | O_NONBLOCK);  //读写，非阻态方式打开；

    if(fb<0)
    {
        sendErrFlg();
    }

    m_activeNotifier=new QSocketNotifier(fb,QSocketNotifier::Read,this);              //创建QSocketNotifier的对象；
    connect(m_activeNotifier,SIGNAL(activated(int)),this,SLOT(readSocketData(int)));

    m_errorNotifier=new QSocketNotifier(fb,QSocketNotifier::Exception,this);          //创建QSocketNotifier的对象；
    connect(m_errorNotifier,SIGNAL(activated(int)),this,SLOT(errorSocket(int)));
}

void xddpComm::readSocketData(int)
{
    char *read_buf;

    int read_buf_size=2000;
    read_buf=new char[read_buf_size];
    memset(read_buf,0,read_buf_size);

    read(fb, read_buf,20000);


    qDebug()<<"get replay message!"<< read_buf;

    emit sendXddpData(read_buf);
}

void xddpComm::errorSocket(int)
{

}

void xddpComm::writeToXddp(QByteArray data)
{

    int size=data.size();
    char *write_buf=data.data();

    write(fb,write_buf,size);
}


void xddpComm::testInterface(QByteArray data)
{
    int size=data.size();
    char *write_buf=data.data();

    write(fb,write_buf,size);
}
