#ifndef KJSON_H
#define KJSON_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <locale>


/********************* JSON������ ************************/
class KJson {
public:
	enum KJsonType {JsonString, JsonNum, JsonBool, JsonNull, JsonJson, JsonArray};

	KJson(KJsonType valueType = JsonJson, KJson* left = nullptr, KJson* right = nullptr, const std::string& keyName = "")
		: type(valueType)
		, prev(left)
		, next(right)
		, child(nullptr)
		, key(keyName){}

	~KJson() {}

	void setKey(std::string val) { this->key = val; }
	void setChild(KJson* val) { this->child = val; }
	void setPrev(KJson* val) { this->prev = val; }
	void setNext(KJson* val) { this->next = val; }

	std::string returnKey() { return this->key; }
	KJson* returnPrev() { return this->prev; }
	KJson* returnNext() { return this->next; }
	KJson* returnChild() { return this->child; }
	KJsonType returnType() { return this->type; }

	// ����ʵ�ּ����������е�override�汾
	// ֮�����ڴ˴����麯����������Ϊ����ͳһ�û���ָ�����������
	// ע�⣬��Ҫ�ô��麯������Ϊ����ÿһ�������඼��Ҫ�������
	virtual void setValue(const std::string& str) {}
	virtual void setValue(bool val) {}
	virtual void setValue(int val) {}
	virtual void setValue(double val) {}

	// �⼸���麯����д��ͬ��������ʽ����Ϊ���޷���ƾ����ֵ��ͬ������
	virtual std::string returnStr() { return ""; }
	virtual bool returnBool() { return 0; }
	virtual bool returnNumType() { return 0; }
	virtual int returnInt() { return 0; }
	virtual double returnDouble() { return 0; }

private:
	KJsonType type;

	// ����ǰ��key-value�����ź��key-value��Ƕ��json����
	KJson *prev, *next, *child;

	std::string key;
};


// valueΪ�ַ�������������
class KJsonString : public KJson {
public:
	KJsonString() : KJson(JsonString) {}
	~KJsonString() {}

	void setValue(const std::string& str) override { this->stringValue = str; }

	std::string returnStr() override { return this->stringValue; }

private:
	std::string stringValue;
};


// valueΪ�����򸡵�������������
class KJsonNum : public KJson {
public:
	KJsonNum() : KJson(JsonNum),numType(true), valueInt(0), valueDouble(0.0) {}
	~KJsonNum() {}

	void setValue(int val) override { this->valueInt = val; }

	void setValue(double val) override { this->valueDouble = val; }

	bool returnNumType() override { return this->numType; }
	int returnInt() override { return this->valueInt; }
	double returnDouble() override { return this->valueDouble; }


private:
	bool numType;  // true����������false������
	int valueInt;
	double valueDouble;
};


// valueΪ����ֵ����������
class KJsonBool: public KJson {
public:
	KJsonBool() : KJson(JsonBool), boolValue(true) {}
	~KJsonBool() {}

	void setValue(bool val) override { this->boolValue = val; }

	bool returnBool() override { return this->boolValue; }

private:
	bool boolValue;
};


// valueΪnull����������
class KJsonNull: public KJson {
public:
	KJsonNull() : KJson(JsonNull) {}
	~KJsonNull() {}

private:
};


// valueΪJson�������������
class KJsonJson: public KJson {
public:
	KJsonJson() : KJson(JsonJson) {}
	~KJsonJson() {}

private:
};


// valueΪ�������������
class KJsonArray: public KJson {
public:
	KJsonArray() : KJson(JsonArray) {}
	~KJsonArray() {}

private:
};


/********************* �ⲿ���ܽӿ� ************************/

// ����������json�ļ�
std::shared_ptr<KJson> parserAll(std::string path);

// ����<<���ڴ�ӡjson�ṹ
std::ostream& operator << (std::ostream& os, KJson* srcJson);

/********************* �ڲ�utils�ӿ� ************************/

// ע��!!!
// �������д�stringstream�ĵط��������ô�ֵ��
// ��ΪҪ�޸�ԭstringstream�еĶ�д��ָ��


// ȥ���Ӷ�д��ָ��λ�ÿ�ʼ�Ŀո�(�Լ��������ַ���ascii��)
std::stringstream& removeWhiteSpace(std::stringstream& srcStream);

// �����ַ�����key-value
KJson* parserString(std::stringstream& srcStream);

// ���������͸�����key-value
KJson* parserNum(std::stringstream& srcStream);

// ����������key-value
KJson* parserBool(std::stringstream& srcStream);

// ������ֵkey-value
KJson* parserNull(std::stringstream& srcStream);

// ����һ��Json����(����Ҳ����Ƕ������Json����)
KJson* parserJson(std::stringstream& srcStream);

// ����������key-value
KJson* parserArray(std::stringstream& srcStream);

// ��ȡһ��key-value��keyֵ
std::string getKeyname(std::stringstream& srcStream);

// ��ȡð�ź��valueֵ
KJson* parseValue(std::stringstream& srcStream);

// �ͷ�����Json�ṹ���ڴ�
void freeJson(KJson* ptrJson);

// ��������<<����ӡjson�ṹ
std::ostream& printString(std::ostream& os, KJson* srcJson);
std::ostream& printNum(std::ostream& os, KJson* srcJson);
std::ostream& printBool(std::ostream& os, KJson* srcJson);
std::ostream& printNull(std::ostream& os, KJson* srcJson);
std::ostream& printJson(std::ostream& os, KJson* srcJson);
std::ostream& printArray(std::ostream& os, KJson* srcJson);

#endif // !KJSON_H
