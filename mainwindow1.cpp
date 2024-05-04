#include "mainwindow1.h"
#include "ui_mainwindow1.h"
#include<QLabel>
#include<QMessageBox>
#include<QVBoxLayout>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSqlQueryModel>
#include<QApplication>
#include <iostream>
#include<QIntValidator>
#include<QBoxLayout>
#include<QLineEdit>
#include<QComboBox>
#include<QMessageBox>
#include <QApplication>
#include <QDebug>
#include<QWidget>
#include <rdv1.h>
#include <QSortFilterProxyModel>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QDesktopServices>
#include <QTextDocumentWriter>
#include <QtCharts>
#include<QtCharts>
#include<QChartView>
#include<QPieSeries>
#include <QPrintDialog>
#include <QFile>
#include <QSortFilterProxyModel>
#include <QtPrintSupport/QPrinter>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>
#include <QTextEdit>
#include <QTextStream>
#include<QWidget>
#include <QtSvg/QSvgRenderer>
#include<QDirModel>
#include <iostream>
#include <fstream>
#include "qrcode.h"
#include <cstdlib>
#include <ctime>
#include <QFile>
#include <QMediaPlaylist>
#include "smtp.h"
#include"recorder.h"
#include<QSqlRecord>
#include <QFileDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <QDir>
using qrcodegen::QrCode;
using qrcodegen::QrSegment;
MainWindow1::MainWindow1(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow1), R(new rdv1()) {
    ui->setupUi(this);

    int ret=A.connect_arduino(); // lancer la connexion à arduino
    switch(ret){
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
     QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));
    // Connect the clicked signal of the displayTableButton to the displayTable slot
    connect(ui->displayTableButton, &QPushButton::clicked, this, &MainWindow1::displayTable);

   }
MainWindow1::~MainWindow1() {
    delete ui;
    delete R;
}
void MainWindow1::displayTable() {
    QSqlQueryModel *model = R->afficher();
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    ui->tab_mat->setModel(proxyModel);
}
void MainWindow1::on_pb_ajouter_clicked() {
    int ID_MATERIELS = ui->ID_MATERIELS->text().toInt();
    QString DESIGNATION = ui->DESIGNATION->currentText();
    QString EMPLACEMENT = ui->EMPLACEMENT->text();
    QString DATE_M = ui->DATE_M->text();
    int PRIX_M= ui->PRIX_M->text().toInt();
    QString FOURNISSEUR = ui->FOURNISSEUR->text();
    QString ETAT = ui->ETAT->currentText();

    if (ui->ID_MATERIELS->text().isEmpty() || ui->DESIGNATION->currentText().isEmpty() || ui->EMPLACEMENT->text().isEmpty()
            || ui->DATE_M->text().isEmpty()|| ui->PRIX_M->text().isEmpty() || ui->FOURNISSEUR->text().isEmpty()|| ui->ETAT->currentText().isEmpty()) {
        QMessageBox::warning(this, tr("Champs manquants"), tr("Veuillez remplir tous les champs."), QMessageBox::Ok);
        return;
    }

    rdv1 R(ID_MATERIELS, DESIGNATION, EMPLACEMENT, DATE_M,PRIX_M,FOURNISSEUR,ETAT); // Instantiate rdv object
     bool test = R.ajouter();

    if (test) {

        QMessageBox msgBox(QMessageBox::Information, QObject::tr("OK"),
                           QObject::tr("Ajout effectué. \nClick Cancel to exit."), QMessageBox::Cancel, this);

        // Connect the finished signal to a custom slot
        connect(&msgBox, SIGNAL(finished(int)), this, SLOT(handleMessageBoxResult(int)));

        msgBox.exec();
    } else {
        QMessageBox msgBox(QMessageBox::Critical, QObject::tr("NOT OK"),
                           QObject::tr("Ajout non effectué. \nClick Cancel to exit."), QMessageBox::Cancel, this);

        // Connect the finished signal to a custom slot
        connect(&msgBox, SIGNAL(finished(int)), this, SLOT(handleMessageBoxResult(int)));

        msgBox.exec();

    }
}

