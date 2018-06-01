#include <iostream>
#include <windows.h>
#include <string>
#include <fstream> 
#include "myString.h"
#include<time.h>

#define MAX_FIELD 10
#define MAX_TYPE  100
#define MAX_WORDS 500

using namespace std;

struct word{
	string name;
	int typeCode;
	string values[MAX_FIELD];
	int valuesCount;
	static int count;
};


struct type {
	int typeCode;
	string typeName;
	string fieldName[MAX_FIELD];
	int fieldcount;
	static int count;
};

struct fieldInf{
	string lines[MAX_FIELD];
	int count;
};


type tabletype[MAX_TYPE];
word words[MAX_WORDS];

int type::count = 0;
int word::count = 0;

void addType();
void addType(type);
void addWord();
void addWord(word);
int  searchWordByName(string, int, int );
int  searchTypeByName(string);
int  searchTypeByCode(int , int, int);
void printWords();
void printWord(int );
void printTypes();
void printNameTypes();
void deleteType();
void deleteWord();
void editWord();
void editType();
void findWord();
fieldInf divString();
void printWordName(int);
void printLogo(){
	cout <<"\n\n";
	cout << "                            __Tu dien Bach Khoa toan thu BKEncyclopedia__ " << endl;
	cout << "                           |                                             |" << endl;
	cout << "                           ***********************************************" << endl;
}
void saveWords()
{
	ofstream out("tu.txt");	
	int i = 0,j = 0;
	for( i= 0;i<word::count-1;i++)
	{
		out << words[i].name << endl<<words[i].typeCode<<endl;
		for(int j = 0; j < words[i].valuesCount;j++)
		{
			out << words[i].values[j]<<endl;
		}
	}
	i = word::count-1;
	out << words[i].name << endl<<words[i].typeCode<<endl;

	for(int j = 0; j < words[i].valuesCount-1;j++)
	{
		out << words[i].values[j]<<endl;
	}
	j = words[i].valuesCount-1;
	out << words[i].values[j];
	out.close();
}

void saveTypes()
{
	ofstream out ("the-loai.txt");
	int i= 0, j = 0;
	for( i =0; i < type::count-1;i++)
	{
		out<<tabletype[i].typeCode << endl<<tabletype[i].typeName<<endl;
		for( j = 0; j < tabletype[i].fieldcount;j++)
		{
			out << tabletype[i].fieldName[j]<<"; ";
		}
		out << endl;
	}
	i  = type::count-1;
	out<<tabletype[i].typeCode << endl<<tabletype[i].typeName<<endl;
	
	for( j = 0; j < tabletype[i].fieldcount-1;j++)
	{
		out << tabletype[i].fieldName[j]<<"; ";
	}
	j = tabletype[i].fieldcount-1;
	out << tabletype[i].fieldName[j]<<"; ";
	out.close();
}

void save()
{   system("cls");
    fflush(stdin);
    printLogo();
	cout << "\n\n               Chuc nang da chon: Luu du lieu vua duoc chinh sua. \n\n";
	saveWords();
	saveTypes();
	cout <<"\n                           ------------------LUU THANH CONG----------------\n"<<endl;
	getchar();
	system("cls");
}

fieldInf divString(string line){
	fieldInf res; res.count = 0;
	
	string chuoi = "";
	int result = 0;
	
	while(result < line.length()) {
		chuoi = "";
		while(line[result]==' ' && result < line.length()) result++;
		while(line[result]!=';' && result < line.length()) chuoi += line[result++];	
		result++;
		if(chuoi.length() > 0) {
				res.lines[res.count++] = chuoi;
		}
	}
	return res;
}

// Nap cac the loai tu file dau vao
void loadTypes(){
	ifstream fi("the-loai.txt");
	string line;
	
	while(!fi.eof()){
		// tao bien newtype de luu the loai moi
		type newtype;
		getline(fi,line);
		// gan ma the loai 
		newtype.typeCode = atoi(line.c_str());
		
		getline(fi,line);
		// gan ten the loai
		newtype.typeName = line; 
		
		getline(fi,line);
		// doc cac truong
		fieldInf res = divString(line);
		// gan cac truong vao newtype
		for(int i = 0; i < res.count;i++) {
			newtype.fieldName[i] = res.lines[i];
		}
		// gan so truong 
		newtype.fieldcount = res.count;
		//tim vi tri, dua newtype vao mang tabletype
		addType(newtype);
	}
	//cout << type::count << "Count"<<endl;
	fi.close();
}

