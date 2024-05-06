#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QLabel>
#include<QMessageBox>
#include<QVBoxLayout>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSqlQueryModel>
#include<QApplication>
#include <iostream>
#include "notepad.h"
#include<QIntValidator>
#include<QBoxLayout>
#include<QLineEdit>
#include<QComboBox>
#include<QMessageBox>
#include <QApplication>
#include <QDebug>
#include<QWidget>
#include <rdv.h>
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
#include "chatsocket.h"
#include "chatserver.h"
#include <QErrorMessage>
#include "personnellist.h"
#include "arduino.h"

using qrcodegen::QrCode;
using qrcodegen::QrSegment;
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), R(new rdv()) {
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
    connect(ui->displayTableButton, &QPushButton::clicked, this, &MainWindow::displayTable);

        // set first day of week
        ui->calendarWidget->setFirstDayOfWeek(Qt::Wednesday);

        // hide week number of year
        ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);

        // show week number of year
        ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::ISOWeekNumbers);
        TCP_Server = new QTcpServer();
        if(TCP_Server->listen(QHostAddress::Any, 8888)) { // Change the port to 8888
               connect(TCP_Server,SIGNAL(newConnection()),this,SLOT(newConnection()));
               QMessageBox::information(this,"Qt","Server Started on port 8888");
           } else {
               QMessageBox::critical(this,"Qt","Failed to start server on port 8888");
           }
   }
MainWindow::~MainWindow() {
    delete ui;
    delete R;
}
void MainWindow::displayTable() {
    QSqlQueryModel *model = R->afficher();
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    ui->tab_rd->setModel(proxyModel);
}
void MainWindow::on_pb_ajouter_clicked() {
    int id_rdv = ui->id_rdv->text().toInt();
    QString date_rdv = ui->date_rdv->text();
    QString type_soin = ui->type_soin->currentText();
    QString duree = ui->duree->currentText();

    if (ui->id_rdv->text().isEmpty() || ui->date_rdv->text().isEmpty() || ui->type_soin->currentText().isEmpty()
            || ui->duree->currentText().isEmpty()) {
        QMessageBox::warning(this, tr("Champs manquants"), tr("Veuillez remplir tous les champs."), QMessageBox::Ok);
        return;
    }

    rdv R(id_rdv, date_rdv, type_soin, duree); // Instantiate rdv object
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

void MainWindow::on_pb_supprimer_clicked() {
    int id_rdv = ui->id_rdv->text().toInt();
    bool exists = R->checkIfRdvExists(id_rdv);
    if (!exists) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Le rendez-vous n'a pas été trouvé dans la base de données.").arg(id_rdv),
                              QMessageBox::Cancel);
        return;
    }

    bool test = R->supprimer(id_rdv);
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

void MainWindow::on_pb_modifier_2_clicked() {
    int id_rdv = ui->id_rdv->text().toInt();
    QString date_rdv = ui->date_rdv->text();
    QString type_soin = ui->type_soin->currentText();
    QString duree = ui->duree->currentText();

    if (ui->id_rdv->text().isEmpty() || ui->date_rdv->text().isEmpty() || ui->type_soin->currentText().isEmpty()
            || ui->duree->currentText().isEmpty()) {
        QMessageBox::warning(this, tr("Champs manquants"), tr("Veuillez remplir tous les champs."), QMessageBox::Ok);
        return;
    }

    bool exists = R->checkIfRdvExists(id_rdv);
    if (!exists) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Le rendez-vous n'a pas été trouvé dans la base de données.").arg(id_rdv),
                              QMessageBox::Cancel);
        return;
    }


    R->setIdRdv(id_rdv);
    R->setDateRdv(date_rdv);
    R->setTypeSoin(type_soin);
 R->setDuree(duree);

    bool test = R->modifierE(id_rdv);
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



