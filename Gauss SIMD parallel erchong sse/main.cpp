#include<iostream>
#include<Windows.h>
#include<emmintrin.h>
#include<fstream>
using namespace std;
const int N = 2000;
float C[N][N];

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

		}
	}
    infile.close();
    cout<<C[0][1]<<endl;
    long long head4, tail4, freq4; // timers

    // similar to CLOCKS_PER_SEC
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq4);
    // start time
    QueryPerformanceCounter((LARGE_INTEGER*)&head4);
    SSEparallelLUsecond();
    QueryPerformanceCounter((LARGE_INTEGER*)&tail4);
    cout << "Col: " << (tail4 - head4) * 1000.0 / freq4 << "ms" << endl;
}

