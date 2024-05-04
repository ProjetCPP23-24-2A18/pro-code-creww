#ifndef RDV1_H
#define RDV1_H
#include "QString"
#include<QString>
#include<QDate>
#include<QSqlQueryModel>
#include<QDate>

class rdv1 {
public:
    rdv1();
   rdv1(int, QString, QString, QString,int,QString,QString);

    int getID_MATERIELS();
    QString getDESIGNATION();
    QString getEMPLACEMENT();
    QString getDATE_M();
    int getPRIX_M();
    QString getFOURNISSEUR();
    QString getETAT();



    void setID_MATERIELS(int);
    void setDESIGNATION(QString);
    void setEMPLACEMENT(QString);
    void setDATE_M(QString);
    void setPRIX_M(int);
    void setFOURNISSEUR(QString);
    void setETAT(QString);


    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool checkIfmatExists(int);
bool modifierE(int ID_MATERIELS);

private:
    int ID_MATERIELS,PRIX_M;
    QString DESIGNATION,EMPLACEMENT, DATE_M,FOURNISSEUR,ETAT;
};

#endif // RDV1_H
