#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstdio>
#include <cctype>
#include <regex>
#include <algorithm>
#include <fstream>
using namespace std;
// 关键字
static vector<string> keyWordKey = {"const", "int", "char", "void", "main", "getint", "if", "else", "switch", "case", "default", "while", "for", "scanf", "printf", "return", "auto", "short", "long", "float", "double", "struct", "union", "enum", "typedef", "unsigned", "signed", "extern", "register", "static", "volatile", "do", "goto", "continue", "break", "sizeof"};

// 关键字种别码
static vector<string> keyWordValue = {"CONSTTK", "INTTK", "CHARTK", "VOIDTK", "MAINTK", "GETINTTK", "IFTK", "ELSETK", "SWITCHTK", "CASETK", "DEFAULTTK", "WHILETK", "FORTK", "SCANFTK", "PRINTFTK", "RETURNTK", "AUTOTK", "SHORTTK", "LONGTK", "FLOATTK", "DOUBLETK", "STRUCTTK", "UNIONTK", "ENUMTK", "TYPEDEFTK", "UNSIGNEDTK", "SIGNEDTK", "EXTERNTK", "REGISTERTK", "STATICTK", "VOLATILLETK", "DOTK", "GOTOTK", "CONTINUETK", "BREAKTK", "SIZEOFTK"};

// 运算符
static vector<string> operationKey = {"+", "-", "*", "/", "<", "<=", ">", ">=", "==", "!=", "="};
// 运算符种别码
static vector<string> operationValue = {"PLUS", "MINU", "MULT", "DIV", "LSS", "LEQ", "GRE", "GEQ", "EQL", "NEQ", "ASSIGN"};

// 界符
static vector<string> symbolKey = {":", ";", ",", "(", ")", "{", "}", "[", "]"};

// 界符种别码
static vector<string> symbolValue = {"COLON", "SEMICN", "COMMA", "LPARENT", "RPARENT", "LBRACE", "RBRACE", "LBRACK", "RBRACK"};

// 语法分析当前单词的种别码
static vector<string> Tokens;
// 语法分析当前单词
static vector<string> vals;

// key为函数的方法名，Value值为1表示该方法是有返回值的方法
static map<string, int> NVoidFunction;

static map<string, string> keywords;
static map<string, string> operations;
static map<string, string> symbols;

// 指向当前所读到字符串的位置的指针
static int p, lines;

// 语法分析当前单词位置
static int q;

