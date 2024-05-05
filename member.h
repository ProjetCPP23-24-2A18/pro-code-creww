#ifndef MEMBER_H
#define MEMBER_H
#include <QString>
#include <QDate>


class Member
{
private:
    QString id;
    QString name;
    QString lastName;
    QDate dateOfBirth;
    QString address;
    QString phone;
    QString email;
    QDate rdv;

    QString imagePath;


public:
    Member();
    Member(const QString &id, const QString &name, const QString &lastName, const QDate &dateOfBirth,
               const QString &address, const QString &phone, const QString &email, const QDate &rdv, const QString &imagePath); // Notice the imagePath


    QString getId() const { return id; }
    QString getName() const { return name; }
    QString getLastName() const { return lastName; }
    QDate   getDateOfBirth() const { return dateOfBirth; }
    QString getAddress() const { return address; }
    QString getPhone() const { return phone; }
    QString getEmail() const { return email; }
    QDate   getRdv() const { return rdv; }
    void    setImagePath(const QString &path) { imagePath = path; };
    QString getImagePath() const { return imagePath; }

};

#endif // MEMBER_H
