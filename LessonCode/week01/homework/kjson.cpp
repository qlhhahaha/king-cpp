#include "kjson.h"

std::string getKeyname(std::stringstream& srcStream) {
	std::string keyName;
	getline(srcStream, keyName, '\"');
	return keyName;
}


std::stringstream& removeWhiteSpace(std::stringstream& srcStream) {
	while (srcStream.peek() > 0 && srcStream.peek() < 33) {
		srcStream.ignore();
	}
	return srcStream;
}


void freeJson(KJson* ptrJson) {
	while (ptrJson) {  // 先删兄弟，再删孩子
		KJson* ptrChild = ptrJson->returnChild();
		if (ptrChild != nullptr)
			freeJson(ptrChild);

		KJson* ptrNext = ptrJson->returnNext();
		delete ptrJson;
		ptrJson = ptrNext;
	}
}


KJson* parseValue(std::stringstream& srcStream) {
	char nextChar = srcStream.peek();

	if (nextChar == '\"')
		return parserString(srcStream);
	else if (isdigit(nextChar) || nextChar == '-')
		return parserNum(srcStream);
	else if (nextChar == 'f' || nextChar == 't')
		return parserBool(srcStream);
	else if (nextChar == 'n')
		return parserNull(srcStream);
	else if (nextChar == '{')
		return parserJson(srcStream);
	else if (nextChar == '[')
		return parserArray(srcStream);
	else {
		// TODO 抛异常
	}
}

KJson* parserString(std::stringstream& srcStream) {
	KJson* itemString = new KJsonString;
	if (itemString == nullptr) {
		// TODO 抛异常
	}

	std::string stringValue;
	srcStream.ignore();  // 跳过左引号
	getline(srcStream, stringValue, '\"');
	itemString->setValue(stringValue);
	return itemString;

}


KJson* parserNum(std::stringstream& srcStream) {
	KJson* itemNum = new KJsonNum;
	if (itemNum == nullptr) {
		//TODO 抛异常
	}

	char nextChar = srcStream.peek();
	std::string numStr;
	bool numType = true;

	while (nextChar == '-' || nextChar == '.' || isdigit(nextChar)) {
		if (nextChar == '.')
			numType = false;  // false代表浮点数

		numStr.push_back(srcStream.get());
		nextChar = srcStream.peek();
	}

	if (numType)
		itemNum->setValue(std::stoi(numStr));
	else
		itemNum->setValue(std::stod(numStr));

	return itemNum;
}


KJson* parserBool(std::stringstream& srcStream) {
	KJson* itemBool = new KJsonBool;
	if (itemBool == nullptr) {
		//TODO 抛异常
	}

	char nextChar = srcStream.peek();
	if (nextChar == 'f') {
		itemBool->setValue(false);
		srcStream.ignore(5);
	}
	else {
		itemBool->setValue(true);
		srcStream.ignore(4);
	}

	return itemBool;
}


KJson* parserNull(std::stringstream& srcStream) {
	KJson* itemNull = new KJsonNull;
	if (itemNull == nullptr) {
		//TODO 抛异常
	}

	srcStream.ignore(4);  

	return itemNull;
}


KJson* parserJson(std::stringstream& srcStream) {
	KJson* itemJson = new KJsonJson;
	if (itemJson == nullptr) {
		// TODO 抛异常
	}

	if (srcStream.peek() != '{') {
		// TODO 抛异常
	}
	srcStream.ignore();  // 跳过左大括号

	// 注意，不能用srcStream.ignore(2)一下跳过左大括号和左引号，因为其间大概率有空格和换行
	removeWhiteSpace(srcStream);
	srcStream.ignore();  // 跳过左引号

	std::string itemKeyname = getKeyname(srcStream);

	srcStream.ignore();  // 跳过冒号

	KJson* itemChild = parseValue(removeWhiteSpace(srcStream));
	itemChild->setKey(itemKeyname);
	itemJson->setChild(itemChild);

	while (srcStream.peek() == ',') {
		srcStream.ignore();
		removeWhiteSpace(srcStream);

		std::string anotherKeyname = getKeyname(srcStream);
		srcStream.ignore();  // 跳过冒号
		KJson* anotherChild = parseValue(removeWhiteSpace(srcStream));

		anotherChild->setKey(anotherKeyname);
		itemChild->setNext(anotherChild);
		anotherChild->setPrev(itemChild);
		itemChild = anotherChild;
		removeWhiteSpace(srcStream);
	}

	srcStream.ignore();  // 跳过右大括号
	return itemJson;
}

KJson* parserArray(std::stringstream& srcStream) {
	KJson* itemArray = new KJsonArray;
	if (itemArray == nullptr) {
		//TODO 抛异常
	}

	srcStream.ignore();  // 跳过左边的中括号

	KJson* itemChild = parseValue(removeWhiteSpace(srcStream));
	itemArray->setChild(itemChild);

	while (srcStream.peek() == ',') {
		srcStream.ignore();
		KJson* anotherChild = parseValue(removeWhiteSpace(srcStream));

		itemChild->setNext(anotherChild);
		anotherChild->setPrev(itemChild);
		itemChild = anotherChild;
		removeWhiteSpace(srcStream);
	}

	srcStream.ignore();  // 跳过右边的中括号
	return itemArray;
}


std::shared_ptr<KJson> parserAll(std::string path) {
	std::stringstream jsonStream;
	std::ifstream fin(path);

	//TODO 文件打开异常处理
	if (!fin.is_open()) {
		
	}
	jsonStream << fin.rdbuf();
	fin.close();

	KJson* parserResult = parserJson(removeWhiteSpace(jsonStream));

	// 若json的结尾反括号}后还有其它非空白字符
	if (removeWhiteSpace(jsonStream).peek() != -1) {
		//TODO 抛异常
	}

	//TODO 解析完后的释放内存和返回指针 
	// 自定义删除器，ptrJson被销毁时自动调用freeJson
	std::shared_ptr<KJson> ptrJson(parserResult, freeJson);
	std::cout << ptrJson.get() << std::endl;
	return ptrJson;
}


std::ostream& operator << (std::ostream& os, KJson* srcJson) {
	return os;
}