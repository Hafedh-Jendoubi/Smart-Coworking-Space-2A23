#ifndef APPLICATION_H
#define APPLICATION_H

#include <QDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include <QTcpSocket>
#include <QHostAddress>
#include <QSystemTrayIcon>

#include "employe.h"
#include "session.h"
#include "mainwindow.h"
#include "mail.h"
#include "arduino.h"

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
    void on_commandLinkButton_5_clicked();
    void on_commandLinkButton_clicked();

    void on_pushButton_ajouter_clicked();
    void on_butto_supp_clicked();
    void on_update_clicked();
    void on_comboBox_2_currentIndexChanged(int index);
    void on_pushButton_recherche_clicked();
    void on_pushButton_PDF_clicked();
    void on_pushButton_envoyer_clicked();
    void on_pushButton_image_clicked();
    void update_label();
    void on_pushButton_7_clicked();
    void on_commandLinkButton_9_clicked();
    void on_commandLinkButton_10_clicked();

    void on_commandLinkButton_11_clicked();

    void on_commandLinkButton_15_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_9_clicked();

private:
    Ui::application *ui;
    Employe E;
    Session S;
    QSystemTrayIcon *notifier;
    QByteArray data;
    QTcpSocket socket;
    Arduino A;
};

#endif // APPLICATION_H
