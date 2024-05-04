#ifndef SMS_H
#define SMS_H

#include <QObject>
#include <QProcess>
class SMS : public QObject
{  Q_OBJECT

public:
    explicit SMS(QObject *parent = nullptr);

        void sendSms(const QString &to, const QString &message);
        void setTwilioCredentials(const QString &accountSid, const QString &authToken, const QString &twilioPhoneNumber);

    signals:
        void finishedSending();

      private slots:
          void handleSmsResponse();

      private:
          QProcess curlProcess;

          // Your Twilio credentials and phone number
          QString accountSid;
          QString authToken;
          QString twilioPhoneNumber;
};

#endif // SMS_H
