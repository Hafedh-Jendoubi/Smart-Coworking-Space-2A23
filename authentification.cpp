#include "authentification.h"

authentification::authentification(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::authentification)
{
    ui->setupUi(this);

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
}

authentification::~authentification()
{
    delete ui;
}

void authentification::on_pushButton_3_clicked()
{
    close();
}

void authentification::on_pushButton_2_clicked()
{
    MainWindow *w = new MainWindow();
    QString email = ui->lineEdit->text();
    QString mdp = ui->lineEdit_2->text();
    QSqlQuery qry;
    if(qry.exec("SELECT * from AUTHENTIFICATION where email='"+email+"'")){
        int count = 0;
        while(qry.next()){
           count++;
        }
        if(count == 1){
            QSqlQuery qry1;
            if(qry1.exec("SELECT * from AUTHENTIFICATION where mdp='"+mdp+"'")){
                int count1 = 0;
                while(qry1.next()){
                    count1++;
                }
                if(count1 == 1){
                    QMessageBox::information(nullptr, QObject::tr("Gestion-Des-Employes"),
                                                      QObject::tr("          Success!\n"
                                                                  "Taper sur cancel pour fermer."), QMessageBox::Cancel);
                    w->getUi()->username->setText(email);
                    QString text = ui->lineEdit->text();
                    QString username;
                    for(int i=0; i<email.length()-10; i++)
                        username[i] = email[i];
                    text = username + " a été connecté(e).";
                    w->getSocket()->write(QByteArray::fromStdString(text.toStdString()));
                    w->show();
                    authentification::close();
                }else{
                    QMessageBox::critical(nullptr, QObject::tr("Gestion-Des-Employes"),
                                                      QObject::tr("Votre mot de passe est incorrect!\n"
                                                                  "  Taper sur cancel pour fermer."), QMessageBox::Cancel);
                }
            }
        }else if(count < 1)
            QMessageBox::critical(nullptr, QObject::tr("Gestion-Des-Employes"),
                                              QObject::tr(" Votre email est incorrect!\n"
                                                          "Taper sur cancel pour fermer."), QMessageBox::Cancel);
    }
}

bool authentification::ajouter(QString email, QString cin_string)
{
    QSqlQuery query1;

    query1.prepare("INSERT INTO AUTHENTIFICATION (Email, MDP) "" VALUES (:email, :mdp)");
    query1.bindValue(":email", email);
    query1.bindValue(":mdp", cin_string);

    return query1.exec();
}
