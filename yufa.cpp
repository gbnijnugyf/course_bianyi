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
static unsigned int p, lines;

// 语法分析当前单词位置
static unsigned int q;

ofstream write("output.txt");
ifstream read("testfile.txt");
void cifaAnalysing(string str);
void init();
void numbersCheck(string str);
void wordCheck(string str);
void stringCheck(string str);
void charCheck(string str);
void symbolCheck(string str);
void parse_begin();
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
void parse_circulate();
void parse_step();
void parse_callFunc();
void parse_value_paraFrom();
void parse_sentenceCol();
void parse_scanf();
void parse_printf();
void parse_return();
void token_match(string expected);

void init()
{
    for (unsigned int i = 0; i < keyWordKey.size(); i++)
    {
        keywords[keyWordKey[i]] = keyWordValue[i];
    }
    for (unsigned int i = 0; i < operationKey.size(); i++)
    {
        operations[operationKey[i]] = operationValue[i];
    }
    for (unsigned int i = 0; i < symbolKey.size(); i++)
    {
        symbols[symbolKey[i]] = symbolValue[i];
    }
}
void cifaAnalysing(string str)
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
            numbersCheck(str);
        }
        else if (isalpha(ch) || ch == '_')
        {
            wordCheck(str);
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
void numbersCheck(string str)
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
void wordCheck(string str)
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

void parse_begin()
{
    q = 0;
    parse_program(); // 从<程序>节点开始
}
void parse_str()
{
    // ＜字符串＞ ::=  "｛十进制编码为32,33,35-126的ASCII字符｝"
    token_match("STRCON");
    write << "<字符串>" << endl;
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
        write << "<程序>" << endl;
    }
}
void parse_const_describe()
{
    // ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
    do
    {
        token_match("CONSTTK");
        parse_const_def(); // ＜常量定义＞
        token_match("SEMICN");
    } while (Tokens[q] == "CONSTTK");
    write << "<常量说明>" << endl;
}

void parse_const_def()
{
    // ＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}| char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
    if (Tokens[q] == "INTTK")
    {
        token_match("INTTK");
        token_match("IDENFR");
        token_match("ASSIGN");
        parse_Int(); // ＜整数＞
        while (Tokens[q] == "COMMA")
        {
            token_match("COMMA");
            token_match("IDENFR");
            token_match("ASSIGN");
            parse_Int();
        }
    }
    if (Tokens[q] == "CHARTK")
    {
        token_match("CHARTK");
        token_match("IDENFR");
        token_match("ASSIGN");
        token_match("CHARCON");
        while (Tokens[q] == "COMMA")
        {
            token_match("COMMA");
            token_match("IDENFR");
            token_match("ASSIGN");
            token_match("CHARCON");
        }
    }
    write << ("<常量定义>") << endl;
}

void parse_unsignedInt()
{
    // ＜无符号整数＞  ::= ＜非零数字＞｛＜数字＞｝| 0
    token_match("INTCON");
    write << ("<无符号整数>") << endl;
}

void parse_Int()
{
    // ＜整数＞ ::= ［＋｜－］＜无符号整数＞
    if (Tokens[q] == "PLUS")
        token_match("PLUS");
    else if (Tokens[q] == "MINU")
        token_match("MINU");
    parse_unsignedInt(); //<无符号整数>
    write << ("<整数>") << endl;
}

void parse_defHead()
{
    // ＜声明头部＞ ::=  int＜标识符＞ |char＜标识符＞
    if (Tokens[q] == "INTTK")
        token_match("INTTK");
    else if (Tokens[q] == "CHARTK")
        token_match("CHARTK");
    token_match("IDENFR");
    write << ("<声明头部>") << endl;
}

