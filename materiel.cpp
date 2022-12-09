#include "materiel.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QObject>
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQuery>
#include<QPainter>
#include<QPdfWriter>
Materiel::Materiel()
{
prix=0 ; reference=""; type="";stock="";
}
Materiel::Materiel (int prix ,QString reference,QString type,QString stock )
{this->prix=prix; this->reference=reference; this->type=type;}int Materiel::getprix(){return prix;}
QString Materiel::getreference(){return reference;}
QString Materiel::gettype(){return type;}
QString Materiel::getstock(){return stock;}
void Materiel::setprix(int prix ){this->prix=prix;}
void Materiel::setreference(QString reference){this->reference=reference;}
void Materiel::settype(QString type){this->type=type;}
void Materiel::setstock(QString stock){this->stock=stock;}
bool Materiel::ajouter()
{


    QSqlQuery query;
    QString res = QString::number(prix);


    query.prepare("insert into TABLE1 (prix, reference, type,stock)"
                  "VALUES (:prix, :reference, :type,:stock)");
    query.bindValue(":prix",res );
    query.bindValue(":reference", reference);
    query.bindValue(":type",type );
    query.bindValue(":stock", stock);
    return query.exec();


}
bool Materiel::supprimer(int prix)
{
    QSqlQuery query;

    QString res=QString::number(prix);
    query.prepare("Delete from TABLE1 where prix= :prix");
    query.bindValue(":prix", res);

    qDebug() <<query.exec();
    return query.exec();
}
QSqlQueryModel* Materiel::afficher()
{


    QSqlQueryModel* model=new QSqlQueryModel();

          model->setQuery("SELECT*  FROM TABLE1");
          model->setHeaderData(0, Qt::Horizontal, QObject::tr("Prix"));
          model->setHeaderData(1, Qt::Horizontal, QObject::tr("Reference"));
          model->setHeaderData(2, Qt::Horizontal, QObject::tr("type"));
          model->setHeaderData(3, Qt::Horizontal, QObject::tr("stock"));
    return model;
}



QSqlQueryModel* Materiel::RechercheMateriel(QString recherche)
 {
     QSqlQueryModel * model= new QSqlQueryModel();
     model->setQuery("SELECT * FROM TABLE1 WHERE PRIX LIKE '"+recherche+"%' OR REFERENCE LIKE '"+recherche+"%' OR TYPE LIKE '"+recherche+"%'");
     model->setHeaderData(0, Qt::Horizontal, QObject::tr("PRIX"));
     model->setHeaderData(1, Qt::Horizontal, QObject::tr("REFERENCE"));
     model->setHeaderData(2, Qt::Horizontal, QObject::tr("TYPE"));
     model->setHeaderData(3, Qt::Horizontal, QObject::tr("STOCK"));


return model;
}

bool Materiel::modifier(int prix)
{
QSqlQuery query;
QString res= QString::number(prix);
query.prepare("Update materiel set PRIX = :prix , REFERENCE = :reference , TYPE = :type , STOCK = :stock ,   where PRIX = :prix ");
query.bindValue(":prix", res);
query.bindValue(":reference", reference);
query.bindValue(":type", type);
query.bindValue(":stock", stock);

return    query.exec();
}

QSqlQueryModel * Materiel::tri_nomcroissant()
{
    QSqlQueryModel * model= new QSqlQueryModel();

          model->setQuery("SELECT * FROM TABLE1 ORDER BY prix ASC ");
          model->setHeaderData(0,Qt::Horizontal,QObject::tr("prix"));
          model->setHeaderData(1,Qt::Horizontal,QObject::tr("reference"));
        model->setHeaderData(2,Qt::Horizontal,QObject::tr("type"));
            model->setHeaderData(3,Qt::Horizontal,QObject::tr("stock"));


    return model;
}
void Materiel::genererPDFact()
{


    QPdfWriter pdf("C:/Users/iheb arfaoui/Dropbox (Entreprise)/PC/Desktop/Atelier_Connexion_2_1/Atelier_Connexion /PDF.pdf");
    QPainter painter(&pdf);
   int i = 4000;
        painter.setPen(Qt::green);
        painter.setFont(QFont("Arial", 50));
        painter.drawText(1100,1200,"Liste Des Materiels");
        painter.setPen(Qt::red);
        painter.setFont(QFont("Arial", 15));
        painter.drawRect(100,100,7300,2600);
        painter.drawRect(0,3000,9600,500);
        painter.setFont(QFont("Arial", 9));
        painter.drawText(200,3300,"prix");
        painter.drawText(1300,3300,"reference");
        painter.drawText(2400,3300,"type");
           painter.drawText(3500,3300,"stock");


        QSqlQuery query;
        query.prepare("select * from TABLE1");
        query.exec();
        while (query.next())
        {
            painter.drawText(200,i,query.value(0).toString());
            painter.drawText(1300,i,query.value(1).toString());
            painter.drawText(2400,i,query.value(2).toString());
            painter.drawText(3500,i,query.value(3).toString());
            painter.drawText(4600,i,query.value(4).toString());


           i = i + 500;
        }


}




