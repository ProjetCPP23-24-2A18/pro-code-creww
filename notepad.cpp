#include "notepad.h"
#include "ui_notepad.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include "mainwindow.h"

Notepad::Notepad(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Notepad)
{
    ui->setupUi(this);
}

Notepad::~Notepad()
{
    delete ui;
}



void Notepad::on_New_clicked()
{
    MainWindow s;

    file_path="";
    ui->textEdit->setText("");
}

void Notepad::on_Open_clicked()
{
    QString file_name= QFileDialog::getOpenFileName(this,"Open file");
    QFile f(file_name);
    file_path=file_name;
    if(!f.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"...","File not open");

    }else
    {
        QTextStream in(&f);
        QString text=in.readAll();
        ui->textEdit->setText(text);
        f.close();
    }
}

void Notepad::on_Save_clicked()
{
    QString file_name = file_path.isEmpty() ? QFileDialog::getSaveFileName(this, "Save file", QDir::currentPath(), "Text files (*.txt)") : file_path;
    if(file_name.isEmpty())
        return; // User canceled or file name is empty

    if (!file_name.endsWith(".txt", Qt::CaseInsensitive)) {
        file_name += ".txt"; // Append .txt extension if not already present
    }

    QFile f(file_name);
    if(!f.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "...", "File not open");
        return;
    }

    QTextStream out(&f);
    QString text = ui->textEdit->toPlainText();
    out << text;
    f.flush();
    f.close();

    file_path = file_name; // Update file_path with the saved file's path
}

void Notepad::on_pushButton_4_clicked()
{
    QString file_name= QFileDialog::getSaveFileName(this,"Open file");
    QFile f(file_name);
    if(!f.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,"...","File not open");

    }else
    {
        QTextStream out(&f);
        QString text=ui->textEdit->toPlainText();
        out<<text;
        f.flush();
        f.close();
    }
}

void Notepad::on_Copy_clicked()
{
    ui->textEdit->copy();
}

void Notepad::on_Paste_clicked()
{
    ui->textEdit->paste();
}

void Notepad::on_Cut_clicked()
{
    ui->textEdit->cut();

}

void Notepad::on_pushButton_10_clicked()
{
    ui->textEdit->undo();
}

void Notepad::on_Redo_clicked()
{
    ui->textEdit->redo();
}

void Notepad::on_pushButton_2_clicked()
{
    QString text;
    text+="Author: baccouri ranin\n";
    text+="Date: dd/mm/yyyy\n";
    text+="NotePad:(C) Notepad (R)\n";

    QMessageBox::about(this,"About Noptepad",text);
}

