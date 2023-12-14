#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

bool compareFiles(const string& file1, const string& file2) {
    ifstream input1(file1);
    ifstream input2(file2);

    if (!input1.is_open() || !input2.is_open()) {
        cout << "Failed to open files." << endl;
        return false;
    }

    // 将文件内容读取到字符串中
    string content1((istreambuf_iterator<char>(input1)), istreambuf_iterator<char>());
    string content2((istreambuf_iterator<char>(input2)), istreambuf_iterator<char>());

    // 去除空格、制表符和换行符
    content1.erase(remove_if(content1.begin(), content1.end(), [](char c) { return isspace(static_cast<unsigned char>(c)); }), content1.end());
    content2.erase(remove_if(content2.begin(), content2.end(), [](char c) { return isspace(static_cast<unsigned char>(c)); }), content2.end());

    input1.close();
    input2.close();

    // 比较两个字符串是否相同
    return content1 == content2;
}

int main() {
    string file1 = "output_cor_yufa.txt";
    string file2 = "output.txt";

    if (compareFiles(file1, file2)) {
        cout << "Files are identical." << endl;
    } else {
        cout << "Files are different." << endl;
    }

    return 0;
}