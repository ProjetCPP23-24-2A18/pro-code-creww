#include "mainwindow2.h"
#include "ui_mainwindow2.h"
#include "pack.h"
#include "connection.h"
#include <QPixmap>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QSqlQuery>
#include <QtCharts>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QtCharts>
#include <QPdfWriter>
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QSslSocket>
#include <QProcess>
#include "arduinoh.h"
#include "connection.h"



void MainWindow2::update_label()
{
    data=A.read_from_arduino();

    if(data=="1")

        ui->label_3->setText("ON"); // si les données reçues de arduino via la liaison série sont égales à 1
    // alors afficher ON

    else if (data=="0")

        ui->label_3->setText("OFF");   // si les données reçues de arduino via la liaison série sont égales à 0
     //alors afficher ON
}

void MainWindow2::generateQRCode(const QString &text, const QString &outputFilePath)
{
    // Script Python pour générer le QR code
    QString pythonScript = R"(
import qrcode
import sys

def generate_qr_code(text, file_path):
    qr = qrcode.QRCode(
        version=1,
        error_correction=qrcode.constants.ERROR_CORRECT_L,
        box_size=10,
        border=4,
    )
    qr.add_data(text)
    qr.make(fit=True)

    img = qr.make_image(fill_color="black", back_color="white")
    img.save(file_path)

if __name__ == "__main__":
    # Récupérer les arguments passés depuis Qt
    text = sys.argv[1]
    output_file = sys.argv[2]
    generate_qr_code(text, output_file)
)";

    // Arguments à passer au script Python
    QStringList args;
    args << text << outputFilePath;

    // Créer un processus pour exécuter le script Python
    QProcess process;
    process.start("python", QStringList() << "-c" << pythonScript << args);
    process.waitForFinished(-1); // Attendre que le processus se termine

    QPixmap qrCodePixmap("qrcodeimg.png");

        // Afficher l'image dans le QLabel
    ui->label_7->setPixmap(qrCodePixmap);

    // Vérifier si le processus s'est terminé correctement
    if (process.exitCode() == 0) {
        qDebug() << "QR code généré avec succès dans le fichier :" << outputFilePath;
    } else {
        qDebug() << "Erreur lors de la génération du QR code.";
    }
}

void MainWindow2::showBarChartFromTableData(){
    QSqlQuery query("SELECT prix FROM packs");

        // Créer un dictionnaire pour stocker le nombre d'occurrences de chaque prix
        QMap<double, int> prixCount;

        // Parcourir les résultats de la requête et compter le nombre d'occurrences de chaque prix
        while (query.next()) {
            double prix = query.value(0).toDouble();
            prixCount[prix]++;
        }

        // Créer un graphique à barres
        QChart *chart = new QChart();
        chart->setTitle("Nombre de packs par prix");

        // Créer une série de barres pour le graphique
        QBarSeries *series = new QBarSeries();

        // Parcourir le dictionnaire de comptage et ajouter des barres à la série
        QMapIterator<double, int> i(prixCount);
        while (i.hasNext()) {
            i.next();
            double prix = i.key();
            int count = i.value();

            // Créer une barre avec le prix et le nombre d'occurrences
            QBarSet *barSet = new QBarSet(QString::number(prix));
            *barSet << count;

            // Ajouter la barre à la série
            series->append(barSet);
        }

        // Ajouter la série au graphique
        chart->addSeries(series);

        // Créer l'axe des x et définir les catégories
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        chart->addAxis(axisX, Qt::AlignBottom);


        // Créer l'axe des y
        QValueAxis *axisY = new QValueAxis();
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        // Créer une vue de graphique avec une taille fixe
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->resize(ui->graphicsView->size()); // Adapter la taille de la vue à celle du QGraphicsView

        // Ajouter le graphique à votre QGraphicsView
        QGraphicsScene *scene = new QGraphicsScene(this);
        scene->addWidget(chartView); // Utiliser addWidget pour ajouter le graphique à la scène
        ui->graphicsView->setScene(scene);

}

