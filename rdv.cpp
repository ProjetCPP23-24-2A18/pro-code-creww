#include "rdv.h"
#include <QSqlQuery>
#include <QtDebug>
#include<QObject>
#include <QIntValidator>
#include<QSqlError>
#include<QString>
#include<QComboBox>
rdv::rdv() {
    id_rdv = 0;
    date_rdv = "";
    duree = "";
    type_soin = "";
}
rdv::rdv(int id_rdv, QString date_rdv, QString type_soin, QString duree)
 {
    this->id_rdv = id_rdv;
    this->date_rdv = date_rdv;
    this->duree = duree;
    this->type_soin = type_soin;
}
int rdv::getIdRdv() { return id_rdv; }
QString rdv::getDateRdv() { return date_rdv; }
QString rdv::getDuree() { return duree; }
QString rdv::getTypeSoin() { return type_soin; }

void rdv::setIdRdv(int id_rdv) { this->id_rdv = id_rdv; }
void rdv::setDateRdv(QString date_rdv) { this->date_rdv = date_rdv; }
void rdv::setDuree(QString duree) { this->duree = duree; }
void rdv::setTypeSoin(QString type_soin) { this->type_soin = type_soin; }

bool rdv::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO GS_RDVS (ID_RDV, DATE_RDV, DUREE, TYPE_SOIN) "
                  "VALUES (:id_rdv, :date_rdv, :duree, :type_soin)");
    query.bindValue(":id_rdv", id_rdv);
    query.bindValue(":date_rdv", date_rdv);
    query.bindValue(":duree", duree);
    query.bindValue(":type_soin", type_soin);

    return query.exec();
}


QSqlQueryModel* rdv::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("select * FROM GS_RDVS");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_RDV"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("TYPE_SOIN"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATE_RDV"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DUREE"));

    return model;
}


bool rdv::supprimer(int id_rdv) {
    QSqlQuery query;
    query.prepare("DELETE FROM GS_RDVS WHERE ID_RDV = :id_rdv");
    query.bindValue(":id_rdv", id_rdv);
    return query.exec();
}

bool rdv::checkIfRdvExists(int id_rdv) {
    QSqlQuery query;
    query.prepare("SELECT ID_RDV FROM GS_RDVS WHERE ID_RDV = :id_rdv");
    query.bindValue(":id_rdv", id_rdv);
    if (query.exec() && query.next()) {
        return true;
    } else {
        return false;
    }
}

bool rdv::modifierE(int id_rdv) {
    QSqlQuery query;
    query.prepare("UPDATE GS_RDVS SET DATE_RDV = :date_rdv, DUREE = :duree, TYPE_SOIN = :type_soin "
                  "WHERE ID_RDV = :id_rdv");
    query.bindValue(":id_rdv", id_rdv);
    query.bindValue(":date_rdv", date_rdv);
    query.bindValue(":duree", duree);
    query.bindValue(":type_soin", type_soin);

    return query.exec();
}
