#include "kjson.h"


int main()
{
	std::locale::global(std::locale("zh_CN.UTF-8"));
	parserAll("tests/test2.json");

	return 0;
}
