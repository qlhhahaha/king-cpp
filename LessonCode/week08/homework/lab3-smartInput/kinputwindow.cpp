#include "kinputwindow.h"

KInputWindow::KInputWindow(QWidget *parent)
    : QMainWindow(parent),
    letterDialog(new KKeyboard(this)),
    numberDialog(new KKeyboard(this)),
    symbolDialog(new KKeyboard(this))
{
    ui.setupUi(this);
    
    // 初始化图片资源
    QPixmap* pixmap = new QPixmap(":/image/logo.png");
    pixmap->scaled(ui.label->size(), Qt::KeepAspectRatio);
    ui.label->setScaledContents(true);
    ui.label->setPixmap(*pixmap);


    // 初始化手写区域
    WId hShow = ui.inputZone->winId();
    this->inputs.inputInit((HWND)hShow);

    IInkCollector* test1 = this->inputs.g_pIInkCollector;
    settingDialog = new KSettingsDialog(test1, this->ui.candidateGrid, this);

    // 连接各按键的信号
    connect(ui.clearStrokeBtn, &QPushButton::clicked, this, &KInputWindow::onClearBtnClicked);
    connect(ui.backStrokeBtn, &QPushButton::clicked, this, &KInputWindow::onBackBtnClicked);
    connect(ui.letterBtn, &QPushButton::clicked, this, &KInputWindow::onLetterBtnClicked);
    connect(ui.numberBtn, &QPushButton::clicked, this, &KInputWindow::onNumberBtnClicked);
    connect(ui.symbolBtn, &QPushButton::clicked, this, &KInputWindow::onSymbolBtnClicked);
    connect(ui.settingBtn, &QPushButton::clicked, this, &KInputWindow::onSettingBtnClicked);
    connect(ui.singleCharBtn, &QPushButton::clicked, this, &KInputWindow::onSingleBtnClicked);
    connect(ui.sentenceBtn, &QPushButton::clicked, this, &KInputWindow::onSentenceBtnClicked);


    connect(ui.guessBtn1, &QPushButton::clicked, this, &KInputWindow::onWordBtnClicked_1);
    connect(ui.guessBtn2, &QPushButton::clicked, this, &KInputWindow::onWordBtnClicked_2);
    connect(ui.guessBtn3, &QPushButton::clicked, this, &KInputWindow::onWordBtnClicked_3);
    connect(ui.guessBtn4, &QPushButton::clicked, this, &KInputWindow::onWordBtnClicked_4);
    connect(ui.guessBtn5, &QPushButton::clicked, this, &KInputWindow::onWordBtnClicked_5);
    connect(ui.guessBtn6, &QPushButton::clicked, this, &KInputWindow::onWordBtnClicked_6);
    connect(ui.guessBtn7, &QPushButton::clicked, this, &KInputWindow::onWordBtnClicked_7);
    connect(ui.guessBtn8, &QPushButton::clicked, this, &KInputWindow::onWordBtnClicked_8);
    connect(ui.guessBtn9, &QPushButton::clicked, this, &KInputWindow::onWordBtnClicked_9);

    letterDialog->hide();
    numberDialog->hide();
    symbolDialog->hide();

    settingDialog->hide();
}


KInputWindow::~KInputWindow() {
    
}
 
void KInputWindow::updateRecoResult() {
    ui.guessBtn1->setText(QString::fromUtf8(QByteArray(inputResult[0].c_str(), inputResult[0].size())));
    ui.guessBtn2->setText(QString::fromUtf8(QByteArray(inputResult[1].c_str(), inputResult[1].size())));
    ui.guessBtn3->setText(QString::fromUtf8(QByteArray(inputResult[2].c_str(), inputResult[2].size())));
    ui.guessBtn4->setText(QString::fromUtf8(QByteArray(inputResult[3].c_str(), inputResult[3].size())));
    ui.guessBtn5->setText(QString::fromUtf8(QByteArray(inputResult[4].c_str(), inputResult[4].size())));
    ui.guessBtn6->setText(QString::fromUtf8(QByteArray(inputResult[5].c_str(), inputResult[5].size())));
    ui.guessBtn7->setText(QString::fromUtf8(QByteArray(inputResult[6].c_str(), inputResult[6].size())));
    ui.guessBtn8->setText(QString::fromUtf8(QByteArray(inputResult[7].c_str(), inputResult[7].size())));
    ui.guessBtn9->setText(QString::fromUtf8(QByteArray(inputResult[8].c_str(), inputResult[8].size())));
    ui.inputResult->setText(ui.guessBtn1->text());
}

