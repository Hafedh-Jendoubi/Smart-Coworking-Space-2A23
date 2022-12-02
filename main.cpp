#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;
    bool test = c.CreateConnect();
    MainWindow m;
    if(test){
        m.show();
        c.ShowPositiveConnectMsg();
    }else{
        c.ShowNegativeConnectMsg();
    }

    return a.exec();
}
