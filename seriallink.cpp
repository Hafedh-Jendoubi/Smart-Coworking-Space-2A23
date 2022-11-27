#include "seriallink.h"

seriallink::seriallink(QObject *parent) : QObject(parent)
{
    serial.setPortName("COM8");
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    connect(&serial, &QSerialPort::readyRead, this, &seriallink::newData);
}

seriallink::~seriallink()
{
    closeConnection();
}

void seriallink::openConnection()
{
    if(!serial.open(QIODevice::ReadWrite))
        qDebug() << "Connexion impossible";
    else
        qDebug() << "Connexion ok";
}

void seriallink::write(const char* messageToWrite)
{
    serial.write(messageToWrite);
}

void seriallink::closeConnection()
{
    serial.close();
}

bool seriallink::isOpen()
{
    return serial.isOpen();
}

bool seriallink::isWritable()
{
    return serial.isWritable();
}

void seriallink::newData()
{
    emit gotNewData(serial.readAll());
}