void KInputWindow::mouseReleaseEvent(QMouseEvent* e) {
    this->inputResult.clear();
    this->inputs.recognize(this->inputResult, true);

    updateRecoResult();
}


void KInputWindow::onClearBtnClicked() {
    this->inputs.clearStrokes();  // 清除存储的笔触
    this->inputResult.clear();  // 清除识别结果

    ui.inputResult->clear();
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

void KInputWindow::onBackBtnClicked() {
    int returnCode = inputs.backStrokes();

    if (returnCode == 2)  // 如果撤销的是最后一个笔画
        onClearBtnClicked();

    if (returnCode == 1) {  // 如果撤销的是前面的那些笔画
        WId hShow = ui.inputZone->winId();
        InvalidateRect((HWND)hShow, NULL, TRUE);

        this->inputResult.clear();
        this->inputs.recognize(this->inputResult, false);
        updateRecoResult();
    }
}


void KInputWindow::onLetterBtnClicked() {
    if (letterDialog->isVisible()) {
        letterDialog->hide();
    }
    else {
        letterDialog->setup("abc");
        letterDialog->show();
    }

    numberDialog->hide();
    symbolDialog->hide();

}


void KInputWindow::onNumberBtnClicked() {
    if (numberDialog->isVisible()) {
        numberDialog->hide();
    }
    else {
        numberDialog->setup("123");
        numberDialog->show();
    }


    letterDialog->hide();
    symbolDialog->hide();

}


void KInputWindow::onSymbolBtnClicked() {
    if (symbolDialog->isVisible()) {
        symbolDialog->hide();
    }
    else {
        symbolDialog->setup("#+=");
        symbolDialog->show();
    }


    letterDialog->hide();
    numberDialog->hide();
}

void KInputWindow::onSettingBtnClicked() {
    if (settingDialog->isVisible())
        settingDialog->hide();
    else
        settingDialog->show();

}


void KInputWindow::onSingleBtnClicked() {
    ui.singleCharBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: rgb(255, 204, 153);"
        "}"
    );

    ui.sentenceBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: rgb(255, 255, 255);"
        "}"
    );

    IInkRecognizerGuide* RecognizerGuide;
    CoCreateInstance(CLSID_InkRecognizerGuide, NULL, CLSCTX_INPROC_SERVER, IID_IInkRecognizerGuide, (void**)&RecognizerGuide);
    InkRecoGuide recoguide;
    RECT rect;

    rect.bottom = 2;//不能为1
    rect.left = 0;
    rect.right = 2;//不能为1
    rect.top = 0;

    recoguide.rectWritingBox = rect;
    recoguide.rectDrawnBox = rect;
    recoguide.cRows = 1;//不要过大
    recoguide.cColumns = 1;
    recoguide.midline = -1;
    RecognizerGuide->put_GuideData(recoguide);
    inputs.g_pIInkRecoContext->putref_Guide(RecognizerGuide);

}


void KInputWindow::onSentenceBtnClicked() {
    ui.singleCharBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: rgb(255, 255, 255);"
        "}"
    );

    ui.sentenceBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: rgb(255, 204, 153);"
        "}"
    );

    // InkRecoGuide中的值全设为0时是默认模式，支持长句书写
    IInkRecognizerGuide* RecognizerGuide;
    CoCreateInstance(CLSID_InkRecognizerGuide, NULL, CLSCTX_INPROC_SERVER, IID_IInkRecognizerGuide, (void**)&RecognizerGuide);
    InkRecoGuide recoguide;
    RECT rect;
    rect.bottom = 0;
    rect.left = 0;
    rect.right = 0;
    rect.top = 0;

    recoguide.rectWritingBox = rect;
    recoguide.rectDrawnBox = rect;
    recoguide.cRows = 0;
    recoguide.cColumns = 0;
    recoguide.midline = 0;
    RecognizerGuide->put_GuideData(recoguide);
    inputs.g_pIInkRecoContext->putref_Guide(RecognizerGuide);
}


void KInputWindow::onWordBtnClicked_1(){

    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn1->text());
}


void KInputWindow::onWordBtnClicked_2() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn2->text());
}


void KInputWindow::onWordBtnClicked_3() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn3->text());
}


void KInputWindow::onWordBtnClicked_4() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn4->text());
}


void KInputWindow::onWordBtnClicked_5() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn5->text());
}


void KInputWindow::onWordBtnClicked_6() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn6->text());
}


void KInputWindow::onWordBtnClicked_7() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn7->text());
}


void KInputWindow::onWordBtnClicked_8() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn8->text());
}

void KInputWindow::onWordBtnClicked_9() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn9->text());
}
