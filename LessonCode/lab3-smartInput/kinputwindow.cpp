#include "kinputwindow.h"


KInputWindow::KInputWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 设置全局 locale 为 UTF-8 编码
    std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>());

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
    letterDialog = new KKeyboard(this->ui.lineEdit, this);
    numberDialog = new KKeyboard(this->ui.lineEdit, this);
    symbolDialog = new KKeyboard(this->ui.lineEdit, this);

    // 连接各按键的信号
    connect(ui.clearStrokeBtn, &QPushButton::clicked, this, &KInputWindow::onClearBtnClicked);
    connect(ui.backStrokeBtn, &QPushButton::clicked, this, &KInputWindow::onBackBtnClicked);
    connect(ui.letterBtn, &QPushButton::clicked, this, &KInputWindow::onLetterBtnClicked);
    connect(ui.numberBtn, &QPushButton::clicked, this, &KInputWindow::onNumberBtnClicked);
    connect(ui.symbolBtn, &QPushButton::clicked, this, &KInputWindow::onSymbolBtnClicked);
    connect(ui.chineseBtn, &QPushButton::clicked, this, &KInputWindow::onChineseBtnClicked);
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
    connect(ui.guessBtn10, &QPushButton::clicked, this, &KInputWindow::onWordBtnClicked_10);
    connect(ui.guessBtn11, &QPushButton::clicked, this, &KInputWindow::onWordBtnClicked_11);
    connect(ui.guessBtn12, &QPushButton::clicked, this, &KInputWindow::onWordBtnClicked_12);
    connect(ui.guessBtn13, &QPushButton::clicked, this, &KInputWindow::onWordBtnClicked_13);

    letterDialog->hide();
    numberDialog->hide();
    symbolDialog->hide();

    settingDialog->hide();

    ui.singleCharBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: rgb(255, 204, 153);"
        "}"
    );

    // 初始化词库
    std::wifstream tfile("dictionary.txt");
    if (!tfile.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return;
    }


    tfile.imbue(utf8_locale);
    std::wstring word;
    int fre;
    while (tfile >> word >> fre) {
        wordFre info;
        info.allWord = word.substr(0, 1);
        info.count = fre;
        dictionary[word] = info;
    }

    int a;
}


KInputWindow::~KInputWindow() {
    delete letterDialog;
    delete numberDialog;
    delete symbolDialog;
}
 
void KInputWindow::updateRecoResult() {
    // 手写识别内容
    std::string tmp = inputResult[0];

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

    // 联想内容, 默认联想第一个按钮中的内容
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::wstring wstr = converter.from_bytes(tmp);
    associateWord(wstr);
    //associateWord(L"大");

}


void KInputWindow::associateWord(std::wstring str) {
    std::vector<std::pair<std::wstring, wordFre>> tVector;

    for (auto curr = dictionary.begin(); curr != dictionary.end(); curr++)
        if (curr->second.allWord == str) {
            tVector.push_back(std::make_pair(curr->first, curr->second));
            //associateRes.push_back(std::make_pair(curr->first, curr->second)); 
        }

    // 使用 lambda 表达式进行排序
    std::sort(tVector.begin(), tVector.end(),
        [] (const std::pair<std::wstring, wordFre>& x,
            const std::pair<std::wstring, wordFre>& y) {
            return x.second.count > y.second.count;
        });

    // 获取 GB2312 编码的 QTextCodec
    //QTextCodec* codec = QTextCodec::codecForName("UTF8");
    if (tVector.size()) {
        // 将 GB2312 编码的 std::string 转换为 QString
        ui.guessBtn10->setText(QString::fromStdWString(tVector[0].first));
        
        tVector.erase(tVector.begin());
    }
    else {
        ui.guessBtn10->setText("NULL");
    }

    if (tVector.size()) {
        ui.guessBtn11->setText(QString::fromStdWString(tVector[0].first));
        tVector.erase(tVector.begin());
    }
    else {
        ui.guessBtn11->setText("NULL");
    }

    if (tVector.size()) {
        ui.guessBtn12->setText(QString::fromStdWString(tVector[0].first));
        tVector.erase(tVector.begin());
    }
    else {
        ui.guessBtn12->setText("NULL");
    }

    if (tVector.size()) {
        ui.guessBtn13->setText(QString::fromStdWString(tVector[0].first));
        tVector.erase(tVector.begin());
    }
    else {
        ui.guessBtn13->setText("NULL");
    }

}


void KInputWindow::updateDictionary(std::wstring str) {
    dictionary[str].count += 1;
    dictionary[str].allWord = str.substr(0, 1);
    //std::ofstream examplefile("dictionary.txt");
    //for (auto iter = dictionary.begin(); iter != dictionary.end(); iter++)
    //    examplefile << iter->first << " " << iter->second.count << endl;
    //examplefile.close();
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


void KInputWindow::onChineseBtnClicked() {
    letterDialog->hide();
    numberDialog->hide();
    symbolDialog->hide();
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
    ui.lineEdit->insert(ui.guessBtn1->text());
    this->inputs.clearStrokes();
}


void KInputWindow::onWordBtnClicked_2() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn2->text());
    ui.lineEdit->insert(ui.guessBtn2->text());
    this->inputs.clearStrokes();
}


void KInputWindow::onWordBtnClicked_3() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn3->text());
    ui.lineEdit->insert(ui.guessBtn3->text());
    this->inputs.clearStrokes();
}


void KInputWindow::onWordBtnClicked_4() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn4->text());
    ui.lineEdit->insert(ui.guessBtn4->text());
    this->inputs.clearStrokes();
}


void KInputWindow::onWordBtnClicked_5() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn5->text());
    ui.lineEdit->insert(ui.guessBtn5->text());
    this->inputs.clearStrokes();
}


void KInputWindow::onWordBtnClicked_6() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn6->text());
    ui.lineEdit->insert(ui.guessBtn6->text());
    this->inputs.clearStrokes();
}


void KInputWindow::onWordBtnClicked_7() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn7->text());
    ui.lineEdit->insert(ui.guessBtn7->text());
    this->inputs.clearStrokes();
}


void KInputWindow::onWordBtnClicked_8() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn8->text());
    ui.lineEdit->insert(ui.guessBtn8->text());
    this->inputs.clearStrokes();
}

void KInputWindow::onWordBtnClicked_9() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn9->text());
    ui.lineEdit->insert(ui.guessBtn9->text());
    this->inputs.clearStrokes();
}

void KInputWindow::onWordBtnClicked_10() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn10->text());
    ui.lineEdit->insert(ui.guessBtn10->text());
    this->inputs.clearStrokes();
    updateDictionary(ui.guessBtn10->text().toStdWString());
}

void KInputWindow::onWordBtnClicked_11() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn11->text());
    ui.lineEdit->insert(ui.guessBtn11->text());
    this->inputs.clearStrokes();
    updateDictionary(ui.guessBtn11->text().toStdWString());
}

void KInputWindow::onWordBtnClicked_12() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn12->text());
    ui.lineEdit->insert(ui.guessBtn12->text());
    this->inputs.clearStrokes();
    updateDictionary(ui.guessBtn12->text().toStdWString());
}

void KInputWindow::onWordBtnClicked_13() {
    ui.inputResult->clear();
    ui.inputResult->setText(ui.guessBtn13->text());
    ui.lineEdit->insert(ui.guessBtn13->text());
    this->inputs.clearStrokes();
    updateDictionary(ui.guessBtn13->text().toStdWString());
}
