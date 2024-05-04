#ifndef PERSONNEL_H
#define PERSONNEL_H
#include <QString>
#include <QDate>


class Personnel
{
private:
    QString id;
    QString name;
    QString lastName;
    QDate dateOfBirth;
    QString address;
    QString phone;
    QString email;
    QString function;

    QString ImagePath;
    void updateEmployeePhotoPath(int personnelId, const QString &ImagePath);


public:
    Personnel();
    Personnel(const QString &id, const QString &name, const QString &lastName, const QDate &dob,
           const QString &address, const QString &phone, const QString &email,
           const QString &function,const QString &ImagePath);


    QString getId() const { return id; }
    QString getName() const { return name; }
    QString getLastName() const { return lastName; }
    QDate getDateOfBirth() const { return dateOfBirth; }

    QString getAddress() const { return address; }
    QString getPhone() const { return phone; }
    QString getEmail() const { return email; }
    QString getFunction() const { return function; }

    QString getImagePath() const { return ImagePath; }
};

#endif // PERSONNEL_H
