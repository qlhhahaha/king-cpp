#ifndef KJSON_H
#define KJSON_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <locale>
#include <vector>
#include <string>
#include <regex>
#include <direct.h>


/********************* JSON解析类 ************************/
class KJson {
public:
	enum KJsonType { JsonString, JsonNum, JsonBool, JsonNull, JsonJson, JsonArray, JsonError };
	enum errorType { wrongText, wrongAllocate, wrongKeyname, wrongIndex };

	KJson(KJsonType valueType = JsonJson, KJson* left = nullptr, KJson* right = nullptr, const std::string& keyName = "")
		: type(valueType)
		, prev(left)
		, next(right)
		, child(nullptr)
		, key(keyName) {
	}


	// 移动构造函数
	KJson(KJson&& other) noexcept
		: type(other.type), prev(other.prev), next(other.next), child(other.child), key(other.key) {
		// 将源对象的指针置空，表明所有权已经转移
		other.prev = nullptr;
		other.next = nullptr;
		other.child = nullptr;
	}


	// 移动赋值运算符
	KJson& operator=(KJson&& other) noexcept {
		if (this != &other) {
			// 释放当前对象的已有资源（如果有）
			if (next != nullptr) {
				delete next;
				next = nullptr;
			}

			if (child != nullptr) {
				delete child;
				child = nullptr;
			}

			if (prev != nullptr) {
				delete prev;
				prev = nullptr;
			}

			// 转移资源
			type = other.type;
			prev = other.prev;
			next = other.next;
			child = other.child;
			key = other.key;

			// 将源对象的指针置空
			other.prev = nullptr;
			other.next = nullptr;
			other.child = nullptr;
		}
		return *this;
	}


	~KJson() {
		// 写法其实和freeJson()很相似，也是递归DFS删除

		// 先处理兄弟节点
		while (this->next != nullptr) {
			KJson* nextToDelete = this->next;
			this->next = nextToDelete->next;

			// 注意：这个delete蕴含了递归，最终逻辑和freeJson()一样
			delete nextToDelete;
		}

		// 再处理子节点
		if (this->child != nullptr) {
			delete this->child;
			this->child = nullptr;
		}
	}

	void setKey(const std::string& val) { this->key = val; }
	void setChild(KJson* val) { this->child = val; }
	void setPrev(KJson* val) { this->prev = val; }
	void setNext(KJson* val) { this->next = val; }

	std::string returnKey() { return this->key; }
	KJson* returnPrev() { return this->prev; }
	KJson* returnNext() { return this->next; }
	KJson* returnChild() { return this->child; }
	KJsonType returnType() { return this->type; }

	// 具体实现见各派生类中的override版本
	// 之所以在此处用虚函数声明，是为了能统一用基类指针管理派生类
	// 注意，不要用纯虚函数，因为不是每一个派生类都需要这个方法
	virtual void setValue(std::string str) {}
	virtual void setValue(bool val) {}
	virtual void setValue(int val) {}
	virtual void setValue(double val) {}

	// 这几个虚函数不写成同名重载形式是因为，无法仅凭返回值不同来重载
	virtual std::string returnStr() { return ""; }
	virtual bool returnBool() { return 0; }
	virtual bool returnNumType() { return 0; }
	virtual int returnInt() { return 0; }
	virtual double returnDouble() { return 0; }


	// 实现动态key-value管理
	virtual KJson* operator [] (std::string keyName) { return nullptr; }
	virtual KJson* operator [] (int index) { return nullptr; }
	virtual KJson* pushBack(KJson* newItem) { return nullptr; }
	virtual void remove(int index) {}
	virtual void remove(std::string keyName) {}

	// 异常处理
	virtual void throwException(errorType type) {}

private:
	KJsonType type;

	// 逗号前的key-value，逗号后的key-value，嵌套json对象
	KJson* prev, * next, * child;

	std::string key;
};


// 注意，该全局变量的定义位置不能变
// 若放在头文件开头则会Kjson类型未定义
// 放在结尾又会导致ptrError变量无法使用
extern KJson* ptrError;


// value为字符串的数据类型
class KJsonString : public KJson {
public:
	KJsonString() : KJson(JsonString) {}
	~KJsonString() {}
	void setValue(std::string str) override { this->stringValue = str; }

	std::string returnStr() override { return this->stringValue; }

private:
	std::string stringValue;
};


// value为整数或浮点数的数据类型
class KJsonNum : public KJson {
public:
	KJsonNum() : KJson(JsonNum), numType(true), valueInt(0), valueDouble(0.0) {}
	~KJsonNum() {}

	void setValue(int val) override { this->valueInt = val; }

	void setValue(double val) override { this->valueDouble = val; }

	bool returnNumType() override { return this->numType; }
	int returnInt() override { return this->valueInt; }
	double returnDouble() override { return this->valueDouble; }


private:
	bool numType;  // true代表整数，false浮点数
	int valueInt;
	double valueDouble;
};


// value为布尔值的数据类型
class KJsonBool : public KJson {
public:
	KJsonBool() : KJson(JsonBool), boolValue(true) {}
	~KJsonBool() {}

	void setValue(bool val) override { this->boolValue = val; }

	bool returnBool() override { return this->boolValue; }

private:
	bool boolValue;
};


// value为null的数据类型
class KJsonNull : public KJson {
public:
	KJsonNull() : KJson(JsonNull) {}
	~KJsonNull() {}

};


// value为Json对象的数据类型
class KJsonJson : public KJson {
public:
	KJsonJson() : KJson(JsonJson) {}
	~KJsonJson() {}


