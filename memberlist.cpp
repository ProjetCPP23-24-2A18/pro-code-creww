#include "memberlist.h"
#include "ui_memberlist.h"
#include "connection.h"
#include "member.h"
#include "smtpt.h"
#include "ImageDelegate.h"

#include <QDebug>
#include <QMessageBox>
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

#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>

#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtWidgets/QVBoxLayout>

#include <QtCharts/QBarSeries>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>

#include <QFileDialog>
#include <QPixmap>



memberlist::memberlist(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::memberlist) , timeStatsModel(nullptr)
    {

    ui->setupUi(this);
    membersModel = new QSqlTableModel(this);
        membersModel->setTable("MEMBERS");
        membersModel->select();
        ui->tableView->setModel(membersModel);
        ui->tableView->setItemDelegateForColumn(11, new ImageDelegate(this));  // Assuming column 11 has the images
        ui->tableView->resizeColumnsToContents();



    connect(ui->confirmButton, &QPushButton::clicked, this, &memberlist::on_confirmButton_clicked);

    connect(ui->cancelButton, &QPushButton::clicked, this, &memberlist::on_cancelButton_clicked);

    connect(ui->modifyButton, &QPushButton::clicked, this, &memberlist::on_modifyButton_clicked);

    connect(ui->deleteButton, &QPushButton::clicked, this, &memberlist::on_deleteButton_clicked);

    connect(ui->filterButton, SIGNAL(clicked()), this, SLOT(applySort()));

    connect(ui->printButton, SIGNAL(clicked()), this, SLOT(printMembers()));

    //connect(ui->uploadImageButton, &QPushButton::clicked, this, &memberlist::on_uploadImageButton_clicked);

}
memberlist::~memberlist()
{
    delete ui;

}



void memberlist::on_pushButton_4_clicked()
{
    isCreatingMember = true;
     ui->stackedWidget->setCurrentIndex(1);
}



