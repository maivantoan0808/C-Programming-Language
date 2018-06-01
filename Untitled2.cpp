#include <iostream>
#include <conio.h>
#include <stdio.h>

using namespace std;

int ** allocMatrix(int **A, int rows, int cols) {
	A = new int*[rows];
	for (int i = 0; i < rows; ++i) {
        A[i] = new int[cols];
	}
	if (A == NULL)
		return 0;
	return A;
}

void inputMatrix(int **A, int rows, int cols, char symbol) {
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j) {
            printf("%c[%d][%d] = ", symbol, (i + 1), (j + 1));
			cin >> A[i][j];
		}
}

void displayMatrix(int **A, int rows, int cols) {
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			printf("%4d", A[i][j]);
		}
		cout << endl;
	} cout << endl;
}

void addMatrix(int **A, int **B, int **C, int rows, int cols) {
	for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; j++)
		C[i][j] = A[i][j] + B[i][j];
}

void freeMatrix(int **A, int rows) {
	if (!A) {
		for (int i = 0; i < rows; i++)
			delete[]A[i];
		delete[]A;
	}
}

int main() {

	int cols, rows;
	int **A, **B, **C;
	cout << "Nhap so hang :";
	cin >> rows;
	cout << endl;
	cout << "Nhap so cot: ";
	cin >> cols;
	cout << endl;
	A = allocMatrix(A, rows, cols);
	B = allocMatrix(B, rows, cols);
	C = allocMatrix(C, rows, cols);

	inputMatrix(A, rows, cols, 'A');
	inputMatrix(B, rows, cols, 'B');

	displayMatrix(A, rows, cols);
	displayMatrix(B, rows, cols);

	cout << endl << "Ma tran tong la : " << endl;
	addMatrix(A, B, C, rows, cols);
	displayMatrix(C, rows, cols);

	freeMatrix(A, rows);
	freeMatrix(B, rows);
	freeMatrix(C, rows);

	getch();
	return 1;
}
