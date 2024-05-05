#include "ui_personnellist.h"
#include "personnellist.h"
#include "connection.h"
#include "personnel.h"
#include "ImageDelegate.h"
#include "mainwindow.h"
#include "sms.h"
#include <QDebug>
#include <QMessageBox>
#include <QMainWindow>
#include <QSqlTableModel>
#include <QIntValidator>
#include <QTableView>
#include <QSqlRecord>
#include <QPdfWriter>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include <QPainter>
#include <QTextDocument>
#include <QTextStream>
#include <QStringListModel>
#include <QWidget>
#include <QCamera>
#include <QCameraImageCapture>
#include <QFileDialog>
#include <QStandardPaths>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QtWidgets>
#include <QCamera>
#include <QCameraImageCapture>
#include <QImage>
#include <QMessageBox>
#include <QDateTime>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QPixmap>
#include <QTextEdit>
#include "arduinosk.h"



personnellist::personnellist(QWidget *parent)
    : QMainWindow(parent)
    , ui (new Ui::personnellist)
{
    ui->setupUi(this);
    smsManager = new SMS(this);
    personnelsModel = new QSqlTableModel(this);
        personnelsModel->setTable("PERSONNEL");
        personnelsModel->select();
        ui->tableView->setModel(personnelsModel);
        ui->tableView->setItemDelegateForColumn(11, new ImageDelegate(this));
            ui->tableView->resizeColumnsToContents();
            cardLogsModel = new QSqlTableModel(this);
            cardLogsModel->setTable("CARD_LOGS");
            cardLogsModel->select();
            ui->tableView_2->setModel(cardLogsModel);
            arduino = new Arduinosk(this);
            arduino->initPort("COM9");
            connect(ui->addMemberButton, &QPushButton::clicked, this, &personnellist::on_addMemberButton_clicked);
            connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &personnellist::on_tableView_selectionChanged);
            connect(arduino, &Arduinosk::cardScanned, this, &personnellist::onCardScanned);

        connect(ui->confirm, &QPushButton::clicked, this, &personnellist::on_confirm_clicked);

    connect(ui->cancel, &QPushButton::clicked, this, &personnellist::on_cancel_clicked);

    connect(ui->pushButton_14, &QPushButton::clicked, this, &personnellist::on_pushButton_14_clicked);

    connect(ui->pushButton_17, &QPushButton::clicked, this, &personnellist::on_pushButton_17_clicked);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &personnellist::showPage1);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &personnellist::showPage4);
    connect(ui->pushButton_11, &QPushButton::clicked, this, &personnellist::showPage2);
    connect(ui->pushButton_12, &QPushButton::clicked, this, &personnellist::showPage3);
    connect(ui->pushButton_13, &QPushButton::clicked, this, &personnellist::showPage5);
    connect(ui->sms, &QPushButton::clicked, this, &personnellist::showPage6);
    connect(ui->pushButton, &QPushButton::clicked, this, &personnellist::showPage7);


     connect(ui->printButton, &QPushButton::clicked, this, &personnellist::on_printButton_clicked);

     connect(ui->uploadImageButton, &QPushButton::clicked, this, &personnellist::on_uploadImageButton_clicked);
     connect(smsManager, &SMS::finishedSending, this, [](){
             qDebug() << "SMS sending process finished.";
         });
}

personnellist::~personnellist()
{
    delete ui;
    delete smsManager;

}
void personnellist::showPage1() {
    ui->stackedWidget->setCurrentIndex(0);
}

void personnellist::showPage2() {
    //ui->stackedWidget->setCurrentIndex(1);
}

void personnellist::showPage3() {
    ui->stackedWidget->setCurrentIndex(2);
}

void personnellist::showPage4() {
    ui->stackedWidget->setCurrentIndex(3);
}

