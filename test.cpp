#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <set>
#include <string>
#include <algorithm>
#include <iterator>
using namespace std;

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
int main()
{
        vector<string> r = explainEX("X-b->1");

        for (const string &t : r)
        {
                cout << t << " ";
        }
        return 0;
}