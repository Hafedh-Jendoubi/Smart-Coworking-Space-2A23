#include "connection.h"

bool Connection::CreateConnect()
{
    bool test = false;

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("test-bd");
    db.setUserName("mahdi");
    db.setPassword("mahdi");

    if(db.open())
       test = true;

    return test;
}

void Connection::ShowPositiveConnectMsg()
{
    QMessageBox::information(nullptr, QObject::tr("Database is open"),
                                      QObject::tr("Connection successful.\n"
                                                  "Click cancel to exit."), QMessageBox::Cancel);

}

void Connection::ShowNegativeConnectMsg()
{
    QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                                   QObject::tr("Connection failed.\n"
                                               "Click cancel to exit."), QMessageBox::Cancel);
}