void printTypes() {
	cout <<"\n                           ----------------DANH SACH THE LOAI--------------\n"<<endl;
	for(int i = 0; i < type::count;i++){
		cout <<"                           Ma The Loai: " << tabletype[i].typeCode << endl;
		cout <<"                           Ten The Loai: " << tabletype[i].typeName << endl;
		cout <<"                           Cac Truong: " ;
		for(int j = 0; j < tabletype[i].fieldcount;j++)  
		{
			cout << tabletype[i].fieldName[j] << "; ";
		}
		cout << "\n                           ---------------";
		cout << endl << endl;
	}
}

void printNameTypes(){
	cout <<"\n                           ----------------DANH SACH THE LOAI---------------\n"<<endl;
	for(int i = 0; i < type::count; i++){
		cout <<"                           "<< tabletype[i].typeCode << ": " << tabletype[i].typeName << endl; 
	}
	cout <<"\n                           --------------------------------------------------\n"<<endl;
}

// tra ve so truong
int getFieldCount(int code)
{
	int i = searchTypeByCode(code,0,type::count);
	return tabletype[i].fieldcount;

}

//Them tu vao mang, sap xep theo ten
void addWord(word newword){
	for(int i = 0; i <= word::count; i++){
		if(newword.name < words[i].name || i == word::count){
			for(int j = word::count; j > i; j--){
				words[j] = words[j-1]; 
			}
			words[i] = newword;
			break;
		}
	}	
	word::count ++;
}

//doc file word
void loadWords(){
	ifstream fi("tu.txt");
	string line;
	
	while(!fi.eof()) {
		word newword;
		getline(fi,line);	newword.name = line; 
		getline(fi,line); 	newword.typeCode = atoi(line.c_str());
		
		int numFields = -1;
		numFields = getFieldCount(newword.typeCode);
		newword.valuesCount = numFields; 
		for(int i= 0;i < numFields;i++)
		{
			getline(fi,line); newword.values[i] = line;
			
		}
		addWord(newword);
	}
	fi.close();
}

//in ra tat ca cac tu
void printWords()
{   clock_t start, finish;
    double time;
	start = clock();
    int code,k;
    fflush(stdin);
    printLogo();
    cout << "\n\n               Chuc nang da chon: In danh sach cac tu. \n\n";
	cout <<"\n                           -----------------DANH SACH CAC TU---------------\n"<<endl;
	for(int i = 0;i < word::count; i++)
	{
		cout <<"Tu " << i << ": " << words[i].name << endl <<"MA THE LOAI: " << words[i].typeCode <<endl;
		code = words[i].typeCode;
		k = searchTypeByCode(code,0,type::count-1);
		for(int j= 0;j < words[i].valuesCount ;j++)
		{
			cout << tabletype[k].fieldName[j] <<": "<< words[i].values[j] <<endl;
			
		}
		cout << "--------------------\n\n";
	}
	finish = clock();
	time = (float)(finish-start)/CLOCKS_PER_SEC;
	cout << "\n                            -----------------------------------------------\n";
	printf("\n               Thao tac xong trong khoang %.10f giay.\n\n",time);
	getchar();
	system("cls");
}

//in ra mot tu o vi tri thu i
void printWord(int i)
{	int code = words[i].typeCode;
    int k = searchTypeByCode(code,0,type::count-1);
	cout << words[i].name << endl;
	for(int j= 0;j < words[i].valuesCount ;j++)
	{
		cout << endl<<tabletype[k].fieldName[j]<<": " << words[i].values[j] <<endl;	
	}		
	
}

void printWordName(int i) {
	cout <<"                           " << words[i].name << endl;		
}

