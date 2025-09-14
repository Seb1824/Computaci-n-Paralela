#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
using namespace std;

#define MAX 500

int main() {
	vector<vector<double>> A(MAX, vector<double>(MAX));
	vector<double> x(MAX);
	vector<double> y(MAX);
	int i, j;

	for (i = 0; i < MAX; i++) {
		x[i] = 1.0; 
		for (j = 0; j < MAX; j++) {
			A[i][j] = (i + j) * 0.5; 
		}
	}


	fill(y.begin(), y.end(), 0.0);
	auto start1 = chrono::high_resolution_clock::now();
	for (i = 0; i < MAX; i++)
		for (j = 0; j < MAX; j++)
			y[i] += A[i][j] * x[j];
	auto end1 = chrono::high_resolution_clock::now();
	chrono::duration<double, milli> dur1 = end1 - start1;


	vector<double> y1 = y;


	fill(y.begin(), y.end(), 0.0);
	auto start2 = chrono::high_resolution_clock::now();
	for (j = 0; j < MAX; j++)
		for (i = 0; i < MAX; i++)
			y[i] += A[i][j] * x[j];
	auto end2 = chrono::high_resolution_clock::now();
	chrono::duration<double, milli> dur2 = end2 - start2;

	cout << fixed << setprecision(3);
	cout << "Tiempo primer bucle (i-j): " << dur1.count() << " ms" << endl;
	cout << "Tiempo segundo bucle (j-i): " << dur2.count() << " ms" << endl;

	bool iguales = true;
	for (i = 0; i < MAX; i++) {
		if (abs(y[i] - y1[i]) > 1e-6) {
			iguales = false;
			break;
		}
	}
	cout << "¿Los resultados son iguales? " << (iguales ? "Sí" : "No") << endl;

	cout << "Algunos valores de y: ";
	for (i = 0; i < 5; i++) cout << y[i] << " ";
	cout << endl;

	return 0;
}