ofstream write("output.txt");
ifstream read("testfile.txt");
void lexAnalysing(string str);
void init();
void digitCheck(string str);
void letterCheck(string str);
void stringCheck(string str);
void charCheck(string str);
void symbolCheck(string str);
void parsing();
void parse_str();
void parse_program();
void parse_const_describe();
void parse_const_def();
void parse_unsignedInt();
void parse_Int();
void parse_defHead();
void parse_var_describe();
void parse_var_def();
void parse_funcReturn();
void parse_funcNonReturn();
void parse_comStatement();
void parse_paraForm();
void parse_mainFunc();
void parse_expression();
void parse_item();
void parse_factor();
void parse_sentence();
void parse_assign_sentence();
void parse_condition_sentence();
void parse_condition();
void parse21_circulate();
void parse_step();
void parse23();
void parse_callFunc();
void parse25();
void parse26();
void parse27();
void parse28();
void parse29();
void MatchToken(string expected);
int main()
{
    init();
    lines = 1;
    string line;
    while (getline(read, line))
    {
        lexAnalysing(line);
        lines++;
    }
    parsing();
    write.close();
    read.close();
    return 0;
}
void init()
{
    for (int i = 0; i < keyWordKey.size(); i++)
    {
        keywords[keyWordKey[i]] = keyWordValue[i];
    }
    for (int i = 0; i < operationKey.size(); i++)
    {
        operations[operationKey[i]] = operationValue[i];
    }
    for (int i = 0; i < symbolKey.size(); i++)
    {
        symbols[symbolKey[i]] = symbolValue[i];
    }
}
void lexAnalysing(string str)
{
    p = 0;
    char ch;
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ') + 1);
    for (; p < str.length(); p++)
    {
        ch = str.at(p);
        if (isdigit(ch))
        {
            digitCheck(str);
        }
        else if (isalpha(ch) || ch == '_')
        {
            letterCheck(str);
        }
        else if (ch == '"')
        {
            stringCheck(str);
        }
        else if (ch == '\'')
        {
            charCheck(str);
        }
        else if (ch == ' ')
        {
            continue;
        }
        else
        {
            symbolCheck(str);
        }
    }
}
void digitCheck(string str)
{

    string token;
    token.push_back(str.at(p++));
    // 判断数字的小数点是否有且是否大于1
    int flag = 0;
    bool err = false;
    char ch;
    for (; p < str.length(); p++)
    {
        ch = str.at(p);
        if (ch == ' ' || (!isalnum(ch) && ch != '.'))
        {
            // 遇到空格符,运算符或者界符
            break;
        }
        else if (err)
        {
            token += ch;
        }
        else
        {
            token += ch;
            if (ch == '.')
            {
                if (flag == 1)
                {
                    err = true;
                }
                else
                {
                    flag++;
                }
            }
            else if (isalpha(ch))
            {
                err = true;
            }
        }
    }
    if (token.at(token.length() - 1) == '.')
    {
        err = true;
    }
    Tokens.emplace_back("INTCON");
    vals.push_back(token);
    if (p != str.length() - 1 || (p == str.length() - 1 && !isdigit(str.at(p))))
    {
        p--;
    }
}
void letterCheck(string str)
{
    string token;
    token.push_back(str.at(p++));
    char ch;
    for (; p < str.length(); p++)
    {
        ch = str.at(p);
        if (!isalnum(ch) && ch != '_')
        {
            break;
        }
        else
        {
            token += ch;
        }
    }
    if (keywords.count(token))
    {
        Tokens.push_back(keywords[token]);
        vals.push_back(token);
    }
    else
    {
        Tokens.emplace_back("IDENFR");
        vals.push_back(token);
    }
    if (p != str.length() - 1 || (p == str.length() - 1 && (!isalnum(str.at(p)) && str.at(p) != '_')))
    {
        p--;
    }
}
void stringCheck(string str)
{

    string token;
    token.push_back(str.at(p++));
    char ch;
    for (; p < str.length(); p++)
    {
        ch = str.at(p);
        token += ch;
        if (ch == '"')
        {
            break;
        }
    }
    token.erase(remove(token.begin(), token.end(), '\"'), token.end());
    Tokens.emplace_back("STRCON");
    vals.emplace_back(token);
}
void charCheck(string str)
{

    string token;
    token.push_back(str.at(p++));
    char ch;
    for (; p < str.length(); p++)
    {
        ch = str.at(p);
        token += ch;
        if (ch == '\'')
        {
            break;
        }
    }
    token.erase(remove(token.begin(), token.end(), '\''), token.end());
    Tokens.emplace_back("CHARCON");
    vals.push_back(token);
}
void symbolCheck(string str)
{

    string token;
    token.push_back(str.at(p++));
    char ch;
    if (symbols.count(token))
    {
        Tokens.push_back(symbols[token]);
        vals.push_back(token);
        p--;
    }
    else
    {
        if (operations.count(token) || token == "!")
        {
            if (p < str.length())
            {
                ch = str.at(p);
                if (operations.count(token + ch))
                {
                    token += ch;
                    p++;
                    if (p < str.length())
                    {
                        ch = str.at(p);
                        if (operations.count(token + ch))
                        {
                            token += ch;
                            Tokens.push_back(operations[token]);
                            vals.push_back(token);
                        }
                        else
                        {
                            p--;
                            Tokens.push_back(operations[token]);
                            vals.push_back(token);
                        }
                    }
                    else
                    {
                        Tokens.push_back(operations[token]);
                        vals.push_back(token);
                    }
                }
                else
                {
                    p--;
                    Tokens.push_back(operations[token]);
                    vals.push_back(token);
                }
            }
        }
        else
        {
            p--;
        }
    }
}