string checkInput(string line){
	int last = line.length();
	while(1){
		last--;
		if(line[last] == 32) line = line.substr(0,last);
		else break;
	}
	while(1){
		if(line[0] == 32) line = line.substr(1,last);
		else break;
	}
	return line;
}

//tim tu theo ten
void findWord(){
	clock_t start, finish;
	double time;
	fflush(stdin);
	do{ printLogo();
        cout << "\n\n               Chuc nang da chon: Tim tu. \n\n";
		cout << "               Nhap tu muon tim kiem hoac nhap 0 de thoat: ";
		string name;
		fflush(stdin);
		getline(cin,name);
		name = checkInput(name);
		if(name == "0") {
	        system("cls");
			break;
	    }   
		start=clock();
		int result = searchWordByName(name,0,word::count);
		finish = clock();
		if(result == -1){
			cout <<"\n                            ----------------KHONG TIM THAY---------------"<<endl<<endl;	
		}
		else{
			cout <<"\n                            ------------------TIM THAY---------------------\n"<<endl;
			printWord(result);
			cout <<"\n                            -----------------------------------------------\n"<<endl;
		}	
		time = (float)(finish-start)/CLOCKS_PER_SEC;
	    printf("\n               Thao tac xong trong khoang %.10f giay.",time);
		getchar();
		system("cls");
	}while(1);
}

//ham nhap tu moi  
void addWord(){
    clock_t start, finish;
    double time;
    string line;
	word newword;
	bool flag = false;
	fflush(stdin);
    printLogo();
    cout << "\n\n               Chuc nang da chon: Them tu. \n";
	printNameTypes();
	while(1){
		cout << "               Nhap ma the loai: ";
		fflush(stdin);
		getline(cin,line);
		start = clock();
		line = checkInput(line);
		if(line != "0" && atoi(line.c_str()) == 0) continue;
		else{
			if(searchTypeByCode(atoi(line.c_str()),0,type::count-1) == -1 || line == "") continue;
			else {
				break;
		    }
		}
	}
	newword.typeCode = atoi(line.c_str());
	int result = searchTypeByCode(newword.typeCode,0,type::count-1);
	finish = clock();
	time = (float)(finish-start)/CLOCKS_PER_SEC;
	cout << "               Ban chon the loai " << tabletype[result].typeName << endl;
	do{ if(flag==true && line!="") cout << "               Tu nay da co trong danh sach."<<endl;
		cout << "               Nhap ten tu moi: ";
		getline(cin,line);
		start = clock();
		line = checkInput(line);
		flag=true;
		if(searchWordByName(line,0,word::count-1) ==-1) break;
	}while(line==""||searchWordByName(line,0,word::count-1)!=-1);
	newword.name = line;
	int soTruong = getFieldCount(newword.typeCode);
	newword.valuesCount = soTruong;
	finish = clock();
	time = time + (float)(finish-start)/CLOCKS_PER_SEC; 
	for(int i = 0; i < soTruong;i++)
	{
		cout << "               Nhap truong "<<tabletype[result].fieldName[i]<<": ";
		getline(cin,line);
		newword.values[i] = line;
	}
	start = clock();
	addWord(newword);
	finish = clock();
	time = time+ (float)(finish-start)/CLOCKS_PER_SEC;
	cout << "                            ----------------THEM THANH CONG---------------\n" << endl;
	printf("               Them thanh cong trong khoang %.10f giay.",time);
	fflush(stdin);
	getchar();
	system("cls");
}

void printNameWords(){
	cout << "                            -----------------DANH SACH TU-----------------" << endl;
	for(int i = 0; i < word::count; i++){
		cout << "                            " << words[i].name << endl;
	}
	cout << "                            -----------------------------------------------\n" << endl;
}

void deleteWord(int i){
	for(int j = i; j < word::count - 1; j++){
				words[j] = words[j+1];
	}
	word::count--;
}