void memberlist::on_modifyButton_clicked()
{
    isCreatingMember = false;
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
        if (!selectedRows.isEmpty()) {
            int row = selectedRows.first().row();
            selectedMemberId = membersModel->data(membersModel->index(row, 0)).toString();


            ui->lineEdit_id->setText(selectedMemberId);
            ui->lineEdit_name->setText(membersModel->data(membersModel->index(row, 1)).toString());
            ui->lineEdit_lastName->setText(membersModel->data(membersModel->index(row, 2)).toString());
            ui->dateEdit_dateOfBirth->setDate(membersModel->data(membersModel->index(row, 3)).toDate());
            ui->lineEdit_address->setText(membersModel->data(membersModel->index(row, 4)).toString());
            ui->lineEdit_phone->setText(membersModel->data(membersModel->index(row, 5)).toString());
            ui->lineEdit_email->setText(membersModel->data(membersModel->index(row, 6)).toString());
            ui->dateEdit_rdv->setDate(membersModel->data(membersModel->index(row, 7)).toDate());




            QString imagePath = membersModel->data(membersModel->index(row, 11)).toString(); // Assuming 11 is the image path column
                    QPixmap image(imagePath);
                    if (!image.isNull()) {
                        ui->imageLabel->setPixmap(image.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    } else {
                        qDebug() << "Image at path" << imagePath << "could not be loaded.";
                        ui->imageLabel->setText("No image available");
                    }

                    ui->lineEdit_id->setReadOnly(true);
                    ui->stackedWidget->setCurrentIndex(2);
                } else {
                    QMessageBox::information(this, "No Selection", "Please select a member to modify.");
                }

}



void memberlist::on_confirmButton_clicked()
{

    QString id = ui->lineEdit_id->text();
    QString name = ui->lineEdit_name->text();
    QString lastName = ui->lineEdit_lastName->text();
    QDate dateOfBirth = ui->dateEdit_dateOfBirth->date();
    QString address = ui->lineEdit_address->text();
    QString phone = ui->lineEdit_phone->text();
    QString email = ui->lineEdit_email->text();
    QDate rdv = ui->dateEdit_rdv->date();

    bool ok;

    if (!id.toInt(&ok) || !ok) {
        QMessageBox::critical(this, "Invalid Input", "ID must contain only Numbers.");
        return;
    }


    if (isCreatingMember) {
        QSqlQuery checkIdQuery;
        checkIdQuery.prepare("SELECT COUNT(*) FROM members WHERE ID = ?");
        checkIdQuery.addBindValue(id);
        if (checkIdQuery.exec()) {
            if (checkIdQuery.next() && checkIdQuery.value(0).toInt() > 0) {
                QMessageBox::critical(this, "Invalid Input", "ID is already exist.");
                return;
            }
        } else {
            QMessageBox::critical(this, "Database Error", "Failed to check ID uniqueness: " + checkIdQuery.lastError().text());
            return;
        }


    if (name.isEmpty() || name.toInt(&ok), ok) {
           QMessageBox::critical(this, "Invalid Input", "Name must contain only letters.");
           return;
       }

    QRegExp lastNameRegExp("^[A-Za-z]+$");
    if (!lastNameRegExp.exactMatch(lastName)) {
        QMessageBox::critical(this, "Invalid Input", "Last name must contain only letters.");
        return;
    }

    if (address.isEmpty()) {
        QMessageBox::critical(this, "Invalid Input", "Address cannot be empty.");
        return;
    }

    QRegExp phoneRegExp("^\\d{8}$");
    if (!phoneRegExp.exactMatch(phone)) {
        QMessageBox::critical(this, "Invalid Input", "Phone number must be exactly 8 Number.");
        return;
    }
    }
    if (isCreatingMember) {
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM members WHERE PHONE = ?");
        checkQuery.addBindValue(phone);
        if (checkQuery.exec()) {
            if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
                QMessageBox::critical(this, "Invalid Input", "Phone number is already exists.");
                return;
            }
        } else {
            QMessageBox::critical(this, "Database Error", "Failed to check phone number uniqueness: " + checkQuery.lastError().text());
            return;
        }
    }

    //QRegExp emailRegExp("^[\\w-\\.]+@[\\w-]+\\.[\\w-]{2,4}$");
   // if (!emailRegExp.exactMatch(email)) {
      //  QMessageBox::critical(this, "Invalid Input", "Email must be in the format example@something.com");
      //  return;
   // }



    if (isCreatingMember) {
        QSqlQuery checkEmailQuery;
        checkEmailQuery.prepare("SELECT COUNT(*) FROM members WHERE EMAIL = ?");
        checkEmailQuery.addBindValue(email);
        if (checkEmailQuery.exec()) {
            if (checkEmailQuery.next() && checkEmailQuery.value(0).toInt() > 0) {
                QMessageBox::critical(this, "Invalid Input", "Email is already exists.");
                return;
            }
        } else {
            QMessageBox::critical(this, "Database Error", "Failed to check email uniqueness: " + checkEmailQuery.lastError().text());
            return;
        }







    }
    Member newMember(id, name, lastName, dateOfBirth, address, phone, email, rdv, imagePath);


    QSqlQuery query;

    query.prepare("INSERT INTO members (ID, NAME, LAST_NAME, DATE_OF_BIRTH, ADDRESS, PHONE, EMAIL, RDV, IMAGE_PATH, ADDED_DATE) "
                          "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, CURRENT_DATE)");

    query.addBindValue(newMember.getId());
    query.addBindValue(newMember.getName());
    query.addBindValue(newMember.getLastName());
    query.addBindValue(newMember.getDateOfBirth());
    query.addBindValue(newMember.getAddress());
    query.addBindValue(newMember.getPhone());
    query.addBindValue(newMember.getEmail());
    query.addBindValue(newMember.getRdv());

    query.addBindValue(memberImagePath);
    if (query.exec()) {
            QMessageBox::information(this, "Success", "Member added successfully!");
            membersModel->select();

            sendConfirmationEmail(newMember.getEmail(), newMember.getName(), newMember.getLastName(), newMember.getId());


            ui->lineEdit_id->clear();
            ui->lineEdit_name->clear();
            ui->lineEdit_lastName->clear();
            ui->dateEdit_dateOfBirth->setDate(QDate(2000, 1, 1));
            ui->lineEdit_address->clear();
            ui->lineEdit_phone->clear();
            ui->lineEdit_email->clear();
             ui->dateEdit_rdv->setDate(QDate(2000, 1, 1));
            ui->imageLabel->clear();
            memberImagePath.clear();
             ui->stackedWidget->setCurrentIndex(1);
        } else {

        query.prepare("UPDATE members SET NAME = ?, LAST_NAME = ?, DATE_OF_BIRTH = ?, ADDRESS = ?, PHONE = ?, EMAIL = ?, RDV = ?, IMAGE_PATH = ? WHERE ID = ?");
        query.addBindValue(id);
        query.addBindValue(name);
        query.addBindValue(lastName);
        query.addBindValue(dateOfBirth);
        query.addBindValue(address);
        query.addBindValue(phone);
        query.addBindValue(email);
        query.addBindValue(rdv);
        query.addBindValue(memberImagePath);
        query.addBindValue(selectedMemberId);

        if (query.exec()) {
            QMessageBox::information(this, "Success", "Member updated successfully!");
            membersModel->select();
            ui->lineEdit_id->clear();
            ui->lineEdit_name->clear();
            ui->lineEdit_lastName->clear();
            ui->dateEdit_dateOfBirth->setDate(QDate(2000, 1, 1));
            ui->lineEdit_address->clear();
            ui->lineEdit_phone->clear();
            ui->lineEdit_email->clear();
            ui->dateEdit_rdv->setDate(QDate(2000, 1, 1));
            ui->imageLabel->clear();
            memberImagePath.clear();
            ui->stackedWidget->setCurrentIndex(1);


        }
    }
  }

