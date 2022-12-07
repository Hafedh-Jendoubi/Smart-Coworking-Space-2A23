#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "session.h"
#include "smtp.h"
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <iostream>
#include <QPrinter>
#include <QPainter>
#include <QLabel>
#include <QColor>
#include <QPrintDialog>
#include <QFileDialog>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    int ret=A.connect_arduino(); //lancer la connexion
    switch(ret)
    {
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
     QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));

    ui->idmat1->setValidator(new QIntValidator(1,10,this));
    ui->le_nbr->setValidator(new QIntValidator(1,30,this));
    ui->le_salle->setValidator(new QIntValidator(1,30,this));
    ui->tab1->setModel(S.afficher());
    notifier = new QSystemTrayIcon(this);
        notifier->setIcon(QIcon("C:/Users/Admin/Desktop/qt documents/Atelier_Connexion/notification.jpg"));
        notifier->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_pushButton_ajouter_clicked()
{
    int idmat=ui->idmat1->text().toInt();
    QString matiere=ui->lemat->text();
    QString nom=ui->le_nom->text();
    QString prenom=ui->le_prenom->text();
    int nbreleve=ui->le_nbr->text().toInt();
    int numsalle=ui->le_salle->text().toInt();
    Session S(idmat,matiere,nom,prenom,nbreleve,numsalle);
   bool test=S.ajouter();
   if(test)
   {
       ui->tab1->setModel(S.afficher());
       notifier->showMessage("Ok"," Session ajoutée",QSystemTrayIcon::Information,10000);
       QMessageBox::information(nullptr, QObject::tr("ok"),
                   QObject::tr("AJOUT successful.\n"
                               "Click Cancel to exit."), QMessageBox::Cancel);

   }
   else
       QMessageBox::critical(nullptr, QObject::tr("NOT OK "),
                   QObject::tr("AJOUT failed.\n"
                               "Click Cancel to exit."), QMessageBox::Cancel);

}

void MainWindow::on_butto_supp_clicked()
{
    Session S1;
    S1.setidmat(ui->supp->text().toInt());
    bool test=S1.supprimer(S1.getidmat());
    QMessageBox msg;
    if(test)
        msg.setText("supp avec succes");
        else
        msg.setText("echec de supp");

 notifier->showMessage("Ok"," session supprimer",QSystemTrayIcon::Information,10000);
   msg.exec();
}

void MainWindow::on_update_clicked()
{
    int idmat=ui->u_idmat->text().toInt();
    QString matiere=ui->u_matiere->text();
    QString nom=ui->u_nom->text();
    QString prenom=ui->u_pre->text();
    int nbreleve=ui->u_nbr->text().toInt();
    int numsalle=ui->u_num->text().toInt();
    Session S(idmat,matiere,nom,prenom,nbreleve,numsalle);
   bool test=S.modifier();
   if(test)
   {
        notifier->showMessage("Ok"," session modifier",QSystemTrayIcon::Information,10000);
       QMessageBox::information(nullptr, QObject::tr("ok"),
                   QObject::tr("modifier avec successful.\n"
                               "Click Cancel to exit."), QMessageBox::Cancel);

   }
   else
       QMessageBox::critical(nullptr, QObject::tr("NOT OK "),
                   QObject::tr("update failed.\n"
                               "Click Cancel to exit."), QMessageBox::Cancel);

}



void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index == 1)
        ui->tab1->setModel(S.tri_idmat());
    else if(index==2)
        ui->tab1->setModel(S.tri_nom());
    else if(index==3)
        ui->tab1->setModel(S.tri_numerosalle());
    else if(index==4)
        ui->tab1->setModel(S.tri_nombre_eleve());

}

void MainWindow::on_pushButton_recherche_clicked()
{
    QString n=ui->recherche->text();
    ui->tab1->setModel(S.recherche(n));
}

void MainWindow::on_pushButton_PDF_clicked()
{
    Session s;
 QPainter painter;
             QString text=s.apercupdf();
             ui->textEdit->setText(text);

                               QPrinter printer ;
                               printer.setPrinterName("imprimer");
                               QImage logo("C:/Users/hafeu/Desktop/Hafedh/Personal-Repository/2A23/Semester 1/Projet QT/Organization/application pictures/logo_simple.png");
                               QPoint logo_cordinates(40, 40);
                                   painter.drawImage(logo_cordinates, logo);
                               QPrintDialog dialog (&printer,this);
                               if(dialog.exec()==QDialog::Rejected) return ;
                               ui->textEdit->print(&printer);

}

void MainWindow::on_pushButton_clicked()
{
    Smtp* smtp = new Smtp("mohamedkhalil.debaieb@esprit.tn", "211JMT5311", "smtp.gmail.com", 465);

       connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));

       QString a=ui->rcpt->text();

       QString b=ui->subject->text();

       QString c=ui->msg->text();


       smtp->sendMail("mohamedkhalil.debaieb@esprit.tn", a , b,c);
}

void MainWindow::on_pushButton_image_clicked()
{
    QString dir = QFileDialog::getOpenFileName(this,tr("Select Picture"),"C:/Users/Admin/Desktop/capture.png");
        QPixmap pic(dir);
        int x = ui->image->width();
        int y = ui->image->height();
        ui->image->setPixmap(pic.scaled(x,y,Qt::KeepAspectRatio));

}

void MainWindow::update_label()
{
    data=A.read_from_arduino();
if(data=="1")
   {ui->label_24->setText("Fire  detected");
    QTime Time  =QTime::currentTime() ;
           S.Date1 = Time.toString("H:m:s a") ;
           S.ajouter1();

     ui->tableView->setModel(S.afficher1());


}
else{
   ui->label_24->setText("Fire NOT detected");


}

}




void MainWindow::on_pushButton_3_clicked()
{
     A.write_to_arduino("5");
}
