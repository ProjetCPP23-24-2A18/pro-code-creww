#ifndef RDV_H
#define RDV_H
#include "QString"
#include<QString>
#include<QDate>
#include<QSqlQueryModel>
#include<QDate>

class rdv {
public:
    rdv();
   rdv(int, QString, QString, QString);

    int getIdRdv();
    QString getDateRdv();
    QString getDuree();
    QString getTypeSoin();

    void setIdRdv(int);
    void setDateRdv(QString);
    void setDuree(QString);
    void setTypeSoin(QString);

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool checkIfRdvExists(int);
bool modifierE(int id_rdv);

private:
    int id_rdv ;
    QString date_rdv,duree, type_soin;
};

#endif // RDV_H
