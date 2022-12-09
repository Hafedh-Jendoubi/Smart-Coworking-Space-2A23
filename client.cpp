#include "client.h"
#include  <QSqlQuery>
#include<QDebug>
#include <QObject>
#include <QSqlError>
#include <QDate>
Client::Client()
{
   id=0 ; nom =" " ; prenom= " " ; date_naissance = " "; CIN =0 ; Email= " " ; telephone =0 ; profession = " "; role=" " ;
}

Client :: Client(int id  ,  QString nom , QString prenom ,  QString date_naissance ,  int CIN ,  QString Email, int telephone,QString  profession ,  QString role)
{this->id=id ; this->nom=nom ; this-> prenom=prenom ; this-> date_naissance=date_naissance ; this->CIN=CIN ; this-> Email=Email ; this -> telephone=telephone ; this-> profession=profession ; this-> role=role ; }


int Client::getid() {return  id ; }
QString Client :: getnom () {return nom;}
QString Client :: getprenom (){return prenom;}
QString Client :: getdate_naissance() {return date_naissance;}
int Client :: getCIN () {return CIN;}
QString Client :: getEmail (){return Email;}
int Client :: gettelephone (){return telephone;}
QString Client :: getprofession (){return profession;}
QString Client :: getrole (){return role;}
void setid (int);
void setnom (QString) ;
void setprenom (QString) ;
void setdate_naissance (QString) ;
void setCIN (int) ;
void setEmail (QString) ;
void settelephone (int) ;
void setprofession (QString) ;
void setrole (QString) ;
bool Client :: ajouter ()
{

    QSqlQuery query;
    QString id_string = QString::number(id);
    QString cin_string = QString::number(CIN);
    QString tel_string = QString::number(telephone);

          query.prepare("INSERT INTO CLIENTS (id, nom, prenom, date_naisssance ,cin, email,telephone , profession, role) "
                        "VALUES (:id, :nom, :prenom,:date_naissance, :cin, :email, :telephone ,  :profession, :role)");
          query.bindValue(":id", id_string);
          query.bindValue(":nom", nom);
          query.bindValue(":prenom",prenom);
          query.bindValue(":date_naissance",date_naissance );
          query.bindValue(":cin", cin_string);
          query.bindValue(":email", Email);
          query.bindValue(":telephone", tel_string);
          query.bindValue(":profession", profession);
          query.bindValue(":role", role);
          return query.exec();
}

QSqlQueryModel * Client:: afficher()
{

    QSqlQueryModel * model=new QSqlQueryModel() ;

      model->setQuery("select * from CLIENTS") ;
      model->setHeaderData(0,Qt::Horizontal,QObject::tr("id")) ;
      model->setHeaderData(1,Qt::Horizontal,QObject::tr("nom")) ;
      model->setHeaderData(2,Qt::Horizontal,QObject::tr("prenom")) ;
      model->setHeaderData(3,Qt::Horizontal,QObject::tr("email")) ;
      model->setHeaderData(4,Qt::Horizontal,QObject::tr("cin")) ;
      model->setHeaderData(5,Qt::Horizontal,QObject::tr("date_naisssance")) ;
      model->setHeaderData(6,Qt::Horizontal,QObject::tr("profession")) ;
      return  model ;
}


bool Client::supprimer(int id){
          QSqlQuery query;
          query.prepare("DELETE FROM CLIENTS WHERE id=:id");
          query.bindValue(":id", id);
          return query.exec();
}

bool Client::modifier(){
          QSqlQuery query;
          query.prepare("UPDATE CLIENTS SET nom=:nom, prenom=:prenom, date_naisssance=:date_naissance ,cin=:cin, email=:email, profession=:profession, role=:role where id=:id");
          query.bindValue(":id", id);
          query.bindValue(":nom", nom);
          query.bindValue(":prenom",prenom);
          query.bindValue(":date_naissance",date_naissance );
          query.bindValue(":cin", CIN);
          query.bindValue(":email", Email);
          query.bindValue(":telephone", telephone);
          query.bindValue(":profession", profession);
          query.bindValue(":role", role);
          return query.exec();
}
QSqlQueryModel * Client::AfficherTrieprofession()
 {
     QSqlQueryModel * model = new QSqlQueryModel();
     model->setQuery("SELECT * FROM CLIENTS ORDER BY profession DESC");
     return model;
 }

 QSqlQueryModel * Client::AfficherTrieemail()
 {
     QSqlQueryModel * model = new QSqlQueryModel();
     model->setQuery("SELECT * FROM CLIENTS ORDER BY email DESC");
     return model;
 }
 QSqlQueryModel * Client::AfficherTrierole()
 {
     QSqlQueryModel * model = new QSqlQueryModel();
     model->setQuery("SELECT * FROM CLIENTS ORDER BY role DESC");
     return model;
 }


 QSqlQueryModel * Client::rechercher(QString nom)
 {
     QSqlQueryModel *model= new QSqlQueryModel();
     QSqlQuery q;
     q.prepare("select * from CLIENTS where id like '"+nom+"%' or nom LIKE '"+nom+"%' or cin like '"+nom+"%' or profession like '"+nom+"%'  or role like' "+nom+"%'");
     q.addBindValue("%"+ nom +"%");
     q.exec();
     model->setQuery(q);
     return (model);
 }
