#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <set>
#include <string>
#include <algorithm>
#include <iterator>
#define EPSILON "~"
using namespace std;
/*
NFA转换为DFA
先求ε-closure(I)：经过任意条ε弧到达的状态以及I本身
1.求DFA初态，DFA初态 = ε-closure({NFA初态})
2.求状态子集，例如：状态子集Ia=ε-closure({表示状态集经过a弧能够到达的}),若出现新状态纳入状态集
3.不断对状态集求步骤2，直至不出现新状态
4.包含NFA终态的集合均为DFA终态

测试数据：
X X-~->3
Y
0 0-a->1
1 1-b->2
2 2-b->Y
3 3-~->0 3-a->3 3-b->3
输出结果应为：
X X-a->0 X-b->1
Y Y-a->0 Y-b->1
0 0-a->0 0-b->2
1 1-a->0 1-b->1
2 2-a->0 2-b->Y
*/
// 判断两个vector是否含有完全相同的元素
template <typename T>
bool isSame(const vector<T> &vec1, const vector<T> &vec2)
{
    if (vec1.size() != vec2.size())
    {
        return false; // 向量大小不同，元素肯定不相同
    }
    set<T> set1(vec1.begin(), vec1.end());
    set<T> set2(vec2.begin(), vec2.end());
    return set1 == set2;
}

typedef struct statusType
{
    bool isSolved;
    vector<string> statu;
    string no; // 编号
    // 为结构体重载==运算符，便于使用addUnique添加元素
    bool operator==(const statusType &other) const
    {
        return isSame(statu, other.statu);
    }
    bool operator<(const statusType &other) const
    {
        // 根据您的排序规则编写比较逻辑
        // 返回 true 表示当前对象应该在 other 对象之前，返回 false 表示当前对象应该在 other 对象之后
        return true;
    }
    bool operator>(const statusType &other) const
    {
        return false;
    }
} statusType;

typedef struct statusLetterType
{
    vector<string> statu;
    string letter; // 输入符号
    bool operator==(const statusType &other) const
    {
        return isSame(statu, other.statu);
    }
} statusLetterType;

vector<vector<string>> OutPutResult; // 最终输出
vector<statusType> AllStatus;        // 状态集

vector<vector<string>> input(string path)
{
    vector<vector<string>> data;

    ifstream inputFile(path);
    if (!inputFile)
    {
        cout << "无法打开文件!" << endl;
        return data;
    }

    string line;
    while (getline(inputFile, line))
    {
        vector<string> tokens;
        stringstream ss(line);
        string token;
        while (ss >> token)
        {
            tokens.push_back(token);
        }
        data.push_back(tokens);
    }

    inputFile.close();

    return data;
}
vector<vector<string>> input()
{
    vector<vector<string>> data;
    string line;
    while (getline(cin, line))
    {
        if (line.size() == 0)
            break; // 如果读取的是空，则读取结束
        vector<string> tokens;
        stringstream ss(line);
        string token;
        while (ss >> token)
        {
            tokens.push_back(token);
        }
        data.push_back(tokens);
    }
    return data;
}

vector<string> splitString(const string &input, const string &delimiter)
{
    vector<string> tokens;
    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = input.find(delimiter, start)) != string::npos)
    {
        string token = input.substr(start, end - start);
        tokens.push_back(token);
        start = end + delimiter.length();
    }

    // 处理最后一个分隔符后的子串
    string lastToken = input.substr(start);
    tokens.push_back(lastToken);

    return tokens;
}
// 解析传导式（忘了叫什么式子了）
vector<string> explainEX(string ex)
{
    vector<string> result; // 3位，分别存放statu1, statu2, in
    vector<string> temp;
    vector<string> temp1;
    temp = splitString(ex, "->");
    temp1 = splitString(ex, "-");
    result.push_back(temp1[0]);
    result.push_back(temp1[1]);
    result.push_back(temp[1]);
    return result;
}

// 求解ε-closure(I)
vector<string> epsilone_clo(vector<vector<string>> matrix, vector<string> I)
{
    vector<string> result = I; // 包含自身
    for (const string &token : I)
    {
        for (const vector<string> &line : matrix)
        {
            // 在矩阵中找到对应的行，且含有传导式
            if (line.size() > 1 && token.compare(line[0]) == 0)
            {
                int exnum = line.size() - 1; // 传导式个数
                // 遍历传导式
                for (int i = 1; i <= exnum; i++)
                {
                    vector<string> exArr = explainEX(line[i]);
                    if (exArr[1].compare(EPSILON) == 0)
                    {
                        vector<string> temp;
                        temp.push_back(exArr[2]);
                        vector<string> r = epsilone_clo(matrix, temp);
                        result.insert(result.end(), r.begin(), r.end());
                    }
                }
            }
        }
    }
    return result;
}

