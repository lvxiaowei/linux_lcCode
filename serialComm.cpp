#include "serialComm.h"

SerialComm::SerialComm(QObject *parent) : QObject(parent)
{
    comm =new QSerialPort();
    initComm();

    connect(comm,SIGNAL(readyRead()),this,SLOT(handleSerialRaw()));

}

void SerialComm::initComm()
{
    comm->setPortName("ttymxc4");
    comm->setBaudRate(QSerialPort::Baud115200);
    comm->setDataBits(QSerialPort::Data8);
    comm->setParity(QSerialPort::NoParity);
    comm->setStopBits(QSerialPort::OneStop);
    comm->setFlowControl(QSerialPort::NoFlowControl);

    if(!comm->open(QIODevice::ReadWrite))
    {
        qDebug()<<"init field";
        return;
    }
}

void SerialComm::handleSerialRaw()
{
    QByteArray ba = comm->readAll();

    emit this->sendSerialData(ba);
}

void SerialComm::writeToSerial(QByteArray ba)
{
    comm->write(ba,ba.size());
}

SerialComm::~SerialComm()
{
    comm->close();
    exit(0);
}
