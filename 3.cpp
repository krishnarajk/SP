#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include<algorithm>

using namespace std;
string dectohex(int i,int j){
	stringstream s;
	s<<setw(j)<<setfill('0')<<hex<<uppercase<<i;
	string res(s.str());
	return res;
}
int main(){
	fstream fin("output1");
	int start,length,tstart,tlength;
	string memory[100];
	string s;
	string buffer;
	stringstream temp;
	fin>>s;
	temp<<s.substr(s.find('^',2)+1,6);
	temp>>hex>>start;
	//cout<<dectohex(start,6);
	temp.clear();
	temp<<s.substr(s.find('^',2)+9,6);
	temp>>hex>>length;
	temp.clear();
	while(fin){
		fin>>buffer;
		if(buffer[0]=='E'){break;}
		s=buffer.substr(2,6);
		temp<<s;
		temp>>hex>>tstart;
		temp.clear();
		//cout<<tstart<<endl;
		s=buffer.substr(9,2);
		temp<<s;
		temp>>hex>>tlength;
		temp.clear();
		int i=12;
		int j=0;
		//cout<<tlength<<endl;	
		while(j<tlength){
			if(buffer[i]=='^'){
				i++;
			}
			s=buffer.substr(i,2);
			memory[tstart+j-start]=s;
			i=i+2;
			j++;
		}
	}
	int i=0;
	while(i<length){
		cout<<dectohex(start+i,4)<<" "<<memory[i]<<endl;
		i++;
	}
	
}
