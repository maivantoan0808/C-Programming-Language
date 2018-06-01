#include <iostream>
#include <cstring>
#include <fstream>
#include <windows.h>
#include <conio.h>

#define boxx 12 //Vi tri x bat dau cua box
#define boxy 6 //Vi tri y bat dau cua box
#define boxs 54 //Box size
#define boxw 10 //So tu hien thi trong khung ket qua tim kiem

using namespace std;

bool updated = false; //Kiem tra xem co thay doi du lieu hay khong

struct word {
	string tu, nghia[3], loai, dongnghia, vidu[3];
	word *left, *right;
};

struct hashtable {
	word *head, *tail;
};

void gotoxy(short x,short y) { //Ham di chuyen con tro den vi tri nao do trong Console
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = {x, y};
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void setcolor(short x) { //Ham thay doi textcolor va backgroundcolor
	HANDLE hConsoleColor;
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleColor, x);
}

void findfirstword(hashtable *tudien, word *&tuhientai) { //Tim vi tri tu dau tien trong tu dien va gan cho tuhientai
	for(int i = 0; i < 26; i++) {
		if(tudien[i].head != NULL) {
			tuhientai = tudien[i].head;
			break;
		}
	}
}

int taptudien(string tu) { //Tim xem tu nay thuoc tap tu dien bam nao
	if(tu != "") {
		char x = tu[0];
		if((x < 123) && (x > 96)) {
			return x - 97;
		}
	}
	return -1;
}

word *leftword(hashtable *tudien, word *current) { //Tim tu phia truoc cua tu
	if(current != NULL) {
		word *left = current->left;
		if(left != NULL) {
			return left;
		}
		else {
			int x = taptudien(current->tu);
			if(x > 0) {
				while(x > 0) {
					if(tudien[x - 1].tail != NULL) {
						return tudien[x - 1].tail;
					}
					x--;
				}
				return NULL;
			}
			else {
				return NULL;
			}
		}
	}
	else return NULL;
}

word *rightword(hashtable *tudien, word *current) { //Tim tu phia sau cua tu
	if(current != NULL) {
		word *right = current->right;
		if(right != NULL) {
			return right;
		}
		else {
			int x = taptudien(current->tu);
			if(x < 25) {
				while(x != 26) {
					if(tudien[x + 1].head != NULL) {
						return tudien[x + 1].head;
					}
					x++;
				}
				return NULL;
			}
			else {
				return NULL;
			}
		}
	}
	else return NULL;
}

void add(word *&head, word *&tail, word *&newword) {
	word *run = head;
	if(head == NULL) {
		head = tail = newword;
	}
	else {
		while(run != NULL) {
			if(run->tu < newword->tu) {
				if(run->right == NULL) {
					run->right = newword;
					newword->left = run;
					tail = newword;
					break;
				}
				else {
					run = run->right;
				}		
			}
			else {
				if(run == head) {
					head = newword;
					head->right = run;
					run->left = head;
					break;
				}
				else {
					word *tmp = run->left;
					tmp->right = newword;
					newword->left = tmp;
					newword->right = run;
					run->left = newword;
					break;
				}
			}
		}
	}
}

void deleteword(hashtable *&tudien, word *&xoa) {
	if(xoa != NULL) {
		int x = taptudien(xoa->tu);
		word *head = tudien[x].head, *tail = tudien[x].tail;
		if(head->tu == xoa->tu) {
			if (head->right == NULL) {
				head = tail = NULL;
			} else {
				head = head->right;
				head->left = NULL;
			}
		} 
		else if(xoa->tu == tail->tu) {
			tail = tail->left;
			tail->right = NULL;
		}
		else {
			word *left = xoa->left, *right = xoa->right;
			left->right = right;
			right->left = left;
			xoa->left = xoa->right = NULL;
		}
		delete xoa;
		xoa = NULL;
		tudien[x].head = head;
		tudien[x].tail = tail;
	}
}

void deletewordcurrent(hashtable *&tudien, word *&tuhientai) {
	if(tuhientai != NULL) {
		word *left = leftword(tudien, tuhientai), *right = rightword(tudien, tuhientai), *tmp = tuhientai;
		if((left == NULL) && (right == NULL)) {
			tuhientai = NULL;
		}
		else {
			if(left == NULL) {
				tuhientai = right;
			}
			else {
				tuhientai = left;
			}
		}
		deleteword(tudien, tmp);
	}
}

void printscreen(word *tmp) { //In 1 tu ra man hinh tim kiem
	if(tmp != NULL) {
		cout << tmp->tu;
	}
}

