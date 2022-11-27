#ifndef AUTHENTIFICATION_H
#define AUTHENTIFICATION_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>

#include "ui_authentification.h"
#include "mainwindow.h"

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
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::authentification *ui;
};

#endif // AUTHENTIFICATION_H
