#include "ksearch.h"

void KSearch::loadDataset() {
	std::ifstream file(this->datasetPath, std::ios::in | std::ios::ate);

	if (!file.is_open()) {
		std::cerr << "unable to open dataset.xml" << std::endl;
		return;
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	this->dataset.resize(size);
	this->dataset.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

	file.close();

	//std::cout << this->dataset << std::endl;
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
}

void KSearch::search() {
	for (const auto& keyword : this->keywords) {
		auto startTime2 = std::chrono::high_resolution_clock::now();
		this->KMP(keyword);
		auto endTime = std::chrono::high_resolution_clock::now();
		long long totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime2).count();

		std::cout << "count:" << this->keywordCount[keyword] << " time:" << totalTime << "ms" << std::endl;
	}
}

void KSearch::KMP(const std::string& keyword) {
	std::size_t pos = this->dataset.find(keyword, 0);
	while (pos != std::string::npos) {
		this->keywordCount[keyword]++;
		pos = this->dataset.find(keyword, pos + 1);
	}

}