#include "rdv1.h"
#include <QSqlQuery>
#include <QtDebug>
#include<QObject>
#include <QIntValidator>
#include<QSqlError>
#include<QString>
#include<QComboBox>
rdv1::rdv1() {
    ID_MATERIELS = 0;
    DESIGNATION = "";
    EMPLACEMENT = "";
    DATE_M = "";
     PRIX_M= 0;
     FOURNISSEUR = "";
     ETAT = "";

}
rdv1::rdv1(int ID_MATERIELS, QString DESIGNATION, QString EMPLACEMENT, QString  DATE_M,int PRIX_M,QString FOURNISSEUR, QString ETAT)
 {
    this->ID_MATERIELS= ID_MATERIELS;
    this->DESIGNATION = DESIGNATION;
    this->EMPLACEMENT = EMPLACEMENT;
    this->DATE_M = DATE_M;
    this->PRIX_M= PRIX_M;
    this->FOURNISSEUR = FOURNISSEUR;
    this->ETAT = ETAT;

}
int rdv1::getID_MATERIELS() { return ID_MATERIELS; }
QString rdv1::getDESIGNATION() { return DESIGNATION; }
QString rdv1::getEMPLACEMENT() { return EMPLACEMENT; }
QString rdv1::getDATE_M() { return DATE_M; }
int rdv1::getPRIX_M() { return PRIX_M; }
QString rdv1::getFOURNISSEUR() { return FOURNISSEUR; }
QString rdv1::getETAT() { return ETAT; }


void rdv1::setID_MATERIELS(int ID_MATERIELS) { this->ID_MATERIELS = ID_MATERIELS; }
void rdv1::setDESIGNATION(QString DESIGNATION) { this->DESIGNATION = DESIGNATION; }
void rdv1::setEMPLACEMENT(QString EMPLACEMENT) { this->EMPLACEMENT = EMPLACEMENT; }
void rdv1::setDATE_M(QString DATE_M) { this->DATE_M = DATE_M; }
void rdv1::setPRIX_M(int PRIX_M) { this->PRIX_M= PRIX_M; }
void rdv1::setFOURNISSEUR(QString FOURNISSEUR) { this->FOURNISSEUR = FOURNISSEUR; }
void rdv1::setETAT(QString ETAT) { this->ETAT = ETAT; }

bool rdv1::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO GS_MATERIELS (ID_MATERIELS, DESIGNATION, EMPLACEMENT, DATE_M, PRIX_M, FOURNISSEUR, ETAT) "
                  "VALUES (:ID_MATERIELS, :DESIGNATION, :EMPLACEMENT, :DATE_M, :PRIX_M, :FOURNISSEUR, :ETAT)");
    query.bindValue(":ID_MATERIELS", ID_MATERIELS);
    query.bindValue(":DESIGNATION", DESIGNATION);
    query.bindValue(":EMPLACEMENT", EMPLACEMENT);
    query.bindValue(":DATE_M", DATE_M);
    query.bindValue(":PRIX_M", PRIX_M);
    query.bindValue(":FOURNISSEUR", FOURNISSEUR);
    query.bindValue(":ETAT", ETAT);

    return query.exec();
}


QSqlQueryModel* rdv1::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("select * FROM GS_MATERIELS");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_MATERIELS"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("DESIGNATION"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("EMPLACEMENT"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE_M"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PRIX_M"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("FOURNISSEUR"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ETAT"));

    return model;
}


bool rdv1::supprimer(int ID_MATERIELS) {
    QSqlQuery query;
    query.prepare("DELETE FROM GS_MATERIELS WHERE ID_MATERIELS = :ID_MATERIELS");
    query.bindValue(":ID_MATERIELS", ID_MATERIELS);
    return query.exec();
}

bool rdv1::checkIfmatExists(int ID_MATERIELS) {
    QSqlQuery query;
    query.prepare("SELECT ID_MATERIELS FROM GS_MATERIELS WHERE ID_MATERIELS = :ID_MATERIELS");
    query.bindValue(":ID_MATERIELS", ID_MATERIELS);
    if (query.exec() && query.next()) {
        return true;
    } else {
        return false;
    }
}

bool rdv1::modifierE(int ID_MATERIELS) {
    QSqlQuery query;
    query.prepare("UPDATE GS_MATERIELS SET DESIGNATION = :DESIGNATION, EMPLACEMENT = :EMPLACEMENT, DATE_M = :DATE_M, PRIX_M = :PRIX_M, FOURNISSEUR = :FOURNISSEUR, ETAT = :ETAT "
                  "WHERE ID_MATERIELS = :ID_MATERIELS");
    query.bindValue(":ID_MATERIELS", ID_MATERIELS);
    query.bindValue(":DESIGNATION", DESIGNATION);
    query.bindValue(":EMPLACEMENT", EMPLACEMENT);
    query.bindValue(":DATE_M", DATE_M);
    query.bindValue(":PRIX_M", PRIX_M);
    query.bindValue(":FOURNISSEUR", FOURNISSEUR);
    query.bindValue(":ETAT", ETAT);

    return query.exec();
}