void deleteWord() {
	clock_t start, finish;
    double time;
    system("cls");
	fflush(stdin);
    printLogo();
    cout << "\n\n               Chuc nang da chon: Xoa tu. \n\n";
	printNameWords();
	if(word::count==0) {
		cout <<"                            ---------------DANH SACH TRONG-----------\n\n";
		return;
	}
	while(1){
		string line;
		cout << "\n               Nhap Tu muon xoa hoac nhap 0 de quay tro lai: ";
		getline(cin,line);
		start = clock();
		line = checkInput(line);
		if(line == "0") {
			system("cls");
			break;	
		}
		int rs = searchWordByName(line,0,word::count-1);
		if(rs != -1){
			deleteWord(rs);
			cout << "\n                            ---------------------DA XOA--------------------";
		}
		else cout << "\n                            -----------KHONG TIM THAY TU MUON XOA----------";
		finish=clock();
		time = (float)(finish-start)/CLOCKS_PER_SEC;
		printf("\n               Thao tac xong trong khoang %.10f giay.\n\n",time);
	}
	system("cls");
}

// Tra ve -1 neu khong tim thay
// tra ve vi tri thu i
// tim kiem nhi phan
int searchWordByName(string name, int first, int last)
{	
	if(first == last){
		if(words[last].name == name) return last;
		else return -1;
	}
	int mid = (first + last)/2;
	if(name == words[mid].name) return mid;
	if(name > words[mid].name) return searchWordByName(name,mid+1,last);
	if(name < words[mid].name) return searchWordByName(name,first,mid); 
}

int searchTypeByName(string name){
	for(int i = 0; i <= type::count; i++){
		if(name == tabletype[i].typeName) return i;
	}
	return -1;
}

int searchTypeByCode(int code, int first, int last)
{	
	if(first == last){
		if(tabletype[last].typeCode == code) return last;
		else return -1;
	}
	int mid = (first + last)/2;
	if(code == tabletype[mid].typeCode) return mid;
	if(code > tabletype[mid].typeCode) return searchTypeByCode(code,mid+1,last);
	if(code < tabletype[mid].typeCode) return searchTypeByCode(code,first,mid); 
}

void Swap(word &w1, word &w2){
	word newword = w1;
	w1 = w2;
	w2 = newword;
}

void editNameWord(int &i){
	clock_t start, finish;
	double time;
	while(1){
		fflush(stdin);
		string line;
		cout << "\n               Nhap ten moi khac null & khac cac ten tu da co hoac nhap 0 de thoat: ";
		getline(cin,line);
		start=clock();
		line = checkInput(line);
		if(line == "0") return;
		if(line == "") continue;
		int rs = searchWordByName(line,0,word::count-1);
		if(rs != -1) continue;
		words[i].name = line;
		if (words[i-1].name > words[i].name || words[i].name > words[i+1].name){
			while(words[i-1].name > words[i].name){
				Swap(words[i-1],words[i]);
				i--;
				if(i == 0 || i == word::count) break;
			}
			while(words[i].name > words[i+1].name){
				Swap(words[i],words[i+1]);
				i++;
				if(i == -1 || i == word::count) break;
			}
			if(i == 0 || i == word::count) break;
		}
		finish = clock();
		time = (float)(finish-start)/CLOCKS_PER_SEC;
		cout << "\n                            --------------DA CAP NHAT THANH CONG--------------\n";
		printf("\n               Thao tac xong trong khoang %.10f giay.\n\n",time);
		getchar();
		break;	
	}
}

void editTypeWord(int &i){
	clock_t start, finish;
	double time;
	string line;
	int code = words[i].typeCode;
	int k = searchTypeByCode(code,0,type::count-1);
	while(1){
		fflush(stdin);
		cout << "\n               The loai hien tai: " << tabletype[k].typeName ;
		cout << "\n               Nhap ten the loai moi khac voi the loai truoc do: ";
		getline(cin,line);
		start = clock();
		line = checkInput(line);
		if(line == "") continue;
		int rs1 = searchTypeByName(line);
		if(rs1 == -1){
			cout << "               The loai khong ton tai!" << endl;
			continue;	
		}
		else{
			cout << "               Ma the loai da chon: " << tabletype[rs1].typeCode << endl;
			words[i].typeCode = tabletype[rs1].typeCode;
			words[i].valuesCount = tabletype[rs1].fieldcount; 
			finish = clock();
			time = (float)(finish-start)/CLOCKS_PER_SEC;
			for(int j = 0; j < tabletype[rs1].fieldcount; j++){
				cout << "               Nhap truong " << tabletype[rs1].fieldName[j] << ": ";
				string line;
				getline(cin,line);
				start = clock();
				line = checkInput(line);
				words[i].values[j] = line;
				finish = clock();
				time = time + (float)(finish-start)/CLOCKS_PER_SEC;
			}
		break;
		}	
	}
	cout << "\n                            --------------DA CAP NHAT THANH CONG--------------\n";
	printf("\n               Thao tac xong trong khoang %.10f giay.\n\n",time);
	getchar();
	system("cls");
}

