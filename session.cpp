#include "session.h"
#include <QSqlQuery>
#include<QDebug>
#include<QObject>
#include <QSqlQueryModel>
Session::Session()
{
idmat=0;
matiere="";
nom="";
prenom="";
nbreleve=0;
numsalle=0;
}

Session::Session(int idmat,QString nom ,QString prenom ,QString matiere,int nbreleve,int numsalle)
{
    this->idmat=idmat;
    this->nom=nom;
    this->prenom=prenom;
    this->matiere=matiere;
    this->nbreleve=nbreleve;
    this->numsalle=numsalle;
}

int Session::getidmat()
{return idmat;}

QString Session::getnom(){return nom;}
QString Session::getprenom(){return prenom;}
QString Session::getmatiere(){return matiere;}

int Session::getnumsalle(){return numsalle;}
int Session::getnbreleve(){return nbreleve;}

void Session::setidmat(int idmat){this->idmat=idmat;}
void Session::setnom(QString nom){this->nom=nom;}
void Session::setprenom(QString prenom){this->prenom=prenom;}
void Session::setmatiere(QString matiere){this->matiere=matiere;}
void Session::setnumsalle(int numsalle){this->numsalle=numsalle;}
void Session::setnbreleve(int nbreleve){this->nbreleve=nbreleve;}

bool Session::ajouter()
{
    QSqlQuery query;
    QString idmat1 = QString::number(idmat);
    QString numsalle1 = QString::number(numsalle);
    QString nbreleve1 = QString::number(nbreleve);
          query.prepare("INSERT INTO Session1 (idmat,nom,prenom,matiere,numsalle,nbreleve) "
                        "VALUES (:idmat,:matiere,:nom,:prenom,:numsalle,:nbreleve)");
          query.bindValue(":idmat",idmat1);
          query.bindValue(":matiere",matiere);
          query.bindValue(":nom",nom);
          query.bindValue(":prenom",prenom);
          query.bindValue(":numsalle",numsalle1);
          query.bindValue(":nbreleve",nbreleve1);
          query.exec();
}

QSqlQueryModel* Session::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel;
          model->setQuery("SELECT * FROM session1 ");
          model->setHeaderData(0, Qt::Horizontal, QObject::tr("idmat"));
          model->setHeaderData(1, Qt::Horizontal, QObject::tr("matiere"));
          model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
          model->setHeaderData(3, Qt::Horizontal, QObject::tr("prenom"));
          model->setHeaderData(4, Qt::Horizontal, QObject::tr("numsalle"));
          model->setHeaderData(5, Qt::Horizontal, QObject::tr("nbreleve"));
return model;
}

bool Session::supprimer(int idmat)
{
    QSqlQuery query;
    QString idmat1 = QString::number(idmat);
          query.prepare("delete from session1 where idmat=:idmat");
          query.bindValue(":idmat",idmat1);
          query.exec();
}


bool Session::modifier()
{
    QSqlQuery query;
    QString idmat1 = QString::number(idmat);
    QString numsalle1 = QString::number(numsalle);
    QString nbreleve1 = QString::number(nbreleve);
          query.prepare("update session1 set matiere=:matiere,nom=:nom,prenom=:prenom,numsalle=:numsalle,nbreleve=:nbreleve where idmat=:idmat ");
          query.bindValue(":idmat",idmat1);
          query.bindValue(":matiere",matiere);
          query.bindValue(":nom",nom);
          query.bindValue(":prenom",prenom);
          query.bindValue(":numsalle",numsalle1);
          query.bindValue(":nbreleve",nbreleve1);
          query.exec();
}

QSqlQueryModel * Session::tri_idmat()
{
    QSqlQuery query;
    QSqlQueryModel* model = new QSqlQueryModel();

    query.prepare("SELECT * FROM session1 ORDER BY idmat");
    query.exec();
    model->setQuery(query);

    return model;
}





QSqlQueryModel * Session::tri_nom()
{
    QSqlQuery query;
    QSqlQueryModel* model = new QSqlQueryModel();

    query.prepare("SELECT * FROM session1 ORDER BY nom");
    query.exec();
    model->setQuery(query);

    return model;
}

QSqlQueryModel * Session::tri_numerosalle()
{
    QSqlQuery query;
    QSqlQueryModel* model = new QSqlQueryModel();

    query.prepare("SELECT * FROM session1 ORDER BY numsalle");
    query.exec();
    model->setQuery(query);

    return model;
}

QSqlQueryModel * Session::tri_nombre_eleve()
{
    QSqlQuery query;
    QSqlQueryModel* model = new QSqlQueryModel();

    query.prepare("SELECT * FROM session1 ORDER BY nbreleve");
    query.exec();
    model->setQuery(query);

    return model;
}

QSqlQueryModel * Session::recherche(QString n)
 {
    QSqlQueryModel* model = new QSqlQueryModel();
     QSqlQuery query;

     query.prepare("select * from session1 where idmat like '"+n+"%' or  nom like '"+n+"%'   or matiere like '"+n+"%'   ");
    query.addBindValue("%"+ n +"%");
    query.exec();
    model->setQuery(query);

    return (model);
 }


QString Session::  apercupdf()
{

    QString text="            ******* LES SESSIONS  ********       \n \n " ;
    QSqlQuery query ;
    QString a,b,c,d,e,f;


     query.exec("select * from session1 ");
     while (query.next())
     {
         a=query.value(0).toString();
         b=query.value(1).toString();
         c=query.value(2).toString();
         d=query.value(3).toString();
         e=query.value(4).toString();
     f=query.value(4).toString();

       text += "\n - idmat : "+a+"\n - matiere : "+ b+"\n  - nom : "+ c+"\n - prenom : "+ d+"\n  - numsalle : "+e+" - nbreleve : "+f+"\n______________\n";



    }

            return text ;
}

bool Session::ajouter1()
{

    QSqlQuery query;

    query.prepare("insert into fire1 (DATE_ACTIVATION ) values (:DATE_ACTIVATION )" );
    query.bindValue(":DATE_ACTIVATION", Date1);

    return query.exec();

};

QSqlQueryModel * Session:: afficher1()
{

    QSqlQueryModel * model=new QSqlQueryModel();
    model->setQuery("select * from fire1" );
    return model;
};
