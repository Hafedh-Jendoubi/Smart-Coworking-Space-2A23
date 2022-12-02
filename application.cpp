#include "application.h"
#include "ui_application.h"

application::application(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::application)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);
    ui->tab_employe->setModel(E.afficher()); ui->textEdit->setReadOnly(true);
    ui->CIN->setValidator(new QIntValidator(0, 99999999, this)); //Control de saisie, 8 chiffres juste pour le CIN!
    ui->CIN_recherche->setValidator(new QIntValidator(0, 99999999, this));
    ui->CIN_12->setValidator(new QIntValidator(0, 99999999, this));
    ui->modify->hide();
    ui->affichage->hide();
    ui->date_naiss->setDate(QDate::currentDate());
    //chat
    socket.connectToHost(QHostAddress("127.0.0.1"), 9999);
    connect(&socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(ui->sendButton, SIGNAL(pressed()), this, SLOT(onSendButtonPressed()));
}

application::~application()
{
    delete ui;
}

void application::on_pushButton_clicked() //Ajout
{
    MainWindow a;
    int CIN = ui->CIN->text().toInt();
    QString Nom = ui->Nom->text();
    QString Prenom = ui->Prenom->text();
    QString email = ui->Email->text();
    int Sexe_answer = ui->Sexe->currentIndex();
    QString Sexe;
    QString adresse = ui->adresse->text();
    QString date_naiss = ui->date_naiss->date().toString("dd.MM.yyyy");
    int Role_answer = ui->role->currentIndex();
    QString role;

    if(Sexe_answer == 0)
        Sexe = "Male";
    else
        Sexe = "Femelle";

    if(Role_answer == 0)
        role = "Manager";
    else
        role = "Employe";

    Employe E(CIN, Nom, Prenom, email, Sexe, adresse, role, date_naiss);
    QSqlQuery qry; QString cin = QString::number(CIN);
    if(qry.exec("SELECT * from EMPLOYE where cin='"+cin+"'")){
        int count = 0;
        while(qry.next()){
           count++;
        }
        if(count == 1){
            QMessageBox::critical(nullptr, QObject::tr("Gestion-Des-Employes"),
                                              QObject::tr("   Employé(e) existe deja!\n"
                                                          "Taper sur cancel pour fermer."), QMessageBox::Cancel);
        }else{
            if(CIN < 9999999 || ui->Nom->text() == "" || ui->Prenom->text() == "" || ui->Email->text() == ""){
                ui->CIN->setStyleSheet("border: 1px solid red;");
                if(CIN > 9999999)
                    ui->CIN->setStyleSheet("");
                if(ui->Nom->text() == "")
                    ui->Nom->setStyleSheet("border: 1px solid red;");
                else
                    ui->Nom->setStyleSheet("");
                if(ui->Prenom->text() == "")
                    ui->Prenom->setStyleSheet("border: 1px solid red;");
                else
                    ui->Prenom->setStyleSheet("");
                if(ui->Email->text() == "")
                    ui->Email->setStyleSheet("border: 1px solid red;");
                else
                    ui->Email->setStyleSheet("");
            }else{
                E.ajouter();
                a.ajouter(email, QString::number(CIN));
                QMessageBox::information(nullptr, QObject::tr("Gestion-Des-Employes"),
                                                  QObject::tr("      Ajout effectué!\n"
                                                              "Taper sur cancel pour fermer."), QMessageBox::Cancel);
                on_pushButton_6_clicked();
            }
        }
    }
    ui->tab_employe->setModel(E.afficher()); //Actualisation du tab
}

