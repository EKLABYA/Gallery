// Write a program to detect tokens in C++
#include <bits/stdc++.h>
using namespace std;

vector<vector<string>> symtab ;

bool isDelimiter(char ch) {
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
        ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}')
        return true;

    return false;
}
 
bool isOperator(char ch) {
    if (ch == '+' || ch == '-' || ch == '>' || ch == '<' ||ch == '=')
        return true;
    
    return false;
}

bool validIdentifier(string str) {
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
        str[0] == '3' || str[0] == '4' || str[0] == '5' ||
        str[0] == '6' || str[0] == '7' || str[0] == '8' ||
        str[0] == '9' || isDelimiter(str[0]) == true)
        return false;

    return true;
}

bool isKeyword(string str) {
    if (str == "if" || str == "else" ||
        str == "while" || str == "do" ||str == "break" ||
        str == "continue" || str == "return" || str == "char"
        || str == "case" || str == "typedef"
        || str == "switch" || str == "unsigned"
        || str == "void" || str == "static"
        || str == "string")
        return true;

    return false;
}

string subString(string str, int left, int right) {
	int i;
	string subStr = "";

	for (i = left; i <= right; i++)
		subStr += str[i];
// 	subStr += '\0';
	return (subStr);
}

string scope = "global";
int id = 1;
void parse(string str, int row) {
    ofstream outfile;
    outfile.open("tokenized.txt", std::ios_base::app);

    int left = 0, right = 0;
    int len = str.length();
    string last_token = "" ;
    while (right <= len && left <= right) {
        if (isDelimiter(str[right]) == false)
            right++;
 
        if (isDelimiter(str[right]) == true && left == right) {
            if (isOperator(str[right]) == true) {
                outfile << str[right] << "  ->   AN OPERATOR\n";
                outfile << "Line No.:  " << row << "   Column No.:  " << right << "\n\n";
            }
            else if (str[right] != ' ') {
                outfile << str[right] << "  ->   A SYMBOL\n";
                outfile << "Line No.:  " << row << "   Column No.:  " << right << "\n\n";
            }
            
            right++;
            left = right;
        } else if (isDelimiter(str[right]) == true && left != right
                   || (right == len && left != right)) {
            string subStr = subString(str, left, right - 1);
            cout<<subStr<<" " ;
            if (subStr == "\n") {
                left = right;
                continue;
            }

            if (isKeyword(subStr) == true){
                outfile << subStr << "  ->   A KEYWORD\n";
                if(subStr != ",") last_token = subStr ;
            }
            else if(subStr[0]=='"'&&subStr[subStr.length()-1]=='"')
                outfile << subStr << "  ->   A VALID STRING\n";
            else if(subStr[0]-'&'==1&&subStr[subStr.length()-1]-'&'==1)
                outfile << subStr << "  ->   A VALID charecter\n";
            else if (validIdentifier(subStr) == true
                     && isDelimiter(str[right - 1]) == false){
                bool found = false ;
                for(auto i:symtab){
                    if(i[1] == subStr){
                        found = true ;
                        break ;
                    }
                }
                if(found)
                outfile << subStr << "  ->   A VALID IDENTIFIER\n";
                else{
                    if(last_token == "string" || last_token == "char" ){
                      outfile << subStr << "  ->   A VALID IDENTIFIER\n";
                      string Id = "id"+to_string(id) ;
                      id++ ;
                      if(str[right]=='('){
                      scope = subStr ;
                      symtab.push_back({Id,subStr,"Function","---"})  ;
                      }
                      else
                      symtab.push_back({Id,subStr,last_token,scope})  ;
                    }
                    else if(last_token=="int" || last_token=="void"){
                      outfile << subStr << "  ->   A VALID IDENTIFIER\n";
                      string Id = "id"+to_string(id) ;
                      id++ ;
                      scope = subStr ;
                      symtab.push_back({Id,subStr,"Function","---"})  ; 
                    }
                    else{
                      outfile << subStr << "  ->   NOT A VALID IDENTIFIER\n";  
                    }
                }
                }
            else if (validIdentifier(subStr) == false
                     && isDelimiter(str[right - 1]) == false)
                outfile << subStr << "  ->   NOT A VALID IDENTIFIER\n";
            outfile << "Line No.:  " << row << "   Column No.:  " << left << "\n\n";
            
            left = right;
        }
    }
    outfile.close();
}

int main() {
    
    ifstream MyReadFile("test.txt");
    string myline;
    int row = 0;
    
    while (getline (MyReadFile, myline)) {
        cout<<myline<<endl;
        parse(myline, row);
        row++;
    }
    MyReadFile.close();
    ofstream outfile;
    outfile.open("symboltable.txt", std::ios_base::app);
    //Printing the symbol table 
    outfile<<"\n\nSymbol Table\n\nid\t\t value\t\t type\t\t  scope\n--------------------\n";
    for(auto i:symtab){
        outfile<<i[0]<<"\t\t"<<i[1]<<"\t\t"<<i[2]<<"\t\t"<<i[3]<<"\n" ;
    }
    outfile.close();
    return (0);
}