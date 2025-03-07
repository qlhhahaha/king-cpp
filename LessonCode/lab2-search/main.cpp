﻿#include "ksearch.h"


int main() {
	std::locale::global(std::locale("en_US.UTF-8"));

	std::string datasetPath = "D:/lab_work_qlh/king/lab-git-push/search_dataset/dataset.xml";
	std::string keywordPath = "D:/lab_work_qlh/king/lab-git-push/search_dataset/keyword.txt";
	std::string outputPath = "D:/lab_work_qlh/king/lab-git-push/search_dataset/output.txt";


	// 输出到output.txt
	std::ofstream outFile(outputPath);
	if (!outFile.is_open()) {
		std::cerr << "fail to open output.txt" << std::endl;
		return 0;
	}
	std::streambuf* coutBuffer = std::cout.rdbuf();
	std::cout.rdbuf(outFile.rdbuf());

	KSearch searchEngine(datasetPath, keywordPath, outputPath);

	auto startTime1 = std::chrono::high_resolution_clock::now();

	// 读取dataset
	searchEngine.loadDataset();
	auto endTime1 = std::chrono::high_resolution_clock::now();
	long long loadTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime1 - startTime1).count();
	std::cout << "load time: " << loadTime << "ms" << std::endl;

	// 读取keyword.txt
	searchEngine.loadKeyword();

	// 字符串匹配
	searchEngine.search();

	auto endTime2 = std::chrono::high_resolution_clock::now();
	long long totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime2 - startTime1).count();
	std::cout << "total time: " << totalTime << "ms" << std::endl;

	outFile.close();

	return 0;
}