void application::on_pushButton_4_clicked() //Suppression
{
    Employe E;
    QModelIndex index = ui->tab_employe->currentIndex();
    int cin = index.data(Qt::DisplayRole).toInt();
    bool test = E.supprimer(cin);
    if(cin == 0) // Not selecting any of the employees
        test = false;

    if(test){
        QMessageBox::information(nullptr, QObject::tr("Gestion-Des-Employes"),
                                          QObject::tr("    Suppression effectué!\n"
                                                      "Taper sur cancel pour fermer."), QMessageBox::Cancel);
    }else{
        QMessageBox::critical(nullptr, QObject::tr("Gestion-Des-Employes"),
                                          QObject::tr("Selectionner un employé à supprimer\n"
                                                      "    Taper sur cancel pour fermer."), QMessageBox::Cancel);
    }

    ui->tab_employe->setModel(E.afficher()); //Actualisation du tab
}

void application::on_comboBox_currentIndexChanged(int index) // Tri
{
    if(index == 1)
        ui->tab_employe->setModel(E.tri_cin());
    else if(index == 2)
        ui->tab_employe->setModel(E.tri_role());
    else if(index == 3)
        ui->tab_employe->setModel(E.tri_nom());
    else
        ui->tab_employe->setModel(E.afficher());
}

void application::on_pushButton_3_clicked() // Modification
{
    ui->modify->show();
    QModelIndex index = ui->tab_employe->currentIndex();
    QString cin = index.data(Qt::DisplayRole).toString();
    QString nom = ui->tab_employe->model()->index(index.row(), 1).data(Qt::DisplayRole).toString();
    QString prenom = ui->tab_employe->model()->index(index.row(), 2).data(Qt::DisplayRole).toString();
    QString sexe = ui->tab_employe->model()->index(index.row(), 5).data(Qt::DisplayRole).toString();
    QString email = ui->tab_employe->model()->index(index.row(), 4).data(Qt::DisplayRole).toString();
    QString adresse = ui->tab_employe->model()->index(index.row(), 6).data(Qt::DisplayRole).toString();
    QString role = ui->tab_employe->model()->index(index.row(), 3).data(Qt::DisplayRole).toString();
    QString date_naiss = ui->tab_employe->model()->index(index.row(), 7).data(Qt::DisplayRole).toString();
    ui->CIN_12->setText(cin);
    ui->CIN_13->setText(nom);
    ui->CIN_14->setText(prenom);
    if(sexe == "Femelle")
        ui->Sexe_4->setCurrentIndex(1);
    else
        ui->Sexe_4->setCurrentIndex(0);
    ui->CIN_15->setText(email);
    ui->CIN_16->setText(adresse);
    if(role == "Manager")
        ui->role_4->setCurrentIndex(0);
    else
        ui->role_4->setCurrentIndex(1);
    ui->date_naiss_4->setDate(QDate::fromString(date_naiss, "dd.MM.yyyy"));
}

void application::on_pushButton_19_clicked()
{
    ui->modify->hide();
}

void application::on_pushButton_20_clicked() // Click on Confirmer
{
    int CIN = ui->CIN_12->text().toInt();
    QString Nom = ui->CIN_13->text();
    QString Prenom = ui->CIN_14->text();
    QString email = ui->CIN_15->text();
    int Sexe_answer = ui->Sexe_4->currentIndex();
    QString Sexe;
    QString adresse = ui->CIN_16->text();
    QString date_naiss = ui->date_naiss_4->date().toString("dd.MM.yyyy");
    int Role_answer = ui->role_4->currentIndex();
    QString role;

    if(Sexe_answer == 0)
        Sexe = "Male";
    else
        Sexe = "Femelle";

    if(Role_answer == 0)
        role = "Manager";
    else
        role = "Employe";

    Employe E(CIN, Nom, Prenom, email, Sexe, adresse, role, date_naiss);
    if(CIN < 9999999 || Nom == "" || Prenom == "" || email == ""){
        ui->CIN_12->setStyleSheet("border: 1px solid red;");
        if(CIN > 9999999)
            ui->CIN_12->setStyleSheet("");
        if(Nom == "")
            ui->CIN_13->setStyleSheet("border: 1px solid red;");
        else
            ui->CIN_13->setStyleSheet("");
        if(Prenom == "")
            ui->CIN_14->setStyleSheet("border: 1px solid red;");
        else
            ui->CIN_14->setStyleSheet("");
        if(email == "")
            ui->CIN_15->setStyleSheet("border: 1px solid red;");
        else
            ui->CIN_15->setStyleSheet("");
    }else{
        QMessageBox msgBox;
        msgBox.setInformativeText("Voulez vous Confirmer?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch(ret){
            case QMessageBox::Save:
                E.modifier();
                ui->tab_employe->setModel(E.afficher());
                ui->modify->hide();
                ui->CIN_12->setStyleSheet("");
                ui->CIN_13->setStyleSheet("");
                ui->CIN_14->setStyleSheet("");
                ui->CIN_15->setStyleSheet("");
                break;

            case QMessageBox::Cancel:
                break;

            default:
               break;
        }
    }
}

