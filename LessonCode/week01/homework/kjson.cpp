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

		srcStream.ignore();  // 跳过左引号
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

	// 自定义删除器，ptrJson被销毁时自动调用freeJson
	std::shared_ptr<KJson> ptrJson(parserResult, freeJson);
	return ptrJson;
}


// 记录打印深度
static int printDepth = 0;

std::ostream& printString(std::ostream& os, KJson* srcJson) {
	os << '\"' << srcJson->returnStr() << '\"';
	return os;
}


std::ostream& printNum(std::ostream& os, KJson* srcJson) {
	if (srcJson->returnNumType())
		os << srcJson->returnInt();
	else
		os << srcJson->returnDouble();

	return os;
}


std::ostream& printBool(std::ostream& os, KJson* srcJson) {
	os << srcJson->returnBool();
	return os;
}


std::ostream& printNull(std::ostream& os, KJson* srcJson) {
	os << "NULL";
	return os;
}


std::ostream& printJson(std::ostream& os, KJson* srcJson) {
	printDepth++;
	for (int i = 0; i < printDepth - 1; i++)
		os << "   ";
	os << "{\n";
	KJson* child = srcJson->returnChild();

	while (child != nullptr) {
		for (int i = 0; i < printDepth; i++)
			os << "   ";

		os << '\"' << child->returnKey() << "\": ";
		os << child;  // 递归重载
		child = child->returnNext();
		if (child != nullptr)
			os << ",";
		os << '\n';
	}

	for (int i = 0; i < printDepth - 1; i++)
		os << "   ";

	printDepth--;
	os << '}';
	return os;
}


std::ostream& printArray(std::ostream& os, KJson* srcJson) {
	os << "[\n";
	KJson* child = srcJson->returnChild();
	while (child != nullptr) {
		os << child;
		child = child->returnNext();
		if (child != nullptr)
			os << ", \n";
	}
	os << "]\n";

	return os;
}


std::ostream& operator << (std::ostream& os, KJson* srcJson) {
	switch (srcJson->returnType())
	{
	case KJson::JsonString:
		return printString(os, srcJson);
		break;

	case KJson::JsonNum:
		return printNum(os, srcJson);
		break;

	case KJson::JsonBool:		
		return printBool(os, srcJson);
		break;

	case KJson::JsonNull:
		return printNull(os, srcJson);
		break;

	case KJson::JsonJson:
		return printJson(os, srcJson);
		break;

	case KJson::JsonArray:
		return printArray(os, srcJson);
		break;

	default:
		return os;
		break;
	}

}


void outputXml(std::shared_ptr<KJson>& ptrJson, const std::string& filename) {
	std::ofstream xmlFile(filename, std::ios::out | std::ios::trunc);
	if (xmlFile.is_open()) {
		xmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		xmlFile << "<root>\n";
		xmlFile << json2Xml(ptrJson.get());
		xmlFile << "</root>\n";
		xmlFile.close();
	}
	else {
		std::cerr << "Unable to open file for writing XML." << std::endl;
	}
}


std::string json2Xml(KJson* json) {
	std::ostringstream xml;

	switch (json->returnType()) {
	case KJson::JsonString:
		xml << "<" << json->returnKey() << ">"
			<< escapeXml(json->returnStr())
			<< "</" << json->returnKey() << ">";
		break;
	case KJson::JsonNum: {
		if (json->returnNumType()) {
			xml << "<" << json->returnKey() << ">"
				<< json->returnInt()
				<< "</" << json->returnKey() << ">";
		}
		else {
			xml << "<" << json->returnKey() << ">"
				<< json->returnDouble()
				<< "</" << json->returnKey() << ">";
		}
		break;
	}
	case KJson::JsonBool:
		xml << "<" << json->returnKey() << ">"
			<< (json->returnBool() ? "true" : "false")
			<< "</" << json->returnKey() << ">";
		break;
	case KJson::JsonNull:
		xml << "<" << json->returnKey() << ">"
			<< "null"
			<< "</" << json->returnKey() << ">";
		break;
	case KJson::JsonJson: {
		xml << "<" << json->returnKey() << ">\n";
		KJson* child = json->returnChild();
		while (child != nullptr) {
			xml << "  " << json2Xml(child);
			child = child->returnNext();
		}
		xml << "</" << json->returnKey() << ">";
		break;
	}
	case KJson::JsonArray: {
		xml << "<" << json->returnKey() << ">\n";
		KJson* child = json->returnChild();
		while (child != nullptr) {
			xml << "  " << json2Xml(child);
			child = child->returnNext();
		}
		xml << "</" << json->returnKey() << ">";
		break;
	}
	default:
		// 处理未知类型
		break;
	}

	// Add a newline after each element for better readability
	xml << "\n";

	return xml.str();
}


std::string escapeXml(std::string& str) {
	std::string escaped;
	for (char c : str) {
		switch (c) {
		case '&':  escaped += "&amp;";   break;
		case '\"': escaped += "&quot;";  break;
		case '\'': escaped += "&apos;";  break;
		case '<':  escaped += "&lt;";    break;
		case '>':  escaped += "&gt;";    break;
		default:   escaped += c;         break;
		}
	}
	return escaped;
}


bool isAllDigits(const std::string& str) {
	std::regex pattern("^\\d+$"); // 定义正则表达式模式
	return std::regex_match(str, pattern); // 检查整个字符串是否匹配模式
}


std::vector<std::string> split(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(str);

	while (std::getline(tokenStream, token, delimiter))
		tokens.push_back(token);

	return tokens;
}
