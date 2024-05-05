#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "homepage.h"

login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_log_clicked()
{
    // Retrieve username and password from line edits
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

    // Prepare the database query
    QSqlQuery query;
    query.prepare("SELECT * FROM login WHERE mail = :username AND pass = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    // Execute the query
    if (query.exec() && query.next()) {
        // Login successful
        QMessageBox::information(this, "Login", "Login successful!");

        // Close the login window
        this->close();

        // Open the homepage window
        homepage *p = new homepage();
        p->show();
    } else {
        // Login failed
        QMessageBox::warning(this, "Login", "Invalid username or password!");
        QMessageBox::critical(this, "Error", "Database error: " + query.lastError().text());
    }

}
