// C语言词法分析器
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <memory.h>
#include <vector>
using namespace std;


/* 不要修改这个标准输入函数 */
void read_prog(string& prog)
{
    char c;
    while(scanf("%c",&c)!=EOF){
        prog += c;
    }
}
/* 你可以添加其他函数 */



string keywords[32]= // 32个关键字
{ 
	"auto", "break", "case", "char", "const",
	"continue", "default", "do", "double", "else",
	"enum", "extern", "float", "for", "goto",
	"if", "int", "long", "register", "return",
	"short", "signed", "sizeof", "static", "struct",
	"switch", "typedef", "union", "unsigned", "void",
	"volatile", "while"
};

struct Unit // 词法分析器中单词
{
	int key; // 对应的编号
	string value; // 实际的值
	Unit * next;
};

int analy(Unit *u, string prog, int tokenCount)
{
	char token;
	int key=0, i, j;
	string value;
    while(tokenCount<=prog.length())
	{
		token = prog[tokenCount++]; // 读取字符
        //cout << "token:" << token << endl;
        //cout << "tokenCount:" << tokenCount << endl;

		if(token=='\n' || token==' ') 
		{ // 处理换行符和空格
			if(prog[tokenCount]==' ') tokenCount++;
		}

		else if((token>='a'&&token<='z')||(token>='A'&&token<='Z')||token=='_')
		{ // 处理标识符
			i=0;
			value+=token;
            token = prog[tokenCount++];
			while((token>='a'&&token<='z')||(token>='A'&&token<='Z')||token=='_'||(token>='0'&&token<='9'))
			{ // 标识符非首字母可以为数字
                //cout << "token2:" << token << endl;
                //cout << "tokenCount2:" << tokenCount << endl;
				value+=token;
                token=prog[tokenCount++];
			}
            tokenCount--;

			for(j=0;j<32;j++)
			{ 	// 匹配已有关键字
				if(value.compare(keywords[j])==0)
				{
                    //cout << "why:" << j << "-" << keywords[j] << endl;
					key=j+1;
					break;
				}
			}
			if(key==0) key=81; // 标识符
			break;
		}

		else if(token>='0'&&token<='9')
		{ // 处理数字
			value+=token;
			for(i=1; prog[tokenCount]>='0'&&prog[tokenCount]<='9'; i++)
			{
				value+=prog[tokenCount++];
			}
			key=80;
			break;	
		}

		else if(token=='-')
		{
			if(prog[tokenCount]=='-')
			{ // --
				tokenCount++;
				value="--";
				key=34;
				break;
			}

			else if(prog[tokenCount]=='=')
			{ // -=
				tokenCount++;
				value="-=";
				key=35;
				break;
			}

			else if(prog[tokenCount]=='>')
			{ // -=
				tokenCount++;
				value="->";
				key=36;
				break;
			}

			else
			{ // -
				value="-";
				key=33;
				break;
			}
			
		}

		else if(token=='!')
		{
			if(prog[tokenCount]=='=')
			{ 
				tokenCount++;
				value="!=";
				key=38;
				break;
			}

			else
			{ 
				value="!";
				key=37;
				break;
			}
			
		}

		else if(token=='%')
		{
			if(prog[tokenCount]=='=')
			{ 
				tokenCount++;
				value="%=";
				key=40;
				break;
			}
            
            else if(prog[tokenCount]=='d')
            {
                tokenCount++;
                value="%d";
                key=81;
                break;
            }
            
            else if(prog[tokenCount]=='u')
            {
                tokenCount++;
                value="%u";
                key=81;
                break;
            }
            
            else if(prog[tokenCount]=='f')
            {
                tokenCount++;
                value="%f";
                key=81;
                break;
            }
            
            else if(prog[tokenCount]=='s')
            {
                tokenCount++;
                value="%s";
                key=81;
                break;
            }
            
            else if(prog[tokenCount]=='c')
            {
                tokenCount++;
                value="%c";
                key=81;
                break;
            }
            
            else if(prog[tokenCount]=='p')
            {
                tokenCount++;
                value="%p";
                key=81;
                break;
            }
            
            else if(prog[tokenCount]=='e')
            {
                tokenCount++;
                value="%e";
                key=81;
                break;
            }
            
            else if(prog[tokenCount]=='x')
            {
                tokenCount++;
                value="%x";
                key=81;
                break;
            }
            
            else if(prog[tokenCount]=='g')
            {
                tokenCount++;
                value="%g";
                key=81;
                break;
            }
            
            else if(prog[tokenCount]=='0')
            {
                tokenCount++;
                value="%0";
                key=81;
                break;
            }

			else
			{ 
				value="%";
				key=39;
				break;
			}
			
		}

		else if(token=='&')
		{
			if(prog[tokenCount]=='&')
			{ 
				tokenCount++;
				value="&&";
				key=42;
				break;
			}

			else if(prog[tokenCount]=='=')
			{ 
				tokenCount++;
				value="&=";
				key=43;
				break;
			}

			else
			{ 
				value="&";
				key=41;
				break;
			}
			
		}

		else if(token=='(')
		{ 
			value="(";
			key=44;
			break;
		}

		else if(token==')')
		{ 
			value=")";
			key=45;
			break;
		}

		else if(token=='*')
		{
			if(prog[tokenCount]=='=')
			{ 
				tokenCount++;
				value="*=";
				key=47;
				break;
			}

			else
			{ 
				value="*";
				key=46;
				break;
			}
			
		}

		else if(token==',')
		{ 
			value=",";
			key=48;
			break;
		}

		else if(token=='.')
		{ 
			value=".";
			key=49;
			break;
		}

		else if(token=='/')
		{
			if(prog[tokenCount]=='/')
			{ // //注释
				value+='/';
				for(i=1;prog[tokenCount]!=0&&prog[tokenCount]!='\n';i++)
				{
					value+=prog[tokenCount++];
				}
                if(!value.empty())
                {
                    value.erase(value.find_last_not_of(" ") + 1);
                }
            
				key=79;
				break;
			}

			else if(prog[tokenCount]=='*')
			{ // /* 注释
				value+="/*";
				tokenCount++;
				i=2;
            
				while(!(prog[tokenCount]=='*' && prog[tokenCount+1]=='/'))
				{
					value+=prog[tokenCount++];
				}
				tokenCount+=2;
				value+="*/";
				key=79;
				break;
			}

			else if(prog[tokenCount]=='=')
			{ // /=
				tokenCount++;
				value="/=";
				key=51;
				break;
			}

			else
			{ // /
				value="/";
				key=50;
				break;
			}
		}

		else if(token==':')
		{ 
			value=":";
			key=52;
			break;
		}

		else if(token==';')
		{ 
			value=";";
			key=53;
			break;
		}

		else if(token=='?')
		{ 
			value="?";
			key=54;
			break;
		}

		else if(token=='[')
		{ 
			value="[";
			key=55;
			break;
		}

		else if(token==']')
		{ 
			value="]";
			key=56;
			break;
		}

		else if(token=='^')
		{
			if(prog[tokenCount]=='=')
			{ 
				tokenCount++;
				value="^=";
				key=58;
				break;
			}

			else
			{ 
				value="^";
				key=57;
				break;
			}
			
		}

		else if(token=='{')
		{ 
			value="{";
			key=59;
			break;
		}

		else if(token=='|')
		{
			if(prog[tokenCount]=='|')
			{ 
				tokenCount++;
				value="||";
				key=61;
				break;
			}

			else if(prog[tokenCount]=='=')
			{ 
				tokenCount++;
				value="|=";
				key=62;
				break;
			}

			else
			{ 
				value="|";
				key=60;
				break;
			}
		}

		else if(token=='}')
		{ 
			value="}";
			key=63;
			break;
		}

		else if(token=='~')
		{ 
			value="~";
			key=64;
			break;
		}

		else if(token=='+')
		{
			if(prog[tokenCount]=='+')
			{ 
				tokenCount++;
				value="++";
				key=66;
				break;
			}

			else if(prog[tokenCount]=='=')
			{ 
				tokenCount++;
				value="+=";
				key=67;
				break;
			}

			else
			{ 
				value="+";
				key=65;
				break;
			}
		}

		else if(token=='<')
		{
			if(prog[tokenCount]=='<')
			{ 
				tokenCount++;
				if(prog[tokenCount]=='=')
				{ 
					tokenCount++;
					value="<<=";
					key=70;
					break;
				}

				else
				{ 
					value="<<";
					key=69;
					break;
				}
			}

			else if(prog[tokenCount]=='=')
			{ 
				tokenCount++;
				value="<=";
				key=71;
				break;
			}

			else
			{ 
				value="<";
				key=68;
				break;
			}
		}

		else if(token=='=')
		{
			if(prog[tokenCount]=='=')
			{ 
				tokenCount++;
				value="==";
				key=73;
				break;
			}

			else
			{ 
				value="=";
				key=72;
				break;
			}
		}

		else if(token=='>')
		{
			if(prog[tokenCount]=='>')
			{ 
				tokenCount++;
				if(prog[tokenCount]=='=')
				{ 
					tokenCount++;
					value=">>=";
					key=77;
					break;
				}

				else
				{ 
					value=">>";
					key=76;
					break;
				}
			}

			else if(prog[tokenCount]=='=')
			{ 
				tokenCount++;
				value=">=";
				key=75;
				break;
			}

			else
			{ 
				value=">";
				key=72;
				break;
			}
		}

		else if(token=='"')
		{ // ""
			value+='"';
			//i=1;
			//while(prog[tokenCount]!='"')
			//	value+=prog[tokenCount++];
			//value+=prog[tokenCount++];
			key=78;
			break;
		}
	}
	if(key==0) return NULL;
	u->key=key;
	u->value=value;
	Unit *n = new Unit();
	n->next=NULL;
	u->next=n;
	return tokenCount;
};




void Analysis()
{
    string prog;
    read_prog(prog);
    /* 骚年们 请开始你们的表演 */
    /********* Begin *********/


	int tokenCount=0;
    int count=0;
    Unit *root;

    root = new Unit();
    root->next=NULL;


    tokenCount = analy(root, prog, tokenCount);
    Unit *current = root->next;

    while(current)
    {
    	cout << ++count << ": <" << root->value << "," << root->key << ">";
    	root = root->next;
        tokenCount = analy(root, prog, tokenCount);
        if(current)
        {
            cout << endl;
            current = current->next;
        }
    	
    } 
    /********* End *********/
    
}