void memberlist::on_pushButton_22_clicked()
{
    membersModel = new QSqlTableModel(this);
    membersModel->setTable("MEMBERS");
    membersModel->select();


    ui->tableView->setModel(membersModel);

    ui->tableView->resizeColumnsToContents();

    if (membersModel->rowCount() == 0) {
        qDebug() << "The model is empty or not properly set up.";
        qDebug() << "Error:" << membersModel->lastError().text();
    }

    membersModel->select();
    ui->stackedWidget->setCurrentIndex(1);
}

void memberlist::on_cancelButton_clicked()
{


        ui->lineEdit_id->clear();
        ui->lineEdit_name->clear();
        ui->lineEdit_lastName->clear();
        ui->dateEdit_dateOfBirth->setDate(QDate(2000, 1, 1));
        ui->lineEdit_address->clear();
        ui->lineEdit_phone->clear();
        ui->lineEdit_email->clear();
        ui->dateEdit_rdv->setDate(QDate(2000, 1, 1));
        ui->imageLabel->clear();
        memberImagePath.clear();
        ui->stackedWidget->setCurrentIndex(1);
        ui->lineEdit_id->setReadOnly(false);

}



void memberlist::on_deleteButton_clicked()
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
        if (!selectedRows.isEmpty()) {
            int row = selectedRows.first().row();
            QString memberId = membersModel->data(membersModel->index(row, 0)).toString();


            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Delete Member", "Are you sure you want to delete this member?",
                                          QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {

                QSqlQuery query;
                query.prepare("DELETE FROM members WHERE ID = ?");
                query.addBindValue(memberId);
                if (query.exec()) {
                    QMessageBox::information(this, "Success", "Member deleted successfully!");
                    membersModel->select();
                } else {
                    QMessageBox::information(this, "Error", "Failed to delete member: " + query.lastError().text());
                }
            }
        } else {
            QMessageBox::information(this, "No Selection", "Please select a member to delete.");
        }
    }




