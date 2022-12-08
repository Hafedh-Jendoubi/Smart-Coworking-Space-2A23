#include "application.h"
#include "ui_application.h"

application::application(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::application)
{
    ui->setupUi(this);

    //Partie Employé(e):
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

    //Partie Session:
    int ret=A.connect_arduino(); //lancer la connexion
    switch(ret)
    {
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
    QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));
    ui->idmat1->setValidator(new QIntValidator(1,10,this));
    ui->le_nbr->setValidator(new QIntValidator(1,30,this));
    ui->le_salle->setValidator(new QIntValidator(1,30,this));
    ui->tab1->setModel(S.afficher());
    notifier = new QSystemTrayIcon(this);
    notifier->setIcon(QIcon("C:/Users/Admin/Desktop/qt documents/Atelier_Connexion/notification.jpg"));
    notifier->show();

    //iheb
    ui->le_prix_1->setValidator (new QIntValidator(0, 9999999, this));
    ui->tab_materiel_1->setModel(M.afficher());
    QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label_1()));
    QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label_2()));

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
    QString filename = "C:/Users/hafeu/Desktop/" + nom + " " + prenom + ".pdf";
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

void application::on_pushButton_8_clicked() //Déconnection a partir du Employe
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

//Partie Session:

void application::on_pushButton_ajouter_clicked()
{
    int idmat=ui->idmat1->text().toInt();
    QString matiere=ui->lemat->text();
    QString nom=ui->le_nom->text();
    QString prenom=ui->le_prenom->text();
    int nbreleve=ui->le_nbr->text().toInt();
    int numsalle=ui->le_salle->text().toInt();
    Session S(idmat,matiere,nom,prenom,nbreleve,numsalle);
   bool test=S.ajouter();
   if(test)
   {
       ui->tab1->setModel(S.afficher());
       notifier->showMessage("Ok"," Session ajoutée",QSystemTrayIcon::Information,10000);
       QMessageBox::information(nullptr, QObject::tr("ok"),
                   QObject::tr("AJOUT successful.\n"
                               "Click Cancel to exit."), QMessageBox::Cancel);

   }
   else
       QMessageBox::critical(nullptr, QObject::tr("NOT OK "),
                   QObject::tr("AJOUT failed.\n"
                               "Click Cancel to exit."), QMessageBox::Cancel);

}

void application::on_butto_supp_clicked()
{
    Session S1;
    S1.setidmat(ui->supp->text().toInt());
    bool test=S1.supprimer(S1.getidmat());
    QMessageBox msg;
    if(test)
        msg.setText("supp avec succes");
        else
        msg.setText("echec de supp");

 notifier->showMessage("Ok"," session supprimer",QSystemTrayIcon::Information,10000);
 ui->tab1->setModel(S.afficher());
   msg.exec();
}

void application::on_update_clicked()
{
    int idmat=ui->u_idmat->text().toInt();
    QString matiere=ui->u_matiere->text();
    QString nom=ui->u_nom->text();
    QString prenom=ui->u_pre->text();
    int nbreleve=ui->u_nbr->text().toInt();
    int numsalle=ui->u_num->text().toInt();
    Session S(idmat,matiere,nom,prenom,nbreleve,numsalle);
   bool test=S.modifier();
   if(test)
   {
        notifier->showMessage("Ok"," session modifier",QSystemTrayIcon::Information,10000);
       QMessageBox::information(nullptr, QObject::tr("ok"),
                   QObject::tr("modifier avec successful.\n"
                               "Click Cancel to exit."), QMessageBox::Cancel);

   }
   else
       QMessageBox::critical(nullptr, QObject::tr("NOT OK "),
                   QObject::tr("update failed.\n"
                               "Click Cancel to exit."), QMessageBox::Cancel);
   ui->tab1->setModel(S.afficher());

}

void application::on_comboBox_2_currentIndexChanged(int index)
{
    if(index == 1)
        ui->tab1->setModel(S.tri_idmat());
    else if(index==2)
        ui->tab1->setModel(S.tri_nom());
    else if(index==3)
        ui->tab1->setModel(S.tri_numerosalle());
    else if(index==4)
        ui->tab1->setModel(S.tri_nombre_eleve());

}

void application::on_pushButton_recherche_clicked()
{
    QString n=ui->recherche->text();
    ui->tab1->setModel(S.recherche(n));
}

void application::on_pushButton_PDF_clicked()
{
    Session s;
    QPainter painter;
    QString text=s.apercupdf();

    ui->textEdit_2->setText(text);
    QPrinter printer ;
    printer.setPrinterName("imprimer");
    QImage logo("C:/Users/hafeu/Desktop/Hafedh/Personal-Repository/2A23/Semester 1/Projet QT/Organization/application pictures/logo_simple.png");
    QPoint logo_cordinates(40, 40);
    painter.drawImage(logo_cordinates, logo);
    QPrintDialog dialog (&printer,this);
    if(dialog.exec()==QDialog::Rejected) return ;
    ui->textEdit_2->print(&printer);
}

