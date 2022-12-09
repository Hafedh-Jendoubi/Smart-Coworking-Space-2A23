#include "employe.h"

Employe::Employe()
{
    CIN = 0;
    Nom = "";
    Prenom = "";
    email = "";
    Sexe = "";
    adresse = "";
    role = "";
    date_naiss = "";
}

Employe::Employe(int CIN, QString Nom, QString Prenom, QString email, QString Sexe, QString adresse, QString role, QString date_naiss)
{
    this->CIN = CIN;
    this->Nom = Nom;
    this->Prenom = Prenom;
    this->email = email;
    this->Sexe = Sexe;
    this->adresse = adresse;
    this->role = role;
    this->date_naiss = date_naiss;
}

bool Employe::ajouter()
{
    QSqlQuery query;
    QString cin_string = QString::number(CIN);

        query.prepare("INSERT INTO EMPLOYE (CIN, Nom, Prenom, Role, Email, Sexe, Adresse, Date_naiss) "" VALUES (:id, :forename, :surname, :role, :email, :Sexe, :adresse, :date_naiss)");
        query.bindValue(":id", cin_string);
        query.bindValue(":forename", Nom);
        query.bindValue(":surname", Prenom);
        query.bindValue(":role", role);
        query.bindValue(":email", email);
        query.bindValue(":Sexe", Sexe);
        query.bindValue(":adresse", adresse);
        query.bindValue(":date_naiss", date_naiss);

        return query.exec();
}

QSqlQueryModel * Employe::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT * FROM employe");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Role"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date Naissance"));

    return model;
}

bool Employe::supprimer(int cin)
{
    QSqlQuery query;

        query.prepare("DELETE From EMPLOYE where cin=:cin");
        query.bindValue(0, cin);

    return query.exec();
}

bool Employe::modifier()
{
    QSqlQuery query;

    query.prepare("Update EMPLOYE set CIN=:cin, Nom=:nom, Prenom=:prenom, Role=:role, Email=:email, Sexe=:sexe, Adresse=:adresse, Date_naiss=:date_naiss where CIN=:cin ");
    query.bindValue(":cin", CIN);
    query.bindValue(":nom", Nom);
    query.bindValue(":prenom", Prenom);
    query.bindValue(":role", role);
    query.bindValue(":email", email);
    query.bindValue(":sexe", Sexe);
    query.bindValue(":adresse", adresse);
    query.bindValue(":date_naiss", date_naiss);

    return query.exec();
}

QSqlQueryModel * Employe::rechercherNom(QString nom)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT * from EMPLOYE where nom like '"+nom+"%' or cin LIKE '"+nom+"%' or prenom like '"+nom+"%' ");
    query.addBindValue("%"+ nom +"%");
    query.exec();
    model->setQuery(query);

    return (model);
}

QSqlQueryModel * Employe::tri_nom()
{
    QSqlQuery query;
    QSqlQueryModel* model = new QSqlQueryModel();

    query.prepare("SELECT * FROM EMPLOYE ORDER BY nom");
    query.exec();
    model->setQuery(query);

    return model;
}

QSqlQueryModel * Employe::tri_role()
{
    QSqlQuery query;
    QSqlQueryModel* model = new QSqlQueryModel();

    query.prepare("SELECT * FROM EMPLOYE ORDER BY role DESC"); //Ordre descendant car le Manager est plus important que l'employe
    query.exec();
    model->setQuery(query);

    return model;
}

QSqlQueryModel * Employe::tri_cin()
{
    QSqlQuery query;
    QSqlQueryModel* model = new QSqlQueryModel();

    query.prepare("SELECT * FROM EMPLOYE ORDER BY cin"); //Ordre descendant car le Manager est plus important que l'employe
    query.exec();
    model->setQuery(query);

    return model;
}
