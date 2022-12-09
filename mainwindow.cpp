#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->scrollArea->hide();
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    //Arduino Setup
    int ret = a.connect_arduino();
    switch(ret){
        case(0):qDebug() << "arduino is available and connected to :" << a.getarduino_port_name();
            break;
        case(1):qDebug() << "arduino is available and not connected to :" << a.getarduino_port_name();
            break;
        case(-1):qDebug() << "arduino not available.";
    }
    QObject::connect(a.getserial(), SIGNAL(readyRead()), this, SLOT(update_label()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_label()
{
    data = a.read_from_arduino();
    data_text += QString::fromStdString(data.toStdString());
    QSqlQuery qry;
    int count = 0;
    if(qry.exec("SELECT * from AUTHENTIFICATION where rfid='"+data_text+"'")){
        while(qry.next()){
           count++;
        }
    }
    if(count == 0){
      if(data_text == " A1 A5 DC 1B\r\n"){
        QMessageBox::critical(nullptr, QObject::tr("Gestion-Des-Employes"),
                                          QObject::tr("      Access Denied!\n"
                                                      "Taper sur cancel pour fermer."), QMessageBox::Cancel);
        data = "";
        data_text = "";
      }else if(data_text == " 83 9A E5 9A\r\n"){
        QMessageBox::information(nullptr, QObject::tr("Gestion-Des-Employes"),
                                          QObject::tr("Access Granted [Jendoubi Hafedh]!\n"
                                                      "  Taper sur cancel pour fermer."), QMessageBox::Cancel);
        data = "";
        data_text = "";
      }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    application *a = new application();
    QString email = ui->lineEdit->text();
    QString mdp = ui->lineEdit_2->text();
    QSqlQuery qry;
    if(qry.exec("SELECT * from AUTHENTIFICATION where email='"+email+"'")){
        int count = 0;
        while(qry.next()){
           count++;
        }
        if(count == 1){
            QSqlQuery qry1;
            if(qry1.exec("SELECT * from AUTHENTIFICATION where mdp='"+mdp+"'")){
                int count1 = 0;
                while(qry1.next()){
                    count1++;
                }
                if(count1 == 1){
                    QMessageBox::information(nullptr, QObject::tr("Gestion-Des-Employes"),
                                                      QObject::tr("          Success!\n"
                                                                  "Taper sur cancel pour fermer."), QMessageBox::Cancel);
                    a->getUi()->username->setText(email);
                    a->getUi()->username_2->setText(email);
                    a->getUi()->username_3->setText(email);
                    QString text = ui->lineEdit->text();
                    QString username;
                    for(int i=0; i<email.length()-10; i++)
                        username[i] = email[i];
                    text = username + " a été connecté(e).";
                    a->getSocket()->write(QByteArray::fromStdString(text.toStdString()));
                    a->show();
                    MainWindow::close();
                }else{
                    QMessageBox::critical(nullptr, QObject::tr("Gestion-Des-Employes"),
                                                      QObject::tr("Votre mot de passe est incorrect!\n"
                                                                  "  Taper sur cancel pour fermer."), QMessageBox::Cancel);
                }
            }
        }else if(count < 1)
            QMessageBox::critical(nullptr, QObject::tr("Gestion-Des-Employes"),
                                              QObject::tr(" Votre email est incorrect!\n"
                                                          "Taper sur cancel pour fermer."), QMessageBox::Cancel);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    close();
}

bool MainWindow::ajouter(QString email, QString cin_string)
{
    QSqlQuery query1;

    query1.prepare("INSERT INTO AUTHENTIFICATION (Email, MDP) "" VALUES (:email, :mdp)");
    query1.bindValue(":email", email);
    query1.bindValue(":mdp", cin_string);

    return query1.exec();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->scrollArea->show();
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->scrollArea->hide();
}
