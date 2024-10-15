#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineSeries>
#include <QChartView>
#include <QChart>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->normalBtn->setIcon(QIcon(":/image/peiqi.png"));
	ui->normalBtn->setIconSize(QSize(30, 30));
	creatChartDemo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::creatChartDemo()
{
    QLineSeries* series = new QLineSeries();
	series->append(0, 6);
	series->append(2, 4);
	series->append(3, 8);
	series->append(7, 4);
	series->append(10, 5);

    QChart* chart = new QChart();
	chart->legend()->hide();
	chart->addSeries(series);
	chart->createDefaultAxes();
	ui->m_pChartView->setChart(chart);
	ui->m_pChartView->setRenderHint(QPainter::Antialiasing);
}