void editValuesWord(int &i){
	clock_t start, finish;
	double time;
	start = clock();
	system("cls");
	int rs = searchTypeByCode(words[i].typeCode,0,type::count);
	cout << "Ma the loai : " << tabletype[rs].typeCode << endl; 
	finish = clock();
	time = (float)(finish-start)/CLOCKS_PER_SEC;
	for(int j = 0; j < tabletype[rs].fieldcount; j++){
		cout << "Nhap truong " << tabletype[rs].fieldName[j] << ", hoac nhan ENTER de thoat: ";	
		string line;
		getline(cin,line);
		start = clock();
		if(line == "") break;
		line = checkInput(line);				
		words[i].values[j] = line;
		finish = clock();
		time = time + (float)(finish-start)/CLOCKS_PER_SEC;
	}
	cout << "\n                            --------------DA CAP NHAT THANH CONG--------------\n";
	printf("\n               Thao tac xong trong khoang %.10f giay.\n\n",time);
	getchar();
	system("cls");
}

void editWord(int i){
	while(1){
	    cout << "\n               Chon chuc nang nhap 0 de quay tro lai\n               1.Ten tu\n               2.Ma the loai\n               3.Cac truong\n               Nhap vao mot so 1-3 de chon cac chuc nang: ";
		string line;
		getline(cin,line);
		line = checkInput(line);
		if(line != "0" && line != "1" && line != "2" && line != "3" ){
		    system("cls");
			cout << "\n               Vui long nhap so hop le: ";
			continue;
		}
		else if(line == "0") {
			system("cls");
			break;	
		}
		else if(line == "1") {
		    editNameWord(i); 
		    cout << "\n               Chon chuc nang hoac nhap 0 de quay tro lai\n               1.Ten tu\n               2.Ma the loai\n               3.Cac truong\n               Nhap vao mot so 1-3 de chon cac chuc nang: ";
	    }
		else if(line == "2") {
			editTypeWord(i);
			cout << "\n               Chon chuc nang hoac nhap 0 de quay tro lai\n               1.Ten tu\n               2.Ma the loai\n               3.Cac truong\n               Nhap vao mot so 1-3 de chon cac chuc nang: ";
	    }
		else if(line == "3") {
		    editValuesWord(i);
		    cout << "\n               Chon chuc nang hoac nhap 0 de quay tro lai\n               1.Ten tu\n               2.Ma the loai\n               3.Cac truong\n               Nhap vao mot so 1-3 de chon cac chuc nang: ";
	    }
		system("cls");
	}
}

void editWord(){
    fflush(stdin);
	while(1){
	    system("cls");
	    printLogo();
	    cout << "\n\n               Chuc nang da chon: Sua tu. \n\n";
		if(word::count == 0) {
			cout << "<------------------------DANH SACH TRONG---------------------->" << endl;
			return;	
		}
		string line;
		cout << "               Nhap Tu muon cap nhat hoac 0 de quay lai: ";
		getline(cin,line);
		line = checkInput(line);
		if(line == "0") break;
		int delWordPos = searchWordByName(line,0,word::count);
		if(delWordPos == -1){
			cout << "\n\n                            ------------KHONG TIM THAY TU MUON SUA---------\n\n";
			getchar();
			continue;
		}
		cout << "\n                            ------------------TU DINH SUA-----------------\n\n";
		printWord(delWordPos); 
		cout << "\n                            -----------------------------------------------\n\n";
		editWord(delWordPos);
	}
	system("cls");
}

