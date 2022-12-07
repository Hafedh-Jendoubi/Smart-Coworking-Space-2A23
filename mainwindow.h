#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "session.h"
#include <QPainter>
#include <QPrinter>
#include <QFile>
#include <QString>
#include <QIntValidator>
#include <QPixmap>
#include <QSystemTrayIcon>
#include "arduino.h"
#include <QSerialPortInfo>
#include <QSerialPort>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_ajouter_clicked();

    void on_butto_supp_clicked();

    void on_update_clicked();


    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_recherche_clicked();

    void on_pushButton_PDF_clicked();

    void on_pushButton_clicked();

    void on_pushButton_image_clicked();

    void update_label();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    Session S;
    QSystemTrayIcon *notifier;
    QByteArray data;
    Arduino A;
};

#endif // MAINWINDOW_H
