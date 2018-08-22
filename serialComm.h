#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QtSerialPort/QSerialPort>

class SerialComm : public QObject
{
    Q_OBJECT
public:
    explicit SerialComm(QObject *parent = nullptr);
    ~SerialComm();

private:
    QSerialPort *comm;
    void initComm();

signals:
    void sendSerialData(QByteArray);

public slots:
    void writeToSerial(QByteArray);

private slots:
    void handleSerialRaw();

};

#endif // SERIALCOMM_H