void MainWindow1::on_pb_supprimer_clicked() {
    int ID_MATERIELS = ui->ID_MATERIELS->text().toInt();
    bool exists = R->checkIfmatExists(ID_MATERIELS);
    if (!exists) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Le rendez-vous n'a pas été trouvé dans la base de données.").arg(ID_MATERIELS),
                              QMessageBox::Cancel);
        return;
    }

    bool test = R->supprimer(ID_MATERIELS);
    if (test) {

        QMessageBox msgBox(QMessageBox::Information, QObject::tr("OK"),
                           QObject::tr("Suppression effectuée. \nClick Cancel to exit."), QMessageBox::Cancel, this);


        connect(&msgBox, SIGNAL(finished(int)), this, SLOT(handleMessageBoxResult(int)));

        msgBox.exec();
    } else {
        QMessageBox msgBox(QMessageBox::Critical, QObject::tr("NOT OK"),
                           QObject::tr("Suppression non effectuée. \nClick Cancel to exit."), QMessageBox::Cancel, this);


        connect(&msgBox, SIGNAL(finished(int)), this, SLOT(handleMessageBoxResult(int)));

        msgBox.exec();

    }
}

void MainWindow1::on_pb_modifier_2_clicked() {
    int ID_MATERIELS = ui->ID_MATERIELS->text().toInt();
    QString DESIGNATION = ui->DESIGNATION->currentText();
    QString EMPLACEMENT = ui->EMPLACEMENT->text();
    QString DATE_M = ui->DATE_M->text();
    int PRIX_M = ui->PRIX_M->text().toInt();
    QString FOURNISSEUR = ui->FOURNISSEUR->text();
    QString ETAT = ui->ETAT->currentText();



    if (ui->ID_MATERIELS->text().isEmpty() || ui->DESIGNATION->currentText().isEmpty() || ui->EMPLACEMENT->text().isEmpty()
                || ui->DATE_M->text().isEmpty()|| ui->PRIX_M->text().isEmpty() || ui->FOURNISSEUR->text().isEmpty()|| ui->ETAT->currentText().isEmpty())  {
        QMessageBox::warning(this, tr("Champs manquants"), tr("Veuillez remplir tous les champs."), QMessageBox::Ok);
        return;
    }

    bool exists = R->checkIfmatExists(ID_MATERIELS);
    if (!exists) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Le rendez-vous n'a pas été trouvé dans la base de données.").arg(ID_MATERIELS),
                              QMessageBox::Cancel);
        return;
    }


    R->setID_MATERIELS(ID_MATERIELS);
    R->setDESIGNATION(DESIGNATION);
    R->setEMPLACEMENT(EMPLACEMENT);
 R->setDATE_M(DATE_M);
  R->setPRIX_M(PRIX_M);
   R->setFOURNISSEUR(FOURNISSEUR);
    R->setETAT(ETAT);




    bool test = R->modifierE(ID_MATERIELS);
    if (test) {

        QMessageBox msgBox(QMessageBox::Information, QObject::tr("OK"),
                           QObject::tr("Modification effectuée. \nClick Cancel to exit."), QMessageBox::Cancel, this);


        connect(&msgBox, SIGNAL(finished(int)), this, SLOT(handleMessageBoxResult(int)));

        msgBox.exec();
    } else {
        QMessageBox msgBox(QMessageBox::Critical, QObject::tr("NOT OK"),
                           QObject::tr("Modification non effectuée. \nClick Cancel to exit."), QMessageBox::Cancel, this);

        connect(&msgBox, SIGNAL(finished(int)), this, SLOT(handleMessageBoxResult(int)));

        msgBox.exec();

    }
}



void MainWindow1::on_tab_mat_clicked(const QModelIndex &index) {
    int row = index.row();
    QString ID_MATERIELS = ui->tab_mat->model()->index(row, 0).data().toString();
    QString DESIGNATION = ui->tab_mat->model()->index(row, 1).data().toString();
    QString EMPLACEMENT = ui->tab_mat->model()->index(row, 2).data().toString();
    QString DATE_M = ui->tab_mat->model()->index(row, 3).data().toString();
    QString PRIX_M = ui->tab_mat->model()->index(row, 4).data().toString();
    QString FOURNISSEUR = ui->tab_mat->model()->index(row, 5).data().toString();
    QString ETAT = ui->tab_mat->model()->index(row, 6).data().toString();


    ui->ID_MATERIELS->setText(ID_MATERIELS);
    ui->DESIGNATION->setCurrentText(DESIGNATION);
    ui->EMPLACEMENT->setText(EMPLACEMENT);
    ui->DATE_M->setText(DATE_M);
    ui->PRIX_M->setText(PRIX_M);
    ui->FOURNISSEUR->setText(FOURNISSEUR);
    ui->ETAT->setCurrentText(ETAT);

}

