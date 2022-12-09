#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "materiel.h"
#include <QMessageBox>
#include <QIntValidator>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QDesktopServices>
#include <cstdlib>
#include<QPainter>
#include<QPdfWriter>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->le_prix->setValidator (new QIntValidator(0, 9999999, this));
    ui->tab_materiel->setModel(M.afficher());
    int ret=A.connect_arduino();
         switch (ret)
         {
         case(0):qDebug()<<"arduino is available and connected to :"<<A.getarduino_port_name();
         break;
         case(1):qDebug()<<"arduino is available but not connected to :"<<A.getarduino_port_name();
         break;

         case(-1):qDebug()<<"arduino is not  available :";
         }
    QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_ajouter_clicked()
{
   int prix=ui->le_prix->text().toInt();
   QString reference=ui->le_reference->text();
   QString type=ui->le_type->text();
   QString stock=ui->le_stock->text();
   Materiel M(prix,reference,type,stock);
   bool test=M.ajouter();
       if(test){
           QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("Ajout effectué\n Click Cancel to exit."),QMessageBox::Cancel);
           ui->tab_materiel->setModel(M.afficher());
       }
       else QMessageBox::information(nullptr, QObject::tr("NOT OK"),QObject::tr("Ajout non effectué\n Click Cancel to exit."),QMessageBox::Cancel);




}

void MainWindow::on_pb_supprimer_clicked()
{
    int prix=ui->le_prix->text().toInt();
    bool test=M.supprimer(prix);
        if(test){
            QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("suppression effectué\n Click Cancel to exit."),QMessageBox::Cancel);
            ui->tab_materiel->setModel(M.afficher());
        }
        else QMessageBox::information(nullptr, QObject::tr("NOT OK"),QObject::tr("Suppression non effectué\n Click Cancel to exit."),QMessageBox::Cancel);


}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    QProcess *process = new QProcess(this);
    QString file = QDir::currentPath() + "/build.bat";
    qDebug() << file;
    // process->start(file);

    QDesktopServices::openUrl(QUrl("file:///"+file, QUrl::TolerantMode));
}

void MainWindow::on_comboBox_3_activated(const QString &arg1)
{
    QString url = "start https://cse.google.com/cse?cx=f108d86fed230418a#gsc.q=" + arg1 + "%20intext%3Atnd";
    system(url.toStdString().c_str());
}



void MainWindow::on_REF_recherche_textChanged(const QString &arg1)
{
    Materiel mat;
       QString rech=ui->REF_recherche->text();
       ui->tab_materiel->setModel(mat.RechercheMateriel(rech));
}

void MainWindow::on_pb_trier_clicked()
{
    Materiel P;


            ui->tab_materiel->setModel(P.tri_nomcroissant());


            bool test=P.tri_nomcroissant();//tri produit
            if (test)
            {


                QMessageBox::information(nullptr,QObject::tr("ok"),
                                         QObject::tr("tri croissante effectué \n"
                                                     "Click Cancel to exist ."),QMessageBox::Cancel);

            }
            else
                 {QMessageBox::critical(nullptr, QObject::tr("nonnnn"),
                              QObject::tr("tri croissante failed.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);}

        }





void MainWindow::on_pb_ajouter_2_clicked()
{

        QPdfWriter pdf("C:\\Users\\iheb arfaoui\\Dropbox (Entreprise)\\PC\\Desktop\\Atelier_Connexion_2_1\\Atelier_Connexion\\PDF_materiel.pdf");

           QPainter painter(&pdf);
           int i = 4000;
                  painter.setPen(Qt::red);
                  painter.setFont(QFont("Time New Roman", 25));
                  painter.drawText(3000,1400,"Liste Des materiels");
                  painter.setPen(Qt::black);
                  painter.setFont(QFont("Time New Roman", 15));
                  painter.drawRect(100,3000,9400,500);
                  painter.setFont(QFont("Time New Roman", 9));
                  painter.drawText(1000,3300,"Prix");
                  painter.drawText(2500,3300,"reference");
                  painter.drawText(4000,3300,"type");
                  painter.drawText(5500,3300,"stock");
                  painter.drawRect(100,3000,9400,9000);

                  QSqlQuery query;
                  query.prepare("select * from TABLE1");
                  query.exec();
                  while (query.next())
                  {
                      painter.drawText(1000,i,query.value(0).toString());
                      painter.drawText(2500,i,query.value(1).toString());
                      painter.drawText(4000,i,query.value(2).toString());
                      painter.drawText(5500,i,query.value(3).toString());
                      painter.drawText(7000,i,query.value(4).toString());


                     i = i + 350;
                  }
                  QMessageBox::information(this, QObject::tr("PDF Enregistré!"),
                  QObject::tr("PDF Enregistré!.\n" "Click Cancel to exit."), QMessageBox::Cancel);
    }
void MainWindow::on_pb_modifier_clicked()
{
    int prix = ui->le_prix->text().toInt();
    QString reference= ui->le_reference->text();
    QString type = ui->le_type->text();
    QString stock = ui->le_stock->text();


  Materiel M (prix,reference,type,stock);
  bool test=M.modifier(prix);
  if(test)
{

      ui->tab_materiel->setModel(M.afficher());//refresh
QMessageBox::information(nullptr, QObject::tr("modifier une materiel"),
                  QObject::tr("matériel modifié.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);

}
  else
      QMessageBox::critical(nullptr, QObject::tr("modifier une materiel"),
                  QObject::tr("Erreur !.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);

}


void MainWindow::on_pushButton_clicked()
{
    A.write_to_arduino("1");
}

void MainWindow::on_pushButton_2_clicked()
{
    A.write_to_arduino("0");
}

void MainWindow::update_label()
{
    data=A.read_from_arduino();
            if(data=="1")
            ui->label_18->setText("porte ouverte");
            else

            if (data=="0")
            ui->label_18->setText("porte fermee");
}

