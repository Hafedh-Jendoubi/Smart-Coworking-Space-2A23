#ifndef STAT_CLIENT_H
#define STAT_CLIENT_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>


QT_CHARTS_USE_NAMESPACE

namespace Ui {
class stat_client;
}

class stat_client : public QDialog
{
    Q_OBJECT

public:
    explicit stat_client(QWidget *parent = nullptr);
    ~stat_client();
    QChartView *chartView ;
    void choix_pie();
private:
    Ui::stat_client *ui;

};


#endif // STAT_CLIENT_H