void displaycurrent(word *tmp) { //Hien thi tu dau tren trong khung ket qua tim kiem
	if(tmp != NULL) {
		cout << " " << tmp->tu;
		for(int i = tmp->tu.length(); i < boxs - 1; i++) cout << " ";
	}
}

void displayword(hashtable *tudien, word *hientai) { //Hien thi danh sach tu trong ket qua tim kiem
	if(hientai == NULL) {
		for(int i = 0; i < boxw; i++) { //Xoa sach khung hien thi ket qua tim kiem
			gotoxy(boxx + 1, boxy + 3 + i);
			for(int j = 0; j < boxs; j++) cout << " ";
		}
	}
	if(hientai != NULL) {
		for(int i = 1; i < boxw; i++) { //Xoa sach khung hien thi ket qua tim kiem
			gotoxy(boxx + 2, boxy + 3 + i);
			for(int j = 1; j < boxs - 1; j++) cout << " ";
		}
		for(int i = 0; i < boxw; i++) {
			if(i == 0) {
				gotoxy(boxx+1, boxy+3+i);
				setcolor(115);
				displaycurrent(hientai);
				setcolor(63);
			}
			else {
				gotoxy(boxx+2, boxy+3+i);
				printscreen(hientai);
			}
			hientai = rightword(tudien, hientai);
		}
	}
}

bool compare(string cantim, word *dangdo) {
	string x = cantim, y = dangdo->tu;
	if(x.length() > y.length()) {
		return false;
	}
	else {
		for(int i = 0; i < x.length(); i++) {
			if(x[i] != y[i]) {
				return false;
			}
		}
		return true;
	}
}

word *search(hashtable *tudien, string searchword) { //Tim kiem tu va tra ve vi tri cua tu tim duoc
	int x = taptudien(searchword);
	if(x < 0) {
		return NULL;
	}
	else {
		word *head = tudien[x].head;
		while(head != NULL) {
			if(compare(searchword, head)) {
				return head;
			}
			head = head->right;
		}
		return NULL;
	}
}

void menu1() {
	gotoxy(18,2);
	setcolor(63);
	cout << "Tab: Them tu moi";
	gotoxy(18,3);
	cout << "Esc: Ket thuc chuong trinh";
	setcolor(63);
}

void menu2() {
	gotoxy(18,2);
	setcolor(63);
	cout << "Backspace: Chinh sua tu nay";
	gotoxy(18,3);
	cout << "Delete: Xoa tu nay";
	gotoxy(18,4);
	cout << "Esc: Quay lai khung tim kiem";
	setcolor(63);
}

void menu3() {
	gotoxy(18,2);
	setcolor(63);
	cout << "Esc: Quay lai tu phia truoc";
	setcolor(63);
}

void menu4() {
	gotoxy(18,2);
	setcolor(63);
	cout << "Esc: Quay lai khung tim kiem";
	setcolor(63);
}

void menu5() {
	gotoxy(18,2);
	setcolor(63);
	cout << "Esc: Quay lai tu dang xem";
	setcolor(63);
}

bool confirmdel(word *&tu) {
	system("cls");
	gotoxy(boxx, boxy);
	cout << "Ban co chac muon xoa tu \"" << tu->tu << "\" khong?";
	gotoxy(boxx, boxy + 1);
	cout << "Nhan phim Enter de xoa!";
	gotoxy(boxx, boxy + 1);
	cout << "Nhan phim Esc de quay tro lai!";
	int key = 0;
	while(key != 13 && key != 27) key = getch();
	system("cls");
	if(key == 13) return true;
	else if(key == 27) return false;
}

void delerror() {
	gotoxy(18, boxy + boxw + 2);
	for(int i = 0; i < 40; i++) cout << " ";
}

void printerror(short k) {
	delerror();
	string e;
	if(k == 1) e = "Bat buoc nhap tu!";
	else if(k == 2) e = "Tu nay da ton tai!";
	else if(k == 3) e = "Vui long nhap nghia cua tu!";
	else if(k == 4) e = "Vui long nhap loai tu!";
	setcolor(62);
	gotoxy(18, boxy + boxw + 2); cout << "ERROR: " << e;
	setcolor(63);
}

string fixtext(string t) {
	if(t != "") {
		while(t[0] == ' ') t.erase(0, 1); //Xoa khoang trang o dau
		while(t[t.length() - 1] == ' ') t.erase(t.length() - 1, 1); //Xoa khoang trang o cuoi
		if(t.length() > 3) //Xoa khoang trang thua o giua
			for(short i = 1; i < t.length() - 2; i++)
				if(t[i] == ' ' && t[i+1] == ' ') {
					t.erase(i, 1);  i--;
				}
		for(short i = 0; i < t.length(); i++) if(t[i] >= 'A' && t[i] <= 'Z') t[i] += 32; //Chuyen chu hoa thanh chu thuong
		return t;
	}
	else return t;
}