void personnellist::showPage5() {
    ui->stackedWidget->setCurrentIndex(4);
}
void personnellist::showPage6() {
    ui->stackedWidget->setCurrentIndex(5);
}
void personnellist::showPage7() {
    ui->stackedWidget->setCurrentIndex(6);
}
void personnellist::on_confirm_clicked()
{

    QString id = ui->lineEdit_id->text();
    QString name = ui->lineEdit_name->text();
    QString lastName = ui->lineEdit_lastName->text();
    QDate dateOfBirth = ui->dateEdit_dateOfBirth->date();

    QString address = ui->lineEdit_address->text();
    QString phone = ui->lineEdit_phone->text();
    QString email = ui->lineEdit_email->text();
    QString function = ui->lineEdit_function->text();
    QString imagePath = get_image_path();


    Personnel newPersonnel(id, name, lastName, dateOfBirth, address, phone, email, function, imagePath);

    QSqlQuery query;
    query.prepare("INSERT INTO personnel (ID, NAME, LAST_NAME, DATE_OF_BIRTH, ADDRESS, PHONE, EMAIL, FUNCTION, IMAGE_PATH) "
                  "VALUES (:id, :name, :lastName, :dateOfBirth, :address, :phone, :email, :function, :imagePath)");

    // Bind values using named placeholders
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":lastName", lastName);
    query.bindValue(":dateOfBirth", dateOfBirth); // Assuming QDate is compatible with database date type
    query.bindValue(":address", address);
    query.bindValue(":phone", phone);
    query.bindValue(":email", email);
    query.bindValue(":function", function);

    query.bindValue(":imagePath", imagePath);

    if (query.exec()) {
            QMessageBox::information(this, "Success", "Personnel added successfully!");
            personnelsModel->select();


            ui->lineEdit_id->clear();
            ui->lineEdit_name->clear();
            ui->lineEdit_lastName->clear();
            ui->dateEdit_dateOfBirth->setDate(QDate(2000, 1, 1));

            ui->lineEdit_address->clear();
            ui->lineEdit_phone->clear();
            ui->lineEdit_email->clear();
            ui->lineEdit_function->clear();
            ui->lineEdit_UserName->clear();
            ui->lineEdit_password->clear();
            ui->imageLabel->clear();
            memberImagePath.clear();

        }
}

void personnellist::on_cancel_clicked()
{

    ui->lineEdit_id->clear();
    ui->lineEdit_name->clear();
    ui->lineEdit_lastName->clear();
    ui->dateEdit_dateOfBirth->setDate(QDate(2000, 1, 1));

    ui->lineEdit_address->clear();
    ui->lineEdit_phone->clear();
    ui->lineEdit_email->clear();
    ui->lineEdit_function->clear();

    ui->imageLabel->clear();
    memberImagePath.clear();
       // ui->stackedWidget->setCurrentIndex(1);
        ui->lineEdit_id->setReadOnly(false);

}

void personnellist::on_pushButton_14_clicked()
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
        if (!selectedRows.isEmpty()) {
            int row = selectedRows.first().row();
            selectedPersonnelId = personnelsModel->data(personnelsModel->index(row, 0)).toString(); // Assuming ID is in the first column


            ui->lineEdit_id->setText(selectedPersonnelId);
            ui->lineEdit_name->setText(personnelsModel->data(personnelsModel->index(row, 1)).toString());
            ui->lineEdit_lastName->setText(personnelsModel->data(personnelsModel->index(row, 2)).toString());
            ui->dateEdit_dateOfBirth->setDate(personnelsModel->data(personnelsModel->index(row, 3)).toDate());

            ui->lineEdit_address->setText(personnelsModel->data(personnelsModel->index(row, 5)).toString());
            ui->lineEdit_phone->setText(personnelsModel->data(personnelsModel->index(row, 6)).toString());
            ui->lineEdit_email->setText(personnelsModel->data(personnelsModel->index(row, 7)).toString());
            ui->lineEdit_function->setText(personnelsModel->data(personnelsModel->index(row, 8)).toString());


            ui->lineEdit_id->setReadOnly(true);


            QString imagePath = personnelsModel->data(personnelsModel->index(row, 11)).toString(); // Assuming 11 is the image path column
                    QPixmap image(imagePath);
                    if (!image.isNull()) {
                        ui->imageLabel->setPixmap(image.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    } else {
                        qDebug() << "Image at path" << imagePath << "could not be loaded.";
                        ui->imageLabel->setText("No image available");
                    }

                    ui->lineEdit_id->setReadOnly(true);

        } else {
            QMessageBox::information(this, "No Selection", "Please select a member to modify.");
        }
}

