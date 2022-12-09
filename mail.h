#ifndef MAIL_H
#define MAIL_H

#include <QDialog>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mail.h"
#include "smtp.h"

namespace Ui {
class mail;
}

class mail : public QDialog
{
    Q_OBJECT

public:
    explicit mail(QWidget *parent = nullptr);
    ~mail();
    Ui::mail* getUi() {return ui;};

private slots:
    void on_pushButton_clicked();

    void on_sendBtn_clicked();

    void on_browseBtn_clicked();

private:
    Ui::mail *ui;
    QStringList files;
};

#endif // MAIL_H