void addType(type newtype) {
	for(int i = 0; i <= type::count; i++){
		if(newtype.typeCode < tabletype[i].typeCode || i == type::count){
			for(int j = type::count; j > i; j--){
				tabletype[j] = tabletype[j-1]; 
			}
			tabletype[i] = newtype;
			break;
		}
	}	
	type::count ++;
}

void addType(){
	clock_t start, finish;
	double time;
    fflush(stdin);
    printLogo();
    cout << "\n\n               Chuc nang da chon: Them the loai." << endl;
	printTypes();
	cout <<"\n                           -------------------------------------------------\n"<<endl;
	type newtype;
	string line;
	while(1){
		cout << "               Nhap ma the loai chua co trong danh sach: ";
		getline(cin,line);
		line = checkInput(line);
		int code = atoi(line.c_str());
		start = clock();
		int kiemtra = searchTypeByCode(code,0,type::count);
		if(code > 0 && kiemtra == -1){
			newtype.typeCode = code;
			finish = clock();
			break;
		} 
	}
	time = (float)(finish-start)/CLOCKS_PER_SEC;
	do{ fflush(stdin);
		cout << "               Nhap ten the loai chua co trong danh sach: ";
		getline(cin,line); 
		start = clock();
		line = checkInput(line);
		if(searchTypeByName(line)==-1) finish = clock();
	} while(line == "" || searchTypeByName(line) != -1);
	time = time + (float)(finish-start)/CLOCKS_PER_SEC;
	newtype.typeName = line; 
	fflush(stdin);
	cout << "               Nhap cac truong (Phan cach bang dau ';'): ";
	getline(cin,line);  
	start = clock();
	line = checkInput(line);	
	fieldInf res = divString(line);
		
	for(int i = 0; i < res.count;i++) {
		newtype.fieldName[i] = res.lines[i];
	}
	newtype.fieldcount = res.count;
	addType(newtype);
	finish = clock();
	time = time + (float)(finish-start)/CLOCKS_PER_SEC;
	cout <<"\n                           ----------------------DA THEM---------------------\n"<<endl;
	printf("\n               Thao tac xong trong khoang %.10f giay.",time);
	fflush(stdin);
	getchar();
	system("cls");
}



void quickSortTypeCode(int L, int R) {
	 int t= tabletype[(L+R)/2].typeCode;
	 int i=L;
	 int j=R;
	 struct type temp;
	 do { while(tabletype[i].typeCode <t) i++;
          while(tabletype[j].typeCode >t) j--;
               if(i<=j) { temp = tabletype[i];
                          tabletype[i]=tabletype[j];
                          tabletype[j]=temp;
                          i++;
                          j--;
			            }
	    }while(i<j);
	if(L<j) quickSortTypeCode(L,j);
	if(i<R) quickSortTypeCode(i,R);    
}

