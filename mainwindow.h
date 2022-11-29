#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QIntValidator>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QTcpSocket>
#include <QHostAddress>

#include "employe.h"
#include "ui_mainwindow.h"
#include "mail.h"
#include "authentification.h"
#include "smtp.h"
#include "seriallink.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow* getUi() {return ui;};
    Employe getEmploye() {return E;};
    QTcpSocket * getSocket() {return &socket;};

private slots:
    void on_pushButton_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_14_clicked();
    void on_pushButton_12_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_CIN_recherche_textChanged(const QString &arg1);
    void on_pushButton_13_clicked();
    void on_pushButton_8_clicked();
    void onReadyRead();
    void onSendButtonPressed();


private:
    Ui::MainWindow *ui;
    Employe E;
    QTcpSocket socket;
};
#endif // MAINWINDOW_H