void memberlist::on_searchButton_clicked()
{
    membersModel->setTable("MEMBERS");
        membersModel->select();

        QString searchQuery = ui->searchLineEdit->text().toLower(); // Convert search query to lowercase

           QString selectQuery = "LOWER(NAME) LIKE '%" + searchQuery + "%' OR LOWER(LAST_NAME) LIKE '%" + searchQuery + "%' OR ID LIKE '%" + searchQuery + "%'";
           membersModel->setFilter(selectQuery);
        membersModel->select();

        ui->searchLineEdit->clear(); // Clear search line
        ui->tableView->resizeColumnsToContents();
}

void memberlist::on_pdfExcelButton_clicked()
{
    QMessageBox msgBox;
        msgBox.setText("Choose the format to export:");
        QPushButton *pdfButton = msgBox.addButton(tr("PDF"), QMessageBox::ActionRole);
        QPushButton *excelButton = msgBox.addButton(tr("Excel"), QMessageBox::ActionRole);

        msgBox.exec();

        if (msgBox.clickedButton() == pdfButton) {
            exportToPDF();
        } else if (msgBox.clickedButton() == excelButton) {
            exportToExcel();
        }
}

void memberlist::exportToPDF() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export PDF"), QString(), "*.pdf");
        if (fileName.isEmpty()) {
            return; // User canceled save
        }

        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);
        printer.setOutputFileName(fileName);

        QTextDocument document;

        QString htmlString = "<html><body><table border='1' cellspacing='0' cellpadding='3'>";

        // Assuming the model is not nullptr and has at least one row and one column
        QAbstractItemModel *model = ui->tableView->model();
        int rows = model->rowCount();
        int columns = model->columnCount();

        // Add header row
        htmlString += "<tr>";
        for (int j = 0; j < columns; ++j) {
            htmlString += "<th>" + model->headerData(j, Qt::Horizontal).toString() + "</th>";
        }
        htmlString += "</tr>";

        // Add data rows
        for (int i = 0; i < rows; ++i) {
            htmlString += "<tr>";
            for (int j = 0; j < columns; ++j) {
                QModelIndex index = model->index(i, j);
                QString cellText = model->data(index).toString();
                htmlString += "<td>" + cellText + "</td>";
            }
            htmlString += "</tr>";
        }

        htmlString += "</table></body></html>";

        document.setHtml(htmlString);

        document.print(&printer);
}

void memberlist::exportToExcel() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Excel"), QString(), "*.csv");
    if (fileName.isEmpty()) {
        return; // User canceled save
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot create file");
        return;
    }

    QTextStream stream(&file);
    const int columnCount = membersModel->columnCount();
    const int rowCount = membersModel->rowCount();

    // Write headers
    for (int i = 0; i < columnCount; ++i) {
        stream << '"' << membersModel->headerData(i, Qt::Horizontal).toString() << '"';
        if (i < columnCount - 1) {
            stream << ",";
        } else {
            stream << "\n";
        }
    }

    // Write data
    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < columnCount; ++j) {
            stream << '"' << membersModel->data(membersModel->index(i, j)).toString().replace("\"", "\"\"") << '"';
            if (j < columnCount - 1) {
                stream << ",";
            } else {
                stream << "\n";
            }
        }
    }

    file.close();
}


void memberlist::applySort() {
    int index = ui->sortComboBox->currentIndex();

    // Clear any existing filters or sorting
    membersModel->setFilter("");
    membersModel->setSort(0, Qt::AscendingOrder); // Reset to the default sort

    switch (index) {
        case 0:
        break;
        case 1: // Sort by increasing Creation Date
            membersModel->setSort(3, Qt::AscendingOrder);
            break;
        case 2: // Sort by descending Creation Date
            membersModel->setSort(3, Qt::DescendingOrder);
            break;
        case 3: // Sort in ascending alphabetical order
             membersModel->setSort(1, Qt::AscendingOrder);
             break;
        case 4: // Sort in descending alphabetical order
             membersModel->setSort(1, Qt::DescendingOrder);
             break;
        default:
            // Handle any other cases or errors
            break;
    }
    ui->sortComboBox->setCurrentIndex(0);
    membersModel->select(); // Apply the sort/filter
}

