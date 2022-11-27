#include "mainwindow.h"
#include "authentification.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;
    bool test = c.CreateConnect();
    authentification w;
    if(test){
        w.show();
        c.ShowPositiveConnectMsg();
    }else{
        c.ShowNegativeConnectMsg();
    }

    return a.exec();
}
