#include "keyboard.h"


KKeyboard::KKeyboard(QWidget* parent)
	: QDialog(parent),
	letterMapper(new QSignalMapper(this)),
	numberMapper(new QSignalMapper(this)),
	symbolMapper(new QSignalMapper(this)) 
{
	keyboardLayout = new QGridLayout(this);

	// 存储小写字母
	ushort start = 'a';
	ushort end = 'z';
	for (ushort i = start; i <= end; ++i) {
		letters.push_back(QChar(i));
	}


	// 存储大写字母
	start = 'A';
	end = 'Z';
	for (ushort i = start; i <= end; ++i) {
		letters.push_back(QChar(i));
	}


	// 存储数字和对应的汉字
	numbers.push_back("0");
	numbers.push_back("1");
	numbers.push_back("2");
	numbers.push_back("3");
	numbers.push_back("4");
	numbers.push_back("5");
	numbers.push_back("6");
	numbers.push_back("7");
	numbers.push_back("8");
	numbers.push_back("9");
	numbers.push_back(QString::fromLocal8Bit("零"));
	numbers.push_back(QString::fromLocal8Bit("一"));
	numbers.push_back(QString::fromLocal8Bit("二"));
	numbers.push_back(QString::fromLocal8Bit("三"));
	numbers.push_back(QString::fromLocal8Bit("四"));
	numbers.push_back(QString::fromLocal8Bit("五"));
	numbers.push_back(QString::fromLocal8Bit("六"));
	numbers.push_back(QString::fromLocal8Bit("七"));
	numbers.push_back(QString::fromLocal8Bit("八"));
	numbers.push_back(QString::fromLocal8Bit("九"));

	// 存储符号
	symbols.push_back(",");
	symbols.push_back(".");
	symbols.push_back("?");
	symbols.push_back("!");
	symbols.push_back(";");
	symbols.push_back(":");
	symbols.push_back("'");
	symbols.push_back("\"");
	symbols.push_back("(");
	symbols.push_back(")");
	symbols.push_back("[");
	symbols.push_back("]");
	symbols.push_back("{");
	symbols.push_back("}");
	symbols.push_back("/");
	symbols.push_back("\\");
	symbols.push_back("|");
	symbols.push_back("@");
	symbols.push_back("#");
	symbols.push_back("$");
	symbols.push_back("%");
	symbols.push_back("^");
	symbols.push_back("*");


	setLayout(keyboardLayout);
}


void KKeyboard::setup(const QString& keyboardType) {
	// 清除之前的布局
	QLayoutItem* child;
	while ((child = keyboardLayout->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	int row = 0;
	int col = 0;

	if (keyboardType == "abc") {
		for (const QString& key : letters) {
			QPushButton* button = new QPushButton(key, this);
			letterMapper->setMapping(button, key);
			connect(button, SIGNAL(clicked()), letterMapper, SLOT(map()));
			keyboardLayout->addWidget(button, row, col);
			col++;
			if (col > 9) {
				col = 0;
				row++;
			}
		}

		connect(letterMapper, SIGNAL(mapped(const QString&)), this, SLOT(onSingleLetterBtnClicked(const QString&)));
	}

	else if (keyboardType == "123") {
		for (const QString& key : numbers) {
			QPushButton* button = new QPushButton(key, this);
			numberMapper->setMapping(button, key);
			connect(button, SIGNAL(clicked()), numberMapper, SLOT(map()));
			keyboardLayout->addWidget(button, row, col);
			col++;
			if (col > 9) {
				col = 0;
				row++;
			}
		}

		connect(numberMapper, SIGNAL(mapped(const QString&)), this, SLOT(onSingleNumberBtnClicked(const QString&)));
	}

	else if (keyboardType == "#+=") {
		for (const QString& key : symbols) {
			QPushButton* button = new QPushButton(key, this);
			symbolMapper->setMapping(button, key);
			connect(button, SIGNAL(clicked()), symbolMapper, SLOT(map()));
			keyboardLayout->addWidget(button, row, col);
			col++;
			if (col > 9) {
				col = 0;
				row++;
			}
		}
		connect(symbolMapper, SIGNAL(mapped(const QString&)), this, SLOT(onSingleSymbolBtnClicked(const QString&)));
	}

}



void KKeyboard::onSingleLetterBtnClicked(const QString& letter) {



}


void KKeyboard::onSingleNumberBtnClicked(const QString& number) {

}


void KKeyboard::onSingleSymbolBtnClicked(const QString& symbol) {


}
