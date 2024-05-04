
#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H
#include <QDialog>
#include <QDialog>
#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "rdv1.h"
#include "audioplayer.h"
#include "arduino.h"
#include <QMainWindow>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow1; }
QT_END_NAMESPACE

class MainWindow1 : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow1(QWidget *parent = nullptr);
    ~MainWindow1();


private slots:
    void on_pb_supprimer_clicked();

    void on_pb_modifier_2_clicked();

    void on_pb_ajouter_clicked();

    void handleMessageBoxResult(int result);



    void on_tab_mat_clicked(const QModelIndex &index);


void displayTable();




void on_lineEdit_textChanged(const QString &arg1);



void sendMail(const QString &to, const QString &subject, const QString &message);
void mailSent(QString status);
void on_sendBtn2_clicked();
void on_AscButton_clicked();

void on_DescButton_clicked();

void on_pb_pdf_clicked();

void on_Imprimer_clicked();
void on_pushsegment_clicked();
void on_pushButton_clicked();
void StatEtat();

void on_statetat_clicked();
void StatDESIGNATION();
void on_statDESIGNATION_clicked();
void on_sendNotification_clicked();



void on_selectbutton_clicked();

void on_playbutton_clicked();

void on_pauseButton_clicked();

void on_volume_valueChanged(int value);

void set_timelineSliderValue();

void on_timeline_valueChanged(int value);

void set_durationText();

void set_currentPositionText(int value);

void on_loop_clicked(bool checked);
void on_pushButton_3_clicked();

private:
  Ui::MainWindow1 *ui;

   Audioplayer audioplayer;
    rdv1 *R;

    QByteArray data; // variable contenant les données reçues

    Arduino A;

};
#endif // MAINWINDOW1_H
