#include <iostream>
#include <conio.h>
using namespace std;
typedef struct times{
	int gio; int phut; int giay;
} time;
time settime(int h, int m, int s);
void Displaytime(time C);
time operator + (time C1, time C2);
time operator - (time C1, time C2);
time settime(int h,int  m,int  s){
	time Tmp;
	Tmp.gio = h; Tmp.phut = m; Tmp.giay = s; 
	while (Tmp.giay >= 60){
		Tmp.phut = Tmp.phut + 1;
		Tmp.giay = Tmp.giay - 60;
	}
	while (Tmp.phut >=60)
	{
		Tmp.gio = Tmp.gio + 1;
		Tmp.phut = Tmp.phut - 60;
	}
	return Tmp;
}
//Cong hai time
time operator + (time C1, time C2) {
	time Tmp;
	Tmp.gio = C1.gio + C2.gio;
	Tmp.phut = C1.phut + C2.phut;
	Tmp.giay = C1.giay + C2.giay;
	if (Tmp.giay >= 60){ Tmp.phut = Tmp.phut + 1;
	Tmp.giay = Tmp.giay - 60;
	}
	if (Tmp.phut >= 60){ Tmp.gio = Tmp.gio + 1;
	Tmp.phut = Tmp.phut - 60;
	}
	return Tmp;
}
//tru hai time
time operator - (time C1, time C2) {
	time Tmp;
	Tmp.gio = C1.gio - C2.gio;
	Tmp.phut = C1.phut - C2.phut;
	Tmp.giay = C1.giay - C2.giay;
	if (Tmp.giay < 0){
		Tmp.phut = Tmp.phut - 1;
		Tmp.giay = Tmp.giay + 60;
	}
	if (Tmp.phut < 0){
		Tmp.gio = Tmp.gio - 1;
		Tmp.phut = Tmp.phut + 60;
	}
	return Tmp;
}
// hien thi time
void Displaytime(time C)
{
	cout << C.gio <<"h"<< C.phut << "m" << C.giay << "s";
}
int main()
{
	int a, b, c,d,e,f;
	time C1, C2, C3, C4; cout << "nhap thoi gian thu nhat \n"; cout << "nhap so gio "; cin >> a;	cout << "nhap so phut ";	cin >> b;	cout << "nhap so giay ";	cin >> c;
	C1 = settime(a, b, c);
	cout << "nhap thoi gian thu hai \n"; cout << "nhap so gio "; cin >> d;	cout << "nhap so phut ";	cin >> e;	cout << "nhap so giay ";	cin >> f;
	C2 = settime(d, e,f); cout << "\n thoi gian thu nhat:"; Displaytime(C1); cout << "\n thoi gian thu hai:"; Displaytime(C2); C3 = C1 + C2;
	C4 = C1 - C2; cout << "\n Tong hai thoi gian nay:"; Displaytime(C3); cout << "\n Hieu hai thoi gian nay:"; Displaytime(C4);
	int n;
	cout << "\n nhan so bat ky de ket thuc" << endl;
	cin >> n;
	return 0;
}
