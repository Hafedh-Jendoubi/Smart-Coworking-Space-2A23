#ifndef AUTHENTIFICATION_H
#define AUTHENTIFICATION_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>

#include "ui_authentification.h"
#include "mainwindow.h"
#include "Arduino.h"

namespace Ui {
class authentification;
}

class authentification : public QDialog
{
    Q_OBJECT

public:
    explicit authentification(QWidget *parent = nullptr);
    ~authentification();
    bool ajouter(QString, QString);

private slots:
    void update_label();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::authentification *ui;
    QByteArray data;
    QString data_text;
    Arduino a;
};

#endif // AUTHENTIFICATION_H
