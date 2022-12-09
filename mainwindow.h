#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "materiel.h"
#include "arduino.h"
namespace Ui {class MainWindow;}

class MainWindow;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
 void update_label();



private slots:
    void on_pb_ajouter_clicked();

    
    void on_pb_supprimer_clicked();

    void on_pb_modifier_clicked();


    //void on_pb_rechercher_clicked();


    void on_comboBox_activated(const QString &arg1);


    void on_comboBox_3_activated(const QString &arg1);
    //void on_comboBox_2_currentIndexChanged(int index);



    void on_REF_recherche_textChanged(const QString &arg1);

    void on_pb_trier_clicked();

    void on_pb_ajouter_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    //void on_label_18_linkActivated(const QString &link);

private:
    Ui::MainWindow *ui;
    Materiel M;
    Arduino A;
        QByteArray data ;

};

#endif // MAINWINDOW_H
