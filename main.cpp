#include "personnellist.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include "personnel.h"
#include "mainwindow.h"
#include "mainwindow1.h"
#include "mainwindow2.h"
#include "memberlist.h"
#include "homepage.h"
#include "login.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection c;
    login l;

    bool test=c.createconnect();
    if(test)
    {

       l.show();
        QMessageBox::information(nullptr, QObject::tr("database is open"),
                    QObject::tr("connection successful.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                    QObject::tr("connection failed.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
    return a.exec();
}