void parse_var_describe()
{
    // ＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;}
    do
    {
        parse_var_def(); // ＜变量定义＞
        token_match("SEMICN");
    } while ((Tokens[q] == "INTTK" || Tokens[q] == "CHARTK") &&
             Tokens[q + 1] == "IDENFR" && Tokens[q + 2] != "LPARENT");
    write << ("<变量说明>") << endl;
}
void parse_var_def()
{
    // ＜变量定义＞ ::= ＜类型标识符＞(＜标识符＞|＜标识符＞'['＜无符号整数＞']'){,(＜标识符＞|＜标识符＞'['＜无符号整数＞']')}
    if (Tokens[q] == "INTTK")
        token_match("INTTK");
    else if (Tokens[q] == "CHARTK")
        token_match("CHARTK");
    token_match("IDENFR");
    if (Tokens[q] == "LBRACK")
    {
        token_match("LBRACK");
        parse_unsignedInt(); //<无符号整数>
        token_match("RBRACK");
    }
    while (Tokens[q] == "COMMA")
    {
        token_match("COMMA");
        token_match("IDENFR");
        if (Tokens[q] == "LBRACK")
        {
            token_match("LBRACK");
            parse_unsignedInt(); //<无符号整数>
            token_match("RBRACK");
        }
    }
    write << ("<变量定义>") << endl;
}
void parse_funcReturn()
{
    // ＜有返回值函数定义＞ ::=  ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
    parse_defHead(); // ＜声明头部＞
    NVoidFunction.insert({vals[q - 1], 1});
    token_match("LPARENT");
    parse_paraForm(); // ＜参数表＞
    token_match("RPARENT");
    token_match("LBRACE");
    parse_comStatement(); // ＜复合语句＞
    token_match("RBRACE");
    write << ("<有返回值函数定义>") << endl;
}
void parse_funcNonReturn()
{
    // ＜无返回值函数定义＞ ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
    token_match("VOIDTK");
    token_match("IDENFR");
    token_match("LPARENT");
    parse_paraForm(); // ＜参数表＞
    token_match("RPARENT");
    token_match("LBRACE");
    parse_comStatement(); // ＜复合语句＞
    token_match("RBRACE");
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
    parse_sentenceCol(); // ＜语句列＞
    write << ("<复合语句>") << endl;
}
void parse_paraForm()
{
    // ＜参数表＞ ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
    if (Tokens[q] != "RPARENT") // 如果下一个token为右小括号，则为空
    {
        if (Tokens[q] == "INTTK")
            token_match("INTTK");
        else if (Tokens[q] == "CHARTK")
            token_match("CHARTK");
        token_match("IDENFR");
        while (Tokens[q] == "COMMA")
        {
            token_match("COMMA");
            if (Tokens[q] == "INTTK")
                token_match("INTTK");
            else if (Tokens[q] == "CHARTK")
                token_match("CHARTK");
            token_match("IDENFR");
        }
    }
    write << ("<参数表>") << endl;
}

void parse_mainFunc()
{
    // ＜主函数＞ ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
    token_match("VOIDTK");
    token_match("MAINTK");
    token_match("LPARENT");
    token_match("RPARENT");
    token_match("LBRACE");
    parse_comStatement(); // ＜复合语句＞
    token_match("RBRACE");
    write << ("<主函数>") << endl;
}

void parse_expression()
{
    // ＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
    if (Tokens[q] == "PLUS")
        token_match("PLUS");
    else if (Tokens[q] == "MINU")
        token_match("MINU");
    parse_item(); // ＜项＞
    while (Tokens[q] == "PLUS" || Tokens[q] == "MINU")
    {
        if (Tokens[q] == "PLUS")
            token_match("PLUS");
        else if (Tokens[q] == "MINU")
            token_match("MINU");
        parse_item(); // ＜项＞
    }
    write << ("<表达式>") << endl;
}

void parse_item()
{
    // ＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
    parse_factor(); // ＜因子＞
    while (Tokens[q] == "MULT" || Tokens[q] == "DIV")
    {
        if (Tokens[q] == "MULT")
            token_match("MULT");
        else if (Tokens[q] == "DIV")
            token_match("DIV");
        parse_factor(); // ＜因子＞
    }
    write << ("<项>") << endl;
}

