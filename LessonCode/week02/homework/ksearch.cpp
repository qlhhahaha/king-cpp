#include "ksearch.h"


void KSearch::loadKeyword(const std::string& keywordPath) {
	std::string line;

	std::ifstream file(keywordPath);
	if (!file.is_open()) {
		std::cerr << "unable to open keyword.txt" << std::endl;
		return;
	}

	while (std::getline(file, line))
		this->keywords.push_back(line);

}