void memberlist::printMembers() {
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    printer.setFullPage(false);

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);

        QRect printArea = printer.pageRect();

        // Get the size of the content you want to print
        QSize tableViewSize = ui->tableView->sizeHint();

        // Scale the content to fit within the print area
        double xscale = printArea.width() / double(tableViewSize.width());
        double yscale = printArea.height() / double(tableViewSize.height());
        double scale = qMin(xscale, yscale);

        // Translate the painter to start drawing from the top left of the print area
        painter.translate(printArea.topLeft());

        // Apply the scale
        painter.scale(scale, scale);

        // Render the table view onto the painter
        ui->tableView->render(&painter);
    }
}




void memberlist::on_statAge_clicked()
{
        clearChart();
        clearChartmember();
        // Ensure the model is initially displaying all members
        membersModel->setTable("MEMBERS");
        membersModel->select();

        // Get current date for age calculation
        QDate currentDate = QDate::currentDate();

        // Initialize counters for each age group
        int under18 = 0;
        int group18_30 = 0;
        int group31_45 = 0;
        int group46_60 = 0;
        int above60 = 0;

        for (int row = 0; row < membersModel->rowCount(); ++row) {
            QDate dob = membersModel->record(row).value("DATE_OF_BIRTH").toDate();
            int age = currentDate.year() - dob.year(); // Calculate age
            if (dob > currentDate.addYears(-age)) age--; // Adjust if the birthday hasn't occurred yet this year

            // Increment the appropriate age group counter
            if (age < 18) {
                under18++;
            } else if (age >= 18 && age <= 30) {
                group18_30++;
            } else if (age >= 31 && age <= 45) {
                group31_45++;
            } else if (age >= 46 && age <= 60) {
                group46_60++;
            } else if (age > 60) {
                above60++;
            }
        }

        // Prepare the statistical data for the list view
        QStringList ageStats;
        ageStats << QString("Under 18: %1").arg(under18);
        ageStats << QString("18-30: %1").arg(group18_30);
        ageStats << QString("31-45: %1").arg(group31_45);
        ageStats << QString("46-60: %1").arg(group46_60);
        ageStats << QString("60+: %1").arg(above60);

        // Assuming you have a QStringListModel* named statsModel that's already connected to your list view
        QStringListModel* ageStatsModel = new QStringListModel(this);
        ageStatsModel->setStringList(ageStats);

        // Assuming ui->listView is your QListView for displaying the stats
        ui->listView->setModel(ageStatsModel);

        // Optionally, you may want to resize the columns to fit the content
        ui->listView->setResizeMode(QListView::Adjust);

        QBarSet *set = new QBarSet("Age Groups");
            *set << under18 << group18_30 << group31_45 << group46_60 << above60;

            // Create a bar series and add the bar set
            QBarSeries *series = new QBarSeries();
            series->append(set);

            // Create a chart and add the series to it
            QChart *chart = new QChart();
            chart->addSeries(series);
            chart->setTitle("Age Distribution");
            chart->setAnimationOptions(QChart::SeriesAnimations);

            // Create the axis and set the labels
            QStringList categories;
            categories << "Under 18" << "18-30" << "31-45" << "46-60" << "60+";
            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            axisX->append(categories);
            chart->createDefaultAxes();
            chart->setAxisX(axisX, series);

            // Make the chart legend visible
            chart->legend()->setVisible(true);
            chart->legend()->setAlignment(Qt::AlignBottom);

            // Create a chart view with the chart
            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);

            // Add the chart view to the placeholder widget's layout
            QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->placeholderWidget->layout());
            if (!layout) {
                layout = new QVBoxLayout();
                ui->placeholderWidget->setLayout(layout);

            }
            layout->addWidget(chartView);

            // Set the string list to the model and then set the model to the list view
            ui->listView->setModel(ageStatsModel);


}