	KJson* operator [] (std::string keyName) override {
		KJson* child = this->returnChild();

		while (keyName != child->returnKey() && child != nullptr)
			child = child->returnNext();

		if (child == nullptr) {
			ptrError->throwException(wrongKeyname);
			return ptrError;
		}

		return child;
	}


	KJson* pushBack(KJson* newItem) override {
		KJson* child = this->returnChild();

		while (child->returnNext() != nullptr)
			child = child->returnNext();

		child->setNext(newItem);
		newItem->setPrev(child);
		newItem->setNext(nullptr);

		return child;
	}


	void remove(std::string keyName) override {
		KJson* removeItem = (*this)[keyName];
		KJson* nextItem = removeItem->returnNext();
		KJson* prevItem = removeItem->returnPrev();

		// 删除首元素
		if (this->returnChild() == removeItem) {
			this->setChild(nextItem);
			if (nextItem != nullptr)
				nextItem->setPrev(nullptr);

			removeItem->setNext(nullptr);
			delete removeItem;
			return;
		}

		// 删除非首元素
		prevItem->setNext(nextItem);
		if (nextItem != nullptr)
			nextItem->setPrev(prevItem);

		removeItem->setNext(nullptr);
		removeItem->setPrev(nullptr);
		delete removeItem;
		return;
	}

};


// value为数组的数据类型
class KJsonArray : public KJson {
public:
	KJsonArray() : KJson(JsonArray) {}
	~KJsonArray() {}

	KJson* operator [] (int index) override {
		KJson* child = this->returnChild();
		while (index && child != nullptr) {
			child = child->returnNext();
			index--;
		}
		if (child == nullptr) {
			ptrError->throwException(wrongIndex);
			return ptrError;
		}
		return child;
	}

	KJson* pushBack(KJson* newItem) override {
		KJson* child = this->returnChild();

		while (child->returnNext() != nullptr)
			child = child->returnNext();

		child->setNext(newItem);
		newItem->setPrev(child);
		newItem->setNext(nullptr);

		return child;
	}

	// 双向链表的删除
	void remove(int index) override {
		KJson* removeItem = (*this)[index];
		KJson* nextItem = removeItem->returnNext();
		KJson* prevItem = removeItem->returnPrev();

		// 删除首元素
		if (index == 0) {
			this->setChild(nextItem);
			if (nextItem != nullptr)
				nextItem->setPrev(nullptr);

			removeItem->setNext(nullptr);
			delete removeItem;
			return;
		}

		// 删除非首元素
		prevItem->setNext(nextItem);
		if (nextItem != nullptr)
			nextItem->setPrev(prevItem);

		removeItem->setNext(nullptr);
		removeItem->setPrev(nullptr);
		delete removeItem;
		return;
	}
};


class KJsonError : public KJson {
public:

	KJsonError() : KJson(JsonError) {};
	~KJsonError() {};

	void throwException(errorType type) override {
		switch (type) {
		case KJsonError::wrongText: this->exception = "throw an exception of \"wrong json text\"";

		case KJsonError::wrongAllocate: this->exception = "throw an exception of \"allocate failed\"";

		case KJsonError::wrongKeyname: this->exception = "throw an exception of \"the keyname does not exist\"";

		case KJsonError::wrongIndex: this->exception = "throw an exception of \"the index is out of range\"";
		}


	}

private:
	std::string exception;

};


/********************* 外部功能接口 ************************/

// 解析完整的json文件
std::shared_ptr<KJson> parserAll(std::string path);

// 重载<<用于打印json结构
std::ostream& operator << (std::ostream& os, KJson* srcJson);

// 控制台交互
void interact();

// 将json输出为xml文件
void outputXml(std::shared_ptr<KJson>& ptrJson, const std::string& filename = "tests/output.xml");

/********************* 内部utils接口 ************************/

// 注意!!!
// 下面所有传stringstream的地方都用引用传值，
// 因为要修改原stringstream中的读写流指针


// 去除从读写流指针位置开始的空格(以及其它非字符型ascii码)
std::stringstream& removeWhiteSpace(std::stringstream& srcStream);

// 解析字符串型key-value
KJson* parserString(std::stringstream& srcStream);

// 解析整数和浮点型key-value
KJson* parserNum(std::stringstream& srcStream);

// 解析布尔型key-value
KJson* parserBool(std::stringstream& srcStream);

// 解析空值key-value
KJson* parserNull(std::stringstream& srcStream);

// 解析一个Json对象(其中也可以嵌套其它Json对象)
KJson* parserJson(std::stringstream& srcStream);

// 解析数组型key-value
KJson* parserArray(std::stringstream& srcStream);

// 获取一个key-value的key值
std::string getKeyname(std::stringstream& srcStream);

// 获取冒号后的value值
KJson* parseValue(std::stringstream& srcStream);

// 释放整个Json结构的内存
void freeJson(KJson* ptrJson);

// 用于重载<<，打印json结构
std::ostream& printString(std::ostream& os, KJson* srcJson);
std::ostream& printNum(std::ostream& os, KJson* srcJson);
std::ostream& printBool(std::ostream& os, KJson* srcJson);
std::ostream& printNull(std::ostream& os, KJson* srcJson);
std::ostream& printJson(std::ostream& os, KJson* srcJson);
std::ostream& printArray(std::ostream& os, KJson* srcJson);

// json结构转为xml结构
std::string json2Xml(KJson* json);

// 用于转义 XML 中的特殊字符
std::string escapeXml(std::string& str);

// 判断str是否为全数字
bool isAllDigits(const std::string& str);

// 分割string
std::vector<std::string> split(const std::string& str, char delimiter);

// 对错误的json文本抛出异常
KJson* wrongText(std::stringstream& srcStream);
KJson* wrongAllocate();



#endif // !KJSON_H
