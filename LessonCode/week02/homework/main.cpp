#include "ksearch.h"




int main() {
	std::locale::global(std::locale("en_US.UTF-8"));

	std::string datasetPath = "D:/lab_work_qlh/king/lab-git-push/search_dataset/dataset.xml";
	std::string keywordPath = "D:/lab_work_qlh/king/lab-git-push/search_dataset/keyword.txt";
	std::string outputPath = "D:/lab_work_qlh/king/lab-git-push/search_dataset/output.txt";

	KSearch* ptrSearch = new KSearch;

	// 读取dataset

	// 读取keyword.txt
	ptrSearch->loadKeyword(keywordPath);

	// 字符串匹配




	// 输出到output.txt


	return 0;
}