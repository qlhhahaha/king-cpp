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
	while (ptrJson) {  // ��ɾ�ֵܣ���ɾ����
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
		// TODO ���쳣
	}
}

KJson* parserString(std::stringstream& srcStream) {
	KJson* itemString = new KJsonString;
	if (itemString == nullptr) {
		// TODO ���쳣
	}

	std::string stringValue;
	srcStream.ignore();  // ����������
	getline(srcStream, stringValue, '\"');
	itemString->setValue(stringValue);
	return itemString;

}


KJson* parserNum(std::stringstream& srcStream) {
	KJson* itemNum = new KJsonNum;
	if (itemNum == nullptr) {
		//TODO ���쳣
	}

	char nextChar = srcStream.peek();
	std::string numStr;
	bool numType = true;

	while (nextChar == '-' || nextChar == '.' || isdigit(nextChar)) {
		if (nextChar == '.')
			numType = false;  // false��������

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
		//TODO ���쳣
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
		//TODO ���쳣
	}

	srcStream.ignore(4);  

	return itemNull;
}


KJson* parserJson(std::stringstream& srcStream) {
	KJson* itemJson = new KJsonJson;
	if (itemJson == nullptr) {
		// TODO ���쳣
	}

	if (srcStream.peek() != '{') {
		// TODO ���쳣
	}
	srcStream.ignore();  // �����������

	// ע�⣬������srcStream.ignore(2)һ������������ź������ţ���Ϊ��������пո�ͻ���
	removeWhiteSpace(srcStream);
	srcStream.ignore();  // ����������

	std::string itemKeyname = getKeyname(srcStream);

	srcStream.ignore();  // ����ð��

	KJson* itemChild = parseValue(removeWhiteSpace(srcStream));
	itemChild->setKey(itemKeyname);
	itemJson->setChild(itemChild);

	while (srcStream.peek() == ',') {
		srcStream.ignore();
		removeWhiteSpace(srcStream);

		std::string anotherKeyname = getKeyname(srcStream);
		srcStream.ignore();  // ����ð��
		KJson* anotherChild = parseValue(removeWhiteSpace(srcStream));

		anotherChild->setKey(anotherKeyname);
		itemChild->setNext(anotherChild);
		anotherChild->setPrev(itemChild);
		itemChild = anotherChild;
		removeWhiteSpace(srcStream);
	}

	srcStream.ignore();  // �����Ҵ�����
	return itemJson;
}

KJson* parserArray(std::stringstream& srcStream) {
	KJson* itemArray = new KJsonArray;
	if (itemArray == nullptr) {
		//TODO ���쳣
	}

	srcStream.ignore();  // ������ߵ�������

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

	srcStream.ignore();  // �����ұߵ�������
	return itemArray;
}


std::shared_ptr<KJson> parserAll(std::string path) {
	std::stringstream jsonStream;
	std::ifstream fin(path);

	//TODO �ļ����쳣����
	if (!fin.is_open()) {
		
	}
	jsonStream << fin.rdbuf();
	fin.close();

	KJson* parserResult = parserJson(removeWhiteSpace(jsonStream));

	// ��json�Ľ�β������}���������ǿհ��ַ�
	if (removeWhiteSpace(jsonStream).peek() != -1) {
		//TODO ���쳣
	}

	//TODO ���������ͷ��ڴ�ͷ���ָ�� 
	// �Զ���ɾ������ptrJson������ʱ�Զ�����freeJson
	std::shared_ptr<KJson> ptrJson(parserResult, freeJson);
	std::cout << ptrJson.get() << std::endl;
	return ptrJson;
}


std::ostream& operator << (std::ostream& os, KJson* srcJson) {
	return os;
}