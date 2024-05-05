#include "personnellist.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include "personnel.h"
#include "mainwindow.h"
#include "mainwindow1.h"

#include "memberlist.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  
    personnellist w;
    MainWindow b;
    MainWindow1 f;
    memberlist t;
    Connection c;
    bool test= c.createconnect();
    if(test)
    {
        w.show();
         b.show();
          f.show();
          t.show();
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
