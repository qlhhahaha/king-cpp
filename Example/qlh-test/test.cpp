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