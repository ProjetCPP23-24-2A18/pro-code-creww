#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>

namespace Ui {
class homepage;
}

class homepage : public QMainWindow
{
    Q_OBJECT

public:
    explicit homepage(QWidget *parent = nullptr);
    ~homepage();

private slots:
    void on_employehp_clicked();

    void on_patienthp_clicked();

    void on_rdvhp_clicked();

    void on_materialhp_clicked();

    void on_packshp_clicked();

private:
    Ui::homepage *ui;
};

#endif // HOMEPAGE_H
