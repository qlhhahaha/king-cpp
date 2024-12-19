#pragma once

#ifndef __KINPUTWINDOW_H__
#define __KINPUTWINDOW_H__

#include <QtWidgets/QMainWindow>
#include "ui_kinputwindow.h"

#include "common.h"
#include "handswriting.h"
#include "keyboard.h"
#include "setting.h"


class KInputWindow : public QMainWindow
{
	Q_OBJECT

public:
	KInputWindow(QWidget* parent = Q_NULLPTR);
	~KInputWindow();

	void updateRecoResult();

	handsInput inputs;
	std::vector<std::string> inputResult;

protected:
	void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
	void onBackBtnClicked();
	void onClearBtnClicked();
	void onLetterBtnClicked();
	void onNumberBtnClicked();
	void onSymbolBtnClicked();
	void onSettingBtnClicked();


	void onWordBtnClicked_1();
	void onWordBtnClicked_2();
	void onWordBtnClicked_3();
	void onWordBtnClicked_4();
	void onWordBtnClicked_5();
	void onWordBtnClicked_6();
	void onWordBtnClicked_7();
	void onWordBtnClicked_8();
	void onWordBtnClicked_9();

private:
	Ui::KInputWindowClass ui;
	KKeyboard* letterDialog;
	KKeyboard* numberDialog;
	KKeyboard* symbolDialog;

	KSettingsDialog* settingDialog;


};

#endif