void MainWindow::on_tab_rd_clicked(const QModelIndex &index) {
    int row = index.row();
    QString id_rdv = ui->tab_rd->model()->index(row, 0).data().toString();
    QString date_rdv = ui->tab_rd->model()->index(row, 1).data().toString();
    QString duree = ui->tab_rd->model()->index(row, 2).data().toString();
    QString type_soin = ui->tab_rd->model()->index(row, 3).data().toString();

    ui->id_rdv->setText(id_rdv);
    ui->date_rdv->setText(date_rdv);
    ui->duree->setCurrentText(duree);
    ui->type_soin->setCurrentText(type_soin);
}

void MainWindow::handleMessageBoxResult(int result) {
    if (result == QMessageBox::Cancel) {
        // User clicked Cancel, do something
    }
}











void MainWindow::on_lineEdit_textChanged(const QString &arg1)
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

    ui->tab_rd->setModel(filteredModel);

}


void MainWindow::on_QR_clicked()
{
    if(ui->tab_rd->currentIndex().row() == -1) {
        QMessageBox::information(nullptr, QObject::tr("qr"),
                                 QObject::tr("Veuillez Choisir un identifiant.\n"
                                             "Click Ok to exit."), QMessageBox::Ok);
    } else {
        // Retrieve data from the selected row in the table
        QModelIndex currentIndex = ui->tab_rd->currentIndex();
        QString id_rdv = ui->tab_rd->model()->index(currentIndex.row(), 0).data().toString();
        QString type_soin = ui->tab_rd->model()->index(currentIndex.row(), 1).data().toString();
        QString date_rdv = ui->tab_rd->model()->index(currentIndex.row(), 2).data().toString();
        QString duree = ui->tab_rd->model()->index(currentIndex.row(), 3).data().toString();

        // Concatenate data into a single string (or use a suitable format)
        QString qrData = "ID: " + id_rdv + "\n"
                         + "Type de soin: " + type_soin + "\n"
                         + "Date du rendez-vous: " + date_rdv + "\n"
                         + "Durée: " + duree;

        // Use the data to generate the QR code
        const QrCode qr = QrCode::encodeText(qrData.toStdString().c_str(), QrCode::Ecc::LOW);

        // Save the QR code to a file
        std::ofstream myfile;
        myfile.open("qrcode.svg");
        myfile << qr.toSvgString(1);
        myfile.close();

        // Load the QR code image
        QPixmap pix("qrcode.svg");

        // Scale the image as needed
        QPixmap scaledPix = pix.scaled(200, 200);

        // Display the QR code image
        ui->QRCODE_3->setPixmap(scaledPix);
    }
}



void MainWindow::on_sendBtn2_clicked()
{
    // Retrieve email, subject, and message from line edits or text edits
    QString to = ui->rcpt->text(); // Assuming emailLineEdit is the QLineEdit for the recipient's email
    QString subject = ui->subject->text(); // Assuming subjectLineEdit is the QLineEdit for the email subject
    QString message = ui->msg->toPlainText(); // Assuming messageTextEdit is the QTextEdit for the email message

    // Call sendMail function with email, subject, and message
    sendMail(to, subject, message);
}

void MainWindow::sendMail(const QString &to, const QString &subject, const QString &message)
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
void MainWindow::mailSent(QString status)
{
    if(status == "Message sent")
        QMessageBox::warning( 0, tr( "Qt Simple SMTP rdv" ), tr( "Message sent!\n\n" ) );
}




void MainWindow::on_AscButton_clicked()
{
    QSqlQueryModel *originalModel = R->afficher();
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(originalModel);


    proxyModel->sort(0, Qt::AscendingOrder);

    ui->tab_rd->setModel(proxyModel);
}

void MainWindow::on_DescButton_clicked()
{
    QSqlQueryModel *originalModel = R->afficher();
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(originalModel);


    proxyModel->sort(0, Qt::DescendingOrder);

    ui->tab_rd->setModel(proxyModel);
}

