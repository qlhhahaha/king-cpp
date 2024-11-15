#include "ksearch.h"

std::mutex mtx;
const int NUM_THREADS = 8;


void KSearch::loadDataset() {
	//std::wstring wFile = std::wstring(this->datasetPath.begin(), this->datasetPath.end());

	// 打开文件
	HANDLE fd = CreateFile(this->datasetPath.c_str(), GENERIC_READ,
		FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (fd == INVALID_HANDLE_VALUE)
		throw std::runtime_error("fail to open dataset.xml");

	// 创建文件映射对象
	HANDLE mapFile = CreateFileMapping(fd, nullptr, PAGE_READONLY, 0, 0, nullptr);
	if (mapFile == nullptr) {
		CloseHandle(fd);
		throw std::runtime_error("fail to create file mapping");
	}

	const long long fileSize = GetFileSize(fd, nullptr);
	const long long chunkSize = fileSize / NUM_THREADS;

	// 将文件内容映射到内存
	char* pBuffer = (char*)MapViewOfFile(mapFile, FILE_MAP_READ, 0, 0, fileSize);
	if (pBuffer == nullptr) {
		// 获取错误码
		DWORD errorCode = GetLastError();

		// 根据错误码输出相应的错误信息
		switch (errorCode) {
		case ERROR_INVALID_PARAMETER:
			std::cerr << "MapViewOfFile参数设置错误" << std::endl;
			break;
		case ERROR_NOT_ENOUGH_MEMORY:
			std::cerr << "系统内存不足，无法完成内存映射视图操作" << std::endl;
			break;
		case ERROR_FILE_INVALID:
			std::cerr << "文件无效，可能已损坏或处于不正确的状态" << std::endl;
			break;
		default:
			std::cerr << "未知错误，错误码: " << errorCode << std::endl;
			break;
		}

		CloseHandle(mapFile);
		CloseHandle(fd);
	}

	std::vector<std::thread> threads;

	for (int i = 0; i < NUM_THREADS; i++) {
		long long start = i * chunkSize;
		long long length = (i == NUM_THREADS - 1) ? (fileSize - start) : chunkSize;
		threads.push_back(std::thread(&KSearch::readFileChunk, this, fd, mapFile, pBuffer, start, length, i));
	}

	for (auto& thread : threads)
		thread.join();

	//for (auto& item : this->chunks)
	//	std::cout << item << std::endl;

	// TODO: 释放资源放到析构函数中
	UnmapViewOfFile(pBuffer);
	CloseHandle(mapFile);
	CloseHandle(fd);
}


void KSearch::readFileChunk(HANDLE fd, HANDLE mapFile, char* pBuffer, long long start, long long length, int threadID) {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	DWORD pageSize = sysInfo.dwPageSize * 1024;

	DWORD offsetHigh = start >> 32;
	DWORD offsetLow = start & 0xFFFFFFFF;
	offsetLow = (offsetLow / pageSize) * pageSize;

	// 将映射的内存区域复制到string中
	const char* chunkData = pBuffer + start;
	//std::lock_guard<std::mutex> guard(mtx);
	this->chunks[threadID].append(chunkData, length);
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

	//for (auto str : this->keywords)
	//	std::cout << str << std::endl;
	std::cout << std::endl;

	// 提前扩容，处理出现次数很多的keyword，但好像没什么用
	for (auto str : this->keywords) {
		// this->keywordPos[str].reserve(7777777);
		this->keywordPosMul[str].resize(NUM_THREADS);
		this->keywordCountMul[str].resize(NUM_THREADS);
	}

}


void KSearch::search() {
	for (const auto& keyword : this->keywords) {
		std::vector<std::thread> threads;

		auto startTime2 = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < NUM_THREADS; i++) {
			threads.push_back(std::thread(&KSearch::linearSearch, this, i, keyword));
		}

		for (auto& thread : threads)
			thread.join();

		for (int i = 0; i < NUM_THREADS; i++) {
			this->keywordCount[keyword] += this->keywordCountMul[keyword][i];
		}

		this->boundarySearch(keyword);

		auto endTime2 = std::chrono::high_resolution_clock::now();
		long long totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime2 - startTime2).count();
		std::cout << "count:" << this->keywordCount[keyword] << " time:" << totalTime << "ms" << std::endl;
	}
	std::cout << std::endl;
}


void KSearch::linearSearch(int threadID, const std::string& keyword) {
	std::size_t pos = this->chunks[threadID].find(keyword, 0);
	while (pos != std::string::npos) {
		//std::lock_guard<std::mutex> guard(mtx);

		// 每个线程的处理结果放到不同的vector中，从而避免使用锁
		this->keywordCountMul[keyword][threadID]++;
		this->keywordPosMul[keyword][threadID].push_back(pos);

		pos = this->chunks[threadID].find(keyword, pos + 1);
	}
}


void KSearch::KMP(const std::string& keyword) {

}

void KSearch::boundarySearch(const std::string& keyword) {
	std::string boundary, tmpStr;
	long long tmpLen = 0;
	int change = 0;
	std::size_t pos = 0;

	for (int i = 0; i < NUM_THREADS - 1; i++) {
		// 添加左边界
		tmpLen = this->chunks[i].length();
		if (tmpLen > keyword.length()) {
			tmpStr = this->chunks[i].substr(tmpLen - keyword.length());
			pos = tmpStr.find(keyword);
			if (pos != std::string::npos)
				change--;

			boundary.append(tmpStr);
		}


		// 添加右边界
		tmpLen = this->chunks[i + 1].length();
		if (tmpLen > keyword.length()) {
			tmpStr = this->chunks[i + 1].substr(0, keyword.length());
			pos = tmpStr.find(keyword);
			if (pos != std::string::npos)
				change--;

			boundary.append(tmpStr);
		}
	}

	pos = boundary.find(keyword, 0);
	while (pos != std::string::npos) {
		this->keywordPos[keyword].push_back(pos);
		this->keywordCount[keyword]++;
		pos = boundary.find(keyword, pos + 1);
	}

	this->keywordCount[keyword] += change;
}