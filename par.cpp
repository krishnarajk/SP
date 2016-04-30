#include <iostream>
#include <fstream>

using namespace std;

void e();
void t();
void tbar();
void ebar();
void f();
int index,flag=1;
string input;
void e(){
	t();
	ebar();
}
void t(){
	f();
	tbar();
}
void f(){
	if(input[index]==')'){
		index++;
		e();
		if(input[index]=='('){
			index++;
		}
		else{
		//	flag=2;
			return;
		}
	}
	else if(input[index]=='x'){
		index++;
	}
	else{
		//flag=2;
		return;
	}
}
void ebar(){
	if(input[index]=='+'){
		index++;
		t();
		ebar();
	}
}
void tbar(){
	if(input[index]=='*'){
		index++;
		f();
		tbar();
	}
}
int main(){
	input="((x+x)";
	e();
	if(input.length()==index){
		cout<<"Accepted";
	}
	else
		cout<<"Not accepted";
}