void editType(){
	clock_t start, finish;
	double time;
	system("cls");
	fflush(stdin);
    printLogo();
    cout << "\n\n               Chuc nang da chon: Sua the loai. \n\n";
	string line;
    int code;
    int code1;
    printTypes();
    do{
	   cout << "               Nhap ten the loai muon sua: ";
	   getline(cin,line); 
	   start = clock();
	   line = checkInput(line);
    }while(searchTypeByName(line)==-1);
	int result = searchTypeByName(line);
	code = tabletype[result].typeCode;
	finish = clock();
	time = (float)(finish-start)/CLOCKS_PER_SEC;
	if(result == -1)
	{
		cout << "\n                           ----------KHONG TIM THAY TEN THE LOAI----------\n\n";
		getchar();
		system("cls");
		return;
	}
	cout << "\n               Chon c/k? de sua hoac khong\n\n";
	
	cout << "               Ban muon sua ten the loai: c/k? ";
	getline(cin,line);
	line = checkInput(line);
	if(line == "c"){
		
		do{
			cout << "               Nhap ten the loai chua co trong danh sach: ";
			getline(cin,line);
			start= clock(); 
			line = checkInput(line);
		}while(line == "" || searchTypeByName(line)!=-1);
		tabletype[result].typeName = line;
		finish = clock();
	}
	time = time + (float)(finish-start)/CLOCKS_PER_SEC;
	cout << "               Ban muon sua ma the loai: c/k? ";
	getline(cin,line);
	line = checkInput(line);
	
	if(line == "c") {
        do{
			cout << "               Nhap ma the loai chua co trong danh sach: ";
			getline(cin,line);
			start = clock();
	        line = checkInput(line);
	        if(line=="0") {
			    code1=0;
		    }
	        else{ 
			    if(atoi(line.c_str())!=0)
			       code1=atoi(line.c_str());
		    }
		}while(line == ""|| searchTypeByCode(code1,0,type::count)!=-1);
		for(int i=0 ; i < word::count; i++) {
		    if( words[i].typeCode == code){
		    	words[i].typeCode = code1;
			}
		}	
		result=searchTypeByCode(code,0,type::count-1);	
		tabletype[result].typeCode = code1;
		finish = clock();
		time = time + (float)(finish-start)/CLOCKS_PER_SEC; 
	}
//	time = time + (float)(finish-start)/CLOCKS_PER_SEC;
	for(int i= 0; i< tabletype[result].fieldcount;i++) {
	    fflush(stdin);
		cout << "               Ban muon sua Ten Truong \""<< tabletype[result].fieldName[i]<< "\": c/k? ";
		getline(cin,line);
		if(line == "k") continue;
		do{ 
			cout << "               Nhap Ten truong moi: ";
			getline(cin,line); 
			start = clock ();
			line = checkInput(line);
		}while(line == "");	
		tabletype[result].fieldName[i]= line;
		finish = clock();
		time = time + (float)(finish-start)/CLOCKS_PER_SEC;
	}
	
	quickSortTypeCode(0,type::count-1);
	cout << "\n                           ---------------DA CAP NHAT THANH CONG--------------\n\n";
	printf("               Thao tac xong trong khoang %.10f giay.",time);
	getchar();
	system("cls");
}

void deleteType(){
    clock_t start, finish;
    double time;
	system("cls");
    printLogo();
    cout << "\n\n               Chuc nang da chon: Xoa the loai.\n\n";
	if(type::count==0) {
	    cout <<"                            ---------------DANH SACH TRONG-----------\n\n";
		return;
	}
	string line;
	fflush(stdin);
	printNameTypes();
	cout << "               Nhap Ten the loai muon xoa: ";
	getline(cin,line);
	start=clock();
	line = checkInput(line);
	if(line=="") {
	    system("cls");
		return;
    }
	int rs =  searchTypeByName(line);
	if(rs != -1){
		int code = tabletype[rs].typeCode;
		for(int i = rs; i < type::count; i++){
			tabletype[i] = tabletype[i+1]; 
		}
		type::count --;	
		for(int i = 0; i < word::count ; i++){
			if(words[i].typeCode == code){
				for(int j = i; j < word::count-1; j++){
					words[j] = words[j+1];
				}
			    i--;	
			    word::count--;
			}
		}	
		cout << "\n                           -----------------DA XOA THANH CONG----------------\n\n";
	}  
	else cout << "\n                           ---------KHONG TIM THAY THE LOAI MUON XOA----------\n\n";
	finish=clock();
	time = (float)(finish-start)/CLOCKS_PER_SEC;
	printf("               Thao tac xong trong khoang %.10f giay.\n",time);
	getchar();
	system("cls");
}
	
