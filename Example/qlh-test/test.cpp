//#include <stdio.h>
//#include <tchar.h>
//#include"iostream"
//#include"fstream"
//#include"map"
//#include"string"
//#include"vector"
//#include <algorithm>
//using namespace std;
//
//#define MAXOUT 4 
//
//typedef struct node
//{
//	string allWord;
//	int count;
//	node() { count = 0; allWord = ""; };
//}wordFre;  //用来从训练词组中提取第一个汉字和记录此词语的出现频数
//
//
//void updateTrainSet(string str);  //根据用户输入更新训练集
//
//void TrainModel();        //根据训练集来生成参数模型
//
//void findMostWord(string str);   //根据用户输入的首个汉字来实现联想功能，并且按出现频数进行排序
//
////根据频数实现对词语的排序
//void sortMapByValue(std::map<std::string, wordFre>& tMap, std::vector<std::pair<std::string, wordFre> >& tVector);
//
//int cmp(const std::pair<std::string, wordFre>& x, const std::pair<std::string, wordFre>& y);
//
//map<string, wordFre> dic; //声明一个全局字典，用来存储训练出来的参数
//
//vector<std::pair<std::string, wordFre> > tVector;  //便于用系统函数实现排序
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	string inputStr;
//	cout << "正在训练样本词库......" << endl;
//	TrainModel();          //程序执行前先要对数据集进行训练
//	while (1)
//	{
//		cout << "请输入想打的词的首个汉字！" << endl;
//		cin >> inputStr;
//		if (inputStr == "E" || inputStr == "e")
//		{
//			break;
//			return 0;
//		}
//		findMostWord(inputStr);
//
//		int chose = 0;
//		cout << endl << "请选择你想打的单词(按数字键选择,0为没有选择的词组) : ";
//		cin >> chose;
//		//当输入参数值为0时，此时数据库中没有用户所要输入的词组，那么第一次输入的时候，需要用户将整个词组都输进去，从而进行学习
//		if (chose == 0)
//		{
//			cout << "请输入完整的词组： ";
//			cin >> inputStr;
//			cout << "请稍后，正在更新数据库......" << endl;
//			updateTrainSet(inputStr);
//
//		}
//		else  //当输出的备选词组有用户需要的词语时，要进行对此词语进行概率提升操作
//		{
//			cout << "你想要打的词组是： " << tVector[chose - 1].first << endl;
//			cout << "请稍后，正在更新数据库......" << endl;
//			updateTrainSet(tVector[chose - 1].first);
//		}
//		tVector.clear();
//	}
//	return 0;
//}
//
////根据用户输入的首个汉字，实现联想词语的排序输出（按出现频数大小排序）
//void findMostWord(string str)
//{
//	map<string, wordFre> candidateWord;
//	map<string, wordFre>::iterator iter;
//
//	for (iter = dic.begin(); iter != dic.end(); iter++)
//	{
//		if (iter->second.allWord == str)
//		{
//			candidateWord.insert(pair<string, wordFre>(iter->first, iter->second));
//		}
//	}
//	sortMapByValue(candidateWord, tVector);
//
//	int count = 0;
//	for (int i = 0; i < tVector.size(); i++)
//	{
//		if (count >= MAXOUT)
//		{
//			break;
//		}
//		count++;
//		cout << count << "." << tVector[i].first << "  ";
//	}
//	candidateWord.erase(candidateWord.begin(), candidateWord.end());
//}
//
////在程序开始运行前，要先进行样本训练
//void TrainModel()
//{
//	std::ifstream tfile("example.txt", ios::in);
//	if (!tfile)
//	{
//		cerr << "file could not be opened" << endl;
//		return;
//	}
//	string word;
//	int fre;
//	while (!tfile.eof())
//	{
//		tfile >> word >> fre;
//		dic[word].allWord = word.substr(0, 2);
//		dic[word].count = fre;
//	}
//}
//
//
////每次用户输入完成之后，根据用户习惯重新更新数据库
//void updateTrainSet(string str)
//{
//	dic[str].count += 2;
//	dic[str].allWord = str.substr(0, 2);
//	ofstream examplefile("example.txt");
//	map<string, wordFre>::iterator iter;
//	for (iter = dic.begin(); iter != dic.end(); iter++)
//	{
//		examplefile << iter->first << " " << iter->second.count << endl;
//	}
//	examplefile.close();
//}
//
////下面两个函数主要实现的是对候选的符合条件的词语进行按频数大小的排序
//int cmp(const std::pair<std::string, wordFre>& x, const std::pair<std::string, wordFre>& y)
//{
//	return x.second.count > y.second.count;
//}
//
//
//void sortMapByValue(std::map<std::string, wordFre>& tMap, std::vector<std::pair<std::string, wordFre> >& tVector)
//{
//	for (std::map<std::string, wordFre>::iterator curr = tMap.begin(); curr != tMap.end(); curr++)
//	{
//		tVector.push_back(std::make_pair(curr->first, curr->second));
//	}
//	std::sort(tVector.begin(), tVector.end(), cmp);
//}

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <locale>
#include <codecvt>

struct wWordInfo {
    std::wstring allWord;
    int count;
};

struct WordInfo {
    std::string allWord;
    int count;
};

int main() {
    // 打开文件，使用 std::wifstream 处理宽字符流
    std::wifstream tfile("example.txt");
    if (!tfile.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return 1;
    }

    // 设置全局 locale 为 UTF-8 编码
    std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>());
    tfile.imbue(utf8_locale);

    std::unordered_map<std::wstring, wWordInfo> wDictionary;
    std::wstring word;
    int fre;
    while (tfile >> word >> fre) {
        wWordInfo info;
        info.allWord = word.substr(0, 2);
        info.count = fre;
        wDictionary[word] = info;
    }


    //// 后续处理 dictionary
    //for (const auto& item : dictionary) {
    //    std::string key = item.first;
    //    const WordInfo& value = item.second;
    //    std::cout << "Word: " << key << ", AllWord: " << value.allWord << ", Count: " << value.count << std::endl;
    //}

    int a;

    //// 输出测试
    //for (const auto& [key, value] : dictionary) {
    //    std::wcout << L"Word: " << key << L", AllWord: " << value.allWord << L", Count: " << value.count << std::endl;
    //}

    return 0;
}