void parsing()
{
    q = 0;
    parse_program(); // 从<程序>节点开始
}
void parse_str()
{
    // ＜字符串＞ ::=  "｛十进制编码为32,33,35-126的ASCII字符｝"
    MatchToken("STRCON");
    write << "<字符串>" << endl;
    cout << "<字符串>" << endl;
}
void parse_program()
{
    // ＜程序＞ ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
    if (Tokens[q] == "CONSTTK")
    {
        parse_const_describe(); // ＜常量说明＞
    }
    if ((Tokens[q] == "INTTK" || Tokens[q] == "CHARTK") && Tokens[q + 1] == "IDENFR" && Tokens[q + 2] != "LPARENT")
    {
        parse_var_describe(); // ＜变量说明＞
    }
    while ((Tokens[q] == "INTTK" || Tokens[q] == "CHARTK" || Tokens[q] == "VOIDTK") &&
           Tokens[q + 1] == "IDENFR" && Tokens[q + 2] == "LPARENT")
    {
        if (Tokens[q] == "VOIDTK")
        {
            parse_funcNonReturn(); // ＜无返回值函数定义＞
        }
        else
        {
            parse_funcReturn(); // ＜有返回值函数定义＞
        }
    }
    if (Tokens[q] == "VOIDTK" && Tokens[q + 1] == "MAINTK")
    {
        parse_mainFunc(); // ＜主函数＞
    }
    if (q == Tokens.size())
    {
        cout << "<程序>" << endl;
        write << "<程序>" << endl;
    }
}
void parse_const_describe()
{
    // ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
    do
    {
        MatchToken("CONSTTK");
        parse_const_def(); // ＜常量定义＞
        MatchToken("SEMICN");
    } while (Tokens[q] == "CONSTTK");
    cout << "<常量说明>" << endl;
    write << "<常量说明>" << endl;
}

void parse_const_def()
{
    // ＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}| char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
    if (Tokens[q] == "INTTK")
    {
        MatchToken("INTTK");
        MatchToken("IDENFR");
        MatchToken("ASSIGN");
        parse_Int(); // ＜整数＞
        while (Tokens[q] == "COMMA")
        {
            MatchToken("COMMA");
            MatchToken("IDENFR");
            MatchToken("ASSIGN");
            parse_Int();
        }
    }
    if (Tokens[q] == "CHARTK")
    {
        MatchToken("CHARTK");
        MatchToken("IDENFR");
        MatchToken("ASSIGN");
        MatchToken("CHARCON");
        while (Tokens[q] == "COMMA")
        {
            MatchToken("COMMA");
            MatchToken("IDENFR");
            MatchToken("ASSIGN");
            MatchToken("CHARCON");
        }
    }
    cout << ("<常量定义>") << endl;
    write << ("<常量定义>") << endl;
}

void parse_unsignedInt()
{
    // ＜无符号整数＞  ::= ＜非零数字＞｛＜数字＞｝| 0
    MatchToken("INTCON");
    cout << ("<无符号整数>") << endl;
    write << ("<无符号整数>") << endl;
}

void parse_Int()
{
    // ＜整数＞ ::= ［＋｜－］＜无符号整数＞
    if (Tokens[q] == "PLUS")
        MatchToken("PLUS");
    else if (Tokens[q] == "MINU")
        MatchToken("MINU");
    parse_unsignedInt(); //<无符号整数>
    cout << ("<整数>") << endl;
    write << ("<整数>") << endl;
}

void parse_defHead()
{
    // ＜声明头部＞ ::=  int＜标识符＞ |char＜标识符＞
    if (Tokens[q] == "INTTK")
        MatchToken("INTTK");
    else if (Tokens[q] == "CHARTK")
        MatchToken("CHARTK");
    MatchToken("IDENFR");
    cout << ("<声明头部>") << endl;
    write << ("<声明头部>") << endl;
}