short checktext(hashtable *tudien, string t, short k) {
	t = fixtext(t);
	if(k == 1) {
		if(t == "") return 1;
		else if(search(tudien, t) != NULL) {
			if(search(tudien, t)->tu.length() == t.length()) return 2;
		}
	}
	else if(k == 2) {
		if(t == "") return 3;
	}
	else if(k == 5) {
		if(t == "") return 4;
	}
	return 0;
}

void addwordbox() {
	setcolor(55);
	gotoxy(boxx, boxy); cout << char(201);
	for(int i = 1; i < boxs + 1; i++) cout << char(205);
	cout << char(187);
	gotoxy(boxx, boxy + 1); cout << char(186) << "            Tu:";
	gotoxy(boxx + boxs + 1, boxy + 1); cout << char(186);
	gotoxy(boxx, boxy + 2); cout << char(186) << "       Nghia 1:";
	gotoxy(boxx + boxs + 1, boxy + 2); cout << char(186);
	gotoxy(boxx, boxy + 3); cout << char(186) << "       Nghia 2:";
	gotoxy(boxx + boxs + 1, boxy + 3); cout << char(186);
	gotoxy(boxx, boxy + 4); cout << char(186) << "       Nghia 3:";
	gotoxy(boxx + boxs + 1, boxy + 4); cout << char(186);
	gotoxy(boxx, boxy + 5); cout << char(186) << "       Loai tu:";
	gotoxy(boxx + boxs + 1, boxy + 5); cout << char(186);
	gotoxy(boxx, boxy + 6); cout << char(186) << " Tu dong nghia:";
	gotoxy(boxx + boxs + 1, boxy + 6); cout << char(186);
	gotoxy(boxx, boxy + 7); cout << char(186) << "       Vi du 1:";
	gotoxy(boxx + boxs + 1, boxy + 7); cout << char(186);
	gotoxy(boxx, boxy + 8); cout << char(186) << "       Vi du 2:";
	gotoxy(boxx + boxs + 1, boxy + 8); cout << char(186);
	gotoxy(boxx, boxy + 9); cout << char(186) << "       Vi du 3:";
	gotoxy(boxx + boxs + 1, boxy + 9); cout << char(186);
	gotoxy(boxx, boxy + 10); cout << char(200);
	for(int i = 1; i < boxs + 1; i++) cout << char(205);
	cout << char(188);
	setcolor(63);
}

void printtextfield(string t, short k) {
	gotoxy(boxx + 17, boxy + k);
	if(t.length() >= boxs - 17) {
		for(int i = 0; i < boxs - 17; i++) cout << t[i];
	}
	else if(t.length() < boxs - 17) {
		cout << t;
		for(int i = 0; i < boxs - 17 - t.length(); i++) cout << " ";
	}
}

