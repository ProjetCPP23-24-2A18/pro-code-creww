#include "arduinosk.h"
#include <QDebug>

Arduinosk::Arduinosk(QObject *parent) : QObject(parent), serial(new QSerialPort(this))
{
    connect(serial, &QSerialPort::readyRead, this, &Arduinosk::onReadyRead);
}

Arduinosk::~Arduinosk()
{
    if (serial->isOpen())
    serial->close();
}

void Arduinosk::initPort(const QString &portName)
{
    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (!serial->open(QIODevice::ReadWrite))
    {
        qWarning() << "Could not open serial port:" << portName;
    }
}

void Arduinosk::onReadyRead()
{
    static QString buffer;
    buffer += serial->readAll();
    if (buffer.contains("\r\n"))
    {
        auto messages = buffer.split("\r\n");
        for (int i = 0; i < messages.count() - 1; ++i)
        {
            processTheCardData(messages[i].toLatin1());
        }
        buffer = messages.last();
    }
}

void Arduinosk::processTheCardData(const QByteArray &data)
{
    QString cardID = QString::fromLatin1(data).trimmed();
    if (!cardID.isEmpty())
    {
        emit cardScanned(cardID);
    }
}
