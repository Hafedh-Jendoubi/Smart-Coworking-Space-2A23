#include "salle.h"
#include <QDebug>

Salle::Salle()
{

}

Salle::Salle(int n, QString b, QString e, QString t)
    : numero{n}, bloc{b}, etat{e}, type{t}
{

}

bool Salle::ajouter()
{
    QSqlQuery query;
    query.prepare("insert into salle (numero, bloc, etat, type)" "values (:numero, :bloc, :etat, :type)");

    QString str_numero = QString::number(numero);
    query.bindValue(":numero", str_numero);
    query.bindValue(":bloc", bloc);
    query.bindValue(":etat", etat);
    query.bindValue(":type", type);

    return query.exec();
}

QSqlQueryModel* Salle::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("select * from salle");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("numero"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("bloc"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("etat"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("type"));

    return model;
}

bool Salle::supprimer(int n)
{
    QSqlQuery query;
    QString n_str = QString::number(n);
    query.prepare("Delete from salle where NUMERO= :numero");
    query.bindValue(":numero", n_str);

    return query.exec();
}

bool Salle::modifier(int numero_salle, QString bloc_salle, QString etat_salle, QString type_salle)
{
    QSqlQuery query;
    QString res= QString::number(numero_salle) ;

    query.prepare("update salle set numero=:numero, bloc=:bloc, etat=:etat, type=:type where numero=:numero");

    query.bindValue(":numero", res) ;
    query.bindValue(":bloc", bloc_salle) ;
    query.bindValue(":etat", etat_salle) ;
    query.bindValue(":type", type_salle);
    return query.exec();

}
