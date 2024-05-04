#include "personnel.h"
#include "connection.h"
#include "personnellist.h"




Personnel::Personnel()
    : id(""), name(""), lastName(""), dateOfBirth(QDate::currentDate()),
       address(""), phone(""),
       email(""), function(""),ImagePath()
{
}

Personnel::Personnel(const QString &id, const QString &name, const QString &lastName, const QDate &dob,
               const QString &address, const QString &phone, const QString &email,
               const QString &function,const QString &imagePath)
    : id(id), name(name), lastName(lastName), dateOfBirth(dob), address(address),
      phone(phone), email(email), function(function), ImagePath(imagePath)
{
}
