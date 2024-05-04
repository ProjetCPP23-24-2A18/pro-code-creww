#include "connection.h"
#include "personnellist.h"
#include "personnel.h"
#include <QDebug>

Connection::Connection()
{

}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("system");
    db.setPassword("esprit@2024");

    if (db.open()) {
        test = true;
        qDebug() << "Database: connection ok";
    } else {
        qDebug() << "Connection failed:" << db.lastError();
    }

    return test;
}
