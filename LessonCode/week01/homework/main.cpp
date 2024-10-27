#include "kjson.h"


void dynamicTest() {
	std::shared_ptr<KJson> ptrJson = parserAll("tests/test4.json");
	std::cout << ptrJson << std::endl << std::endl;
	std::wcout << L"key：website对应的value为：" << std::endl;

	KJson* nest = (*ptrJson)["website"];
	std::cout << nest << std::endl;

	std::wcout << L"website中的第0个value：" << std::endl;
	std::cout << (*nest)[0] << std::endl;

	KJson* nest1 = (*nest)[0];
	std::wcout << L"website中的第0个value中，名为name1的value：" << std::endl;
	std::cout << (*nest1)["name1"] << std::endl;

	//std::cout << (*(*nest)[0])[0] << std::endl;

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
	// TODO 异常输入防御性检查

	std::shared_ptr<KJson>ptrJson = parserAll("tests/test1.json");
	std::wcout << L"打印整个json：" << std::endl;
	std::cout << ptrJson << std::endl << std::endl;
	
	while (1) {
		std::wcout << L"键入数字来选择功能：" << std::endl;
		std::wcout << L"1、解析json" << std::endl;
		std::wcout << L"2、插入item" << std::endl;
		std::wcout << L"3、删除item" << std::endl;
		std::wcout << L"4、输出到output.json" << std::endl;
		std::wcout << L"5、输出到output.xml" << std::endl;
		std::wcout << L"6、退出" << std::endl;

		int funNum = 1;
		std::cin >> funNum;
		std::cin.ignore();

		if (funNum == 1) {
			while (1) {
				std::wcout << L"输入键名，程序返回键值(键入quit重回功能选择)" << std::endl;

				// 获取输入的每一个单词
				std::string inputStr;
				std::getline(std::cin, inputStr);

				if (inputStr == "quit")
					break;

				std::vector<std::string> words = split(inputStr, ' ');
				std::vector<KJson*> nest(words.size(), nullptr);
				nest[0] = (*ptrJson)[words[0]];
				for (int i = 1; i < nest.size(); i++) {
					if (isAllDigits(words[i])) {
						int numIndex = std::stoi(words[i]);
						nest[i] = (*nest[i - 1])[numIndex];
					}
					else {
						nest[i] = (*nest[i - 1])[words[i]];
					}
				}

				std::cout << nest.back() << std::endl;
			}
		}

		else if (funNum == 2) {
			std::wcout << L"先通过键入key来选择到某一个item" << std::endl;

			std::string inputStr;
			std::getline(std::cin, inputStr);

			std::vector<std::string> words = split(inputStr, ' ');
			std::vector<KJson*> nest(words.size(), nullptr);
			nest[0] = (*ptrJson)[words[0]];
			for (int i = 1; i < nest.size(); i++) {
				if (isAllDigits(words[i])) {
					int numIndex = std::stoi(words[i]);
					nest[i] = (*nest[i - 1])[numIndex];
				}
				else {
					nest[i] = (*nest[i - 1])[words[i]];
				}
			}
			std::cout << nest.back() << std::endl;

			std::wcout << L"再键入key和value，程序将其添加到尾部" << std::endl;
			std::getline(std::cin, inputStr);
			std::vector<std::string> insertWords= split(inputStr, ' ');

			KJson* newItem = new KJsonString;
			newItem->setKey(insertWords[0]);
			newItem->setValue(insertWords[1]);
			nest.back()->pushBack(newItem);

			std::wcout << L"增加后的结果" << std::endl;
			std::cout << nest.back() << std::endl;
		}

		else if (funNum == 3) {
			std::wcout << L"先通过键入key来选择到某一个item" << std::endl;
			
			std::string inputStr;
			std::getline(std::cin, inputStr);

			std::vector<std::string> words = split(inputStr, ' ');
			std::vector<KJson*> nest(words.size(), nullptr);
			nest[0] = (*ptrJson)[words[0]];
			for (int i = 1; i < nest.size(); i++) {
				if (isAllDigits(words[i])) {
					int numIndex = std::stoi(words[i]);
					nest[i] = (*nest[i - 1])[numIndex];
				}
				else {
					nest[i] = (*nest[i - 1])[words[i]];
				}
			}
			std::cout << nest.back() << std::endl;
			std::wcout << L"再键入index或keyname(空格隔开)，程序将删除对应的value" << std::endl;
			std::cin >> inputStr;
			if (isAllDigits(inputStr)) {
				int indexNum = std::stoi(inputStr);
				nest.back()->remove(indexNum);
			}
			else {
				std::string keyName = inputStr;
				nest.back()->remove(keyName);
			}

			std::wcout << L"删除后的结果" << std::endl;
			std::cout << nest.back() << std::endl;
		}

		else if (funNum == 4) {
			std::wcout << L"打印现在的json" << std::endl;
			std::cout << ptrJson << std::endl << std::endl;
			std::wcout << L"将其输出到output.json文件" << std::endl;

			std::ofstream outputJson("tests/output.json", 
				std::ios::out | std::ios::trunc);

			if (!outputJson.is_open()) {
				std::cerr << "fail to open output.json" << std::endl;
				return;
			}

			outputJson << ptrJson;
			outputJson.close();
			std::wcout << L"输出成功！" << std::endl << std::endl;
		}

		else if (funNum == 5) {
			outputXml(ptrJson);
			std::wcout << L"已成功转化为xml序列化格式，并输出到tests/output.xml中！" << std::endl << std::endl;
		}

		else if (funNum == 6) {
			return;
		}

		else {
			std::wcout << L"输入错误，退出" << std::endl;
			return;
		}
	}

}



int main()
{
	std::locale::global(std::locale("zh_CN.UTF-8"));
	//dynamicTest();
	interact();

	return 0;
}