void parse_var_describe()
{
    // ＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;}
    do
    {
        parse_var_def(); // ＜变量定义＞
        MatchToken("SEMICN");
    } while ((Tokens[q] == "INTTK" || Tokens[q] == "CHARTK") &&
             Tokens[q + 1] == "IDENFR" && Tokens[q + 2] != "LPARENT");
    cout << ("<变量说明>") << endl;
    write << ("<变量说明>") << endl;
}
void parse_var_def()
{
    // ＜变量定义＞ ::= ＜类型标识符＞(＜标识符＞|＜标识符＞'['＜无符号整数＞']'){,(＜标识符＞|＜标识符＞'['＜无符号整数＞']')}
    if (Tokens[q] == "INTTK")
        MatchToken("INTTK");
    else if (Tokens[q] == "CHARTK")
        MatchToken("CHARTK");
    MatchToken("IDENFR");
    if (Tokens[q] == "LBRACK")
    {
        MatchToken("LBRACK");
        parse_unsignedInt(); //<无符号整数>
        MatchToken("RBRACK");
    }
    while (Tokens[q] == "COMMA")
    {
        MatchToken("COMMA");
        MatchToken("IDENFR");
        if (Tokens[q] == "LBRACK")
        {
            MatchToken("LBRACK");
            parse_unsignedInt(); //<无符号整数>
            MatchToken("RBRACK");
        }
    }
    cout << ("<变量定义>") << endl;
    write << ("<变量定义>") << endl;
}
void parse_funcReturn()
{
    // ＜有返回值函数定义＞ ::=  ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
    parse_defHead(); // ＜声明头部＞
    NVoidFunction.insert({vals[q - 1], 1});
    MatchToken("LPARENT");
    parse_paraForm(); // ＜参数表＞
    MatchToken("RPARENT");
    MatchToken("LBRACE");
    parse_comStatement(); // ＜复合语句＞
    MatchToken("RBRACE");
    cout << ("<有返回值函数定义>") << endl;
    write << ("<有返回值函数定义>") << endl;
}
void parse_funcNonReturn()
{
    // ＜无返回值函数定义＞ ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
    MatchToken("VOIDTK");
    MatchToken("IDENFR");
    MatchToken("LPARENT");
    parse_paraForm(); // ＜参数表＞
    MatchToken("RPARENT");
    MatchToken("LBRACE");
    parse_comStatement(); // ＜复合语句＞
    MatchToken("RBRACE");
    cout << ("<无返回值函数定义>") << endl;
    write << ("<无返回值函数定义>") << endl;
}
void parse_comStatement()
{
    // ＜复合语句＞ ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
    if (Tokens[q] == "CONSTTK")
    {
        parse_const_describe(); // ＜常量说明＞
    }
    if ((Tokens[q] == "INTTK" || Tokens[q] == "CHARTK") &&
        Tokens[q + 1] == "IDENFR" && Tokens[q + 2] != "LPARENT")
    {
        parse_var_describe(); // ＜变量说明＞
    }
    parse26(); // ＜语句列＞
    cout << ("<复合语句>") << endl;
    write << ("<复合语句>") << endl;
}
void parse_paraForm()
{
    // ＜参数表＞ ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
    if (Tokens[q] != "RPARENT") // 如果下一个token为右小括号，则为空
    {
        if (Tokens[q] == "INTTK")
            MatchToken("INTTK");
        else if (Tokens[q] == "CHARTK")
            MatchToken("CHARTK");
        MatchToken("IDENFR");
        while (Tokens[q] == "COMMA")
        {
            MatchToken("COMMA");
            if (Tokens[q] == "INTTK")
                MatchToken("INTTK");
            else if (Tokens[q] == "CHARTK")
                MatchToken("CHARTK");
            MatchToken("IDENFR");
        }
    }
    cout << ("<参数表>") << endl;
    write << ("<参数表>") << endl;
}

void parse_mainFunc()
{
    // ＜主函数＞ ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
    MatchToken("VOIDTK");
    MatchToken("MAINTK");
    MatchToken("LPARENT");
    MatchToken("RPARENT");
    MatchToken("LBRACE");
    parse_comStatement(); // ＜复合语句＞
    MatchToken("RBRACE");
    cout << ("<主函数>") << endl;
    write << ("<主函数>") << endl;
}

void parse_expression()
{
    // ＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
    if (Tokens[q] == "PLUS")
        MatchToken("PLUS");
    else if (Tokens[q] == "MINU")
        MatchToken("MINU");
    parse_item(); // ＜项＞
    while (Tokens[q] == "PLUS" || Tokens[q] == "MINU")
    {
        if (Tokens[q] == "PLUS")
            MatchToken("PLUS");
        else if (Tokens[q] == "MINU")
            MatchToken("MINU");
        parse_item(); // ＜项＞
    }
    cout << ("<表达式>") << endl;
    write << ("<表达式>") << endl;
}

void parse_item()
{
    // ＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
    parse_factor(); // ＜因子＞
    while (Tokens[q] == "MULT" || Tokens[q] == "DIV")
    {
        if (Tokens[q] == "MULT")
            MatchToken("MULT");
        else if (Tokens[q] == "DIV")
            MatchToken("DIV");
        parse_factor(); // ＜因子＞
    }
    cout << ("<项>") << endl;
    write << ("<项>") << endl;
}

void parse_factor()
{
    // ＜因子＞ ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']'|'('＜表达式＞')'｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞
    if (Tokens[q] == "IDENFR")
    {
        if (Tokens[q + 1] == "LBRACK")
        {
            MatchToken("IDENFR");
            MatchToken("LBRACK");
            parse_expression(); // ＜表达式＞
            MatchToken("RBRACK");
        }
        else if (Tokens[q + 1] == "LPARENT")
            parse_callFunc(); // ＜有返回值函数调用语句＞
        else
            MatchToken("IDENFR");
    }
    else if (Tokens[q] == "LPARENT")
    {
        MatchToken("LPARENT");
        parse_expression(); // ＜表达式＞
        MatchToken("RPARENT");
    }
    else if (Tokens[q] == "INTCON" || Tokens[q] == "PLUS" || Tokens[q] == "MINU")
        parse_Int(); // ＜整数＞
    else if (Tokens[q] == "CHARCON")
        MatchToken("CHARCON");
    cout << ("<因子>") << endl;
    write << ("<因子>") << endl;
}

