#include "pack.h"
#include <QtSql>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QDebug>
pack::pack(int id, QString nom, QString services,QString description,QString prix,QString duree)
{

  this->id=id;
  this->nom =nom;
  this->services=services;
  this->description=description;
  this->prix=prix;
  this->duree=duree;

}

bool pack::ajouter(){
    QSqlQuery query;
    QString res = QString::number(id);
    // Vérifier si l'ID existe déjà
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM packs WHERE id = :id");
        checkQuery.bindValue(":id", id);
        checkQuery.exec();
        checkQuery.first(); // Déplace le curseur sur le premier enregistrement
        int count = checkQuery.value(0).toInt(); // Récupère le nombre d'enregistrements avec cet ID

        if (count > 0) {
            // L'ID existe déjà, afficher un message pour demander à l'utilisateur s'il veut modifier cette ligne
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(nullptr, QObject::tr("ID existant"),
                                          QObject::tr("Voulez-vous modifier cette ligne ?"),
                                          QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::No) {
                return false; // Annuler l'ajout
            }
        }

    query.prepare("insert  into packs (id,nom,services,description,prix,duree) values (:id , :nom, :services,:description,:prix,:duree)");
    query.bindValue(":id",res);
    query.bindValue(":nom",nom);
    query.bindValue(":services",services);
    query.bindValue(":description",description);
    query.bindValue(":prix",prix);
    query.bindValue(":duree",duree);

    return query.exec();
};

bool pack::supprimer(int id){
    QSqlQuery query; QString res=QString::number(id);
    qDebug() << "id :" << res; // Affiche l'ID dans la sortie de débogage
    query.prepare("delete from packs where id=:id");
    query.bindValue(":id",res);
    return query.exec();
}

QSqlQueryModel * pack::afficher(){
    QSqlQueryModel * model=new QSqlQueryModel();
    model -> setQuery("select * from packs");
    model -> setHeaderData(0,Qt::Horizontal,QObject::tr("id"));
    model -> setHeaderData(1,Qt::Horizontal,QObject::tr("nom"));
    model -> setHeaderData(2,Qt::Horizontal,QObject::tr("services"));
    model -> setHeaderData(3,Qt::Horizontal,QObject::tr("description"));
    model -> setHeaderData(4,Qt::Horizontal,QObject::tr("prix"));
    model -> setHeaderData(5,Qt::Horizontal,QObject::tr("duree"));
    return model;
}





