#ifndef PERSONNELLIST_H
#define PERSONNELLIST_H
#include <QSqlTableModel>
#include <QMainWindow>
#include <QStringListModel>
#include <QItemSelectionModel>
#include <QTableView>
#include <sms.h>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "arduinosk.h"
QT_BEGIN_NAMESPACE
namespace Ui { class personnellist; }
QT_END_NAMESPACE

class personnellist : public QMainWindow
{
    Q_OBJECT

public:
    personnellist(QWidget *parent = nullptr);
    ~personnellist();
    QString parseCardID(const QString &dataFromArduino);

public slots:
    void on_tableView_selectionChanged(const QItemSelection &selected, const QItemSelection &/*deselected*/)  ;

private slots:




    void on_confirm_clicked();

    void on_cancel_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_20_clicked();

    void showPage1();

    void showPage2();

    void showPage3();

    void showPage4();

    void showPage5();
    void showPage6();

    void on_searchButton_clicked();
    void on_pushButton_3_clicked();


    void on_pdfExcelButton_clicked();
    void exportToPDF(const QString& filename, QTableView* tableView);
    void exportToExcel();
    void on_printButton_clicked();


    QString on_uploadImageButton_clicked()  ;
    void displaySelectedMemberImage(int personnelId);


    QString captureImageFromCamera();
    QString get_image_path();

    void on_sms_clicked();

    void on_pushButton_2_clicked();
    void on_refreche_2_clicked();
    void showPage7();




    void onCardScanned(const QString &cardID);
    void on_addMemberButton_clicked();
    void updateStatusLabelColor(bool cardExists);

    void on_pushButton_clicked();

    void on_pushButton_11_clicked();

private:
    Ui::personnellist *ui;
    SMS *smsManager;
    QSqlTableModel *personnelsModel;
    QSqlTableModel *cardLogsModel;
    QString selectedPersonnelId;
    QString imagePath;
    QString memberImagePath;
    bool isCreatingPersonnel;
    Arduinosk *arduino;
    bool checkIfCardExists(const QString& cardID);
    void updateEmployeePhotoPath(int personnelId, const QString &ImagePath);
};
#endif // PERSONNELLIST_H
