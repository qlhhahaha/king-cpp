#ifndef KSEARCH_H
#define KSEARCH_H

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<locale>

class KSearch {
public:
	KSearch();
	~KSearch();

	void loadKeyword(const std::string& keywordPath);

private:
	std::vector<std::string> keywords;

}







#endif