void listedWord() {
    clock_t start, finish;
    double time;
	while(1){
		system("cls");
		fflush(stdin);
        printLogo();
        cout << "\n\n               Chuc nang da chon: Thong ke cac tu thuoc cung the loai. \n\n";
        printNameTypes();
		string line;
		cout << "               Nhap ten the loai can thong ke hoac nhap 0 de thoat: ";
		getline(cin,line);
		line = checkInput(line);
		cout << "\n\n";
		if(line == ""){
			cout << "                            -------------Nhap khong hop le!!--------------\n" << endl;
			continue;
		}
		if(line == "0") break;
		start=clock();
		int result = searchTypeByName(line);
		if(result == -1) {
			cout << "                            -------------TEN THE LOAI KHONG DUNG----------\n";
			getchar();
			continue;
		}
		cout << "                           ---------DANH SACH CAC TU CUNG THE LOAI---------"<<endl;
		for(int i = 0; i < word::count;i++){
			if(words[i].typeCode == tabletype[result].typeCode) {
				printWordName(i);
				cout << endl;
			}
		}
		finish=clock();
		time = (float)(finish-start)/CLOCKS_PER_SEC;
		cout << "                           -------------------------------------------------\n"<<endl;
		fflush(stdin);
		printf("               Thong ke xong trong khoang %.10f giay.\n\n",time);
		getchar();
	}
	system("cls");
}
void sayHello(){
	 cout <<"\n\n\n";
	 cout <<"                      *      *      ------*      *         *           *------\n";
	 cout <<"                      |______|      |_____       |         |           /      \\\n";
	 cout <<"                      |      |      |_____       |____     |____       \\______/\n";
	 Sleep(1500);
	 system("cls");
}
void sayGoodbye(){
	 printLogo();
	 cout <<"\n\n";
	 cout <<"            ------*      *------      *------      *------            *------      *      *      ------*\n";
	 cout <<"            |    ___     /      \\     /      \\      |     \\            |____/        \\___/       |_____\n";
	 cout <<"            |_____|      \\______/     \\______/      |_____/            |____\\          |         |_____\n";
     cout <<"\n\n";
}

void menu()
{
	system("COLOR 0A");
	string choose;
	int choose1;
	sayHello();
	while(choose1!=12){
		printLogo();
		cout << "\n\n\n\t                Cac chuc nang cua tu dien: \n\n" << endl;
		cout << "\t                    1. Tim tu.  " << endl;
		cout << "\t                    2. Them tu. " << endl;
		cout << "\t                    3. Them the loai. " << endl;
		cout << "\t                    4. Thong ke cac tu thuoc cung mot the loai. " << endl;
		cout << "\t                    5. Sua tu. " << endl;
		cout << "\t                    6. Xoa tu. " << endl;
		cout << "\t                    7. Xoa the loai. " << endl;
		cout << "\t                    8. Sua the loai. " << endl;
		cout << "\t                    9. In danh sach cac the loai. " << endl;
		cout << "\t                    10. In danh sach cac tu. " << endl;
		cout << "\t                    11. Luu du lieu. " << endl;
		cout << "\t                    12. Thoat chuong trinh. \n\n" << endl;
		fflush(stdin);
		cout << "\t                Chon mot chuc nang tu 1->12 ! ";
		cin >> choose;
		choose = checkInput(choose);
		choose1 = atoi(choose.c_str());
		if( choose1 == 0) {
		    system("cls");
		    cout << "Moi nhap lai!" << endl;
			continue;	    
	    }
	    else {
	        system("cls");
		    switch(choose1){
			    case 1: cin.ignore(); findWord(); break;
			    case 2: cin.ignore(); addWord(); break;
			    case 3: cin.ignore();addType(); break;
			    case 4: cin.ignore(); listedWord(); break;
			    case 5: cin.ignore();editWord(); break; 
			    case 6: cin.ignore(); deleteWord(); break;
			    case 7: cin.ignore();deleteType(); break;
			    case 8: cin.ignore();editType(); break;
			    case 9: {
			    	   clock_t start, finish;
			    	   double time;
			    	   start = clock();
				       printLogo();
					   cout << "\n\n               Chuc nang da chon: In danh sach cac the loai. \n\n";
				       printTypes();
				       finish = clock();
				       time = (float)(finish-start)/CLOCKS_PER_SEC;
				       cout << "\n                            -----------------------------------------------\n";
	                   printf("\n               Thao tac xong trong khoang %.10f giay.\n\n",time);
				       fflush(stdin);
				       getchar();
				       system("cls");
					   break;
				}
			    case 10: printWords(); break;
			    case 11: save();break;
			    case 12: sayGoodbye(); break;
			    default: cout << "Moi nhap lai!" << endl;
		    }
	    }
		fflush(stdin);		
	}
}

int main()
{
	loadTypes();
	loadWords();
	menu();
	return 0;
}