void parse_sentence()
{
    /*
    ＜语句＞ ::= ＜条件语句＞｜＜循环语句＞| '{'＜语句列＞'}'
        | ＜有返回值函数调用语句＞; |＜无返回值函数调用语句＞;｜＜赋值语句＞;
        ｜＜读语句＞;｜＜写语句＞;｜＜空＞;|＜返回语句＞;
    */
    if (Tokens[q] == "IFTK")
        parse_condition_sentence(); // ＜条件语句＞
    else if (Tokens[q] == "WHILETK" || Tokens[q] == "DOTK" || Tokens[q] == "FORTK")
        parse21_circulate(); // ＜循环语句＞
    else if (Tokens[q] == "LBRACE")
    {
        MatchToken("LBRACE");
        parse26(); // ＜语句列＞
        MatchToken("RBRACE");
    }
    else if (Tokens[q] == "IDENFR")
    {
        if (Tokens[q + 1] == "LPARENT")
        {
            parse_callFunc(); // ＜有无返回值函数调用语句＞
            MatchToken("SEMICN");
        }
        else
        {
            parse_assign_sentence(); // ＜赋值语句＞
            MatchToken("SEMICN");
        }
    }
    else if (Tokens[q] == "SCANFTK")
    {
        parse27(); // ＜读语句＞
        MatchToken("SEMICN");
    }
    else if (Tokens[q] == "PRINTFTK")
    {
        parse28(); // ＜写语句＞
        MatchToken("SEMICN");
    }
    else if (Tokens[q] == "RETURNTK")
    {
        parse29(); // ＜返回语句＞
        MatchToken("SEMICN");
    }
    else if (Tokens[q] == "SEMICN")
        MatchToken("SEMICN");
    cout << ("<语句>") << endl;
    write << ("<语句>") << endl;
}

void parse_assign_sentence()
{
    // ＜赋值语句＞ ::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞
    MatchToken("IDENFR");
    if (Tokens[q] == "LBRACK")
    {
        MatchToken("LBRACK");
        parse_expression(); // ＜表达式＞
        MatchToken("RBRACK");
    }
    MatchToken("ASSIGN");
    parse_expression(); // ＜表达式＞
    cout << ("<赋值语句>") << endl;
    write << ("<赋值语句>") << endl;
}

void parse_condition_sentence()
{
    // ＜条件语句＞ ::= if '('＜条件＞')'＜语句＞［else＜语句＞］
    MatchToken("IFTK");
    MatchToken("LPARENT");
    parse_condition(); // ＜条件＞
    MatchToken("RPARENT");
    parse_sentence(); // ＜语句＞
    if (Tokens[q] == "ELSETK")
    {
        MatchToken("ELSETK");
        parse_sentence(); // ＜语句＞
    }
    cout << ("<条件语句>") << endl;
    write << ("<条件语句>") << endl;
}

void parse_condition()
{
    // ＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞ //整型表达式之间才能进行关系运算｜＜表达式＞
    //  表达式为整型，其值为0条件为假，值不为0时条件为真
    parse_expression(); // ＜表达式＞
    if (Tokens[q] == "LSS" || Tokens[q] == "LEQ" || Tokens[q] == "GRE" || Tokens[q] == "GEQ" || Tokens[q] == "EQL" || Tokens[q] == "NEQ")
    {
        MatchToken((string)Tokens[q]);
        parse_expression(); // ＜表达式＞
    }
    cout << ("<条件>") << endl;
    write << ("<条件>") << endl;
}

