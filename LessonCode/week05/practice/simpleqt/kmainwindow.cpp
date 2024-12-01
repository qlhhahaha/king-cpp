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
            resultLabel->setText("����ǣ�" + QString::number(area));
        }
        else {
            resultLabel->setText("����ǣ���������Ч�İ뾶");
        }
        });

    connect(circumferenceButton, &QPushButton::clicked, this, [=]() {
        bool ok;
        double radius = radiusEdit->text().toDouble(&ok);
        if (ok) {
            double circumference = 2 * 3.14 * radius;
            resultLabel->setText("����ǣ�" + QString::number(circumference));
        }
        else {
            resultLabel->setText("����ǣ���������Ч�İ뾶");
        }
        });


}

