#ifndef __KINPUTWINDOW_H__
#define __KINPUTWINDOW_H__

#include <QtWidgets/QMainWindow>
#include "ui_kinputwindow.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPixmap>
#include "handswriting.h"


class KInputWindow : public QMainWindow
{
    Q_OBJECT

public:
    KInputWindow(QWidget* parent = Q_NULLPTR);
    ~KInputWindow();

    handsInput inputs;
	std::vector<std::string> inputResult;

protected:
    void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
    //void onBtnWordClicked_1();
    //void onBtnBackClicked();
    void onClearBtnClicked();


private:
    Ui::KInputWindowClass ui;

};

#endif