void addword(hashtable *&tudien) {
	system("cls");
	int keyhit;
	short dem;
	string text, field[9];
	field[0] = "";
	field[1] = "";
	field[2] = "";
	field[3] = "";
	field[4] = "";
	field[5] = "";
	field[6] = "";
	field[7] = "";
	field[8] = "";
	addwordbox();
	menu4();
	for(int k = 1; k < 10; k++) {
		keyhit = 0;
		dem = field[k-1].length();
		text = field[k-1];
		while(keyhit != 13) {
			gotoxy(boxx + 17, boxy + k);
			if(dem < boxs - 17) {
				cout << text;
				for(int i = 1; i < boxs - 16 - dem; i++) cout << " ";
			}
			else {
				for(int i = dem - boxs + 17; i < dem; i++) cout << text[i];
			}
			if(dem < boxs - 17) gotoxy(boxx + 17 + dem, boxy + k); //Xuat ra vi tri con tro khi khung nhap chua bi tran
			else gotoxy(boxx + boxs, boxy + k); //Xuat ra vi tri con tro khi khung nhap bi tran
			keyhit = getch();
			if(keyhit == 224) {
				keyhit = getch();
				if(keyhit == 72) {
					if(checktext(tudien, text, k) != 0) {
						keyhit = 0;
						printerror(checktext(tudien, text, k));
					}
					else {
						text = fixtext(text);
						printtextfield(text, k);
						field[k-1] = text;
						delerror();
						if(k > 1) k--;
						dem = field[k-1].length();
						text = field[k-1];
					}
				}
				else if(keyhit == 80) {
					if(checktext(tudien, text, k) != 0) {
						keyhit = 0;
						printerror(checktext(tudien, text, k));
					}
					else {	
						text = fixtext(text);
						printtextfield(text, k);
						field[k-1] = text;
						delerror();
						if(k < 9) k++;
						dem = field[k-1].length();
						text = field[k-1];
					}
				}
			}
			else if(keyhit == 8) {
				if(dem > 0 && dem < boxs - 16) {
					dem--;
					text = text.substr(0, text.size() - 1);
				}
				else if(dem > 0) {
					dem--;
					text = text.substr(0, text.size() - 1);
				}
			}
			else if(dem < boxs - 17 && ((keyhit > 96 && keyhit < 123) || (keyhit > 64 && keyhit < 91) || (keyhit == 32) || (keyhit == 39) || (keyhit == 45) || (keyhit == 63) || (keyhit == 33))) {
				if(keyhit == 63 || keyhit == 33) {
					if(k == 7 || k == 8 || k == 9) {
						dem++;
						text += char(keyhit);
					}
				}
				else {
					dem++;
					text += char(keyhit);
				}
			}
			else if((keyhit > 96 && keyhit < 123) || (keyhit > 64 && keyhit < 91) || (keyhit == 32) || (keyhit == 39) || (keyhit == 45) || (keyhit == 63) || (keyhit == 33)) {
				if(keyhit == 63 || keyhit == 33) {
					if(k == 7 || k == 8 || k == 9) {
						dem++;
						text += char(keyhit);
					}
				}
				else {
					dem++;
					text += char(keyhit);
				}
			}
			else if(keyhit == 27) break;
			else if(keyhit == 13) if(checktext(tudien, text, k) != 0) {
				keyhit = 0;
				printerror(checktext(tudien, text, k));
			}
		}
		if(keyhit == 27) break;
		text = fixtext(text);
		printtextfield(text, k);
		field[k-1] = text;
		delerror();
	}
	if(keyhit != 27) {
		word *tu = new word();
		tu->tu = field[0];
		tu->nghia[0] = field[1];
		tu->nghia[1] = field[2];
		tu->nghia[2] = field[3];
		tu->loai = field[4];
		tu->dongnghia = field[5];
		tu->vidu[0] = field[6];
		tu->vidu[1] = field[7];
		tu->vidu[2] = field[8];
		add(tudien[taptudien(tu->tu)].head,tudien[taptudien(tu->tu)].tail,tu);
		updated = true;
	}
	system("cls");
}

