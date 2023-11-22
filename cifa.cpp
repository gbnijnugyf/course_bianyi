#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <queue>
#include <utility>
#include <map>

using namespace std;

map<string, int> RESERVEDWORDS;
void initializeReservedWords()
{
    RESERVEDWORDS.insert(make_pair("const", 4)); // 从4开始编号对应类别“CONSTTK”......
    RESERVEDWORDS.insert(make_pair("int", 5));
    RESERVEDWORDS.insert(make_pair("char", 6));
    RESERVEDWORDS.insert(make_pair("void", 7));
    RESERVEDWORDS.insert(make_pair("main", 8));
    RESERVEDWORDS.insert(make_pair("if", 9));
    RESERVEDWORDS.insert(make_pair("else", 10));
    RESERVEDWORDS.insert(make_pair("do", 11));
    RESERVEDWORDS.insert(make_pair("while", 12));
    RESERVEDWORDS.insert(make_pair("for", 13));
    RESERVEDWORDS.insert(make_pair("scanf", 14));
    RESERVEDWORDS.insert(make_pair("printf", 15));
    RESERVEDWORDS.insert(make_pair("return", 16));
}

string CATEGORY[36] = {"IDENFR", "INTCON", "CHARCON", "STRCON", "CONSTTK",
                       "INTTK", "CHARTK", "VOIDTK", "MAINTK", "IFTK", "ELSETK", "DOTK", "WHILETK",
                       "FORTK", "SCANFTK", "PRINTFTK", "RETURNTK", "PLUS", "MINU", "MULT", "DIV", "LSS",
                       "LEQ", "GRE", "GEQ", "EQL", "NEQ", "ASSIGN", "SEMICN", "COMMA", "LPARENT", "RPARENT",
                       "LBRACK", "RBRACK", "LBRACE", "RBRACE"};

vector<char> codes;

// 判断是否是字母
bool myIsLetter(char c)
{
    if (isalpha(c) || (c == '_'))
        return true;
    return false;
}
// 判断是否是数字
bool myIsDigit(char c)
{
    return isdigit(c);
}
// 判断是否是保留字
int isReserved(string &s)
{
    auto it = RESERVEDWORDS.find(s);
    if (it != RESERVEDWORDS.end())
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}
// 读取文件
void read(string originCodeFile)
{
    ifstream input;
    input.open(originCodeFile.c_str());
    if (!input.is_open())
    {
        cout << "Fail to open testfile.txt." << endl;
        exit(0);
    }
    // 通过移动文件指针至末尾得到文件大小（字节）
    input.seekg(0, ios::end);
    int fileSize = input.tellg();
    input.seekg(0, ios::beg);

    char *buffer = new char[fileSize + 1];
    input.read(buffer, fileSize);
    buffer[fileSize] = ' ';
    input.close();
    codes.assign(buffer, buffer + fileSize + 1);
    delete[] buffer;
}

void singleSign(string &word, int cateNo, char c, ofstream &output)
{
    word.push_back(c);
    output << CATEGORY[cateNo] << " " << word << endl;
}

void cifaAnalysis(string outputTokenFile)
{
    ofstream output(outputTokenFile.c_str());
    unsigned i = 0;
    // 逐一读取字符
    char c = codes[i++];
    while (i < codes.size())
    {
        string word = "";
        while (c == ' ' || c == '\t' || c == '\n')
            c = codes[i++];
        if (myIsLetter(c))
        {
            // 处理标识符或保留字
            while (myIsLetter(c) || myIsDigit(c))
            {
                word.push_back(c);
                c = codes[i++];
            }
            if (isReserved(word)) // 是保留字
            {
                output << CATEGORY[isReserved(word)] << " " << word << endl;
            }
            else // 是标识符
            {
                output << CATEGORY[0] << " " << word << endl;
            }
        }
        else if (myIsDigit(c)) // 处理整数
        {
            while (myIsDigit(c))
            {
                word.push_back(c);
                c = codes[i++];
            }
            output << CATEGORY[1] << " " << word << endl;
        }
        else // 处理其他符号
        {
            if (c == '+')
            {
                singleSign(word, 17, c, output);
            }
            else if (c == '-')
            {
                singleSign(word, 18, c, output);
            }
            else if (c == '*')
            {
                singleSign(word, 19, c, output);
            }
            else if (c == '/')
            {
                singleSign(word, 20, c, output);
            }
            else if (c == '<')
            {
                word.push_back(c);
                c = codes[i++];
                if (c == '=')
                {
                    singleSign(word, 22, c, output);
                }
                else
                {
                    output << CATEGORY[21] << " " << word << endl;
                }
            }
            else if (c == '>')
            {
                word.push_back(c);
                c = codes[i++];
                if (c == '=')
                {
                    singleSign(word, 24, c, output);
                }
                else
                {
                    output << CATEGORY[23] << " " << word << endl;
                }
            }
            else if (c == '=')
            {
                word.push_back(c);
                c = codes[i++];
                if (c == '=')
                {
                    singleSign(word, 25, c, output);
                }
                else
                {
                    output << CATEGORY[27] << " " << word << endl;
                }
            }
            else if (c == '!')
            {
                word.push_back(c);
                c = codes[i++];
                word.push_back(c);
                if (c == '=')
                {
                    output << CATEGORY[26] << " " << word << endl;
                }
            }
            else if (c == ';')
            {
                singleSign(word, 28, c, output);
            }
            else if (c == ',')
            {
                singleSign(word, 29, c, output);
            }
            else if (c == '(')
            {
                singleSign(word, 30, c, output);
            }
            else if (c == ')')
            {
                singleSign(word, 31, c, output);
            }
            else if (c == '[')
            {
                singleSign(word, 32, c, output);
            }
            else if (c == ']')
            {
                singleSign(word, 33, c, output);
            }
            else if (c == '{')
            {
                singleSign(word, 34, c, output);
            }
            else if (c == '}')
            {
                singleSign(word, 35, c, output);
            }
            else if (int(c) == 34)
            {
                c = codes[i++];
                while (int(c) != 34)
                {
                    word.push_back(c);
                    c = codes[i++];
                }
                output << CATEGORY[3] << " " << word << endl;
            }
            else if (int(c) == 39)
            {
                c = codes[i++];
                while (int(c) != 39)
                {
                    word.push_back(c);
                    c = codes[i++];
                }
                output << CATEGORY[2] << " " << word << endl;
            }
            c = codes[i];
            i++;
        }
    }
    codes.clear();
    output.close();
}
void checkFile(string ouputFile)
{
    // 读取文件内容到向量中
    ifstream file(ouputFile.c_str());
    if (file.is_open())
    {
        vector<string> lines;
        string line;
        while (getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();
        // 若不合理删除最后一行
        if (lines[lines.size() - 1].compare("PRINTFTK printf")==0)
        {
            lines.pop_back();
        }
        // 将向量中的内容写回文件
        ofstream finalFile(ouputFile.c_str());
        if (finalFile.is_open())
        {
            for (const auto &line : lines)
            {
                finalFile << line << endl;
            }
            finalFile.close();
        }
        else
        {
            cout << "无法写入文件" << endl;
        }
    }
    else
    {
        cout << "无法打开文件" << endl;
    }
}

int main()
{
    initializeReservedWords();
    read("testfile.txt");
    cifaAnalysis("output.txt");
    checkFile("output.txt");
    return 0;
}
