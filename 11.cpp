#include <iostream>
#include <fstream>
#include <list>
#include <sstream>

using namespace std;

list<string>code;

int main(){
	fstream fin("input1");
	string line;
	while(getline(fin,line)){
		stringstream s(line);
		string el;
		while(getline(s,el,' ')){
			code.push_back(el);
		}
		string first=code.front();
		code.pop_front();
		code.pop_front();
		string second=code.front();
		code.pop_front();
		cout<<"MOV "<<first<<","<<second<<endl;
		if(!code.empty()){

			string pstring=code.front();
			code.pop_front();
			if(pstring=="+")
				cout<<"ADD "<<first<<",";
			else if(pstring=="-")
				cout<<"SUB "<<first<<",";
			else if(pstring=="*")
				cout<<"MUL "<<first<<",";
			else if(pstring=="/")
				cout<<"DIV "<<first<<",";
			cout<<code.front()<<endl;
			code.pop_front();
		}
	}
}

