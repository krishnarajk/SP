#include <iostream>
#include <sstream>
#include <map>
#include <list>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

class Line
{
public:
	string label,opcode,operand;
	Line(string a,string b,string c){
		label=a;
		opcode=b;
		operand=c;
	}
	
};

Line getline(fstream &fin){
	string l,op,ope;
	char c;
	fin.get(c);
	if(c=='\n')
		fin.get(c);
	if(c=='\t')
		l="";
	else{
		fin.seekg(-1,fin.cur);
		fin>>l;
	}
	fin>>op;
	fin>>ope;
	return Line(l,op,ope);
}
string dectohex(int i,int j){
	stringstream s;
	s<<setw(j)<<setfill('0')<<uppercase<<hex<<i;
	string res(s.str());
	return res;
}
class Textrecord
{
public:
	string record;
	int start,length;
	Textrecord(int s){
		start=s;
		length=0;
	}
	void push(string t){
		record+="^"+t;
		length+=t.length();
	}
	void write(fstream &fout){
		if(length==0) return;
		fout<<"T^"<<setfill('0')<<setw(6)<<uppercase<<hex<<start<<"^"<<setw(2)<<length/2;
		fout<<record<<endl;
	}
	
};

class Op
{
public:
	int val;
	list<int>address;
	Op(){
		val=-1;
	}
	
};
map<string,string>optab;
map<string,Op>symtab;
int main(){
	fstream fin("asm"),fout("output.txt"),fop("optab");
	int locctr=0,startaddress=0;
	string opcode,objcode;
	Line line=getline(fin);
	if(line.opcode=="START"){
		stringstream s;
		s<<line.operand;
		s>>hex>>locctr;
		startaddress=locctr;
	}
	while(fop){
		fop>>opcode>>objcode;
		optab[opcode]=objcode;
	}
	fout<<"H^"<<line.label<<"^"<<dectohex(startaddress,6)<<"^"<<"000000"<<endl;
	int addpos=line.label.length()+10;
	Textrecord *trecord=new Textrecord(startaddress);
	while(line.opcode!="END"){
		objcode="";
		if(line.label!=""){
			symtab[line.label].val=locctr;
			list<int>address=symtab[line.label].address;
			if(address.size()!=0){
				trecord->write(fout);
				for(list<int>::iterator i=address.begin();i!=address.end();i++){
					trecord=new Textrecord(*i+1);
					trecord->push(dectohex(locctr,4));
					trecord->write(fout);
				}
				trecord=new Textrecord(locctr);
			}

		}
		if(optab.find(line.opcode)!=optab.end()){
			objcode=optab[line.opcode];
			Op op =symtab[line.operand];
			if(op.val==-1){
				symtab[line.operand].address.push_back(locctr);
				objcode+="0000";
			}
			else{
				objcode+=dectohex(op.val,4);
			}
			locctr+=3;
		}
		else if(line.opcode=="WORD"){
			locctr+=3;
			objcode=dectohex(atoi(line.operand.c_str()),6);
		}
		else if(line.opcode=="RESW"){
			int len=3*(atoi(line.operand.c_str()));
			locctr+=len;
			trecord->write(fout);
			trecord=new Textrecord(locctr);

		}
		else if(line.opcode=="RESB"){
			int len=atoi(line.operand.c_str());
			locctr+=len;
			trecord->write(fout);
			trecord=new Textrecord(locctr);
		}
		else if(line.opcode=="BYTE"){
			int len=line.operand.length()-3;
			stringstream s;
			if(line.operand[0]=='X'){
				s<<line.operand.substr(2,len);

			}
			objcode=s.str();
			locctr+=len/2;
		}
		trecord->push(objcode);
		line=getline(fin);

	}
	trecord->write(fout);
	fout<<"E^"<<dectohex(startaddress,6)<<endl;
	fout.seekp(addpos,fin.beg);
	fout<<dectohex(locctr-startaddress,6);
	return 0;
}