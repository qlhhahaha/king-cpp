﻿#ifndef KSEARCH_H
#define KSEARCH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <locale>
#include <chrono>
#include <unordered_map>
#include <stdexcept>
#include <windows.h>
#include <mutex>
#include <thread>

extern const int NUM_THREADS;

class KSearch {
public:
	KSearch(std::string datasetPath, std::string keywordPath, std::string outputPath) :
		datasetPath(datasetPath)
		, keywordPath(keywordPath)
		, outputPath(outputPath)
		, chunks(NUM_THREADS){
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
	std::vector<std::string> chunks;
	std::vector<std::string> keywords;
	std::unordered_map<std::string, int> keywordCount;


	void KMP(const std::string& keyword);
	void linearSearch(int threadID, const std::string& keyword);
	void readFileChunk(HANDLE fd, HANDLE mapFile, char* pBuffer, long long start, long long length, int threadID);

};





#endif