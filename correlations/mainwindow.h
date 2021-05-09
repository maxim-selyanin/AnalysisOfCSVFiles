#pragma once

#include <QMainWindow>
#include <QtCharts>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QPushButton>
#include "stats.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(::features::data<std::string> d, QWidget *parent = nullptr);
    MainWindow(::features::data<int> d, QWidget *parent = nullptr);
    MainWindow(::features::data<double> d, QWidget *parent = nullptr);

    MainWindow(const ::sample<std::string> &s, QWidget *parent = nullptr);
    MainWindow(const ::sample<int> &s, QWidget *parent = nullptr);
    MainWindow(const ::sample<double> &s, QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    void show_data(::features::data<std::string> d);
    void show_data(::features::data<int> d);
    void show_data(::features::data<double> d);

    Ui::MainWindow *ui;
};