void parse_factor()
{
    // ＜因子＞ ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']'|'('＜表达式＞')'｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞
    if (Tokens[q] == "IDENFR")
    {
        if (Tokens[q + 1] == "LBRACK")
        {
            token_match("IDENFR");
            token_match("LBRACK");
            parse_expression(); // ＜表达式＞
            token_match("RBRACK");
        }
        else if (Tokens[q + 1] == "LPARENT")
            parse_callFunc(); // ＜有返回值函数调用语句＞
        else
            token_match("IDENFR");
    }
    else if (Tokens[q] == "LPARENT")
    {
        token_match("LPARENT");
        parse_expression(); // ＜表达式＞
        token_match("RPARENT");
    }
    else if (Tokens[q] == "INTCON" || Tokens[q] == "PLUS" || Tokens[q] == "MINU")
        parse_Int(); // ＜整数＞
    else if (Tokens[q] == "CHARCON")
        token_match("CHARCON");
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
        parse_circulate(); // ＜循环语句＞
    else if (Tokens[q] == "LBRACE")
    {
        token_match("LBRACE");
        parse_sentenceCol(); // ＜语句列＞
        token_match("RBRACE");
    }
    else if (Tokens[q] == "IDENFR")
    {
        if (Tokens[q + 1] == "LPARENT")
        {
            parse_callFunc(); // ＜有无返回值函数调用语句＞
            token_match("SEMICN");
        }
        else
        {
            parse_assign_sentence(); // ＜赋值语句＞
            token_match("SEMICN");
        }
    }
    else if (Tokens[q] == "SCANFTK")
    {
        parse_scanf(); // ＜读语句＞
        token_match("SEMICN");
    }
    else if (Tokens[q] == "PRINTFTK")
    {
        parse_printf(); // ＜写语句＞
        token_match("SEMICN");
    }
    else if (Tokens[q] == "RETURNTK")
    {
        parse_return(); // ＜返回语句＞
        token_match("SEMICN");
    }
    else if (Tokens[q] == "SEMICN")
        token_match("SEMICN");
    write << ("<语句>") << endl;
}

void parse_assign_sentence()
{
    // ＜赋值语句＞ ::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞
    token_match("IDENFR");
    if (Tokens[q] == "LBRACK")
    {
        token_match("LBRACK");
        parse_expression(); // ＜表达式＞
        token_match("RBRACK");
    }
    token_match("ASSIGN");
    parse_expression(); // ＜表达式＞
    write << ("<赋值语句>") << endl;
}

void parse_condition_sentence()
{
    // ＜条件语句＞ ::= if '('＜条件＞')'＜语句＞［else＜语句＞］
    token_match("IFTK");
    token_match("LPARENT");
    parse_condition(); // ＜条件＞
    token_match("RPARENT");
    parse_sentence(); // ＜语句＞
    if (Tokens[q] == "ELSETK")
    {
        token_match("ELSETK");
        parse_sentence(); // ＜语句＞
    }
    write << ("<条件语句>") << endl;
}

void parse_condition()
{
    // ＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞ //整型表达式之间才能进行关系运算｜＜表达式＞
    //  表达式为整型，其值为0条件为假，值不为0时条件为真
    parse_expression(); // ＜表达式＞
    if (Tokens[q] == "LSS" || Tokens[q] == "LEQ" || Tokens[q] == "GRE" || Tokens[q] == "GEQ" || Tokens[q] == "EQL" || Tokens[q] == "NEQ")
    {
        token_match((string)Tokens[q]);
        parse_expression(); // ＜表达式＞
    }
    write << ("<条件>") << endl;
}

