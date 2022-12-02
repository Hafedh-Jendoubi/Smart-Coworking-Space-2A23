#ifndef APPLICATION_H
#define APPLICATION_H

#include <QDialog>
#include <QPrinter>
#include <QPainter>
#include <QTcpSocket>
#include <QHostAddress>

#include "employe.h"
#include "mainwindow.h"
#include "mail.h"

namespace Ui {
class application;
}

class application : public QDialog
{
    Q_OBJECT

public:
    explicit application(QWidget *parent = nullptr);
    ~application();
    Ui::application* getUi() {return ui;};
    Employe getEmploye() {return E;};
    QTcpSocket * getSocket() {return &socket;};

private slots:
    void on_pushButton_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_19_clicked();
    void on_pushButton_20_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_17_clicked();
    void on_pushButton_15_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_CIN_recherche_textChanged(const QString &arg1);
    void on_pushButton_16_clicked();
    void on_pushButton_8_clicked();
    void onReadyRead();
    void onSendButtonPressed();
    void on_commandLinkButton_9_clicked();

private:
    Ui::application *ui;
    Employe E;
    QTcpSocket socket;
};

#endif // APPLICATION_H
