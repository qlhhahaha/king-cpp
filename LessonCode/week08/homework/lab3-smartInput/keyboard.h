#pragma once

#ifndef __KEYBOARD_H__
#define __KEYBORAD_H__

#include "common.h"

class KKeyboard : public QDialog {
	Q_OBJECT

public:
	explicit KKeyboard(QLineEdit* parentEdit, QWidget* parent = nullptr);
	~KKeyboard();
	void setup(const QString& keyboardType);

private slots:
	void onSingleLetterBtnClicked(const QString& letter);
	void onSingleNumberBtnClicked(const QString& number);
	void onSingleSymbolBtnClicked(const QString& symbol);

private:
	QLineEdit* keyboardEdit;
	QGridLayout* keyboardLayout;
	QSignalMapper* letterMapper;
	QSignalMapper* numberMapper;
	QSignalMapper* symbolMapper;
	QVector<QChar> letters;
	QVector<QString> numbers;
	QVector<QString> symbols;
};


#endif