void editword(hashtable *tudien, word *&hientai) {
	system("cls");
	int keyhit;
	short dem;
	string text, field[9];
	field[0] = hientai->tu;
	field[1] = hientai->nghia[0];
	field[2] = hientai->nghia[1];
	field[3] = hientai->nghia[2];
	field[4] = hientai->loai;
	field[5] = hientai->dongnghia;
	field[6] = hientai->vidu[0];
	field[7] = hientai->vidu[1];
	field[8] = hientai->vidu[2];
	addwordbox();
	menu5();
	for(short k = 1; k < 10; k++) { //In gia tri cua cac field ra man hinh
		text = field[k-1];
		dem = field[k-1].length();
		gotoxy(boxx + 17, boxy + k);
		if(dem < boxs - 17) {
			cout << text;
			for(int i = 1; i < boxs - 16 - dem; i++) cout << " ";
		}
		else {
			for(int i = 0; i < boxs - 17; i++) cout << text[i];
		}
	}
	for(short k = 1; k < 10; k++) {
		keyhit = 0;
		dem = field[k-1].length();
		text = field[k-1];
		while(keyhit != 13) {
			gotoxy(boxx + 17, boxy + k);
			if(dem < boxs - 17) {
				cout << text;
				for(int i = 1; i < boxs - 16 - dem; i++) cout << " ";
			}
			else {
				for(int i = dem - boxs + 17; i < dem; i++) cout << text[i];
			}
			if(dem < boxs - 17) gotoxy(boxx + 17 + dem, boxy + k); //Xuat ra vi tri con tro khi khung nhap chua bi tran
			else gotoxy(boxx + boxs, boxy + k); //Xuat ra vi tri con tro khi khung nhap bi tran
			keyhit = getch();
			if(keyhit == 224) {
				keyhit = getch();
				if(keyhit == 72) {
					if(k != 1) {
						if(checktext(tudien, text, k) != 0) {
							keyhit = 0;
							printerror(checktext(tudien, text, k));
						}
						else {
							text = fixtext(text);
							printtextfield(text, k);
							field[k-1] = text;
							delerror();
							if(k > 1) k--;
							dem = field[k-1].length();
							text = field[k-1];
						}
					}
					else if(fixtext(text) != field[0]) {
						if(checktext(tudien, text, k) != 0) {
							keyhit = 0;
							printerror(checktext(tudien, text, k));
						}
						else {
							text = fixtext(text);
							printtextfield(text, k);
							field[k-1] = text;
							delerror();
							if(k > 1) k--;
							dem = field[k-1].length();
							text = field[k-1];
						}
					}
					else {
						text = fixtext(text);
						printtextfield(text, k);
						field[k-1] = text;
						delerror();
						if(k > 1) k--;
						dem = field[k-1].length();
						text = field[k-1];
					}
				}
				else if(keyhit == 80) {
					if(k != 1) {
						if(checktext(tudien, text, k) != 0) {
							keyhit = 0;
							printerror(checktext(tudien, text, k));
						}
						else {
							text = fixtext(text);
							printtextfield(text, k);
							field[k-1] = text;
							delerror();
							if(k < 9) k++;
							dem = field[k-1].length();
							text = field[k-1];
						}
					}
					else if(fixtext(text) != field[0]) {
						if(checktext(tudien, text, k) != 0) {
							keyhit = 0;
							printerror(checktext(tudien, text, k));
						}
						else {
							text = fixtext(text);
							printtextfield(text, k);
							field[k-1] = text;
							delerror();
							if(k < 9) k++;
							dem = field[k-1].length();
							text = field[k-1];
						}
					}
					else {
						text = fixtext(text);
						printtextfield(text, k);
						field[k-1] = text;
						delerror();
						if(k < 9) k++;
						dem = field[k-1].length();
						text = field[k-1];
					}
				}
			}
			else if(keyhit == 8) {
				if(dem > 0 && dem < boxs - 16) {
					dem--;
					text = text.substr(0, text.size() - 1);
				}
				else if(dem > 0) {
					dem--;
					text = text.substr(0, text.size() - 1);
				}
			}
			else if(dem < boxs - 17 && ((keyhit > 96 && keyhit < 123) || (keyhit > 64 && keyhit < 91) || (keyhit == 32) || (keyhit == 39) || (keyhit == 45) || (keyhit == 63) || (keyhit == 33))) {
				if(keyhit == 63 || keyhit == 33) {
					if(k == 7 || k == 8 || k == 9) {
						dem++;
						text += char(keyhit);
					}
				}
				else {
					dem++;
					text += char(keyhit);
				}
			}
			else if((keyhit > 96 && keyhit < 123) || (keyhit > 64 && keyhit < 91) || (keyhit == 32) || (keyhit == 39) || (keyhit == 45) || (keyhit == 63) || (keyhit == 33)) {
				if(keyhit == 63 || keyhit == 33) {
					if(k == 7 || k == 8 || k == 9) {
						dem++;
						text += char(keyhit);
					}
				}
				else {
					dem++;
					text += char(keyhit);
				}
			}
			else if(keyhit == 27) break;
			else if(keyhit == 13) {
				if(k != 1) {
					if(checktext(tudien, text, k) != 0) {
						keyhit = 0;
						printerror(checktext(tudien, text, k));
					}	
				}
				else if(fixtext(text) != field[0]) { //Neu k = 1, text khong thay doi thi khong can check...
					if(checktext(tudien, text, k) != 0) {
						keyhit = 0;
						printerror(checktext(tudien, text, k));
					}
				}
			}
		}
		if(keyhit == 27) break;
		text = fixtext(text);
		printtextfield(text, k);
		field[k-1] = text;
		delerror();
	}
	if(keyhit != 27) {
		hientai->tu = field[0];
		hientai->nghia[0] = field[1];
		hientai->nghia[1] = field[2];
		hientai->nghia[2] = field[3];
		hientai->loai = field[4];
		hientai->dongnghia = field[5];
		hientai->vidu[0] = field[6];
		hientai->vidu[1] = field[7];
		hientai->vidu[2] = field[8];
		updated = true;
	}
	system("cls");
}

void dicbox() { //Ham ve khung nhap va khung ket qua tim kiem
	setcolor(55);
	gotoxy(boxx, boxy); cout << char(201);
	for(int i = 1; i < boxs + 1; i++) cout << char(205);
	cout << char(187);
	gotoxy(boxx, boxy + 1); cout << char(186) << " Tim kiem:";
	gotoxy(boxx + boxs + 1, boxy + 1); cout << char(186);
	gotoxy(boxx, boxy + 2); cout << char(204);
	for(int i = 1; i < boxs + 1; i++) {
		cout << char(205);
	}
	cout << char(185);
	for(int i = boxy + 3; i < boxy + 3 + boxw; i++) {
		gotoxy(boxx, i);
		cout << char(186);
		gotoxy(boxx + boxs + 1, i);
		cout << char(186);
	}
	gotoxy(boxx, boxy + boxw + 3); cout << char(200);
	for(int i = 1; i < boxs + 1; i++) cout << char(205);
	cout << char(188);
	setcolor(63);
}

