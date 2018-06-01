#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

struct word {
	string tu, nghia[3], loai, dongnghia, vidu[3];
	word *left, *right;
};

struct hashtable {
	word *head, *tail;
};

void addlast(word *&head, word *&tail, word *&newword) {
	word *run = head;
	if(head == NULL) {
		head = tail = newword;
	}
	else {
		tail->right = newword;
		newword->left = tail;
		tail = newword;
	}
}

void tachchuoi(string line, word *&tmp){
	int d = 0, i = 0;
	string tach = "";
	while(line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->tu = tach;
	tach = "";
	while(line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->loai = tach;
	tach = "";
	while(line[i] != '@'){
		tach += line[i];
		i++;
	}
	i++;
	tmp->dongnghia = tach;
	tach = "";

	while(line[i] != '@'){
		tach += line[i];
		i++;
	}
	i++;
	tmp->nghia[0] = tach;
	tach = "";

	while(line[i] != '@'){
		tach += line[i];
		i++;
	}
	i++;
	tmp->nghia[1] = tach;
	tach = "";
	while(line[i] != '@'){
		tach += line[i];
		i++;
	}
	i++;
	tmp->nghia[2] = tach;
	tach = "";
	while(line[i] != '@'){
		tach += line[i];
		i++;
	}
	i++;
	tmp->vidu[0] = tach;
	tach = "";
	while(line[i] != '@'){
		tach += line[i];
		i++;
	}
	i++;
	tmp->vidu[1] = tach;
	tach = "";
	while(line[i] != '@'){
		tach += line[i];
		i++;
	}
	i++;
	tmp->vidu[2] = tach;
}

void importword(hashtable *&tudien, string line) {
	for(int i = 97; i < 124; i++) {
		if(line[0] == char(i)) {
			word *newword = new word();
			tachchuoi(line, newword);
			newword->left = NULL;
			newword->right = NULL;
			addlast(tudien[i - 97].head, tudien[97-i].tail, newword);
			break;
		}
	}
}

void readf(hashtable *&tudien, word *&tuhientai) {
	ifstream fi("data.txt");
	string line;
	if(fi.is_open()) {
		while(getline(fi, line)) {
			importword(tudien, line);
		}
		fi.close();
	}
	for(int i = 0; i < 26; i++) { //Thac mac....
		if(tudien[i].head != NULL) {
			tuhientai = tudien[i].head;
			break;
		}
	}
}

void writef(hashtable *tudien) {
	ofstream fo("data.txt");
	if(fo.is_open()) {
		for(int i=0; i < 26; i++) {
			word *head = tudien[i].head;
			while(head != NULL) {
				fo << head->tu
					<< "@" << head->loai
					<< "@" << head->dongnghia
					<< "@" << head->nghia[0]
					<< "@" << head->nghia[1]
					<< "@" << head->nghia[2]
					<< "@" << head->vidu[0]
					<< "@" << head->vidu[1]
					<< "@" << head->vidu[2] << "@\n";
				head = head->right;
			}
		}
		fo.close();
	}
}

int main() {
	word *tuhientai = NULL;
	hashtable *tudien = new hashtable[26];
	for(int i = 0; i < 26; i++) {
		tudien[i].tail = NULL;
		tudien[i].head = NULL;
	};
	readf(tudien, tuhientai);
 	writef(tudien);
	return 0;
}
