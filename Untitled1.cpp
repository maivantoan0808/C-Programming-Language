#include <iostream>
#include <conio.h>
#include <stdio.h>
using namespace std;

int allocMatrix(int **A, int rows, int cols) {
	*A = new int[cols*rows];
	if (*A == NULL)
		return 0;
	return 1;
}

void inputMatrix(int *A, int rows, int cols, char symbol) {
	for(int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j) {
			printf("%c[%d][%d]", symbol, (i + 1), (j + 1));
			cin >> A[i*rows + j];
		}
}

void displayMatrix(int *A, int rows, int cols) {
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			printf("%4d", A[i*rows+j]);
		}
		cout << endl;
	} cout << endl;		
}

void addMatrix(int *A, int *B, int *C, int rows, int cols) {
	for (int i = 0; i < (cols*rows); ++i)
		C[i] = A[i] + B[i];
}

void freeMatrix(int *A) {
	if (!A)
		delete[]A;
}

int main() {
	int cols, rows;
	int *A, *B, *C;
	cout << "Nhap so hang :"; 
	cin >> rows;
	cout << endl;
	cout << "Nhap so cot: ";
	cin >> cols;
	cout << endl;
	allocMatrix(&A, rows, cols);
	allocMatrix(&B, rows, cols);
	allocMatrix(&C, rows, cols);

	inputMatrix(A, rows, cols, 'A');
	inputMatrix(B, rows, cols, 'B');

	displayMatrix(A, rows, cols);
	displayMatrix(B, rows, cols);

	cout << endl << "Ma tran tong la : " << endl;
	addMatrix(A, B, C, rows, cols);
	displayMatrix(C, rows, cols);

	freeMatrix(A);
	freeMatrix(B);
	freeMatrix(C);

	//system("pause");
	//return 1;
}
