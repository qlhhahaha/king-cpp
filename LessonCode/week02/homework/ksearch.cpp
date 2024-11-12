#include "ksearch.h"


void KSearch::loadDataset() {
	//std::wstring wFile = std::wstring(this->datasetPath.begin(), this->datasetPath.end());

	// 打开文件
	HANDLE file = CreateFile(this->datasetPath.c_str(), GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
		throw std::runtime_error("fail to open dataset.xml");

	// 创建文件映射对象
	HANDLE mapFile = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);
	if (mapFile == NULL) {
		CloseHandle(file);
		throw std::runtime_error("fail to create file mapping");
	}

	// 将文件内容映射到内存
	LPVOID pBuffer = MapViewOfFile(mapFile, FILE_MAP_READ, 0, 0, 0);
	if (pBuffer == NULL) {
		CloseHandle(mapFile);
		CloseHandle(file);
		throw std::runtime_error("fail to map view of file");
	}

	DWORD fileSize = GetFileSize(file, NULL);
	this->dataset.assign(static_cast<char*>(pBuffer), fileSize);

	// 释放资源
	UnmapViewOfFile(pBuffer);
	CloseHandle(mapFile);
	CloseHandle(file);
}

void KSearch::loadKeyword() {
	std::string line;

	std::ifstream file(this->keywordPath);
	if (!file.is_open()) {
		std::cerr << "unable to open keyword.txt" << std::endl;
		return;
	}

	while (std::getline(file, line))
		this->keywords.push_back(line);

	file.close();

	for (auto str : this->keywords)
		std::cout << str << std::endl;
	std::cout << std::endl;
}

void KSearch::search() {
	for (const auto& keyword : this->keywords) {
		auto startTime2 = std::chrono::high_resolution_clock::now();
		this->linearSearch(keyword);
		auto endTime2 = std::chrono::high_resolution_clock::now();
		long long totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime2 - startTime2).count();

		std::cout << "count:" << this->keywordCount[keyword] << " time:" << totalTime << "ms" << std::endl;
	}
}

void KSearch::linearSearch(const std::string& keyword) {
	std::size_t pos = this->dataset.find(keyword, 0);
	while (pos != std::string::npos) {
		this->keywordCount[keyword]++;
		pos = this->dataset.find(keyword, pos + 1);
	}
}

void KSearch::KMP(const std::string& keyword) {

}
