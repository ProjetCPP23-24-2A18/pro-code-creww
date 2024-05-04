#include "dumessengerconnectiondialog.h"
#include "ui_dumessengerconnectiondialog.h"
#include <qdebug.h>
#include "mainwindow.h"
DuMessengerConnectionDialog::DuMessengerConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DuMessengerConnectionDialog)
{
    ui->setupUi(this);

    // Set default host and port
    ui->hostname->setText("localhost"); // Set the default hostname
    ui->port->setValue(8080); // Set the default port

    // Automatically accept the dialog
    accept();
}

DuMessengerConnectionDialog::~DuMessengerConnectionDialog()
{
    delete ui;
}