// 通过NFA初态X寻找DFA初态
vector<string> findInit(vector<vector<string>> matrix, vector<string> I = {"X"})
{
    return epsilone_clo(matrix, I);
}

// 创建模板函数addUnique，意在实现仅添加不存在原vector的元素
template <typename T>
void addUnique(vector<T> &vec, const T &element)
{
    if (find(vec.begin(), vec.end(), element) == vec.end())
    {
        vec.push_back(element);
    }
}

// 寻找所有输入符
vector<string> findAllLetter(vector<vector<string>> matrix)
{
    vector<string> result;
    for (const vector<string> &line : matrix)
    {
        // 如果没有传导式则跳过
        if (line.size() < 1)
        {
            continue;
        }
        // 若有，则遍历传导式
        int exnum = line.size() - 1; // 传导式个数
        for (int i = 1; i <= exnum; i++)
        {
            vector<string> temp = explainEX(line[i]);
            if (temp[1].compare(EPSILON) == 0)
            {
                continue;
            }
            addUnique(result, temp[1]);
        }
    }
    return result;
}

// 状态集通过输入字符（经过该字符letter的弧能够到达，不包含自身）,letter不能为ε，且必须在输入符集合里
vector<string> freshByToken(vector<vector<string>> matrix, vector<string> I, string letter)
{
    vector<string> result;
    for (const string &token : I)
    {
        // 在矩阵中找到对应的行，且含有传导式
        for (const vector<string> &line : matrix)
        {
            if (token.compare(line[0]) == 0 && line.size() > 1)
            {
                int exnum = line.size() - 1; // 传导式个数
                // 遍历传导式
                for (int i = 1; i <= exnum; i++)
                {
                    vector<string> temp = explainEX(line[i]);
                    // 是否是对应的输入符号
                    if (temp[1].compare(letter) == 0)
                    {
                        result.push_back(temp[2]);
                    }
                }
            }
        }
    }
    return result;
}

