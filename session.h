#ifndef SESSION_H
#define SESSION_H
#include<QString>
#include<QSqlQueryModel>
class Session
{
public:
    Session();
    Session(int,QString,QString,QString,int,int);
    int getidmat();
    QString getnom();
    QString getprenom();
    QString getmatiere();
    int getnumsalle();
    int getnbreleve();
    void setidmat(int);
    void setnom(QString);
    void setprenom(QString);
    void setmatiere(QString);
    void setnumsalle(int);
    void setnbreleve(int);
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier();
    QSqlQueryModel* tri_idmat();
    QSqlQueryModel* tri_nom();
    QSqlQueryModel* tri_numerosalle();
    QSqlQueryModel* tri_nombre_eleve();
    QSqlQueryModel * recherche (QString);
      QString apercupdf();
      bool  ajouter1() ;
      QSqlQueryModel *afficher1();
     QString Date1 ;
private:
    int    idmat;
    QString nom,prenom,matiere;
    int numsalle,nbreleve;


};

#endif // SESSION_H