void personnellist::on_pushButton_17_clicked()
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
        if (!selectedRows.isEmpty()) {
            int row = selectedRows.first().row();
            QString personnelId = personnelsModel->data(personnelsModel->index(row, 0)).toString();


            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Delete Personnel", "Are you sure you want to delete this personnel?",
                                          QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {

                QSqlQuery query;
                query.prepare("DELETE FROM personnel WHERE ID = ?");
                query.addBindValue(personnelId);
                if (query.exec()) {
                    QMessageBox::information(this, "Success", "personnel deleted successfully!");
                    personnelsModel->select();
                } else {
                    QMessageBox::information(this, "Error", "Failed to delete personnel: " + query.lastError().text());
                }
            }
        } else {
            QMessageBox::information(this, "No Selection", "Please select a personnel to delete.");
        }
}

void personnellist::on_pushButton_20_clicked()
{
    personnelsModel = new QSqlTableModel(this);
    personnelsModel->setTable("PERSONNEL");
    personnelsModel->select();


    ui->tableView->setModel(personnelsModel);
    ui->tableView->resizeColumnsToContents();

    if (personnelsModel->rowCount() == 0) {
        qDebug() << "The model is empty or not properly set up.";
        qDebug() << "Error:" << personnelsModel->lastError().text();
    }

    personnelsModel->select();

}


void personnellist::on_searchButton_clicked()
{
    QString searchQuery = ui->searchLineEdit->text().trimmed();
    if (!searchQuery.isEmpty())
    {
        QString selectQuery = "ID = '" + searchQuery + "'";
        personnelsModel->setFilter(selectQuery);
        personnelsModel->select();
        if (personnelsModel->rowCount() == 0)
        {
            QMessageBox::information(this, "Not Found", "No PERS found with the provided ID.");
            selectedPersonnelId.clear();
        } else {
            selectedPersonnelId = searchQuery;
            ui->labelMessage->setText("perso with ID " + searchQuery + " found. Ready to add card.");
               }
     } else {
             QMessageBox::warning(this, "Input Error", "Please enter an ID to search.");
             selectedPersonnelId.clear();
            }
}



void personnellist::on_pdfExcelButton_clicked()
{
    QMessageBox msgBox;
        msgBox.setText("Choose the format to export:");
        QPushButton *pdfButton = msgBox.addButton(tr("PDF"), QMessageBox::ActionRole);
        QPushButton *excelButton = msgBox.addButton(tr("Excel"), QMessageBox::ActionRole);

        msgBox.exec();

        if (msgBox.clickedButton() == pdfButton) {
           void exportToPDF(const QString& filename, QTableView* tableView);
        } else if (msgBox.clickedButton() == excelButton) {
            exportToExcel();
        }
        QString filename = QFileDialog::getSaveFileName(this, "Save PDF", "", "*.pdf");
        if (!filename.isEmpty())
        {
            exportToPDF(filename, ui->tableView);
        }
}

void personnellist::exportToPDF(const QString& filename, QTableView* tableView) {
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);


    if (!printer.isValid()) {
        qDebug() << "Printer is not valid. Aborting export to PDF.";
        return;
    }

    QPainter painter(&printer);


    if (!painter.isActive()) {
        qDebug() << "Painter is not active. Aborting export to PDF.";
        return;
    }

    double xscale = printer.pageRect().width() / double(tableView->width());
    double yscale = printer.pageRect().height() / double(tableView->height());
    double scale = qMin(xscale, yscale);
    painter.translate(printer.paperRect().center());
    painter.scale(scale, scale);
    painter.translate(-tableView->width()/2, -tableView->height()/2);

    tableView->render(&painter);

    painter.end();
}
void personnellist::exportToExcel() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Excel"), QString(), "*.csv");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot create file");
        return;
    }

    QTextStream stream(&file);
    const int columnCount = personnelsModel->columnCount();
    const int rowCount = personnelsModel->rowCount();


    for (int i = 0; i < columnCount; ++i) {
        stream << '"' << personnelsModel->headerData(i, Qt::Horizontal).toString() << '"';
        if (i < columnCount - 1) {
            stream << ",";
        } else {
            stream << "\n";
        }
    }


    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < columnCount; ++j) {
            stream << '"' << personnelsModel->data(personnelsModel->index(i, j)).toString().replace("\"", "\"\"") << '"';
            if (j < columnCount - 1) {
                stream << ",";
            } else {
                stream << "\n";
            }
        }
    }

    file.close();
}