// 从初态开始求状态子集
vector<statusType> statuSet(vector<vector<string>> data, vector<statusType> status, vector<string> allLetter)
{
    vector<statusType> status_;
    bool flag = false; // 设置标志，是否开始数字编号
    status_.insert(status_.end(), status.begin(), status.end());
    for (unsigned int i = 0; i < status_.size(); i++)
    {
        if (!status_[i].isSolved) // 未被求解过
        {
            for (const string &letter : allLetter)
            {
                vector<string> statu_temp = epsilone_clo(data, freshByToken(data, status_[i].statu, letter));
                // 加入状态集
                if (statu_temp.size() == 0)
                {
                    continue;
                }
                statusType temp;
                temp.isSolved = false;
                temp.statu = statu_temp;

                if (find(statu_temp.begin(), statu_temp.end(), "X") != statu_temp.end()) // 包含X，初态
                {
                    temp.no = "X";
                }
                else if (find(statu_temp.begin(), statu_temp.end(), "Y") != statu_temp.end()) // 包含Y，终态
                {
                    // 查找状态集中是否已有“Y”编号
                    if (find_if(status_.begin(), status_.end(), [](const statusType &status)
                                { return status.no.compare("Y") == 0; }) != status_.end()) // 已有Y编号
                    {
                        // 判断是仅有一个Y，还是已有多个Y
                        vector<statusType>::iterator it1 = find_if(status_.begin(), status_.end(), [](const statusType &status)
                                                                   {   bool c1 = status.no.find("Y");
                                bool c2 =  (status.no.length()>1);
                                return c1 && c2; });
                        if (it1 != status_.end()) // 有多个Y
                        {
                            string lastNo; // 用于记录上一个Y
                            // 往前查找上一个Y
                            for (vector<statusType>::iterator it = status_.end() - 1; it != status_.begin(); it--)
                            {
                                if ((*it).no.find("Y"))
                                {
                                    lastNo = (*it).no;
                                    break;
                                }
                            }
                            string n = lastNo.substr(1);
                            n = to_string(atoi(n.c_str()) + 1);
                            temp.no = "Y" + n;
                        }
                        else // 仅有一个Y
                        {
                            temp.no = "Y1";
                        }
                    }
                    else
                    {
                        temp.no = "Y";
                    }
                }
                else
                {
                    if (!flag) // flag为false，证明还没开始数字编号
                    {
                        temp.no = "0";
                        flag = true;
                    }
                    else
                    {
                        string lastNo; // 用于记录上一个编号
                        // 往前查找上一个编号
                        for (vector<statusType>::iterator it = status_.end() - 1; it != status_.begin(); it--)
                        {
                            if ((*it).no.compare("X") != 0 && (*it).no.compare("Y") != 0)
                            {
                                lastNo = (*it).no;
                                break;
                            }
                        }
                        temp.no = to_string(atoi(lastNo.c_str()) + 1);
                    }
                }

                addUnique(status_, temp);
            }
        }
        status_[i].isSolved = true;
    }
    return status_;
}
// 从状态集中找出对应集合返回编号
string findSetNo(vector<statusType> status, vector<string> set)
{
    vector<statusType> status_test = status;
    vector<statusType>::iterator it = find_if(status_test.begin(), status_test.end(), [&set](const statusType &sta)
                                              { return isSame(sta.statu, set); });
    if (it == status_test.end()){
        return "-1";
    }
    return (*it).no;
}
// 输出传导式
void outputEx(string statu1, string letter, string statu2)
{
    cout << statu1 << "-" << letter << "->" << statu2;
}
// 输出行
void outputLine(vector<vector<string>> data, vector<statusType> temp_copy, vector<statusType> &status, string no, vector<string> allLetter)
{
    vector<statusType>::iterator it = find_if(status.begin(), status.end(), [&no](const statusType &status)
                                              { return status.no.compare(no) == 0; });
    if (it != status.end())
    {
        vector<statusLetterType> sAll;
        for (const string &letter : allLetter)
        {
            statusLetterType sa;
            sa.statu = epsilone_clo(data, freshByToken(data, (*it).statu, letter));
            sa.letter = letter;
            sAll.push_back(sa);
        }
        if (sAll.size() == 0)
        {
            // 没有传导式，不输出
            status.erase(it);
            return;
        }
        cout << no;
        for (const statusLetterType &sa : sAll)
        {
            if (sa.statu.size() > 0 && temp_copy.size()>0)
            {
                cout << " ";
                string setNo = findSetNo(temp_copy, sa.statu);
                if (setNo.compare("-1")==0){
                    continue;
                }
                outputEx(no, sa.letter, setNo);
            }
        }
        cout << endl;
    }
    status.erase(it);
}
// 状态集排序以符合输出顺序
bool statusSrot(statusType a, statusType b)
{
    if (a.no.compare("X") == 0 || b.no.compare("X") == 0) // 其中一个是X
    {
        if (a.no.compare("X") == 0)
        {
            return a < b;
        }
        else
        {
            return a > b;
        }
    }
    else if (a.no.find("Y")!=string::npos && b.no.find("Y")!=string::npos) // 两个Y相比较
    {
        if (a.no < b.no)
        {
            return a < b;
        }
        else
        {
            return a > b;
        }
    }
    else if ((a.no.find("Y")!=string::npos && b.no.find("Y")==string::npos) || (a.no.find("Y")==string::npos && b.no.find("Y")!=string::npos)) // 一个为Y、一个为数字
    {
        if (a.no.find("Y")!=string::npos)
        {
            return a < b;
        }
        else
        {
            return a > b;
        }
    }
    else // 两个均为数字
    {
        if (a.no < b.no)
        {
            return a < b;
        }
        return a > b;
    }
    // return a < b;
}
int main()
{
    // string path = "NFAinput.txt";
    vector<vector<string>> data = input();

    // 准备工作,找到所有输入符
    vector<string> allLetter = findAllLetter(data);
    // 1.求DFA初态，DFA初态 = ε-closure({NFA初态})
    vector<string> init = findInit(data);
    // 加入状态集
    statusType temp;
    temp.isSolved = false;
    temp.statu = init;
    temp.no = "X";
    addUnique(AllStatus, temp);

    vector<statusType> temp_ = statuSet(data, AllStatus, allLetter); // temp_为状态集合

    // 状态集副本
    vector<statusType> temp_copy = temp_;
    // 输出
    sort(temp_copy.begin(), temp_copy.end(), statusSrot);
    for (const statusType &toGetNo : temp_copy)
    {
        outputLine(data, temp_copy, temp_, toGetNo.no, allLetter);
    }

    return 0;
}