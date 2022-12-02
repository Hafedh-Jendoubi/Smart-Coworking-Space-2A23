#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

class Connection{
  public:
    Connection() {};
    bool CreateConnect();
    void ShowPositiveConnectMsg();
    void ShowNegativeConnectMsg();
};

#endif // CONNECTION_H