void MainWindow1::handleMessageBoxResult(int result) {
    if (result == QMessageBox::Cancel) {
        // User clicked Cancel, do something
    }
}











void MainWindow1::on_lineEdit_textChanged(const QString &arg1)
{
    QString nom = arg1.trimmed();

    QSqlQueryModel *originalModel = R->afficher();
    int numRows = originalModel->rowCount();
    int numCols = originalModel->columnCount();


    QStandardItemModel *filteredModel = new QStandardItemModel(numRows, numCols, this);


    QStringList headerLabels;
    for (int col = 0; col < numCols; ++col) {
        headerLabels << originalModel->headerData(col, Qt::Horizontal).toString();
    }
    filteredModel->setHorizontalHeaderLabels(headerLabels);


    for (int row = 0; row < numRows; ++row) {
        bool matchFound = false;
        for (int col = 0; col < numCols; ++col) {
            QModelIndex index = originalModel->index(row, col);
            QString data = originalModel->data(index).toString();
            if (data.contains(nom, Qt::CaseInsensitive)) {
                matchFound = true;
                break;
            }
        }
        if (matchFound) {
            for (int col = 0; col < numCols; ++col) {
                QModelIndex index = originalModel->index(row, col);
                QString data = originalModel->data(index).toString();
                filteredModel->setData(filteredModel->index(row, col), data);
            }
        }
    }

    ui->tab_mat->setModel(filteredModel);

}




void MainWindow1::on_sendBtn2_clicked()
{
    // Retrieve email, subject, and message from line edits or text edits
    QString to = ui->rcpt->text(); // Assuming emailLineEdit is the QLineEdit for the recipient's email
    QString subject = ui->subject->text(); // Assuming subjectLineEdit is the QLineEdit for the email subject
    QString message = ui->msg->toPlainText(); // Assuming messageTextEdit is the QTextEdit for the email message

    // Call sendMail function with email, subject, and message
    sendMail(to, subject, message);
}

void MainWindow1::sendMail(const QString &to, const QString &subject, const QString &message)
{
    // Assuming you have access to the necessary variables for SMTP configuration
    QString user = "culeks.here@gmail.com";
    QString pass = "zyxyuzfhephcrpjh";
    QString host = "smtp.gmail.com";
    int port = 465; // Use the appropriate port for your SMTP server
    int timeout = 30000; // Timeout value in milliseconds

    QString from = "culeks.here@gmail.com"; // Update sender email

    // Create an instance of the Smtp class
    Smtp *smtp = new Smtp(user, pass, host, port, timeout);

    // Send the email
    smtp->sendMail(from, to, subject, message);
}
void MainWindow1::mailSent(QString status)
{
    if(status == "Message sent")
        QMessageBox::warning( 0, tr( "Qt Simple SMTP rdv" ), tr( "Message sent!\n\n" ) );
}




void MainWindow1::on_AscButton_clicked()
{
    QSqlQueryModel *originalModel = R->afficher();
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(originalModel);


    proxyModel->sort(0, Qt::AscendingOrder);

    ui->tab_mat->setModel(proxyModel);
}

void MainWindow1::on_DescButton_clicked()
{
    QSqlQueryModel *originalModel = R->afficher();
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(originalModel);


    proxyModel->sort(0, Qt::DescendingOrder);

    ui->tab_mat->setModel(proxyModel);
}