void application::on_pushButton_6_clicked() //clicking on Reset
{
    ui->CIN->setText("");
    ui->Nom->setText("");
    ui->Prenom->setText("");
    ui->Email->setText("");
    ui->adresse->setText("");
    ui->Sexe->setCurrentIndex(0);
    ui->role->setCurrentIndex(0);
    ui->date_naiss->setDate(QDate::currentDate());
    ui->CIN->setStyleSheet("");
    ui->Nom->setStyleSheet("");
    ui->Prenom->setStyleSheet("");
    ui->Email->setStyleSheet("");
}

void application::on_pushButton_2_clicked()
{
    ui->affichage->show();
    QModelIndex index = ui->tab_employe->currentIndex();
    QString cin = index.data(Qt::DisplayRole).toString();
    QString nom = ui->tab_employe->model()->index(index.row(), 1).data(Qt::DisplayRole).toString();
    QString prenom = ui->tab_employe->model()->index(index.row(), 2).data(Qt::DisplayRole).toString();
    QString sexe = ui->tab_employe->model()->index(index.row(), 5).data(Qt::DisplayRole).toString();
    QString email = ui->tab_employe->model()->index(index.row(), 4).data(Qt::DisplayRole).toString();
    QString adresse = ui->tab_employe->model()->index(index.row(), 6).data(Qt::DisplayRole).toString();
    QString role = ui->tab_employe->model()->index(index.row(), 3).data(Qt::DisplayRole).toString();
    QString date_naiss = ui->tab_employe->model()->index(index.row(), 7).data(Qt::DisplayRole).toString();

    ui->cin_text_2->setText(cin);
    ui->nom_text_2->setText(nom);
    ui->prenom_text_2->setText(prenom);
    ui->sexe_text_2->setText(sexe);
    ui->email_text_2->setText(email);
    ui->adresse_text_2->setText(adresse);
    ui->role_text_2->setText(role);
    ui->date_naiss_text_2->setText(date_naiss);
}

void application::on_pushButton_17_clicked()
{
    ui->affichage->hide();
}

