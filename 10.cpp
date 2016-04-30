#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>

using namespace std;

stack<char>post;
stack<string>solution;
string input;
bool isoperator(char s){
	switch(s){
		case '+':case '-':case '*':case '/':return true;
		case '(':case ')':case'=': return true;
		default:return false; 
	};
}
int isp(char s){
	if(s=='+'||s=='-') return 2;
	else if(s=='*'||s=='/') return 3;
	else if(s=='(') return 0;
	else if(s=='=') return 0;
}
int icp(char s){
	if(s=='+'||s=='-') return 2;
	else if(s=='*'||s=='/') return 3;
	else if(s=='(') return 7;
	else if(s=='=') return 0;
	else if(s==')') return 1;
}
int main(){
	fstream fin("input1");
	//fin>>input;
	string el;
	while(getline(fin,input)){
		//s.clear();
		stringstream s(input);
		//s<<input;
		stringstream postfix;
		while(getline(s,el,' ')){
			
			char e=el[0];
			if(!isoperator(e)){
				postfix<<el<<" ";

			}
			else if(post.empty()||isp(post.top())<icp(e)){
				post.push(e);
			}
			else{
				while(!post.empty()&&isp(post.top())>=icp(e)){
					postfix<<post.top()<<" ";
					post.pop();
				}
				if(e==')'){
					post.pop();
				}
				else
					post.push(e);
			}
		}
		while(!post.empty()){
			postfix<<post.top()<<" ";
			post.pop();
		}
	

		string posfixexp(postfix.str());
	//	cout<<posfixexp<<endl;
		string result, pstring;
		int i=0;
		while(getline(postfix,pstring,' ')){
			result="";
			char el=pstring[0];
			if(!isoperator(el)){
				solution.push(pstring);
			}
			else{
				string first=solution.top();
				solution.pop();
				string second=solution.top();
				solution.pop();
				if(el=='='){
					result=second+" "+el+" "+first;
				}
				else{
					result="t"+to_string(i++);
					cout<<result+" = "+second+" "+el+" "+first<<endl;
				}
				solution.push(result);
		
			}
		}
		cout<<result<<endl;
	}
}
