#include "kinputwindow.h"

KInputWindow::KInputWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // ��ʼ��ͼƬ��Դ
    QPixmap* pixmap = new QPixmap(":/image/logo.png");
    pixmap->scaled(ui.label->size(), Qt::KeepAspectRatio);
    ui.label->setScaledContents(true);
    ui.label->setPixmap(*pixmap);


    // ��ʼ����д����
    WId hShow = ui.inputZone->winId();
    this->inputs.inputInit((HWND)hShow);

    // ���Ӹ��������ź�
    connect(ui.clearStrokeBtn, &QPushButton::clicked, this, &KInputWindow::onClearBtnClicked);
}


KInputWindow::~KInputWindow() {
    
}
 

void KInputWindow::mouseReleaseEvent(QMouseEvent* e) {
    this->inputResult.clear();
    this->inputs.recognize(this->inputResult);

    ui.guessBtn1->setText(QString::fromUtf8(QByteArray(inputResult[0].c_str(), inputResult[0].size())));
    ui.guessBtn2->setText(QString::fromUtf8(QByteArray(inputResult[1].c_str(), inputResult[1].size())));
    ui.guessBtn3->setText(QString::fromUtf8(QByteArray(inputResult[2].c_str(), inputResult[2].size())));
    ui.guessBtn4->setText(QString::fromUtf8(QByteArray(inputResult[3].c_str(), inputResult[3].size())));
    ui.guessBtn5->setText(QString::fromUtf8(QByteArray(inputResult[4].c_str(), inputResult[4].size())));
    ui.guessBtn6->setText(QString::fromUtf8(QByteArray(inputResult[5].c_str(), inputResult[5].size())));
    ui.guessBtn7->setText(QString::fromUtf8(QByteArray(inputResult[6].c_str(), inputResult[6].size())));
    ui.guessBtn8->setText(QString::fromUtf8(QByteArray(inputResult[7].c_str(), inputResult[7].size())));
    ui.guessBtn9->setText(QString::fromUtf8(QByteArray(inputResult[8].c_str(), inputResult[8].size())));
}


void KInputWindow::onClearBtnClicked() {
    this->inputs.clearStrokes();  // ����洢�ıʴ�
    this->inputResult.clear();  // ���ʶ����

    ui.inputZone->clear();
    ui.guessBtn1->setText("1");
    ui.guessBtn2->setText("2");
    ui.guessBtn3->setText("3");
    ui.guessBtn4->setText("4");
    ui.guessBtn5->setText("5");
    ui.guessBtn6->setText("6");
    ui.guessBtn7->setText("7");
    ui.guessBtn8->setText("8");
    ui.guessBtn9->setText("9");
    ui.guessBtn10->setText("10");
    ui.guessBtn11->setText("11");
    ui.guessBtn12->setText("12");
    ui.guessBtn13->setText("13");
}