void MainWindow1::on_pb_pdf_clicked()
{
    // Retrieve the data from tab_mat
    QAbstractItemModel *model = ui->tab_mat->model();
    if (!model)
        return;

    // Create a PDF file
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", QString(), "PDF Files (*.pdf)");
    if (fileName.isEmpty())
        return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    // Start painting on the PDF
    QPainter painter;
    if (!painter.begin(&printer))
    {
        QMessageBox::warning(this, "Warning", "Failed to open PDF file for writing.");
        return;
    }

    // Set up font and metrics
    QFont font = painter.font();
    font.setPointSize(10); // Adjust as needed
    painter.setFont(font);
    QFontMetrics fontMetrics(font);

    int cellWidth = 120; // Adjust as needed
    int cellHeight = fontMetrics.height() + 5;

    int rows = model->rowCount();
    int columns = model->columnCount();

    // Draw headers
    painter.setPen(Qt::black);
    for (int column = 0; column < columns; ++column)
    {
        QString headerText = model->headerData(column, Qt::Horizontal, Qt::DisplayRole).toString();
        QRect headerRect(column * cellWidth, 0, cellWidth, cellHeight);
        painter.drawText(headerRect, Qt::AlignCenter, headerText);
    }

    // Draw grid lines and cell contents
    for (int row = 0; row < rows; ++row)
    {
        for (int column = 0; column < columns; ++column)
        {
            QModelIndex index = model->index(row, column);
            QString text = model->data(index, Qt::DisplayRole).toString();

            // Draw text in the current cell
            QRect cellRect(column * cellWidth, (row + 1) * cellHeight, cellWidth, cellHeight);
            painter.drawText(cellRect, Qt::AlignLeft | Qt::AlignVCenter, text);

            // Draw horizontal grid lines
            painter.drawLine(0, (row + 1) * cellHeight, columns * cellWidth, (row + 1) * cellHeight);
        }
    }

    // Draw vertical grid lines
    for (int column = 0; column < columns + 1; ++column)
    {
        painter.drawLine(column * cellWidth, 0, column * cellWidth, (rows + 1) * cellHeight);
    }

    // End painting
    painter.end();

    QMessageBox::information(this, "PDF Created", "PDF file has been successfully created.");
}


void MainWindow1::on_Imprimer_clicked()
{
    // Retrieve the data from tab_mat
    QAbstractItemModel *model = ui->tab_mat->model();
    if (!model)
        return;

    // Prepare the data for printing
    QString printData;
    int rows = model->rowCount();
    int columns = model->columnCount();
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            QModelIndex index = model->index(row, col);
            QVariant data = model->data(index);
            printData.append(data.toString());
            printData.append("\t"); // Add tab as separator
        }
        printData.append("\n"); // Add newline after each row
    }

    // Print the data
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        painter.drawText(100, 100, printData); // Adjust position as needed
    }
}

void MainWindow1::on_pushButton_clicked()
{
    displayTable(); // Call the function to refresh the table
}



void MainWindow1::StatEtat()
{
    QSqlQuery queryTotal, queryEtat;

    qreal total = 0;
    QMap<QString, qreal> etatCounts;

    // Get total count of records
    if (!queryTotal.exec("SELECT COUNT(*) FROM GS_MATERIELS")) {
        qDebug() << "Error executing query for total count:" << queryTotal.lastError().text();
        return;
    }
    if (queryTotal.next()) {
        total = queryTotal.value(0).toDouble();
    }

    // Get count of each type of etat
    if (!queryEtat.exec("SELECT ETAT, COUNT(*) FROM GS_MATERIELS GROUP BY ETAT")) {
        qDebug() << "Error executing query for etat count:" << queryEtat.lastError().text();
        return;
    }
    while (queryEtat.next()) {
        QString etat = queryEtat.value(0).toString();
        qreal count = queryEtat.value(1).toDouble();
        etatCounts[etat] = count;
    }

    // Create pie series and slices
    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.35);

    QMapIterator<QString, qreal> i(etatCounts);
    while (i.hasNext()) {
        i.next();
        QString etat = i.key();
        qreal count = i.value();
        qreal percentage = (total != 0) ? (count / total) * 100 : 0;

        QPieSlice *slice = series->append(etat, percentage);
        // Set color for each type of etat
        if (etat == "En stock") {
            slice->setBrush(QColor(Qt::blue));
        } else if (etat == "En cours") {
            slice->setBrush(QColor(Qt::red));
        } else if (etat == "Hors service") {
            slice->setBrush(QColor(Qt::green));
        }
        // Set label for the slice
        slice->setLabel(etat + ": " + QString::number(percentage, 'f', 2) + "%");
    }

    // Create and configure the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTitle("Répartition par état");

    // Display the chart
    QChartView *chartView = new QChartView(chart, ui->lab_chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(600, 600);
    chartView->show();
}