void stringout(string n, short d) { //Ham xuat ra chuoi string va tu dong xuong dong trong khung xem tu chi tiet
	short dem = 0;
	for(int i = 0; i < n.length(); i++) {
		cout << n[i];
		if((i + 1) % (boxs - 2) == 0) {
			dem++;
			gotoxy(boxx + 2, boxy + d + dem);
		}
	}
}

short linecount(string n) { //Ham dem xem de xuat ra tu nay can bao nhieu dong
	if(n == "") return 0;
	else {
		short d = n.length() / (boxs - 2);
		short e = n.length() % (boxs - 2);
		if(e != 0) d++;
		return d;
	}
}

void detailbox(word *hientai) { //Ham ve khung hien thi thong tin tu can tra
	setcolor(55);
	string nghia = "nghia: ";
	nghia += hientai->nghia[0];
	if(hientai->nghia[1] != "") nghia = nghia + "; " + hientai->nghia[1];
	if(hientai->nghia[2] != "") nghia = nghia + "; " + hientai->nghia[2];
	short d = 2;
	if(hientai->dongnghia != "") d += linecount("Tu dong nghia: " + hientai->dongnghia + " (Tab: Xem tu dong nghia)");
	d += linecount(nghia);
	if(hientai->vidu[0] != "") d += linecount("Vi du 1: " + hientai->vidu[0]);
	if(hientai->vidu[1] != "") d += linecount("Vi du 2: " + hientai->vidu[1]);
	if(hientai->vidu[2] != "") d += linecount("Vi du 3: " + hientai->vidu[2]);
	gotoxy(boxx, boxy); cout << char(201);
	for(int i = 1; i < boxs + 1; i++) cout << char(205);
	cout << char(187);
	for(int i = boxy + 1; i < boxy + 1 + d; i++) {
		gotoxy(boxx, i);
		cout << char(186);
		gotoxy(boxx + boxs + 1, i);
		cout << char(186);
	}
	gotoxy(boxx, boxy + d + 1); cout << char(200);
	for(int i = 1; i < boxs + 1; i++) cout << char(205);
	cout << char(188);
	setcolor(63);
}

void notsynonym() {
	system("cls");
	gotoxy(boxx, boxy);
	cout << "Khong co thong tin cua tu dong nghia";
	gotoxy(boxx, boxy + 1);
	cout << "Nhan phim Esc de quay lai tu phia truoc";
	getch();
	system("cls");
}

void synonym(hashtable *tudien, word *hientai) {
	system("cls");
	short keyhit = 0;
	while(keyhit != 27) {
		if(hientai != NULL) {
			short d = 3;
			menu3();
			detailbox(hientai);
			gotoxy(boxx + 2, boxy + 1); //In tu dang xem
			cout << hientai->tu;
			gotoxy(boxx + 2, boxy + 2); //In loai tu
			cout << "Loai tu: " << hientai->loai;
			if(hientai->dongnghia != "") { //In tu dong nghia
				gotoxy(boxx + 2, boxy + d);
				d += linecount("Tu dong nghia: " + hientai->dongnghia + " (Tab: Xem tu dong nghia)");
				stringout("Tu dong nghia: " + hientai->dongnghia + " (Tab: Xem tu dong nghia)",3);
			}
			gotoxy(boxx + 2, boxy + d); //In ra cac nghia
			string nghia = "Nghia: "; 
			nghia += hientai->nghia[0];
			if(hientai->nghia[1] != "") nghia = nghia + "; " + hientai->nghia[1];
			if(hientai->nghia[2] != "") nghia = nghia + "; " + hientai->nghia[2];
			stringout(nghia, d);
			d += linecount(nghia);
			if(hientai->vidu[0] != "") { //In ra vi du 1
				gotoxy(boxx + 2, boxy + d);
				stringout("Vi du 1: " + hientai->vidu[0], d);
				d += linecount("Vi du 1: " + hientai->vidu[0]);
			}
			if(hientai->vidu[1] != "") { //In ra vi du 2
				gotoxy(boxx + 2, boxy + d);
				stringout("Vi du 2: " + hientai->vidu[1], d);
				d += linecount("Vi du 2: " + hientai->vidu[1]);
			}
			if(hientai->vidu[2] != "") { //In ra vi du 3
				gotoxy(boxx + 2, boxy + d);
				stringout("Vi du 3: " + hientai->vidu[2], d);
			}
			keyhit = getch();
    		if(hientai->dongnghia != "" && keyhit == 9 && search(tudien, hientai->dongnghia) != NULL) {
    			if(hientai->dongnghia.length() == search(tudien, hientai->dongnghia)->tu.length()) synonym(tudien, search(tudien, hientai->dongnghia));
    			else notsynonym();
			}
			else if(hientai->dongnghia != "" && keyhit == 9 && search(tudien, hientai->dongnghia) == NULL) notsynonym();
		}
		else {
			keyhit = 27;
		}
	}
	system("cls");
}

