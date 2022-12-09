#ifndef CLIENT_H
#define CLIENT_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
class Client
{
public:
    Client();
    Client(int ,QString ,QString , QString , int,QString,int,QString,QString);
    int getid() ;
    QString getnom () ;
    QString getprenom () ;
    QString getdate_naissance () ;
    int getCIN () ;
    QString getEmail ();
    int gettelephone ();
    QString getprofession ();
    QString getrole ();
    void setid (int);
    void setnom (QString) ;
    void setprenom (QString) ;
    void setdate_naissance (QString) ;
    void setCIN (int) ;
    void setEmail (QString) ;
    void settelephone (int) ;
    void setprofession (QString) ;
    void setrole (QString) ;
    bool ajouter() ;
    QSqlQueryModel * afficher();

     QSqlQueryModel * rechercher(QString);
    bool supprimer(int id);
    bool modifier();
    QSqlQueryModel * AfficherTrieprofession();
    QSqlQueryModel * AfficherTrieemail();
    QSqlQueryModel * AfficherTrierole();




private:
    int id , CIN , telephone ;
    QString nom , prenom , date_naissance, Email ,  profession , role ;

};

#endif // CLIENT_H
