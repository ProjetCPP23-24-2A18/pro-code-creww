#ifndef PACK_H
#define PACK_H
#include <QString>
#include <QDialog>
#include <QString>
#include <QTableView>
#include <QDate>
#include <QtSql/QSqlQueryModel>
#include <QSqlError>
class pack
{
public:
    void setnom(QString n);
    void setservices(QString n);
    void setdescription(QString n);
    void setprix(QString n);
    void setstatus(QString n);
    pack(){};
    pack(int id, QString nom, QString services,QString description,QString prix,QString duree);
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int id);

private:
    QString nom,services,description,prix,duree;
    int id ;
};

#endif // PACK_H
