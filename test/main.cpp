#include<iostream>
#include<Windows.h>
#include<emmintrin.h>
#include<fstream>
using namespace std;
const int N = 2000;
float m[N][N],A[N][N],B[N][N],C[N][N];

void serialLU() {
	for (int k = 0; k < N; k++) {
		for (int j = k + 1; j < N; j++) {
			m[k][j] = m[k][j] / m[k][k];
		}
		m[k][k] = 1.0;
		for (int i = k+1; i < N; i++) {
			for (int j = k+1; j < N; j++) {
				m[i][j] = m[i][j] - m[i][k] * m[k][j];
			}
			m[i][k] = 0;
		}
	}
}
void SSEparallelLUboth() {


	__m128 vt, va, vb, vc;
	for (int k = 0; k < N; k++)
	{

		vt = _mm_set_ps1(A[k][k]);
		int j;
		for (j = k + 1; j + 4 <= N; j += 4)
		{
			va = _mm_loadu_ps(&A[k][j]);
			va = _mm_div_ps(va, vt);
			_mm_storeu_ps(&A[k][j], va);
		}
		while (j < N) {
			A[k][j] = A[k][j] / A[k][k];
			j++;
		}
		A[k][k] = 1.0;
		for (int i = k + 1; i < N; i++)
		{


			vt = _mm_set_ps1(A[i][k]);
			int j = 0;
			for (j = k + 1; j + 4 <= N; j += 4)
			{
				va = _mm_loadu_ps(&A[k][j]);
				vb = _mm_loadu_ps(&A[i][j]);
				vc = _mm_mul_ps(vt, va);
				vb = _mm_sub_ps(vb, vc);
				_mm_storeu_ps(&A[i][j], vb);
			}
			while (j < N) {
				A[i][j] = A[i][j] - A[i][k] * A[k][j];
				j++;
			}
			A[i][k] = 0;

		}
	}
}
void SSEparallelLUfirst()//二重循环不并行化
{
	__m128 vt, va, vb, vc;
	for (int k = 0; k < N; k++)
	{

		vt = _mm_set_ps1(B[k][k]);
		for (int j = k + 1; j < N; j++) {
			B[k][j] = B[k][j] / B[k][k];
		}
		B[k][k] = 1.0;
		for (int i = k + 1; i < N; i++)
		{
			vt = _mm_set_ps1(B[i][k]);
			int j = 0;
			for (j = k + 1; j + 4 <= N; j += 4)
			{
				va = _mm_loadu_ps(&B[k][j]);
				vb = _mm_loadu_ps(&B[i][j]);
				vc = _mm_mul_ps(vt, va);
				vb = _mm_sub_ps(vb, vc);
				_mm_storeu_ps(&B[i][j], vb);
			}
			while (j < N) {
				B[i][j] = B[i][j] - B[i][k] * B[k][j];
				j++;
			}
			B[i][k] = 0;
		}
	}
}
void SSEparallelLUsecond()//三重循环不并行化
{
	__m128 vt, va, vb, vc;
	for (int k = 0; k < N; k++)
	{

		vt = _mm_set_ps1(C[k][k]);
		int j;
		for (j = k + 1; j + 4 <= N; j += 4)
		{
			va = _mm_loadu_ps(&C[k][j]);
			va = _mm_div_ps(va, vt);
			_mm_storeu_ps(&C[k][j], va);
		}
		while (j < N) {
			C[k][j] = C[k][j] / C[k][k];
			j++;
		}
		C[k][k] = 1.0;
		for (int i = k + 1; i < N; i++) {
			for (int j = k + 1; j < N; j++) {
				C[i][j] = C[i][j] - C[i][k] * C[k][j];
			}
			C[i][k] = 0;
		}
	}
}
int main()
{
	ifstream infile("F:\\example.txt");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			char c;
			infile >> C[i][j];
			m[i][j]=C[i][j];
			A[i][j]=C[i][j];

			B[i][j]=C[i][j];


		}
	}
    infile.close();
	long long head, tail, freq; // timers

	 // similar to CLOCKS_PER_SEC
		 QueryPerformanceFrequency((LARGE_INTEGER*)  & freq);
	 // start time
		 QueryPerformanceCounter((LARGE_INTEGER*)& head);
	serialLU();
	QueryPerformanceCounter((LARGE_INTEGER*) & tail);
	 cout << "serialCol: " << (tail-head) * 1000.0 / freq << "ms" << endl;


	 long long head2, tail2, freq2; // timers

	 // similar to CLOCKS_PER_SEC
	 QueryPerformanceFrequency((LARGE_INTEGER*)&freq2);
	 // start time
	 QueryPerformanceCounter((LARGE_INTEGER*)&head2);
	 SSEparallelLUboth();
	 QueryPerformanceCounter((LARGE_INTEGER*)&tail2);
	 cout << "Col: " << (tail2 - head2) * 1000.0 / freq2 << "ms" << endl;


	 long long head3, tail3, freq3; // timers

	 // similar to CLOCKS_PER_SEC
	 QueryPerformanceFrequency((LARGE_INTEGER*)&freq3);
	 // start time
	 QueryPerformanceCounter((LARGE_INTEGER*)&head3);
	 SSEparallelLUfirst();
	 QueryPerformanceCounter((LARGE_INTEGER*)&tail3);
	 cout << "Col: " << (tail3 - head3) * 1000.0 / freq3 << "ms" << endl;


	 long long head4, tail4, freq4; // timers

	 // similar to CLOCKS_PER_SEC
	 QueryPerformanceFrequency((LARGE_INTEGER*)&freq4);
	 // start time
	 QueryPerformanceCounter((LARGE_INTEGER*)&head4);
	 SSEparallelLUsecond();
	 QueryPerformanceCounter((LARGE_INTEGER*)&tail4);
	 cout << "Col: " << (tail4 - head4) * 1000.0 / freq4 << "ms" << endl;
}