void personnellist::on_printButton_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    printer.setFullPage(false);

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);

        QRect printArea = printer.pageRect();


        QSize tableViewSize = ui->tableView->sizeHint();


        double xscale = printArea.width() / double(tableViewSize.width());
        double yscale = printArea.height() / double(tableViewSize.height());
        double scale = qMin(xscale, yscale);


        painter.translate(printArea.topLeft());


        painter.scale(scale, scale);


        ui->tableView->render(&painter);
    }
}
void personnellist::displaySelectedMemberImage(int PersonnelId) {
    // Retrieve the member's image path from the database and display it
    QSqlQuery query;
    query.prepare("SELECT IMAGE_PATH FROM personnel WHERE ID = ?");
    query.addBindValue(PersonnelId);
    if(query.exec() && query.next()) {
        QString imagePath = query.value(0).toString();
        QPixmap image(imagePath);
        ui->imageLabel->setPixmap(image.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

QString personnellist::on_uploadImageButton_clicked() {
    // Open file dialog and get selected image path
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose Image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (!fileName.isEmpty()) {
        // Update UI with image
        QPixmap pixmap(fileName);
        ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio));
        memberImagePath = fileName;
    }
}

QString personnellist::get_image_path() {
    // Open file dialog and get selected image path
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose Image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (!fileName.isEmpty()) {
        return fileName;
    } else {
        // Handle case where user cancels or no file is selected
        return "";  // Or provide a custom message using QMessageBox
    }
}





void personnellist::on_pushButton_2_clicked()

    {
            QString phoneNumber = ui->numero->text(); // Replace with your actual LineEdit for phone number
            QString message = ui->message->text();
             // Replace with your actual TextEdit for message

            if (!phoneNumber.isEmpty() && !message.isEmpty()) {
                smsManager->sendSms(phoneNumber, message);
            } else {
                qDebug() << "Phone number or message is empty.";
            }
    }


void personnellist::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}