void MainWindow1::on_statetat_clicked()
{
    QDialog *dialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(dialog);

    StatEtat();

    layout->addWidget(ui->lab_chart);

    dialog->setWindowTitle("Répartition par état");
    dialog->resize(800, 600);

    dialog->exec();
}
void MainWindow1::StatDESIGNATION()
{
    QSqlQuery queryTotal, queryDesignation;

    qreal total = 0;
    QMap<QString, qreal> designationCounts;

    // Get total count of records
    if (!queryTotal.exec("SELECT COUNT(*) FROM GS_MATERIELS")) {
        qDebug() << "Error executing query for total count:" << queryTotal.lastError().text();
        return;
    }
    if (queryTotal.next()) {
        total = queryTotal.value(0).toDouble();
    }

    // Get count of each type of designation
    if (!queryDesignation.exec("SELECT DESIGNATION, COUNT(*) FROM GS_MATERIELS GROUP BY DESIGNATION")) {
        qDebug() << "Error executing query for designation count:" << queryDesignation.lastError().text();
        return;
    }
    while (queryDesignation.next()) {
        QString designation = queryDesignation.value(0).toString();
        qreal count = queryDesignation.value(1).toDouble();
        designationCounts[designation] = count;
    }

    // Create pie series and slices
    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.35);

    QMapIterator<QString, qreal> i(designationCounts);
    while (i.hasNext()) {
        i.next();
        QString designation = i.key();
        qreal count = i.value();
        qreal percentage = (total != 0) ? (count / total) * 100 : 0;

        QPieSlice *slice = series->append(designation, percentage);
        // Set color for each type of designation
        // You can customize the colors based on your requirements
        // Here's just an example with random colors
        slice->setBrush(QColor::fromRgb(qrand() % 256, qrand() % 256, qrand() % 256));
        // Set label for the slice
        slice->setLabel(designation + ": " + QString::number(percentage, 'f', 2) + "%");
    }

    // Create and configure the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTitle("Répartition par designation");

    // Display the chart
    QChartView *chartView = new QChartView(chart, ui->lab_chart2);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(600, 600);
    chartView->show();
}

void MainWindow1::on_statDESIGNATION_clicked()
{
    QDialog *dialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(dialog);

    StatDESIGNATION();

    layout->addWidget(ui->lab_chart2);

    dialog->setWindowTitle("Répartition par designation");
    dialog->resize(800, 600);

    dialog->exec();
}

void MainWindow1::on_sendNotification_clicked()
{
    QTime time = ui->timeEdit->time();
        int sec = QTime(0, 0).msecsTo(time);
        QString message = ui->textEdit->toPlainText();
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon("E:/not.png"));
    trayIcon->setToolTip("Application");
    QTimer::singleShot(sec, trayIcon, [trayIcon,message] {
        trayIcon->showMessage("reservation",message, QSystemTrayIcon::Information, 5000);});
    connect(trayIcon, &QSystemTrayIcon::messageClicked, trayIcon, &QSystemTrayIcon::hide);
    connect(trayIcon, &QSystemTrayIcon::activated, trayIcon, &QSystemTrayIcon::hide);
    trayIcon->show();
}



void MainWindow1::on_pushButton_3_clicked()
{
    recorder *rec = new recorder(this);
    rec->setWindowTitle("Enregistrer une image");
    rec->exec();
}