void MainWindow::on_pb_pdf_clicked()
{
    // Retrieve the data from tab_rd
    QAbstractItemModel *model = ui->tab_rd->model();
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


void MainWindow::on_Imprimer_clicked()
{
    // Retrieve the data from tab_rd
    QAbstractItemModel *model = ui->tab_rd->model();
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

void MainWindow::on_pushButton_clicked()
{
    displayTable(); // Call the function to refresh the table
}

void MainWindow::Stat()
{
    QSqlQuery queryTotal, queryType;

    qreal total = 0;
    QMap<QString, qreal> typeCounts;

    // Get total count of records
    if (!queryTotal.exec("SELECT COUNT(*) FROM GS_RDVS")) {
        qDebug() << "Error executing query for total count:" << queryTotal.lastError().text();
        return;
    }
    if (queryTotal.next()) {
        total = queryTotal.value(0).toDouble();
    }

    // Get count of each type of soin
    if (!queryType.exec("SELECT TYPE_SOIN, COUNT(*) FROM GS_RDVS GROUP BY TYPE_SOIN")) {
        qDebug() << "Error executing query for type count:" << queryType.lastError().text();
        return;
    }
    while (queryType.next()) {
        QString type = queryType.value(0).toString();
        qreal count = queryType.value(1).toDouble();
        typeCounts[type] = count;
    }

    // Create pie series and slices
    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.35);

    QMapIterator<QString, qreal> i(typeCounts);
    while (i.hasNext()) {
        i.next();
        QString type = i.key();
        qreal count = i.value();
        qreal percentage = (total != 0) ? (count / total) * 100 : 0;

        QPieSlice *slice = series->append(type, percentage);
        // Set color for each type of soin
        if (type == "massage") {
            slice->setBrush(QColor(Qt::blue));
        } else if (type == "reeducation") {
            slice->setBrush(QColor(Qt::red));
        }
        // Set label for the slice
        slice->setLabel(type + ": " + QString::number(percentage, 'f', 2) + "%");
    }

    // Create and configure the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTitle("Répartition par type de soin");

    // Display the chart
    QChartView *chartView = new QChartView(chart, ui->lab_chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(600, 600);
    chartView->show();
}
void MainWindow::on_stattype_clicked()
{
    QDialog *dialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(dialog);


    Stat();

    layout->addWidget(ui->lab_chart);

    dialog->setWindowTitle("Répartition par type de soin");
    dialog->resize(800, 600);

    dialog->exec();
}

void MainWindow::StatDuree()
{
    QSqlQuery queryTotal, queryDuree;

    qreal total = 0;
    qreal count1Hour = 0;
    qreal count1Hour30 = 0;

    // Get total count of records
    if (!queryTotal.exec("SELECT COUNT(*) FROM GS_RDVS")) {
        qDebug() << "Error executing query for total count:" << queryTotal.lastError().text();
        return;
    }
    if (queryTotal.next()) {
        total = queryTotal.value(0).toDouble();
    }

    // Get count of records with duration 1 hour
    if (!queryDuree.exec("SELECT COUNT(*) FROM GS_RDVS WHERE DUREE = '1h'")) {
        qDebug() << "Error executing query for 1 hour count:" << queryDuree.lastError().text();
        return;
    }
    if (queryDuree.next()) {
        count1Hour = queryDuree.value(0).toDouble();
    }

    // Get count of records with duration 1.5 hours
    if (!queryDuree.exec("SELECT COUNT(*) FROM GS_RDVS WHERE DUREE = '1h30'")) {
        qDebug() << "Error executing query for 1.5 hour count:" << queryDuree.lastError().text();
        return;
    }
    if (queryDuree.next()) {
        count1Hour30 = queryDuree.value(0).toDouble();
    }

    // Calculate percentages
    qreal percentage1Hour = (total != 0) ? (count1Hour / total) * 100 : 0;
    qreal percentage1Hour30 = (total != 0) ? (count1Hour30 / total) * 100 : 0;

    // Create pie series and slices
    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.35);

    // Add slices for 1 hour and 1.5 hours
    QPieSlice *slice1Hour = series->append("1h", percentage1Hour);
    QPieSlice *slice1Hour30 = series->append("1h30", percentage1Hour30);

    // Customize colors as needed
    slice1Hour->setBrush(QColor(Qt::blue));
    slice1Hour30->setBrush(QColor(Qt::red));

    // Set labels for the slices
    slice1Hour->setLabel("1h: " + QString::number(percentage1Hour, 'f', 2) + "%");
    slice1Hour30->setLabel("1h30: " + QString::number(percentage1Hour30, 'f', 2) + "%");

    // Create and configure the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTitle("Répartition par durée");

    // Display the chart
    QChartView *chartView = new QChartView(chart, ui->lab_chart2);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(600, 600);
    chartView->show();
}


void MainWindow::on_statduree_clicked()
{
    QDialog *dialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(dialog);

    StatDuree();

    layout->addWidget(ui->lab_chart2);

    dialog->setWindowTitle("Répartition par durée");
    dialog->resize(800, 600);

    dialog->exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    Notepad p;
    p.setModal(true);
    p.exec();
}



void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    qDebug() << "Selected Date : " << date;
}

