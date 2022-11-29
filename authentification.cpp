#include "authentification.h"

authentification::authentification(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::authentification)
{
    ui->setupUi(this);

    ui->scrollArea->hide();
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    //Arduino Setup
    int ret = a.connect_arduino();
    switch(ret){
        case(0):qDebug() << "arduino is available and connected to :" << a.getarduino_port_name();
            break;
        case(1):qDebug() << "arduino is available and not connected to :" << a.getarduino_port_name();
            break;
        case(-1):qDebug() << "arduino not available.";
    }
    QObject::connect(a.getserial(), SIGNAL(readyRead()), this, SLOT(update_label()));
}

authentification::~authentification()
{
    delete ui;
}

void authentification::update_label()
{
    data = a.read_from_arduino();
    data_text += QString::fromStdString(data.toStdString());

    if(data_text == " A1 A5 DC 1B\r\n"){
        QMessageBox::critical(nullptr, QObject::tr("Gestion-Des-Employes"),
                                          QObject::tr("      Access Denied!\n"
                                                      "Taper sur cancel pour fermer."), QMessageBox::Cancel);
        data = "";
        data_text = "";
    }else if(data_text == " 83 9A E5 9A\r\n"){
        QMessageBox::information(nullptr, QObject::tr("Gestion-Des-Employes"),
                                          QObject::tr("      Access Granted!\n"
                                                      "Taper sur cancel pour fermer."), QMessageBox::Cancel);
        data = "";
        data_text = "";
    }
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

void authentification::on_pushButton_4_clicked()
{
    ui->scrollArea->show();
}
