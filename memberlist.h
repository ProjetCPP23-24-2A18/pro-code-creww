#ifndef MEMBERLIST_H
#define MEMBERLIST_H
#include <QMainWindow>
#include <QSqlTableModel>
#include <QStringListModel>
#include <QItemSelectionModel>



QT_BEGIN_NAMESPACE
namespace Ui { class memberlist; }
QT_END_NAMESPACE

class memberlist : public QMainWindow
{
    Q_OBJECT

public:
    memberlist(QWidget *parent = nullptr);
    ~memberlist();

private slots:

    void on_confirmButton_clicked();

    void on_pushButton_4_clicked();



    void on_modifyButton_clicked();




    void on_pushButton_22_clicked();


    void on_cancelButton_clicked();



    void on_deleteButton_clicked();





    void on_searchButton_clicked();

    void on_pdfExcelButton_clicked();
    void exportToPDF();
    void exportToExcel();
    void applySort();
    void printMembers();

    void on_statisticsButton_clicked();

    void on_statAge_clicked();

    void on_statTimePeriod_clicked();
    void clearChart();
    void clearChartmember();
    void sendConfirmationEmail(const QString &email, const QString &name, const QString &lastName, const QString &memberId);
    void mailStatus(const QString &status) ;
    void on_uploadImageButton_clicked();
    void displaySelectedMemberImage(int memberId);
    void on_tableView_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void on_modifyButton_3_clicked();

    void on_statAge_3_clicked();

private:
    Ui::memberlist *ui;
    QSqlTableModel *membersModel;
    QStringListModel* timeStatsModel;
     QString selectedMemberId;
     QString imagePath;
     QString memberImagePath;
     bool isCreatingMember;



};


#endif // MEMBERLIST_H