// By this event you can get selected year and month in calendar
void MainWindow::on_calendarWidget_currentPageChanged(int year, int month)
{
    qDebug() << "Selected Year : " << year;
    qDebug() << "Selected Month : " << month;
}

void MainWindow::newConnection()
{
 while (TCP_Server->hasPendingConnections())
{
     Add_New_Client_Connection(TCP_Server->nextPendingConnection());
 }
}

void MainWindow::Read_Data_From_Socket()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) {
        qDebug() << "Error: Could not cast sender to QTcpSocket";
        return;
    }

    if (socket->bytesAvailable() > 0) {
        QByteArray Message_From_Server = socket->readAll();
        if (Message_From_Server.isEmpty()) {
            qDebug() << "Error: Empty message received from client";
            return;
        }

        QString Message = "Client : " + QString::number(socket->socketDescriptor()) + " :: " + QString(Message_From_Server);
        ui->textEdit_Client_Messages->append(Message);
    } else {
        qDebug() << "No data available to read from socket";
    }

    if (socket->error() != QAbstractSocket::UnknownSocketError) {
        qDebug() << "Socket error: " << socket->errorString();
    }
}

void MainWindow::Add_New_Client_Connection(QTcpSocket *socket)
{
    Client_Connection_List.append(socket);
    connect(socket, SIGNAL(readyRead()), this, SLOT(Read_Data_From_Socket()));

    ui->comboBox_Client_list->addItem(QString::number(socket->socketDescriptor()));
     QString Client = "Client : " + QString::number(socket->socketDescriptor()) + " Connected With The Server.";
    ui->textEdit_Client_Messages->append(Client);
}


void MainWindow::on_pushButton_Send_Message_Type_clicked()
{
    QString Message_For_Client = ui->lineEdit_Message_For_Client->text();
    QString Receiver = ui->comboBox_Client_list->currentText();
    if (ui->comboBox_Send_Message_Type->currentText() == "All") // Send Message to all Connected Clients
    {
foreach(QTcpSocket *socket, Client_Connection_List)
{
    socket->write(Message_For_Client.toStdString().c_str());
}
    }
    else //send Message To Only Selected Connected Clients
         {
        foreach(QTcpSocket *socket, Client_Connection_List)
        {
            if (socket->socketDescriptor() == Receiver.toLongLong())
            {
                socket->write(Message_For_Client.toStdString().c_str());
            }
        }

    }
}
void MainWindow::on_pushB_clicked()
{
    QString text = ui->id_rdv->text();

       // Convert the QString to a QByteArray
       QByteArray data = text.toUtf8();

       // Write the data to Arduino
       A.write_to_arduino(data);
}

void MainWindow::on_AscButton_2_clicked()
{
    personnellist p;
    p.show();
}
