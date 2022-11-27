#ifndef EMPLOYE_H
#define EMPLOYE_H
#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtDebug>
#include <QObject>

class Employe{
  private:
    int CIN;
    QString Nom;
    QString Prenom;
    QString email;
    QString Sexe;
    QString adresse;
    QString date_naiss;
    QString role;
  public:
    Employe();
    Employe(int, QString, QString, QString, QString, QString, QString, QString);
    ~Employe() {};
    int getCIN() {return CIN;};
    QString getNom() {return Nom;};
    QString getPrenom() {return Prenom;};
    QString getEmail() {return email;};
    QString getSexe() {return Sexe;};
    QString getAdresse() {return adresse;};
    QString getDate_naiss() {return date_naiss;};
    QString getRole() {return role;};
    void setCIN(int x) {CIN = x;};
    void setNom(QString ch) {Nom = ch;};
    void setPrenom(QString ch) {Prenom = ch;};
    void setEmail(QString ch) {email = ch;};
    void setSexe(QString ch) {Sexe = ch;};
    void setAdresse(QString ch) {adresse = ch;};
    void setDate_naiss(QString ch) {date_naiss = ch;};
    void setRole(QString ch) {role = ch;};
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool modifier();
    QSqlQueryModel * rechercherNom(QString);
    QSqlQueryModel *  tri_nom();
    QSqlQueryModel *  tri_role();
    QSqlQueryModel *  tri_cin();
};

#endif // EMPLOYE_H
