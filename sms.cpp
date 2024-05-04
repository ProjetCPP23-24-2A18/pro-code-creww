#include "sms.h"
#include <QDebug>
void SMS::setTwilioCredentials(const QString &accountSid, const QString &authToken, const QString &twilioPhoneNumber)
{
    this->accountSid = accountSid;
    this->authToken = authToken;
    this->twilioPhoneNumber = twilioPhoneNumber;
}
SMS::SMS(QObject *parent) : QObject(parent)
{
    // Initialize your Twilio credentials and phone number
    authToken = "a66c19bf01ded9e91217bb4f853e3589";
    twilioPhoneNumber = "+12515125980";

    connect(&curlProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &SMS::handleSmsResponse);
}

void SMS::sendSms(const QString &to, const QString &message)
{

        qDebug() << "Before sending SMS...";

        QStringList arguments;
        arguments << "-X" << "POST"
                  << "https://api.twilio.com/2010-04-01/Accounts/" + accountSid + "/Messages.json"
                  << "--data-urlencode" << "To=" + to
                  << "--data-urlencode" << "From=" + twilioPhoneNumber
                  << "--data-urlencode" << "Body=" + message
                  << "-u" << accountSid + ":" + authToken;

        // Start the curl process
        curlProcess.start("curl", arguments);

        // Wait for the process to finish (with a timeout to prevent infinite waiting)
        if (!curlProcess.waitForFinished(30000)) {
            // If the process didn't finish within the timeout, handle the error
            qDebug() << "Error: Timed out waiting for curl process to finish.";
        }

        // Check the exit code of the process
        int exitCode = curlProcess.exitCode();
        qDebug() << "Curl process finished with exit code:" << exitCode;

        // Handle the response based on the exit code (you might need to adjust this part)
        if (exitCode == 0) {
            qDebug() << "SMS sent successfully!";
        } else {
            qDebug() << "Error sending SMS. Exit code:" << exitCode;
            qDebug() << "Error details:" << curlProcess.errorString();
        }

        qDebug() << "Sending SMS completed.";
    }


void SMS::handleSmsResponse()
{
    qDebug() << "Handling SMS response...";

    if (curlProcess.exitCode() == 0) {
        qDebug() << "SMS sent successfully!";
    } else {
        qDebug() << "Error sending SMS. Exit code:" << curlProcess.exitCode();
        qDebug() << "Error details:" << curlProcess.readAllStandardError();
    }

    // Emit the signal to indicate that SMS sending is finished
    emit finishedSending();

}