void detailword(hashtable *tudien, word *&hientai) { //Ham xuat ra man hinh thong tin chi tiet cua tu can tra
	system("cls");
	short keyhit = 0;
	while(keyhit != 27) {
		if(hientai != NULL) {
			short d = 3;
			menu2();
			detailbox(hientai);	
			gotoxy(boxx + 2, boxy + 1); //In tu dang xem
			cout << hientai->tu;
			gotoxy(boxx + 2, boxy + 2); //In loai tu
			cout << "Loai tu: " << hientai->loai; 
			if(hientai->dongnghia != "") { //In tu dong nghia
				gotoxy(boxx + 2, boxy + d);
				d += linecount("Tu dong nghia: " + hientai->dongnghia + " (Tab: Xem tu dong nghia)");
				stringout("Tu dong nghia: " + hientai->dongnghia + " (Tab: Xem tu dong nghia)",3);
			}
			gotoxy(boxx + 2, boxy + d); //In ra cac nghia
			string nghia = "Nghia: "; 
			nghia += hientai->nghia[0];
			if(hientai->nghia[1] != "") nghia = nghia + "; " + hientai->nghia[1];
			if(hientai->nghia[2] != "") nghia = nghia + "; " + hientai->nghia[2];
			stringout(nghia, d);
			d += linecount(nghia);
			if(hientai->vidu[0] != "") { //In ra vi du 1
				gotoxy(boxx + 2, boxy + d);
				stringout("Vi du 1: " + hientai->vidu[0], d);
				d += linecount("Vi du 1: " + hientai->vidu[0]);
			}
			if(hientai->vidu[1] != "") { //In ra vi du 2
				gotoxy(boxx + 2, boxy + d);
				stringout("Vi du 2: " + hientai->vidu[1], d);
				d += linecount("Vi du 2: " + hientai->vidu[1]);
			}
			if(hientai->vidu[2] != "") { //In ra vi du 3
				gotoxy(boxx + 2, boxy + d);
				stringout("Vi du 2: " + hientai->vidu[2], d);
			}
    		keyhit = getch();
    		if(keyhit == 8) editword(tudien, hientai);
    		else if(keyhit == 83) {
    			if(confirmdel(hientai)) {
    				deletewordcurrent(tudien, hientai);
    				keyhit = 27;
					updated = true;
				}
			}
    		else if(hientai->dongnghia != "" && keyhit == 9 && search(tudien, hientai->dongnghia) != NULL) {
    			if(hientai->dongnghia.length() == search(tudien, hientai->dongnghia)->tu.length()) synonym(tudien, search(tudien, hientai->dongnghia));
    			else notsynonym();
			}
			else if(hientai->dongnghia != "" && keyhit == 9 && search(tudien, hientai->dongnghia) == NULL) notsynonym();
		}
		else {
			keyhit = 27;
		}
	}
	system("cls");
}