MainWindow2::MainWindow2(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow2)
{

    ui->setupUi(this);
    QPixmap pix("C:/Users/h.chebaane/Downloads/PROJETc/image.png");
    QPixmap pix2("C:/Users/h.chebaane/Downloads/PROJETc/image2.png");
    qDebug() << QSqlDatabase::drivers();
    Ptmp = pack();
    ui->tableView->setModel(Ptmp.afficher());
    showBarChartFromTableData();
    int ret=A.connect_arduino(); // lancer la connexion à arduino
    switch(ret){
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
     QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
     //le slot up

}

MainWindow2::~MainWindow2()
{
    delete ui;
}
void MainWindow2::on_tableView_clicked(const QModelIndex &index) {
    if (index.isValid()) {
        int row = index.row();
        int colId = 0; // Indice de la colonne ID dans votre modèle
        int colNom = 1; // Indice de la colonne nom dans votre modèle
        int colServices = 2; // Indice de la colonne services dans votre modèle
        int colDescription = 3; // Indice de la colonne description dans votre modèle
        int colPrix = 4; // Indice de la colonne prix dans votre modèle
        int colDuree = 5; // Indice de la colonne duree dans votre modèle

        // Récupérer les données de la ligne sélectionnée
        QVariant idData = ui->tableView->model()->data(ui->tableView->model()->index(row, colId));
        QVariant nomData = ui->tableView->model()->data(ui->tableView->model()->index(row, colNom));
        QVariant servicesData = ui->tableView->model()->data(ui->tableView->model()->index(row, colServices));
        QVariant descriptionData = ui->tableView->model()->data(ui->tableView->model()->index(row, colDescription));
        QVariant prixData = ui->tableView->model()->data(ui->tableView->model()->index(row, colPrix));
        QVariant dureeData = ui->tableView->model()->data(ui->tableView->model()->index(row, colDuree));

        // Afficher les données dans les champs d'édition
        ui->lineEdit_ID->setText(idData.toString());
        ui->lineEdit_nom->setText(nomData.toString());
        ui->lineEdit_services->setText(servicesData.toString());
        ui->lineEdit_description->setText(descriptionData.toString());
        ui->lineEdit_prix->setText(prixData.toString());
        ui->lineEdit_duree->setText(dureeData.toString());
        showBarChartFromTableData();
        generateQRCode("Nom : "+nomData.toString()+" \nServices : "+servicesData.toString()+" \nPrix : "+prixData.toString()+" \nDurée : "+dureeData.toString(),"qrcodeimg.png");
    }

}
void MainWindow2::on_pushButton_ajouter_clicked(){

    int id= ui->lineEdit_ID->text().toInt();
    QString nom=ui->lineEdit_nom->text();
    QString services=ui->lineEdit_services->text();
    QString description=ui->lineEdit_description->text();
    QString prix=ui->lineEdit_prix->text();
    QString duree=ui->lineEdit_duree->text();
    pack p(id,nom,services,description,prix,duree);
    int prixint = ui->lineEdit_prix->text().toInt();

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT AVG(prix) AS moyenne_prix FROM packs");
    checkQuery.exec();
    checkQuery.first(); // Déplace le curseur sur le premier enregistrement
    int avg_prix = checkQuery.value(0).toInt();

    if (prixint > avg_prix) {
        A.write_to_arduino("1");
        // Convertir la moyenne en une chaîne de caractères pour l'affichage
        QString avg_prix_str = QString::number(avg_prix);

        // Afficher la moyenne dans une boîte de dialogue QMessageBox
        QMessageBox::information(nullptr, QObject::tr("OK"), "Le prix est supérieur à la moyenne : " + avg_prix_str, QMessageBox::Cancel);
    }

    else {
        A.write_to_arduino("0");
        QMessageBox::information(nullptr,QObject::tr("OK"),QObject::tr("Le prix est correct "),QMessageBox::Cancel);
    }

    bool test = p.ajouter();
    if (test)
    {
        QMessageBox::information(nullptr,QObject::tr("OK"),QObject::tr("Ajout effectue \n""click cancel to exit"),QMessageBox::Cancel);

    }
    else
        QMessageBox::critical(nullptr,QObject::tr("Not OK"),QObject::tr("click cancel to exit"),QMessageBox::Cancel);
    ui->tableView->setModel(Ptmp.afficher());
    showBarChartFromTableData();
}

void MainWindow2::on_pushButton_supprimer_clicked(){
    int id=ui->lineEdit_ID->text().toInt();

    bool test = Ptmp.supprimer(id);
    if (test)
    {
        QMessageBox::information(nullptr,QObject::tr("OK"),QObject::tr("suppression effectuee \n""click cancel to exit"),QMessageBox::Cancel);

    }
    else
        QMessageBox::critical(nullptr,QObject::tr("Not OK"),QObject::tr("erreur lors de la supression , verifiez l'ID"),QMessageBox::Cancel);
    ui->tableView->setModel(Ptmp.afficher());
    showBarChartFromTableData();
}

void MainWindow2::on_lineEdit_search_textChanged(const QString &searchText) {
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QSqlQuery query;

    // Construction de la requête SQL pour récupérer les données filtrées en fonction du texte de recherche
    QString queryString = "SELECT * FROM packs WHERE nom LIKE '%" + searchText + "%' OR services LIKE '%" + searchText + "%' OR description LIKE '%" + searchText + "%'OR id LIKE '%" + searchText + "%'OR prix LIKE '%" + searchText + "%'";
    query.prepare(queryString);

    // Exécution de la requête
    if (query.exec()) {
        model->setQuery(query);
        ui->tableView->setModel(model); // Appliquer le modèle avec les données filtrées au QTableView
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
    }
}



void MainWindow2::on_comboBox_currentTextChanged(const QString &arg1)
{
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(ui->tableView->model());

    // Déterminez le critère de tri en fonction de l'option sélectionnée dans la liste déroulante
    QString sortCriteria;
    if (arg1 == "Prix") {

        sortCriteria = "prix";
    } else if (arg1 == "Durée") {
        sortCriteria = "duree";
    } else {
        // Gérer d'autres options de tri si nécessaire
    }

    // Appliquer le tri sur le critère sélectionné
    if (!sortCriteria.isEmpty()) { // Ignorer la casse lors du tri

        // Déterminer le numéro de colonne à utiliser pour le tri en fonction de sortCriteria
        int columnToSort = 0; // Par défaut, trier par la première colonne
        if (sortCriteria == "prix") {
            columnToSort = 4; // Tri par la colonne Prix (supposant que c'est la cinquième colonne, ajustez selon votre modèle de données)
        } else if (sortCriteria == "duree") {
            columnToSort = 5; // Tri par la colonne Durée (supposant que c'est la sixième colonne, ajustez selon votre modèle de données)
        }

        // Effectuer le tri
        qDebug() << "ca marche !"<<columnToSort ;
        proxyModel->sort(columnToSort, Qt::AscendingOrder); // Tri ascendant par défaut
    }

    // Appliquer le modèle trié au QTableView
    ui->tableView->setModel(proxyModel);

}



void MainWindow2::on_pushButton_4_clicked()
{
    // Créer un objet QPdfWriter pour écrire le PDF dans un fichier
        QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", QDir::currentPath(), "PDF Files (*.pdf)");
        QPdfWriter writer(fileName);

        // Configurer le PDF
        writer.setPageSize(QPageSize(QPageSize::A4));
        writer.setPageMargins(QMarginsF(30, 30, 30, 30));

        // Commencer une nouvelle page
        QPainter painter(&writer);
        painter.setRenderHint(QPainter::Antialiasing);
        QFont font("Arial", 16); // Changer la taille de la police
        painter.setFont(font);

        // Écrire les noms des colonnes
        int colCount = ui->tableView->model()->columnCount();
        qreal x = 30, y = 30; // Position initiale dans le PDF

        for (int col = 0; col < colCount; ++col) {
            QString columnName = ui->tableView->model()->headerData(col, Qt::Horizontal).toString();
            QRectF textRect = painter.boundingRect(QRectF(0, 0, 200, 200), Qt::AlignLeft, columnName); // Calculer la taille de la zone de dessin
            painter.drawText(x, y, textRect.width(), textRect.height(), Qt::AlignLeft, columnName);
            x += textRect.width() + 500; // Espacement horizontal entre les colonnes
        }

        // Écrire les données de la table dans le PDF
        int rowCount = ui->tableView->model()->rowCount();
        x = 30; // Réinitialiser la position horizontale
        y += 800; // Espacement vertical entre les noms des colonnes et les données

        for (int row = 0; row < rowCount; ++row) {
            qreal currentY = y; // Stocker la position Y actuelle pour chaque ligne
            for (int col = 0; col < colCount; ++col) {
                QString text = ui->tableView->model()->data(ui->tableView->model()->index(row, col)).toString();
                QRectF textRect = painter.boundingRect(QRectF(0, 0, 200, 200), Qt::AlignLeft, text); // Calculer la taille de la zone de dessin
                painter.drawText(x, currentY, textRect.width(), textRect.height(), Qt::AlignLeft, text);
                qDebug()<<"y : "<<y;
                x += textRect.width() + 500; // Espacement horizontal entre les colonnes
            }
            y += 800; // Espacement vertical entre les lignes
            x = 30; // Réinitialiser la position horizontale
        }
        // Fermer le PDF
        painter.end();
}

void MainWindow2::on_pushButton_supprimer_2_clicked()
{
    // Configuration du serveur SMTP
        QString smtpServer = "smtp.gmail.com";
        int smtpPort = 587; // Port SMTP pour une connexion sécurisée SSL/TLS
        QString username = "hr.chebaane.ningen@gmail.com";
        QString password = "Essfifa2018;;";

        // Création d'un socket SSL pour la connexion SMTP
        QSslSocket smtpSocket;
        smtpSocket.connectToHostEncrypted(smtpServer, smtpPort);

        if (smtpSocket.waitForConnected()) {
            qDebug() << "Connected to SMTP server.";

            // Attendre que le serveur SMTP envoie le message de bienvenue
            if (smtpSocket.waitForReadyRead()) {
                qDebug() << "Server response:" << smtpSocket.readAll();
            }

            // Envoyer les commandes SMTP pour l'authentification et l'envoi de l'e-mail
            smtpSocket.write("EHLO localhost\r\n");
            smtpSocket.write("AUTH LOGIN\r\n");
            smtpSocket.write(QByteArray().append(username).toBase64() + "\r\n");
            smtpSocket.write(QByteArray().append(password).toBase64() + "\r\n");
            smtpSocket.write("MAIL FROM:<hr.chebaane.ningen@gmail.com>\r\n");
            smtpSocket.write("RCPT TO:<hr.chebaane.ningen@gmail.com>\r\n");
            smtpSocket.write("DATA\r\n");
            smtpSocket.write("Subject: Test d'e-mail via SMTP\r\n");
            smtpSocket.write("From: hr.chebaane.ningen@gmail.com\r\n");
            smtpSocket.write("To: hr.chebaane.ningen@gmail.com\r\n");
            smtpSocket.write("\r\n");
            smtpSocket.write("Ceci est un test d'e-mail envoyé via SMTP avec Qt.\r\n");
            smtpSocket.write(".\r\n"); // Terminer le corps de l'e-mail
            smtpSocket.write("QUIT\r\n");

            // Attendre que toutes les commandes SMTP soient envoyées et le message de réponse du serveur
            if (smtpSocket.waitForBytesWritten() && smtpSocket.waitForReadyRead()) {
                qDebug() << "Server response:" << smtpSocket.readAll();
            } else {
                qDebug() << "Error sending SMTP commands:" << smtpSocket.errorString();
            }

            } else {
                qDebug() << "Error connecting to SMTP server:" << smtpSocket.errorString();
            }

        // Fermer la connexion SMTP
        smtpSocket.close();
}