void application::on_pushButton_envoyer_clicked()
{
    Smtp* smtp = new Smtp("mohamedkhalil.debaieb@esprit.tn", "211JMT5311", "smtp.gmail.com", 465);

       connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));

       QString a=ui->rcpt->text();

       QString b=ui->subject->text();

       QString c=ui->msg->text();


       smtp->sendMail("mohamedkhalil.debaieb@esprit.tn", a , b,c);
}

void application::on_pushButton_image_clicked()
{
    QString dir = QFileDialog::getOpenFileName(this,tr("Select Picture"),"C:/Users/Admin/Desktop/capture.png");
        QPixmap pic(dir);
        int x = ui->image->width();
        int y = ui->image->height();
        ui->image->setPixmap(pic.scaled(x,y,Qt::KeepAspectRatio));
}

void application::update_label()
{
    data=A.read_from_arduino();

    if(data=="1"){
        ui->label_47->setText("Fire  detected");
        QTime Time  =QTime::currentTime() ;
        S.Date1 = Time.toString("H:m:s a") ;
        S.ajouter1();
        ui->tableView->setModel(S.afficher1());
    }else{
        ui->label_47->setText("Fire NOT detected");
    }
}

void application::on_pushButton_7_clicked()
{
     A.write_to_arduino("5");
}

void application::on_pushButton_21_clicked() //Déconnection a partir du Session
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

//Partie Client:

void application::on_pushButton_9_clicked() //Déconnection a partir du Client
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

//Partie Salle:


//Partie Materiel:

void application::on_pb_ajouter_1_clicked()
{
   int prix=ui->le_prix_1->text().toInt();
   QString reference=ui->le_reference_1->text();
   QString type=ui->le_type_1->text();
   QString stock=ui->le_stock_1->text();
   Materiel M(prix,reference,type,stock);
   bool test=M.ajouter();
       if(test){
           QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("Ajout effectué\n Click Cancel to exit."),QMessageBox::Cancel);
           ui->tab_materiel_1->setModel(M.afficher());
       }
       else QMessageBox::information(nullptr, QObject::tr("NOT OK"),QObject::tr("Ajout non effectué\n Click Cancel to exit."),QMessageBox::Cancel);




}

void application::on_pb_supprimer_1_clicked()
{
    int prix=ui->le_prix_1->text().toInt();
    bool test=M.supprimer(prix);
        if(test){
            QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("suppression effectué\n Click Cancel to exit."),QMessageBox::Cancel);
            ui->tab_materiel_1->setModel(M.afficher());
        }
        else QMessageBox::information(nullptr, QObject::tr("NOT OK"),QObject::tr("Suppression non effectué\n Click Cancel to exit."),QMessageBox::Cancel);


}

void application::on_comboBox_1_activated(const QString &arg1)
{
    QProcess *process = new QProcess(this);
    QString file = QDir::currentPath() + "/build.bat";
    qDebug() << file;
   // process->start(file);
//    QDesktopServices::openUrl(QUrl("file:///"+file, QUrl::TolerantMode));
}

void application::on_comboBox_3_1_activated(const QString &arg1)
{
    QString url = "start https://cse.google.com/cse?cx=f108d86fed230418a#gsc.q=" + arg1 + "%20intext%3Atnd";
    system(url.toStdString().c_str());
}



void application::on_REF_recherche_1_textChanged(const QString &arg1)
{
    Materiel mat;
       QString rech=ui->REF_recherche_1->text();
       ui->tab_materiel_1->setModel(mat.RechercheMateriel(rech));
}

