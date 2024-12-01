#ifndef __KMAINWINDOW_H__
#define __KMAINWINDOW_H__

#include <QtWidgets/QMainWindow>
#include "ui_kmainwindow.h"
#include <QLabel>


class KMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    KMainWindow(QWidget* parent = Q_NULLPTR);
private:
    Ui::KMainWindowClass ui;

};

#endif