void memberlist::on_statTimePeriod_clicked()
{
    clearChart();
    // Variables to hold counts
        int membersAddedToday = 0;
        int membersAddedThisWeek = 0;
        int membersAddedThisMonth = 0;

        // Get the current date
        QDate currentDate = QDate::currentDate();
        QDate startOfWeek = currentDate.addDays(-currentDate.dayOfWeek() + 1); // Adjust to the first day of the current week
        QDate startOfMonth = currentDate.addDays(-currentDate.day() + 1); // First day of the month

        // Format dates as strings
        QString todayDateString = currentDate.toString("yyyy-MM-dd");
        QString thisWeekDateString = QString("%1 to %2").arg(startOfWeek.toString("yyyy-MM-dd"), currentDate.toString("yyyy-MM-dd"));
        QString thisMonthDateString = QString("%1 to %2").arg(startOfMonth.toString("yyyy-MM-dd"), currentDate.toString("yyyy-MM-dd"));

        // Query for members added today
        QSqlQuery queryToday;
        queryToday.prepare("SELECT COUNT(*) FROM MEMBERS WHERE TRUNC(ADDED_DATE) = TRUNC(SYSDATE)");
        if (queryToday.exec() && queryToday.next()) {
            membersAddedToday = queryToday.value(0).toInt();
        }

        // Query for members added this week
        QSqlQuery queryThisWeek;
        queryThisWeek.prepare("SELECT COUNT(*) FROM MEMBERS WHERE TRUNC(ADDED_DATE) >= TRUNC(:startOfWeek) AND TRUNC(ADDED_DATE) <= TRUNC(SYSDATE)");
        queryThisWeek.bindValue(":startOfWeek", startOfWeek);
        if (queryThisWeek.exec() && queryThisWeek.next()) {
            membersAddedThisWeek = queryThisWeek.value(0).toInt();
        }

        // Query for members added this month
        QSqlQuery queryThisMonth;
        queryThisMonth.prepare("SELECT COUNT(*) FROM MEMBERS WHERE TRUNC(ADDED_DATE) >= TRUNC(:startOfMonth) AND TRUNC(ADDED_DATE) <= TRUNC(SYSDATE)");
        queryThisMonth.bindValue(":startOfMonth", startOfMonth);
        if (queryThisMonth.exec() && queryThisMonth.next()) {
            membersAddedThisMonth = queryThisMonth.value(0).toInt();
        }

        // Prepare the statistical data for the list view
        QStringList stats;
        stats << QString("Members added on %1: %2").arg(todayDateString).arg(membersAddedToday);
        stats << QString("Members added during the week of %1: %2").arg(thisWeekDateString).arg(membersAddedThisWeek);
        stats << QString("Members added during the month of %1: %2").arg(thisMonthDateString).arg(membersAddedThisMonth);

        // Create a QStringListModel to hold the stats if it does not already exist
        if (!timeStatsModel) {
            timeStatsModel = new QStringListModel(this);
        }

        // Set the string list to the model and then set the model to the list view
        timeStatsModel->setStringList(stats);
        ui->listView->setModel(timeStatsModel);
        QBarSet *barSet = new QBarSet("Members");
            *barSet << membersAddedToday << membersAddedThisWeek << membersAddedThisMonth;

            QBarSeries *series = new QBarSeries();
            series->append(barSet);

            QChart *chart = new QChart();
            chart->addSeries(series);
            chart->setTitle("Member Statistics");
            chart->setAnimationOptions(QChart::SeriesAnimations);

            QStringList categories;
            categories << "Today" << "This Week" << "This Month";
            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            axisX->append(categories);
            chart->createDefaultAxes();
            chart->setAxisX(axisX, series);

            chart->legend()->setVisible(true);
            chart->legend()->setAlignment(Qt::AlignBottom);

            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);

            // Assuming you have a placeholder widget in your UI to display the chart
            // Remove any existing chart first
            QLayoutItem *child;
            while ((child = ui->placeholderWidget->layout()->takeAt(0)) != 0) {
                delete child->widget(); // this will delete the chart view
                delete child;
            }

            // Add the new chart to the placeholder widget
            ui->placeholderWidget->layout()->addWidget(chartView);


            // Set the string list to the model and then set the model to the list view
            timeStatsModel->setStringList(stats);
            ui->listView->setModel(timeStatsModel);
}

