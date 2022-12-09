#include "stat_client.h"
#include "ui_stat_client.h"

stat_client::stat_client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::stat_client)
{
    ui->setupUi(this);
}

stat_client::~stat_client()
{
    delete ui;
}

//pie chart
    void stat_client::choix_pie()
    {
    QSqlQuery q1,q2,q3;
    qreal tot=0,c1=0,c2=0;

    q1.prepare("SELECT * FROM CLIENTS");
    q1.exec();

    q2.prepare("SELECT * FROM CLIENTS WHERE ROLE='teacher'");
    q2.exec();

    q3.prepare("SELECT * FROM CLIENTS WHERE ROLE='client'");
    q3.exec();



    while (q1.next()){tot++;}
    while (q2.next()){c1++;}
    while (q3.next()){c2++;}


    c1=c1/tot;
    c2=c2/tot;

    // Define slices and percentage of whole they take up
    QPieSeries *series = new QPieSeries();
    series->append("TEACHER",c1);
    series->append("CLIENT",c2);



    // Create the chart widget
    QChart *chart = new QChart();

    // Add data to chart with title and show legend
    chart->addSeries(series);
    chart->legend()->show();


    // Used to display the chart
    chartView = new QChartView(chart,ui->graphicsView);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400,400);

    chartView->show();
    }
