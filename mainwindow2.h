#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H
#include <QMainWindow>
#include "pack.h"
#include <QSqlDatabase>
#include <QSql>
#include "arduinoh.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow2; }
QT_END_NAMESPACE

class MainWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow2(QWidget *parent = nullptr);
    ~MainWindow2();
    void showBarChartFromTableData();
    void generateQRCode(const QString &text, const QString &outputFilePath);
private slots:
    void update_label();   // slot permettant la mise à jour du label état de la lampe 1,
        // ce slot est lancé à chaque réception d'un message de Arduino

    void on_pushButton_ajouter_clicked();
    void on_pushButton_supprimer_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_lineEdit_search_textChanged(const QString &arg1);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_4_clicked();

    void on_pushButton_supprimer_2_clicked();

private:
    Ui::MainWindow2 *ui;
    pack Ptmp;
    QByteArray data; // variable contenant les données reçues
    Arduinoh A;
};

#endif // MAINWINDOW2_H
