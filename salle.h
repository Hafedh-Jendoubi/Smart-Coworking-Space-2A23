#ifndef SALLE_H
#define SALLE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Salle
{
public:
    Salle();
    Salle(int, QString, QString, QString);

    int getNumero() const { return numero; }
    const QString& getBloc() const { return bloc; }
    const QString& getEtat() const { return etat; }
    const QString& getType() const { return type; }
    void setNumero(int n) { numero = n; }

//    void setNumero()
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int n);
    bool modifier(int numero_salle, QString bloc_salle, QString etat_salle, QString type_salle);
private:
    int numero;
    QString bloc, etat, type;
};

#endif // SALLE_H
