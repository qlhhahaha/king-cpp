#include "kjson.h"

void dynamicTest() {
	std::shared_ptr<KJson> ptrJson = parserAll("tests/test4.json");
	std::cout << ptrJson << std::endl << std::endl;
	std::wcout << L"key：website对应的value为：" << std::endl;

	KJson* nest = (*ptrJson)["website"];
	std::cout << nest << std::endl;

	std::wcout << L"website中的第0个元素为：" << std::endl;
	std::cout << (*nest)[0] << std::endl;

	std::wcout << L"在website尾部插入一个string：" << std::endl;
	KJson* newItem = new KJsonString;
	std::string str = "an inserted new string";
	newItem->setValue(str);
	nest->pushBack(newItem);
	std::cout << nest << std::endl;

	std::wcout << L"删除尾部元素：" << std::endl;
	nest->remove(1);
	std::cout << nest << std::endl;

}


void interact() {

}

int main()
{
	std::locale::global(std::locale("zh_CN.UTF-8"));
	//parserAll("tests/test4.json");
	dynamicTest();

	//interact();

	return 0;
}