void memberlist::clearChart()
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->chartFrame->layout());
    if (layout) {
        QLayoutItem *child;
        while ((child = layout->takeAt(0)) != nullptr) {
            if (child->widget()) {
                delete child->widget(); // This will delete the chart view
            }
            delete child;
        }
    }
}


void memberlist::clearChartmember()
{
    // Assuming 'placeholderWidget' is a QWidget or QFrame in your UI where the stats are displayed
    if (ui->placeholderWidget->layout() != nullptr) {
        // Remove and delete all widgets from the layout
        QLayoutItem *child;
        while ((child = ui->placeholderWidget->layout()->takeAt(0)) != nullptr) {
            delete child->widget(); // This will delete the chart view
            delete child;           // This will delete the layout item
        }
    }

    // Optionally, remove the layout itself
    delete ui->placeholderWidget->layout();
    ui->placeholderWidget->setLayout(new QVBoxLayout()); // Set a new clean layout
}



void memberlist::sendConfirmationEmail(const QString &email, const QString &name, const QString &lastName, const QString &memberId)
{
    // Assuming smtp is an existing class you have for sending emails and isValidEmail is a function you've implemented to validate emails
    if (!email.isEmpty()) {
        smtpt* mailSender = new smtpt("yckenway7@gmail.com", "njfs nwqt iomy wddj", "smtp.gmail.com", 465);
        connect(mailSender, SIGNAL(status(QString)), this, SLOT(mailStatus(QString)));

        QString subject = "Confirmation of Membership";
        QString body = QString("Dear %1 %2,\n\n"
                               "Your membership with ID %3 has been successfully created. "
                               "Please proceed with your payment to activate your account.\n\n"
                               "Best Regards,\nYour Team").arg(name, lastName, memberId);

        mailSender->sendMail("yckenway7@gmail.com", email, subject, body);
    } else {
        qDebug() << "Invalid email address provided:" << email;
    }
}

void memberlist::mailStatus(const QString &status) {
    qDebug() << "Email status:" << status;
}

void memberlist::on_uploadImageButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose Image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));
        if (!fileName.isEmpty()) {
            QPixmap pixmap(fileName);
            ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio));
            memberImagePath = fileName;

        // If you're creating a new member, just store the path in the member object
        // If updating, you'll need to update the database record for the member
        if (!isCreatingMember) {
            QSqlQuery query;
            query.prepare("UPDATE members SET IMAGE_PATH = ? WHERE ID = ?");
            query.addBindValue(memberImagePath);
            query.addBindValue(selectedMemberId);
            if (!query.exec()) {
                qDebug() << "Error updating member's image path:" << query.lastError();
            } else {
                // Refresh your model to update the view
                membersModel->select();
            }
        }
    }
}


void memberlist::displaySelectedMemberImage(int memberId) {
    // Retrieve the member's image path from the database and display it
    QSqlQuery query;
    query.prepare("SELECT IMAGE_PATH FROM members WHERE ID = ?");
    query.addBindValue(memberId);
    if(query.exec() && query.next()) {
        QString imagePath = query.value(0).toString();
        QPixmap image(imagePath);
        ui->imageLabel->setPixmap(image.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

// Call this method when the member selection changes, e.g., when a row in your table is clicked
void memberlist::on_tableView_selectionChanged(const QItemSelection &selected, const QItemSelection &/*deselected*/)  {
    // Assuming you have only one selection at a time
    const QModelIndexList indexes = selected.indexes();
    if (indexes.size() > 0) {
        int memberId = indexes.at(0).data().toInt(); // This assumes the member ID is in the first column
        displaySelectedMemberImage(memberId);
    }
}


void memberlist::on_modifyButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void memberlist::on_statisticsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void memberlist::on_statAge_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