void personnellist::onCardScanned(const QString &dataFromArduino)
{
    QString cardID = parseCardID(dataFromArduino);
    if (cardID.isEmpty())
    {
        return;
    }
    QSqlDatabase::database().transaction();
    QSqlQuery checkCardQuery;
    checkCardQuery.prepare("SELECT ID FROM  personnel WHERE CARD_ID = :cardID AND ID != :personnelId");
    checkCardQuery.bindValue(":cardID", cardID);
    checkCardQuery.bindValue(":personnelId", selectedPersonnelId);
    if (!checkCardQuery.exec())
    {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "Database Error", "Could not query the database: " + checkCardQuery.lastError().text());
        return;
    }
    if (checkCardQuery.next())
    {
        QSqlDatabase::database().rollback();
        QMessageBox::warning(this, "Card Error", "This card is already assigned to another member.");
        return;
    }
    QSqlQuery updateMemberQuery;
    updateMemberQuery.prepare("UPDATE personnel SET CARD_ID = :cardID WHERE ID = :personnelId");
    updateMemberQuery.bindValue(":cardID", cardID);
    updateMemberQuery.bindValue(":personnelId", selectedPersonnelId);
    if (!updateMemberQuery.exec())
    {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "Database Error", "Could not save the card to the database: " + updateMemberQuery.lastError().text());
        return;
    }
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM CARD_LOGS WHERE CARD_ID = :cardID");
    checkQuery.bindValue(":cardID", cardID);
    if (!checkQuery.exec())
    {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "Database Error", "Error checking for existing card logs: " + checkQuery.lastError().text());
        return;
    }
    int count = 0;
    if (checkQuery.next())
    {
        count = checkQuery.value(0).toInt();
    }
    QSqlQuery query;
    if (count == 0)
    {
        query.prepare("INSERT INTO CARD_LOGS (CARD_ID, LAST_ACCESS, ACCESS_TIME, ACCESS_STATUS) " "VALUES (:cardID, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP, 'Access granted.')");
    } else {
               query.prepare("UPDATE CARD_LOGS SET LAST_ACCESS = CURRENT_TIMESTAMP, " "ACCESS_TIME = CURRENT_TIMESTAMP, ACCESS_STATUS = 'Access granted.' " "WHERE CARD_ID = :cardID");
           }
    query.bindValue(":cardID", cardID);
     if (!query.exec())
     {
           QSqlDatabase::database().rollback();
           QMessageBox::critical(this, "Database Error", "Could not update the CARD_LOGS table: " + query.lastError().text());
           return;
     }
     QSqlDatabase::database().commit();
     QMessageBox::information(this, "Success", "Access logged for card ID: " + cardID);
     bool cardExists = checkIfCardExists(cardID);
     updateStatusLabelColor(cardExists);
 }
QString personnellist::parseCardID(const QString &dataFromArduino)
{
    QRegularExpression regex("Card ID: (\\w+)");
    QRegularExpressionMatch match = regex.match(dataFromArduino);
    if (match.hasMatch())
    {
        return match.captured(1);
    }
    return QString();
}

void personnellist::on_addMemberButton_clicked()
{
    QString searchQuery = ui->searchLineEdit->text().trimmed();
    if (!searchQuery.isEmpty())
    {
        QMessageBox::warning(this, "No personnel Selected", "Please search and find a member to add a card to.");
    } else {
        ui->labelMessage->setText("No personnel Selected "" Please search and find a member to add a card to..");
           }
}

void personnellist::on_refreche_2_clicked()
{
    cardLogsModel = new QSqlTableModel(this);
    cardLogsModel->setTable("CARD_LOGS");
    cardLogsModel->select();
    ui->tableView_2->setModel(cardLogsModel);
    if (cardLogsModel->rowCount() == 0)
    {
        qDebug() << "The model is empty or not properly set up.";
        qDebug() << "Error:" << cardLogsModel->lastError().text();
    }
    cardLogsModel->select();
    ui->stackedWidget->setCurrentIndex(1);
}

void personnellist::updateStatusLabelColor(bool cardExists)
{
    if (cardExists)
    {
        ui->accessStatusLabel2VERT->setStyleSheet("QLabel { background-color: green; }");
        ui->accessStatusLabelRED->setStyleSheet("");
    } else {
                 ui->accessStatusLabelRED->setStyleSheet("QLabel { background-color: red; }");
                 ui->accessStatusLabel2VERT->setStyleSheet("");
           }
}

bool personnellist::checkIfCardExists(const QString& cardID) {
    QSqlQuery query;

    query.prepare("SELECT COUNT(*) FROM PERSONNEL WHERE CARD_ID = :cardID");
    query.bindValue(":cardID", cardID);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count > 0; // If count is greater than 0, the card ID exists in the PERSONNEL table
    } else {
        qDebug() << "Error checking if card exists in PERSONNEL table:" << query.lastError().text();
        return false; // Assume the card ID does not exist if there's an error
    }
}

void personnellist::on_tableView_selectionChanged(const QItemSelection &selected, const QItemSelection &/*deselected*/)
{
    const QModelIndexList indexes = selected.indexes();
    if (indexes.size() > 0)
    {
        int personnelId = indexes.at(0).data().toInt();
        displaySelectedMemberImage(personnelId);
    }
}


void personnellist::on_pushButton_clicked()
{

}

void personnellist::on_pushButton_11_clicked()
{MainWindow w;
    w.show();

}
