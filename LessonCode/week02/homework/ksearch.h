#ifndef KSEARCH_H
#define KSEARCH_H

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<locale>
#include<chrono>
#include<unordered_map>



class KSearch {
public:
	KSearch(std::string datasetPath, std::string keywordPath, std::string outputPath) :
		datasetPath(datasetPath)
		, keywordPath(keywordPath)
		, outputPath(outputPath) {
	}

	~KSearch() {}

	void loadDataset();
	void loadKeyword();
	void search();



private:
	const std::string datasetPath;
	const std::string keywordPath;
	const std::string outputPath;

	std::string dataset;
	std::vector<std::string> keywords;

	std::unordered_map<std::string, int> keywordCount;

	void KMP(const std::string& keyword);
};





#endif