void MainWindow1::on_selectbutton_clicked() // If select file button is clicked
{
    QString filter = "All File (*.ogg *.wav *.mp3)"; // Filter to show only audio files

    audioplayer.file_path = QFileDialog::getOpenFileName(this, "Select a file", QDir::homePath(), filter); // Get file path

    audioplayer.file_info = new QFileInfo(audioplayer.file_path); // Initializing QFileInfo

    if(audioplayer.file_path.isEmpty() || (audioplayer.file_info->suffix() != "mp3" && // If no file was selected or it's not an audio file
                                           audioplayer.file_info->suffix() != "wav" &&
                                           audioplayer.file_info->suffix() != "ogg"))
    {
        QMessageBox::warning(this, "Error", "Something went wrong!");
    }
    else
    {
        QMessageBox::information(this, "Succes", audioplayer.file_info->fileName() + " has been selected!");
        audioplayer.playlist->clear();
        audioplayer.playlist->addMedia(QUrl(audioplayer.file_path)); //Adding file to playlist

        audioplayer.player->setPlaylist(audioplayer.playlist);
        audioplayer.player->setVolume(50);

        ui->audioname->setText(audioplayer.file_info->fileName());

        connect(audioplayer.player, &QMediaPlayer::durationChanged, this, [&](qint64 duration) // If caught signal that duration changed
        {                                                                                      // change text label content in ui
            set_durationText();
            qDebug() << duration;
        });

        ui->timeline->setValue(0); // Set the timeline slider to 0 when new audio is loaded
    }
}


void MainWindow1::on_playbutton_clicked() // If play button is clicked
{
    if(!audioplayer.file_path.isEmpty()) // If no file was selected don't connect SLOT to avoid crash
    {
        connect(audioplayer.timer, SIGNAL(timeout()), this, SLOT(set_timelineSliderValue())); // Execute set_timelineSliderValue() every 100 ms
    }
    else
    {
        QMessageBox::warning(this, "Error", "Select a file!");
    }

    audioplayer.player->setVolume(ui->volume->sliderPosition()); // Change the volume if initial position of volume slider changed

    audioplayer.player->play();
}

void MainWindow1::on_pauseButton_clicked() // If pause button is clicked
{
    disconnect(audioplayer.timer, SIGNAL(timeout()), this, SLOT(set_timelineSliderValue())); // Stop exectuing set_timelineSliderValue() every 100 ms

    audioplayer.player->pause();
}


void MainWindow1::on_volume_valueChanged(int value) // Change volume in dependence of volume slider's position
{
    audioplayer.player->setVolume(value);
}

void MainWindow1::set_timelineSliderValue() // Move slider's position in dependance of audio's position
{
    set_currentPositionText(0);

    if(audioplayer.player->duration() != 0) // If audio reahced the end, don't change timeline slider value to avoid crash
        ui->timeline->setValue(audioplayer.player->position() * 100 / audioplayer.player->duration());
}


void MainWindow1::on_timeline_valueChanged(int value) // Change audio's position if slider's position changed
{
    set_currentPositionText(value);

    audioplayer.player->setPosition(audioplayer.player->duration() * value / 100);
}

void MainWindow1::set_durationText() // Set label text duration of audio
{
    qint64 duration = audioplayer.player->duration();
    QTime time(0,(duration % (1000 * 60 * 60)) / (1000 * 60),(duration % (1000 * 60)) / 1000);
    ui->duration->setText(time.toString("mm:ss"));
}

void MainWindow1::set_currentPositionText(int value) // Set label text current position of audio
{
    qint64 duration;

    if(value == 0)
    {
        duration = audioplayer.player->position();
    }
    else
    {
        duration = audioplayer.player->duration() * value / 100;
    }
    QTime time(0,(duration % (1000 * 60 * 60)) / (1000 * 60),(duration % (1000 * 60)) / 1000);
    ui->currentPosition->setText(time.toString("mm:ss"));
}

void MainWindow1::on_loop_clicked(bool checked) // If loop box checked then loop the audio
{
    if(checked)
    {
        audioplayer.playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }
    else
    {
        audioplayer.playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    }
}

void MainWindow1::on_pushsegment_clicked() {
    // Read the text from the line edit
    QString text = ui->ID_MATERIELS->text();

    // Ensure the number is two digits by padding with zeros if needed
    QString formattedNumber = text.rightJustified(2);

    // Construct the command
    QByteArray command;
    command.append("0");
    command.append(formattedNumber.toUtf8());

    // Send the command to Arduino
    A.write_to_arduino(command);
}
