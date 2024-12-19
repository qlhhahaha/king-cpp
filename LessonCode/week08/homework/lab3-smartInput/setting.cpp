#include "setting.h"


KSettingsDialog::KSettingsDialog(IInkCollector* parentInkCollector, QGridLayout* parentGrid, QWidget* parent)
    : QDialog(parent), settingGrid(parentGrid), settingInkCollector(parentInkCollector)
{
    setupUI();
    //loadCurrentSettings();
}


void KSettingsDialog::setupUI() {
    setWindowTitle(QString::fromLocal8Bit("设置"));


    colorComboBox = new QComboBox(this);
    colorComboBox->addItem("Red", QColor(Qt::red));
    colorComboBox->addItem("Black", QColor(Qt::black));
    colorComboBox->addItem("Green", QColor(Qt::green));
    colorComboBox->addItem("Blue", QColor(Qt::blue));
    colorComboBox->addItem("Yellow", QColor(Qt::yellow));
    colorComboBox->setCurrentIndex(0);
    connect(colorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onColorSelected(int)));


    thicknessSlider = new QSlider(Qt::Horizontal, this);
    thicknessSlider->setMinimum(30);
    thicknessSlider->setMaximum(90);
    thicknessSlider->setValue(54);
    connect(thicknessSlider, SIGNAL(valueChanged(int)), this, SLOT(onThicknessChanged(int)));
    

    sizeComboBox = new QComboBox(this);
    sizeComboBox->addItem(QString::fromLocal8Bit("小"), 9);
    sizeComboBox->addItem(QString::fromLocal8Bit("中（默认）"), 11);
    sizeComboBox->addItem(QString::fromLocal8Bit("大"), 16);
    sizeComboBox->setCurrentIndex(1);
    connect(sizeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFontSizeChanged(int)));


    resetButton = new QPushButton(QString::fromLocal8Bit("恢复默认设置"), this);
    connect(resetButton, &QPushButton::clicked, this, &KSettingsDialog::onResetDefaults);


    QVBoxLayout* layout = new QVBoxLayout(this);


    QHBoxLayout* colorLayout = new QHBoxLayout();
    colorLayout->addWidget(new QLabel(QString::fromLocal8Bit("墨迹颜色"), this));
    colorLayout->addWidget(colorComboBox);
    layout->addLayout(colorLayout);


    QHBoxLayout* thicknessLayout = new QHBoxLayout();
    thicknessLayout->addWidget(new QLabel(QString::fromLocal8Bit("墨迹粗细"), this));
    thicknessLayout->addWidget(thicknessSlider);
    layout->addLayout(thicknessLayout);


    QHBoxLayout* fontSizeLayout = new QHBoxLayout();
    fontSizeLayout->addWidget(new QLabel(QString::fromLocal8Bit("候选词字号"), this));
    fontSizeLayout->addWidget(sizeComboBox);
    layout->addLayout(fontSizeLayout);


    layout->addWidget(resetButton);

}


void KSettingsDialog::onColorSelected(int index) {
    applySettings();
}


void KSettingsDialog::onThicknessChanged(int value) {
    applySettings();
}


void KSettingsDialog::onFontSizeChanged(int value) {
    applySettings();
}


void KSettingsDialog::onResetDefaults() {
    colorComboBox->setCurrentIndex(0); // 假设红色是默认颜色
    thicknessSlider->setValue(53);    // tablet sdk的默认粗细为53
    sizeComboBox->setCurrentIndex(1);   // 假设默认字号为 11(中号)


    applySettings();


    QMessageBox::information(this, QString::fromLocal8Bit("设置"), QString::fromLocal8Bit("已恢复默认设置"));
}


void KSettingsDialog::applySettings() {
    QColor color = colorComboBox->itemData(colorComboBox->currentIndex()).value<QColor>();
    int thickness = thicknessSlider->value();
    int fontSize = sizeComboBox->itemData(sizeComboBox->currentIndex()).value<int>();


    if (SUCCEEDED(settingInkCollector->get_DefaultDrawingAttributes(&settingAttributes))) {
        // 设置颜色
        settingAttributes->put_Color(RGB(color.red(), color.green(), color.blue()));

        // 设置粗细
        settingAttributes->put_Width((float)thickness);

        // 将设置应用到 InkCollector
        //settingInkCollector->putref_DefaultDrawingAttributes(settingAttributes);

        // 刷新 InkCollector
        //settingInkCollector->put_Enabled(VARIANT_FALSE);
        //settingInkCollector->put_Enabled(VARIANT_TRUE);
    }

    for (int i = 0; i < settingGrid->count(); ++i) {
        QLayoutItem* item = settingGrid->itemAt(i);
        if (item) {
            QWidget* widget = item->widget();
            if (widget && qobject_cast<QPushButton*>(widget)) {
                QPushButton* pushButton = qobject_cast<QPushButton*>(widget);
                QFont font = pushButton->font();
                font.setPointSize(fontSize);
                pushButton->setFont(font);
                //pushButton->updateGeometry();
                pushButton->update();
            }
        }
    }
}

