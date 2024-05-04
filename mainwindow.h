
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDialog>
#include <QDialog>
#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "rdv.h"
#include <QCompleter>
#include <QDirModel>
#include<QTcpSocket>
#include <QDebug>
#include <QString>
#include <QDateTime>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QList>
#include <QByteArray>
#include <QString>
#include <QMessageBox>
#include "arduino.h"
#include <arduino.h>
#include <QMainWindow>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pb_supprimer_clicked();

    void on_pb_modifier_2_clicked();

    void on_pb_ajouter_clicked();

    void handleMessageBoxResult(int result);

    void on_calendarWidget_clicked(const QDate &date);

    void on_calendarWidget_currentPageChanged(int year, int month);

    void on_tab_rd_clicked(const QModelIndex &index);


void displayTable();




void on_lineEdit_textChanged(const QString &arg1);

void on_QR_clicked();

void sendMail(const QString &to, const QString &subject, const QString &message);
void mailSent(QString status);
void on_sendBtn2_clicked();
void on_AscButton_clicked();

void on_DescButton_clicked();

void on_pb_pdf_clicked();

void on_Imprimer_clicked();

void on_pushButton_clicked();

void on_stattype_clicked();
void Stat();
void on_statduree_clicked();
void StatDuree();
void on_pushButton_2_clicked();

void on_pushB_clicked();
void newConnection();
void Read_Data_From_Socket();
void on_pushButton_Send_Message_Type_clicked();
void on_AscButton_2_clicked();

private:
  void Add_New_Client_Connection(QTcpSocket *socket);
private:
  Ui::MainWindow *ui;
  QCompleter* Model_Completer;
  QTcpServer *TCP_Server;
  QList<QTcpSocket*>   Client_Connection_List;

    rdv *R;
     QByteArray data;

    Arduino A;

};
#endif // MAINWINDOW_H