void parse21_circulate()
{
    // ＜循环语句＞ ::=  while '('＜条件＞')'＜语句＞
    //           |do＜语句＞while '('＜条件＞')'
    //           |for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞
    if (Tokens[q] == "WHILETK")
    {
        MatchToken("WHILETK");
        MatchToken("LPARENT");
        parse_condition(); // ＜条件＞
        MatchToken("RPARENT");
        parse_sentence(); // ＜语句＞
    }
    else if (Tokens[q] == "DOTK")
    {
        MatchToken("DOTK");
        parse_sentence(); // ＜语句＞
        MatchToken("WHILETK");
        MatchToken("LPARENT");
        parse_condition(); // ＜条件＞
        MatchToken("RPARENT");
    }
    else if (Tokens[q] == "FORTK")
    {
        MatchToken("FORTK");
        MatchToken("LPARENT");
        MatchToken("IDENFR");
        MatchToken("ASSIGN");
        parse_expression(); // ＜表达式＞
        MatchToken("SEMICN");
        parse_condition(); // ＜条件＞
        MatchToken("SEMICN");
        MatchToken("IDENFR");
        MatchToken("ASSIGN");
        MatchToken("IDENFR");
        if (Tokens[q] == "PLUS")
            MatchToken("PLUS");
        else if (Tokens[q] == "MINU")
            MatchToken("MINU");
        parse_step(); // ＜步长＞
        MatchToken("RPARENT");
        parse_sentence(); // ＜语句＞
    }
    cout << ("<循环语句>") << endl;
    write << ("<循环语句>") << endl;
}

void parse_step()
{
    // ＜步长＞::= ＜无符号整数＞
    parse_unsignedInt(); // ＜无符号整数＞
    cout << ("<步长>") << endl;
    write << ("<步长>") << endl;
}

void parse_callFunc()
{
    // ＜有无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
    int FunctionType = 0;
    if (NVoidFunction.count(vals[q]))
    {
        FunctionType = NVoidFunction[vals[q]];
    }
    MatchToken("IDENFR");
    MatchToken("LPARENT");
    parse25(); // ＜值参数表＞
    MatchToken("RPARENT");
    if (FunctionType == 1)
    {
        cout << ("<有返回值函数调用语句>") << endl;
        write << ("<有返回值函数调用语句>") << endl;
    }
    else
    {
        cout << ("<无返回值函数调用语句>") << endl;
        write << ("<无返回值函数调用语句>") << endl;
    }
}

void parse25()
{
    // ＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞
    if (Tokens[q] != "RPARENT")
    {
        parse_expression(); // ＜表达式＞
        while (Tokens[q] == "COMMA")
        {
            MatchToken("COMMA");
            parse_expression();
        }
    }
    cout << ("<值参数表>") << endl;
    write << ("<值参数表>") << endl;
}

void parse26()
{
    // ＜语句列＞ ::= ｛＜语句＞｝
    while (Tokens[q] != "RBRACE")
        parse_sentence(); // ＜语句＞
    cout << ("<语句列>") << endl;
    write << ("<语句列>") << endl;
}

void parse27()
{
    // ＜读语句＞ ::=  scanf '('＜标识符＞{,＜标识符＞}')'
    MatchToken("SCANFTK");
    MatchToken("LPARENT");
    MatchToken("IDENFR");
    while (Tokens[q] == "COMMA")
    {
        MatchToken("COMMA");
        MatchToken("IDENFR");
    }
    MatchToken("RPARENT");
    cout << ("<读语句>") << endl;
    write << ("<读语句>") << endl;
}

void parse28()
{
    // ＜写语句＞ ::= printf '(' ＜字符串＞,＜表达式＞ ')'
    //           | printf '('＜字符串＞ ')'
    //           | printf '('＜表达式＞')'
    MatchToken("PRINTFTK");
    MatchToken("LPARENT");
    if (Tokens[q] == "STRCON")
        parse_str(); // ＜字符串＞
    else
        parse_expression(); // ＜表达式＞
    if (Tokens[q] == "COMMA")
    {
        MatchToken("COMMA");
        parse_expression(); // ＜表达式＞
    }
    MatchToken("RPARENT");
    cout << ("<写语句>") << endl;
    write << ("<写语句>") << endl;
}

void parse29()
{
    // ＜返回语句＞ ::=  return['('＜表达式＞')']
    MatchToken("RETURNTK");
    if (Tokens[q] == "LPARENT")
    {
        MatchToken("LPARENT");
        parse_expression(); // ＜表达式＞
        MatchToken("RPARENT");
    }
    cout << ("<返回语句>") << endl;
    write << ("<返回语句>") << endl;
}

void MatchToken(string expected)
{
    if (Tokens[q] == expected)
    {
        // cout << Tokens[q] << " " << vals[q] << endl;
        write << Tokens[q] << " " << vals[q] << endl;
        q++; // 下一个单词
    }
    else
    {
        cout << "syntax error!" << endl;
        exit(0);
    }
}