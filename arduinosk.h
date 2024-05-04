#ifndef ARDUINOSK_H
#define ARDUINOSK_H

#include <QObject>
#include <QSerialPort>

class Arduinosk : public QObject
{
    Q_OBJECT

    public:
        explicit Arduinosk(QObject *parent = nullptr);
        ~Arduinosk();
        void initPort(const QString &portName);

    signals:
        void cardScanned(const QString &cardID);

    private slots:
        void onReadyRead();

    private:
        QSerialPort *serial;
        void processTheCardData(const QByteArray &data);
};

#endif // ARDUINOSK_H