void parse_circulate()
{
    // ＜循环语句＞ ::=  while '('＜条件＞')'＜语句＞
    //           |do＜语句＞while '('＜条件＞')'
    //           |for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞
    if (Tokens[q] == "WHILETK")
    {
        token_match("WHILETK");
        token_match("LPARENT");
        parse_condition(); // ＜条件＞
        token_match("RPARENT");
        parse_sentence(); // ＜语句＞
    }
    else if (Tokens[q] == "DOTK")
    {
        token_match("DOTK");
        parse_sentence(); // ＜语句＞
        token_match("WHILETK");
        token_match("LPARENT");
        parse_condition(); // ＜条件＞
        token_match("RPARENT");
    }
    else if (Tokens[q] == "FORTK")
    {
        token_match("FORTK");
        token_match("LPARENT");
        token_match("IDENFR");
        token_match("ASSIGN");
        parse_expression(); // ＜表达式＞
        token_match("SEMICN");
        parse_condition(); // ＜条件＞
        token_match("SEMICN");
        token_match("IDENFR");
        token_match("ASSIGN");
        token_match("IDENFR");
        if (Tokens[q] == "PLUS")
            token_match("PLUS");
        else if (Tokens[q] == "MINU")
            token_match("MINU");
        parse_step(); // ＜步长＞
        token_match("RPARENT");
        parse_sentence(); // ＜语句＞
    }
    write << ("<循环语句>") << endl;
}

void parse_step()
{
    // ＜步长＞::= ＜无符号整数＞
    parse_unsignedInt(); // ＜无符号整数＞
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
    token_match("IDENFR");
    token_match("LPARENT");
    parse_value_paraFrom(); // ＜值参数表＞
    token_match("RPARENT");
    if (FunctionType == 1)
    {
        write << ("<有返回值函数调用语句>") << endl;
    }
    else
    {
        write << ("<无返回值函数调用语句>") << endl;
    }
}

void parse_value_paraFrom()
{
    // ＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞
    if (Tokens[q] != "RPARENT")
    {
        parse_expression(); // ＜表达式＞
        while (Tokens[q] == "COMMA")
        {
            token_match("COMMA");
            parse_expression();
        }
    }
    write << ("<值参数表>") << endl;
}

void parse_sentenceCol()
{
    // ＜语句列＞ ::= ｛＜语句＞｝
    while (Tokens[q] != "RBRACE")
        parse_sentence(); // ＜语句＞
    write << ("<语句列>") << endl;
}

void parse_scanf()
{
    // ＜读语句＞ ::=  scanf '('＜标识符＞{,＜标识符＞}')'
    token_match("SCANFTK");
    token_match("LPARENT");
    token_match("IDENFR");
    while (Tokens[q] == "COMMA")
    {
        token_match("COMMA");
        token_match("IDENFR");
    }
    token_match("RPARENT");
    write << ("<读语句>") << endl;
}

void parse_printf()
{
    // ＜写语句＞ ::= printf '(' ＜字符串＞,＜表达式＞ ')'
    //           | printf '('＜字符串＞ ')'
    //           | printf '('＜表达式＞')'
    token_match("PRINTFTK");
    token_match("LPARENT");
    if (Tokens[q] == "STRCON")
        parse_str(); // ＜字符串＞
    else
        parse_expression(); // ＜表达式＞
    if (Tokens[q] == "COMMA")
    {
        token_match("COMMA");
        parse_expression(); // ＜表达式＞
    }
    token_match("RPARENT");
    write << ("<写语句>") << endl;
}

void parse_return()
{
    // ＜返回语句＞ ::=  return['('＜表达式＞')']
    token_match("RETURNTK");
    if (Tokens[q] == "LPARENT")
    {
        token_match("LPARENT");
        parse_expression(); // ＜表达式＞
        token_match("RPARENT");
    }
    write << ("<返回语句>") << endl;
}

void token_match(string expected)
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

int main()
{
    init();
    lines = 1;
    string line;
    while (getline(read, line))
    {
        cifaAnalysing(line);
        lines++;
    }
    parse_begin();
    write.close();
    read.close();
    return 0;
}