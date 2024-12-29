#pragma once

#ifndef __KINPUTWINDOW_H__
#define __KINPUTWINDOW_H__

#include <QtWidgets/QMainWindow>
#include "ui_kinputwindow.h"

#include "common.h"
#include "handswriting.h"
#include "keyboard.h"
#include "setting.h"


typedef struct node
{
	std::wstring allWord;
	int count;
	node() { count = 0; allWord = L""; };
}wordFre;  //用来从训练词组中提取第一个汉字和记录此词语的出现频数


class KInputWindow : public QMainWindow
{
	Q_OBJECT

public:
	KInputWindow(QWidget* parent = Q_NULLPTR);
	~KInputWindow();

	void updateRecoResult();
	void associateWord(std::wstring str);
	void updateDictionary(std::wstring str);
		
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
	void onChineseBtnClicked();
	void onSettingBtnClicked();
	void onSingleBtnClicked();
	void onSentenceBtnClicked();
	

	void onWordBtnClicked_1();
	void onWordBtnClicked_2();
	void onWordBtnClicked_3();
	void onWordBtnClicked_4();
	void onWordBtnClicked_5();
	void onWordBtnClicked_6();
	void onWordBtnClicked_7();
	void onWordBtnClicked_8();
	void onWordBtnClicked_9();
	void onWordBtnClicked_10();
	void onWordBtnClicked_11();
	void onWordBtnClicked_12();
	void onWordBtnClicked_13();

private:
	Ui::KInputWindowClass ui;
	KKeyboard* letterDialog;
	KKeyboard* numberDialog;
	KKeyboard* symbolDialog;

	KSettingsDialog* settingDialog;

	//存储词典内容
	std::unordered_map<std::wstring, wordFre> dictionary; 

	// 存储每次联想结果
	std::vector<std::pair<std::wstring, wordFre>> associateRes;
};

#endif
