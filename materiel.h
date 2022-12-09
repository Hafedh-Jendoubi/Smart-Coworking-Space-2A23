#ifndef MATERIEL_H
#define MATERIEL_H
#include <QString>
#include <QSqlQueryModel>
#include <QSqlQuery>
class Materiel
{
public:
    Materiel();
    Materiel (int,QString,QString,QString);
    int getprix();
    QString getreference();
    QString gettype();
    QString getstock();
    void setprix (int);
    void setreference(QString);
    void settype(QString);
    void setstock(QString);
    bool ajouter ();
   QSqlQueryModel* afficher ();
   bool supprimer(int);
   bool modifier(int);
   QSqlQueryModel* RechercheMateriel(QString );

   QSqlQueryModel * tri_nomcroissant();
   void genererPDFact();




private:
int prix;
QString reference,type,stock;
};

#endif // MATERIEL_H
