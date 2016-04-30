#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

string dectohex(int i,int j){
	stringstream s;
	s<<setw(j)<<setfill('0')<<hex<<uppercase<<i;
	string res(s.str());
	return res;
}
int main(){
	string s,buffer;
	stringstream temp;
	fstream fin("output");
	int start,length,tstart,tlength,naddr,val,mask;
	cout<<"input the address :";
	cin>>s;
	temp<<s;
	temp>>hex>>naddr;
	string memory[100];
	fin>>buffer;
	s=buffer.substr(buffer.find('^',2)+1,6);
	temp<<s;
	temp>>hex>>start;
	//cout<<dectohex(start,6);
	temp.clear();
	s=buffer.substr(buffer.find('^',2)+8,6);
	temp<<s;
	temp>>hex>>length;
	temp.clear();
	while(fin){
		fin>>buffer;
		if(buffer[0]=='E'){
			break;
		}
		s=buffer.substr(2,6);
		temp<<s;
		temp>>hex>>tstart;
		temp.clear();
		s=buffer.substr(9,2);
		temp<<s;
		temp>>hex>>tlength;
		temp.clear();
		s=buffer.substr(12,3);
		temp<<s;
		temp>>hex>>mask;
		temp.clear();
		int i=15,j=0;
		int x=2048;
		while(j<tlength){
			if(buffer[i]=='^')
				i++;
			s=buffer.substr(i,2);
			memory[tstart-start+j]=s;
			i+=2;
			j++;
			if(buffer[i]!='^'&&j!=tlength){
				if((mask&x)==x){
					s=buffer.substr(i,4);
					i=i+4;
					temp<<s;
					temp>>hex>>val;
					val=val+naddr;
					s=dectohex(val,4);
					memory[tstart-start+j]=s.substr(0,2);
					j++;
					memory[tstart-start+j]=s.substr(2,2);
					j++;		
				}
			}
			x=x>>1;
		}

	}
	int i=0;
	while(i<length){
		cout<<dectohex(naddr+i,4)<<" "<<memory[i]<<endl;
		i++;
	}
}