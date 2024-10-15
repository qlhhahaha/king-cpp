#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->normalBtn->setIcon(QIcon(":/image/peiqi.png"));
    ui->normalBtn->setIconSize(QSize(30, 30));
}

MainWindow::~MainWindow()
{
    delete ui;
}

