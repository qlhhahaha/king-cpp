#include "kmainwindow.h"
#include <QPixmap>
#include <cmath>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>



KMainWindow::KMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //ui.label->setPixmap(QPixmap(":/image/peiqi.png"));
    QPushButton* areaButton = findChild<QPushButton*>("areaButton");
    QPushButton* circumferenceButton = findChild<QPushButton*>("circumferenceButton");
    QLineEdit* radiusEdit = findChild<QLineEdit*>("radiusEdit");
    QLabel* resultLabel = findChild<QLabel*>("resultLabel");

    connect(areaButton, &QPushButton::clicked, this, [=]() {
        bool ok;
        double radius = radiusEdit->text().toDouble(&ok);
        if (ok) {
            double area = 3.14 * radius * radius;
            resultLabel->setText("结果是：" + QString::number(area));
        }
        else {
            resultLabel->setText("结果是：请输入有效的半径");
        }
        });

    connect(circumferenceButton, &QPushButton::clicked, this, [=]() {
        bool ok;
        double radius = radiusEdit->text().toDouble(&ok);
        if (ok) {
            double circumference = 2 * 3.14 * radius;
            resultLabel->setText("结果是：" + QString::number(circumference));
        }
        else {
            resultLabel->setText("结果是：请输入有效的半径");
        }
        });


}

