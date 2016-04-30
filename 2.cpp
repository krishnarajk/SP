#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include <algorithm>

using namespace std;

class Line
{
public:
	string label,opcode,operand,saddress;
	Line(string a,string b,string c,string d=""){
		label=a;
		opcode=b;
		operand=c;
		saddress=d;
	}
	
};

Line getline(fstream &fin){
	string l,op,ope;
	char c;
	fin.get(c);
	if(c=='\n'){
		fin.get(c);
	}
	if(c=='\t'){
		l="";
	}
	else{
		fin.seekg(-1,fin.cur);
		fin>>l;
	}
	fin>>op;
	fin>>ope;
	return Line(l,op,ope);
}
Line getlineint(fstream &fint){
	string sadd,l,op,ope;
	char c;
	fint>>sadd;
	fint.get(c);
	if(c=='\n')	fint.get(c);
	fint.get(c);
	if(c=='\t'){
		l="";
	}
	else{
		fint.seekg(-1,fint.cur);
		fint>>l;
	}
	fint>>op;
	fint>>ope;
	return Line(l,op,ope,sadd);

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
		fout<<"T^"<<setw(6)<<setfill('0')<<hex<<uppercase<<start<<"^"<<setw(2)<<length/2;
		fout<<record<<endl;
	}
	
};
string dectohex(int i,int j){
	stringstream s;
	s<<setw(j)<<setfill('0')<<hex<<uppercase<<i;
	string res(s.str());
	return res;
}
map<string,string>optab;
map<string,int>symtab;
int main(){
	fstream fin("asm"),fout("output.txt"),fop("optab"),fint("intermediate.txt");
	string objcode,opcode;

	int locctr=0,startaddress=0;
	while(fop){
		fop>>opcode>>objcode;
		optab[opcode]=objcode;
	}
	Line line= getline(fin);
	if(line.opcode=="START"){
		stringstream s;
		s<<line.operand;
		s>>hex>>locctr;
		startaddress=locctr;
	}
	fint<<dectohex(startaddress,4)<<"\t"<<line.label<<"\t"<<line.opcode<<"\t"<<line.operand<<endl;
	line=getline(fin);
	while(line.opcode!="END"){
		fint<<dectohex(locctr,4)<<"\t"<<line.label<<"\t"<<line.opcode<<"\t"<<line.operand<<endl;
		if(line.label!=""){
			if(symtab.find(line.label)!=symtab.end()){
			int flag=1;
			cout<<"error!!!";
			return 0;
			}
			else{
			symtab[line.label]=locctr;
			}
		}
		if(line.opcode=="WORD"){
			locctr+=3;
		}
		else if(line.opcode=="RESW"){
			int len=3*(atoi(line.operand.c_str()));
			locctr+=len;
		}
		else if(line.opcode=="RESB"){
			int len=atoi(line.operand.c_str());
			locctr+=len;
		}
		else if(line.opcode=="BYTE"){
			int len=line.operand.length()-3;
			locctr+=len/2;
		}
		else
			locctr+=3;
		line=getline(fin);
	}
	fint<<dectohex(locctr,4)<<"\t"<<line.label<<"\t"<<line.opcode<<"\t"<<line.operand<<endl;
	int plength=locctr-startaddress;
	//cout<<dectohex(plength,6);
	fin.close();
	fint.seekg(0,fint.beg);
	Line lineint= getlineint(fint);
//	cout<<lineint.saddress<<"\t"<<lineint.label<<"\t"<<lineint.opcode<<"\t"<<lineint.operand<<endl;
//	cout<<lineint.opcode;
	if(lineint.opcode=="START"){
		fout<<"H^"<<lineint.label<<"^"<<dectohex(startaddress,6)<<"^"<<dectohex(plength,6)<<endl;
		lineint=getlineint(fint);
	}
	Textrecord *trecord= new Textrecord(startaddress);
	while(lineint.opcode!="END"){
		objcode="";
		if(optab.find(lineint.opcode)!=optab.end()){
			objcode=optab[lineint.opcode];
			objcode+=dectohex(symtab[lineint.operand],4);
		}
		else if(lineint.opcode=="WORD"){
			objcode=dectohex(atoi(lineint.operand.c_str()),6);
		}
		else if(lineint.opcode=="BYTE"){
			int len=lineint.operand.length()-3;
			stringstream s;
			if(lineint.operand[0]=='X'){
				s<<lineint.operand.substr(2,len);
			} 
			objcode=s.str();
		}
		//else if(lineint.opcode=="RESB"){
		//	trecord->write(fout);
		//	trecord=new Textrecord()
		//}
		trecord->push(objcode);
		lineint=getlineint(fint);
	}
	trecord->write(fout);
	fout<<"E^"<<dectohex(startaddress,6)<<endl;
//	for(map<string,int>::iterator i=symtab.begin();i!=symtab.end();i++){
//		cout<<i->first<<" "<<i->second<<endl;
//	}
}