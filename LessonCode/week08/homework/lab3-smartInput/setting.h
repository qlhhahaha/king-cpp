#pragma once

#ifndef __INPUT_SETTING_H__
#define __INPUT_SETTING_H__

#include "common.h"

class KSettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit KSettingsDialog(IInkCollector* parentInkCollector, QGridLayout* parentGrid, QWidget* parent = nullptr);
    ~KSettingsDialog();

private slots:
    void onColorSelected(int index);
    void onThicknessChanged(int value);
    void onFontSizeChanged(int value);
    void onResetDefaults();

private:
    QGridLayout* settingGrid;

    IInkCollector* settingInkCollector;
    IInkDrawingAttributes* settingAttributes;

    QComboBox* colorComboBox;
    QComboBox* sizeComboBox;
    QSlider* thicknessSlider;
    QPushButton* resetButton;

    void setupUI();
    void applySettings();
};


#endif