void searchbox(hashtable *&tudien, word *&tuhientai) { //Ham khung nhap tu khoa va ket qua tim kiem
	setcolor(63);
	bool changed = false;
	string intext = "";
	short dem = 0;
	int keyhit = 0;
	while(keyhit != 27) {
		menu1();
		dicbox();
		
		gotoxy(boxx + 12, boxy + 1); //Viet lai nhung chu cai da nhap trong khung tim kiem
		if(dem < boxs - 12) cout << intext;
		else for (int i = dem - boxs + 12; i < dem; i++) cout << intext[i];
		
		while(keyhit != 13 || tuhientai == NULL) {
			if(changed) {
				if(fixtext(intext) != "") tuhientai = search(tudien, fixtext(intext));
				else findfirstword(tudien, tuhientai);
			} 
			displayword(tudien, tuhientai); //Hien thi ket qua tim kiem
			
			if(dem < boxs - 12) gotoxy(boxx + 12 + dem, boxy + 1); //Xuat ra vi tri con tro khi khung nhap chua bi tran
			else gotoxy(boxx + boxs, boxy + 1); //Xuat ra vi tri con tro khi khung nhap bi tran
			
			keyhit = getch();
			if(keyhit == 224) {
				keyhit = getch();
				if(keyhit == 72) {
					if(leftword(tudien, tuhientai) != NULL) tuhientai = leftword(tudien, tuhientai);
				}
				else if(keyhit == 80) {
					if(rightword(tudien, tuhientai) != NULL) tuhientai = rightword(tudien, tuhientai);
				}
				changed = false;
			}
			else if(keyhit == 8) { //Kiem tra phim backspace de xoa...
				if(dem > 0 && dem < boxs - 11) {
					dem--;
					intext = intext.substr(0, intext.size() - 1); //Xoa ky tu cuoi trong string
					gotoxy(boxx + 12, boxy + 1);
					cout << intext;
					for(int i = 1; i < boxs - 11 - dem; i++) cout << " ";
				}
				else if(dem > 0) {
					dem--;
					intext = intext.substr(0, intext.size() - 1);
					gotoxy(boxx + 12, boxy + 1);
					for(int i = dem - boxs + 12; i < dem; i++) cout << intext[i];
				}
				changed = true;
			}
			else if(dem < boxs - 12 && ((keyhit > 96 && keyhit < 123) || (keyhit > 64 && keyhit < 91) || (keyhit == 32) || (keyhit == 39) || (keyhit == 45))) {
				dem++;
				intext += char(keyhit);
				gotoxy(boxx + 12, boxy + 1);
				cout << intext;
				for(int i = 1; i < boxs - 11 - dem; i++) cout << " ";
				changed = true;
			}
			else if((keyhit > 96 && keyhit < 123) || (keyhit > 64 && keyhit < 91) || (keyhit == 32) || (keyhit == 39) || (keyhit == 45)) {
				dem++;
				intext += char(keyhit);
				gotoxy(boxx + 12, boxy + 1);
				for (int i = dem - boxs + 12; i < dem; i++) cout << intext[i];
				changed = true;
			}
			else if(keyhit == 27) break;
			else if(keyhit == 9) break;
	    }
	    if(keyhit == 13) {
	   	 	detailword(tudien, tuhientai);
	   	 	keyhit = 0;
		}
		else if(keyhit == 9) {
			addword(tudien);
		}
		else {
			system("cls");
			gotoxy(boxx, boxy); cout << "Nhan Esc mot lan nua de thoat chuong trinh";
			gotoxy(boxx, boxy + 1); cout << "Nhan bat ky phim nao khac de quay lai chuong trinh";
			keyhit = getch();
			if(keyhit != 27) {
				if(keyhit == 224) keyhit = getch();
				keyhit = 0;
			}
			system("cls");
		}
    }
}
/* END Layout */

void addlast(word *&head, word *&tail, word *&newword) {
	if(head == NULL) {
		head = tail = newword;
	}
	else {
		tail->right = newword;
		newword->left = tail;
		tail = newword;
	}
}

void tachchuoi(string line, word *&tmp) { //Ham tach chuoi ra thanh va gan vao nhung vi tri phu hop
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
	while(line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->dongnghia = tach;
	tach = "";

	while(line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->nghia[0] = tach;
	tach = "";

	while(line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->nghia[1] = tach;
	tach = "";
	while(line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->nghia[2] = tach;
	tach = "";
	while(line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->vidu[0] = tach;
	tach = "";
	while(line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->vidu[1] = tach;
	tach = "";
	while(line[i] != '@') {
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
			addlast(tudien[i - 97].head, tudien[i - 97].tail, newword);
			break;
		}
	}
}

void readf(hashtable *&tudien, word *&tuhientai) { //Doc file
	ifstream fi("data.txt");
	string line;
	if(fi.is_open()) {
		while(getline(fi, line)) {
			importword(tudien, line);
		}
		fi.close();
	}
	findfirstword(tudien, tuhientai);
}

void writef(hashtable *tudien) { //Ghi file
	ofstream fo("data.txt");
	if(fo.is_open()) {
		for(int i = 0; i < 26; i++) {
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
	system("color 3f");
	SetConsoleTitle("Dictionary - ZFULL.net");
	word *tuhientai = NULL;
	hashtable *tudien = new hashtable[26];
	for(int i = 0; i < 26; i++) {
		tudien[i].tail = NULL;
		tudien[i].head = NULL;
	};
	readf(tudien, tuhientai);
	searchbox(tudien, tuhientai);
	if(updated) writef(tudien);
	gotoxy(0, boxw+10);
	return 0;
}
