#include "kjson.h"

std::vector<std::string> split(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(str);

	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}


bool isAllDigits(const std::string& str) {
	std::regex pattern("^\\d+$"); // 定义正则表达式模式
	return std::regex_match(str, pattern); // 检查整个字符串是否匹配模式
}


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
	std::shared_ptr<KJson>ptrJson = parserAll("tests/test1.json");
	std::wcout << L"打印整个json：" << std::endl;
	std::cout << ptrJson << std::endl << std::endl;
	
	while (1) {
		std::wcout << L"键入数字来选择功能：" << std::endl;
		std::wcout << L"1、解析json" << std::endl;
		std::wcout << L"2、插入item" << std::endl;
		std::wcout << L"3、删除item" << std::endl;
		std::wcout << L"4、退出" << std::endl;

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
			std::wcout << L"再键入index或keyname，程序将删除对应的value" << std::endl;
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
	//parserAll("tests/test4.json");
	//dynamicTest();

	interact();

	return 0;
}