void application::on_pushButton_15_clicked() // PDF Button
{
    QModelIndex index = ui->tab_employe->currentIndex();
    QString cin = index.data(Qt::DisplayRole).toString();
    QString nom = ui->tab_employe->model()->index(index.row(), 1).data(Qt::DisplayRole).toString();
    QString prenom = ui->tab_employe->model()->index(index.row(), 2).data(Qt::DisplayRole).toString();
    QString sexe = ui->tab_employe->model()->index(index.row(), 5).data(Qt::DisplayRole).toString();
    QString email = ui->tab_employe->model()->index(index.row(), 4).data(Qt::DisplayRole).toString();
    QString adresse = ui->tab_employe->model()->index(index.row(), 6).data(Qt::DisplayRole).toString();
    QString role = ui->tab_employe->model()->index(index.row(), 3).data(Qt::DisplayRole).toString();
    QString date_naiss = ui->tab_employe->model()->index(index.row(), 7).data(Qt::DisplayRole).toString();
    QString filename = "E:/hafeu/Documents/Gestion-Des-Employes/PDF files/" + nom + " " + prenom + ".pdf";
    QPrinter printer;
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(filename);
    QPainter painter;
    QFont logoname("Corbel Light", 25, QFont::Bold), title("Bahnschrift Light", 25, QFont::Bold), f("Aria", 18, QFont::Bold), f1("Arial", 15);
        if (! painter.begin(&printer)) { // failed to open file
            qWarning("failed to open file, is it writable?");
        }
    QImage logo("C:/Users/hafeu/Desktop/Hafedh/Personal-Repository/2A23/Semester 1/Projet QT/Organization/application pictures/logo_simple.png");
    QPoint logo_cordinates(40, 40);
        painter.drawImage(logo_cordinates, logo);
        painter.setFont(logoname); painter.drawText(620, 80, "C-Space");
        painter.setFont(title);
        painter.drawText(200, 180, "Information d'Employé(e)");
        painter.setFont(f1);
        painter.drawText(80, 250, "CIN: "); painter.drawText(80, 280, "Nom: ");
        painter.drawText(80, 310, "Prenom: "); painter.drawText(80, 340, "Sexe:");
        painter.drawText(80, 370, "Email: "); painter.drawText(80, 400, "Adresse: ");
        painter.drawText(80, 430, "Role: "); painter.drawText(80, 460, "Date de Naissance: ");
        painter.setFont(f1);
        painter.drawText(125, 250, cin); painter.drawText(130, 280, nom);
        painter.drawText(160, 310, prenom); painter.drawText(135, 340, sexe);
        painter.drawText(140, 370, email); painter.drawText(165, 400, adresse);
        painter.drawText(130, 430, role); painter.drawText(255, 460, date_naiss);
        painter.end();
        QMessageBox::information(nullptr, QObject::tr("Gestion-Des-Employes"),
                                          QObject::tr(" PDF a été crée avec succes!\n"
                                                      "Taper sur cancel pour fermer."), QMessageBox::Cancel);
}

void application::on_CIN_recherche_textChanged(const QString &arg1) //Recherche avancé
{
    ui->tab_employe->setModel(E.rechercherNom(arg1));
}

void application::on_pushButton_16_clicked()
{
    QModelIndex index = ui->tab_employe->currentIndex();
    QString email = ui->tab_employe->model()->index(index.row(), 4).data(Qt::DisplayRole).toString();
    mail *m = new mail(this);
    m->getUi()->rcpt->setText(email);
    m->show();
}

void application::on_pushButton_8_clicked()
{
    MainWindow * a = new MainWindow();
    QString text = ui->lineEdit->text();
    QString username;
    for(int i=0; i<ui->username->text().length()-10; i++)
        username[i] = ui->username->text()[i];
    text = username + " a été déconnecté(e).";
    socket.write(QByteArray::fromStdString(text.toStdString()));
    QMessageBox::information(nullptr, QObject::tr("Gestion-Des-Employes"),
                                      QObject::tr(" Déconnection avec succes!\n"
                                                  "Taper sur cancel pour fermer."), QMessageBox::Cancel);
    application::close();
    a->show();
}

//chat
void application::onReadyRead()
{
    QByteArray data = socket.readAll();

    ui->textEdit->append(QString::fromStdString(data.toStdString()));
}

void application::onSendButtonPressed()
{
    if(ui->lineEdit->text() == "")
        ui->lineEdit->setStyleSheet("border: 1px solid red;");
    else{
        ui->lineEdit->setStyleSheet("");
        QTime t = t.currentTime();
        QString text = ui->lineEdit->text();
        QString username;
        for(int i=0; i<ui->username->text().length()-10; i++)
            username[i] = ui->username->text()[i];
        text = "[" + QString::number(t.hour()) + ":" + QString::number(t.minute()) + "] " + username + ": " + text;
        ui->lineEdit->setText("");
        socket.write(QByteArray::fromStdString(text.toStdString()));
   }
}

void application::on_commandLinkButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
