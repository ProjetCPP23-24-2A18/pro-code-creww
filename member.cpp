#include "member.h"
#include "connection.h"
#include "memberlist.h"



Member::Member() :
    id(""), name(""), lastName(""), dateOfBirth(QDate::currentDate()),
    address(""), phone(""), email(""), rdv(QDate::currentDate()) ,
    imagePath("")  // Initialize imagePath in your constructor
{
}

Member::Member(const QString &id, const QString &name, const QString &lastName, const QDate &dateOfBirth,
               const QString &address, const QString &phone, const QString &email, const QDate &rdv , const QString &imagePath)
    : id(id), name(name), lastName(lastName), dateOfBirth(dateOfBirth),address(address), phone(phone), email(email), rdv(rdv), imagePath(imagePath) // Initializer list includes imagePath
{
    // Constructor body, if any
}

