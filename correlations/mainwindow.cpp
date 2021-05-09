#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include "stats.h"
#include <iostream>
#include <QLineEdit>
#include <QLabel>
#include <QSizePolicy>

class Chart{
    public:

    static QChartView* Make_q(const features::quantitative &q)
    {
        QBarSeries *series = new QBarSeries();

        QBarSet *set_q = new QBarSet("Quantity");
        QBarSet *set_e = new QBarSet("Empty");
        QBarSet *set_u = new QBarSet("Unique");

        *set_q << q.quantity;
        *set_e << q.empty;
        *set_u << q.unique;

        series->append(set_q);
        series->append(set_e);
        series->append(set_u);

        QChart *chart = new QChart();
        chart->addSeries(series);

        chart->setTitle("Quantitative characteristics");
        chart->setAnimationOptions(QChart::SeriesAnimations);


        QStringList categories;
        categories << "";
        QBarCategoryAxis *axis = new QBarCategoryAxis();
        axis ->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        QChartView *chartView = new QChartView(chart);
        return chartView;
    }

    template<typename T>
    static QChartView* Make_s(const features::stat<T> &s)
    {
        QBarSeries *series = new QBarSeries();

        QBarSet *set_max  = new QBarSet("Max");
        QBarSet *set_min  = new QBarSet("Min");
        QBarSet *set_mean = new QBarSet("Mean");
        QBarSet *set_s    = new QBarSet("Standard deviation");
        QBarSet *set_med  = new QBarSet("Median");
        QBarSet *set_q    = new QBarSet("Quartile range");

        if constexpr (!std::is_same_v<T, std::string>) {
            *set_max  << s.max;
            *set_min  << s.min;
            *set_mean << s.mean;
            *set_s    << s.standard_deviation;
            *set_med  << s.median;
            *set_q    << s.quartile_range;
        } else {
            *set_max  << -1;
            *set_min  << -1;
            *set_mean << -1;
            *set_s    << -1;
            *set_med  << -1;
            *set_q    << -1;
        }

        series->append(set_max );
        series->append(set_min );
        series->append(set_mean);
        series->append(set_s   );
        series->append(set_med );
        series->append(set_q   );

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Statistical characteristics");

        chart->setAnimationOptions(QChart::SeriesAnimations);


        QStringList categories;
        categories << "";
        QBarCategoryAxis *axis = new QBarCategoryAxis();
        axis ->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        QChartView *chartView = new QChartView(chart);
        return chartView;
    }
};

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::MainWindow(::features::data<std::string> d, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    show_data(d);
}

void MainWindow::show_data(::features::data<std::string> d)
{
    QLabel *l = new QLabel(d.name.data(), this);
    Chart::Make_q(d.q)->setParent(ui->horizontalFrame);
    Chart::Make_s(d.s)->setParent(ui->horizontalFrame_2);
}

MainWindow::MainWindow(::features::data<int> d, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    show_data(d);
}

void MainWindow::show_data(::features::data<int> d)
{
    QLabel *l = new QLabel(d.name.data(), this);
    Chart::Make_q(d.q)->setParent(ui->horizontalFrame);
    Chart::Make_s(d.s)->setParent(ui->horizontalFrame_2);
}

MainWindow::MainWindow(::features::data<double> d, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    show_data(d);
}

void MainWindow::show_data(::features::data<double> d)
{
    QLabel *l = new QLabel(d.name.data(), this);
    Chart::Make_q(d.q)->setParent(ui->horizontalFrame);
    Chart::Make_s(d.s)->setParent(ui->horizontalFrame_2);
}

MainWindow::MainWindow(const ::sample<std::string> &s, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    show_data({s.name, s.get_quantitative(), s.get_stat()});
}
MainWindow::MainWindow(const ::sample<int> &s, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    show_data({s.name, s.get_quantitative(), s.get_stat()});
}
MainWindow::MainWindow(const ::sample<double> &s, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    show_data({s.name, s.get_quantitative(), s.get_stat()});
}
