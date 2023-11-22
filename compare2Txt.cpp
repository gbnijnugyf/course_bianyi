#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>

bool compareFiles(const std::string& file1, const std::string& file2) {
    std::ifstream input1(file1);
    std::ifstream input2(file2);

    if (!input1.is_open() || !input2.is_open()) {
        std::cout << "Failed to open files." << std::endl;
        return false;
    }

    // 将文件内容读取到字符串中
    std::string content1((std::istreambuf_iterator<char>(input1)), std::istreambuf_iterator<char>());
    std::string content2((std::istreambuf_iterator<char>(input2)), std::istreambuf_iterator<char>());

    // 去除空格、制表符和换行符
    content1.erase(std::remove_if(content1.begin(), content1.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }), content1.end());
    content2.erase(std::remove_if(content2.begin(), content2.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }), content2.end());

    input1.close();
    input2.close();

    // 比较两个字符串是否相同
    return content1 == content2;
}

int main() {
    std::string file1 = "output_cor.txt";
    std::string file2 = "output.txt";

    if (compareFiles(file1, file2)) {
        std::cout << "Files are identical." << std::endl;
    } else {
        std::cout << "Files are different." << std::endl;
    }

    return 0;
}