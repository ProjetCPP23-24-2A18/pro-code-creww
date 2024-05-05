#include "homepage.h"
#include "ui_homepage.h"
#include "personnellist.h"
#include "memberlist.h"
#include "mainwindow.h"
#include "mainwindow1.h"
#include "mainwindow2.h"

homepage::homepage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::homepage)
{
    ui->setupUi(this);
}

homepage::~homepage()
{
    delete ui;
}

void homepage::on_employehp_clicked()
{
    // Instantiate the personnel list window
      personnellist *w = new personnellist(this);
      // Show the personnel list window
      w->show();
}

void homepage::on_patienthp_clicked()
{
      memberlist *t = new memberlist(this);
      t->show();
}

void homepage::on_rdvhp_clicked()
{
      MainWindow *m = new MainWindow(this);
      m->show();
}

void homepage::on_materialhp_clicked()
{
    MainWindow1 *n = new MainWindow1(this);
    n->show();
}

void homepage::on_packshp_clicked()
{
    MainWindow2 *h = new MainWindow2(this);
    h->show();
}