void application::on_pb_trier_1_clicked()
{
    Materiel P;


            ui->tab_materiel_1->setModel(P.tri_nomcroissant());


            bool test=P.tri_nomcroissant();//tri produit
            if (test)
            {


                QMessageBox::information(nullptr,QObject::tr("ok"),
                                         QObject::tr("tri croissante effectué \n"
                                                     "Click Cancel to exist ."),QMessageBox::Cancel);

            }
            else
                 {QMessageBox::critical(nullptr, QObject::tr("nonnnn"),
                              QObject::tr("tri croissante failed.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);}

        }





void application::on_pb_ajouter_2_1_clicked()
{

        QPdfWriter pdf("C:/Users/hafeu/Desktop/PDF_materiel.pdf");

           QPainter painter(&pdf);
           int i = 4000;
                  painter.setPen(Qt::red);
                  painter.setFont(QFont("Time New Roman", 25));
                  painter.drawText(3000,1400,"Liste Des materiels");
                  painter.setPen(Qt::black);
                  painter.setFont(QFont("Time New Roman", 15));
                  painter.drawRect(100,3000,9400,500);
                  painter.setFont(QFont("Time New Roman", 9));
                  painter.drawText(1000,3300,"Prix");
                  painter.drawText(2500,3300,"reference");
                  painter.drawText(4000,3300,"type");
                  painter.drawText(5500,3300,"stock");
                  painter.drawRect(100,3000,9400,9000);

                  QSqlQuery query;
                  query.prepare("select * from TABLE1");
                  query.exec();
                  while (query.next())
                  {
                      painter.drawText(1000,i,query.value(0).toString());
                      painter.drawText(2500,i,query.value(1).toString());
                      painter.drawText(4000,i,query.value(2).toString());
                      painter.drawText(5500,i,query.value(3).toString());
                      painter.drawText(7000,i,query.value(4).toString());


                     i = i + 350;
                  }
                  QMessageBox::information(this, QObject::tr("PDF Enregistré!"),
                  QObject::tr("PDF Enregistré!.\n" "Click Cancel to exit."), QMessageBox::Cancel);
    }
void application::on_pb_modifier_1_clicked()
{
    int prix = ui->le_prix_1->text().toInt();
    QString reference= ui->le_reference_1->text();
    QString type = ui->le_type_1->text();
    QString stock = ui->le_stock_1->text();


  Materiel M (prix,reference,type,stock);
  bool test=M.modifier();
  if(test)
{

      ui->tab_materiel_1->setModel(M.afficher());//refresh
QMessageBox::information(nullptr, QObject::tr("modifier une matériel"),
                  QObject::tr("matériel modifié.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);

}
  else
      QMessageBox::critical(nullptr, QObject::tr("modifier une matériel"),
                  QObject::tr("Erreur !.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);

}


void application::on_pushButton_1_clicked()
{
    A.write_to_arduino("1");
}

void application::on_pushButton_2_1_clicked()
{
    A.write_to_arduino("0");
}

void application::update_label_1()
{
    data=A.read_from_arduino();
            if(data=="1")
            ui->label_18_1->setText("porte ouverte");
            else

            if (data=="0")
            ui->label_18_1->setText("porte fermee");
}

//iheb


void application::on_pd_ajouter_1_clicked()
{      int id =ui->leid_1 ->text ().toInt();
       QString nom=ui->lenom_1->text();
       QString prenom=ui->leprenom_1->text();
       QString datenaissance=ui->ledatenaissance_1->text();
       int CIN=ui->lecin_1->text().toInt();
       QString Email=ui->leemail_1->text();
      int telephone=ui->letelephone_1->text().toInt();
       QString profession=ui->leprofeesion_1->text();
       QString role=ui->lerole_1->text();
    Client C(id, nom , prenom , Email , CIN , datenaissance, telephone , profession , role ) ;
       bool test=C.ajouter();
          QMessageBox msgBox;
          if (test)
               msgBox.setText("Ajout avec succès");
          else
               msgBox.setText("Echec de l'ajout");
               msgBox.exec();
       }



//void application::on_lerole_cursorPositionChanged(int arg1, int arg2)
//{

//}

void application::on_pushButton_5_clicked()
{
    Client c;
    ui->tableClient_1->setModel(c.afficher());

}


void application::on_delete_btn_1_clicked()
{   Client c;
    int id = ui->id_delete_1->text().toInt();
    bool test=c.supprimer(id);
       QMessageBox msgBox;
       if (test && id!=0){
            msgBox.setText("Supprimer avec succès");
            ui->tableClient_1->setModel(c.afficher());}
       else
            msgBox.setText("delete error");
            msgBox.exec();

}

void application::on_pd_ajouter_2_1_clicked()
{
    int id =ui->leid_2_1->text ().toInt();
          QString nom=ui->lenom_2_1->text();
          QString prenom=ui->leprenom_2_1->text();
          QString datenaissance=ui->ledatenaissance_2_1->text();
          int CIN=ui->lecin_2_1->text().toInt();
          QString Email=ui->leemail_2_1->text();
         int telephone=ui->letelephone_2_1->text().toInt();
          QString profession=ui->leprofeesion_2_1->text();
          QString role=ui->lerole_2_1->text();
       Client C(id, nom , prenom , Email , CIN , datenaissance, telephone , profession , role ) ;
          bool test=C.modifier();
             QMessageBox msgBox;
             if (test)
                  msgBox.setText("Modification avec succès");
             else
                  msgBox.setText("Echec de la Modification");
                  msgBox.exec();

}


void application::on_comboTri_1_currentTextChanged(const QString &arg1)
{      Client C ;
    QString tri = ui->comboTri_1->currentText();
           if(tri=="profession"){
            ui->tableClient_1->setModel(C.AfficherTrieprofession());
    }else if (tri=="email"){
               ui->tableClient_1->setModel(C.AfficherTrieemail());
           }else
               {
               ui->tableClient_1->setModel(C.AfficherTrierole());
                }

}




void application::on_pdf_1_clicked()
{
    QString strStream;
                QTextStream out(&strStream);

                const int rowCount = ui->tableClient_1->model()->rowCount();
                const int columnCount = ui->tableClient_1->model()->columnCount();

                out <<  "<html>\n"
                    "<head>\n"
                    "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                    <<  QString("<title>%1</title>\n").arg("strTitle")
                    <<  "</head>\n"
                    "<body bgcolor=#ffffff link=#5000A0>\n"

                    //     "<align='right'> " << datefich << "</align>"
                    "<center><h1 style='color:#732626;'>Liste des clients</h1></br></br><table border=1 cellspacing=0 cellpadding=2 color=#732626>\n";

                // headers

                //les noms des colonnes
                out << "<thead><tr bgcolor=#f0f0f0> <th style='color:#732626;'>Numero</th>";
                for (int column = 0; column < columnCount; column++)
                    if (!ui->tableClient_1->isColumnHidden(column))
                        out << QString("<th style='color:#732626;'>%1</th>").arg(ui->tableClient_1->model()->headerData(column, Qt::Horizontal).toString());
                out << "</tr></thead>\n";


                //remplir le tableau
                for (int row = 0; row < rowCount; row++)
                {
                    out << "<tr> <td  bkcolor=0>" << row + 1 << "</td>";
                    for (int column = 0; column < columnCount; column++)
                    {
                        if (!ui->tableClient_1->isColumnHidden(column))
                        {
                            QString data = ui->tableClient_1->model()->data(ui->tableClient_1->model()->index(row, column)).toString().simplified();
                            out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                        }
                    }
                    out << "</tr>\n";
                }
                out <<  "</table> </center>\n"
                    "</body>\n"
                    "</html>\n";

                QString fileName = QFileDialog::getSaveFileName((QWidget * )0, "Sauvegarder en PDF", QString(), "*.pdf");
                if (QFileInfo(fileName).suffix().isEmpty())
                {
                    fileName.append(".pdf");
                }

                QPrinter printer (QPrinter::PrinterResolution);
                printer.setOutputFormat(QPrinter::PdfFormat);
                printer.setPaperSize(QPrinter::A4);

                printer.setOutputFileName(fileName);

                QTextDocument doc;
                doc.setHtml(strStream);
                doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
                doc.print(&printer);
}



void application::on_rechercherText_1_textChanged(const QString &arg1)
{

          ui->tableClient_1->setModel(C.rechercher(arg1));
}


void application::on_pushButton_3_1_clicked()
{
    stat_client *s = new stat_client();

            s->setWindowTitle("Les Statistiques selon le role");
            s->choix_pie();
            s->show();
}



void application::update_label_2(){


    data=A.read_from_arduino();
if(data=="1")
   {ui->label_19_1 ->setText("Fire detected");



}
else{
   ui->label_19_1 ->setText("Fire NOT detected");


}

}

void application::on_pushButton_2_2_clicked()
{
    Smtp* smtp = new Smtp("medmahdi.selmi@esprit.tn","211JMT5848", "smtp.gmail.com", 465);

    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));

    QString a=ui->recvr_1->text();

    QString b=ui->subject_1->text();

    QString c=ui->mail_1->text();



    smtp->sendMail("medmahdi.selmi@esprit.tn", a , b,c);
}

void application::on_pushButton_4_1_clicked()
{
    A.write_to_arduino("5");
}


void application::on_sms_1_clicked()
{
    Smtp_client* smtp = new Smtp_client("medmahdi.selmi@esprit.tn","211JMT5848", "smtp.gmail.com");
                //connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));


                    smtp->sendMail("medmahdi.selmi@esprit.tn", "medmahdi.selmi@esprit.tn" , ui->message_1->text()," ");
}

void application::on_sendBtn_clicked()
{
    Smtp* smtp = new Smtp("mediheb.arfaoui@esprit.tn", "211JMT9146", "smtp.gmail.com", 465);

        connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));

        QString a=ui->email->text();

        QString b=ui->objet->text();

        QString c=ui->message->text();


        smtp->sendMail("mediheb.arfaoui@esprit.tn", a , b,c);
}

void application::on_commandLinkButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void application::on_commandLinkButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void application::on_commandLinkButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void application::on_commandLinkButton_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void application::on_commandLinkButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void application::on_commandLinkButton_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void application::on_commandLinkButton_11_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void application::on_commandLinkButton_14_clicked()
{
   ui->stackedWidget->setCurrentIndex(3);
}

void application::on_commandLinkButton_15_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void application::on_commandLinkButton_17_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void application::on_commandLinkButton_18_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void application::on_commandLinkButton_19_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
