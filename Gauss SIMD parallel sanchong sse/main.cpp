#include<iostream>
#include<Windows.h>
#include<emmintrin.h>
#include<fstream>
using namespace std;
const int N = 2000;
float B[N][N];

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
int main()
{

    ifstream infile("F:\\example.txt");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			char c;
			infile >> B[i][j];

		}
	}
    infile.close();
    cout<<B[0][1]<<endl;
    long long head4, tail4, freq4; // timers

    // similar to CLOCKS_PER_SEC
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq4);
    // start time
    QueryPerformanceCounter((LARGE_INTEGER*)&head4);
    SSEparallelLUfirst();
    QueryPerformanceCounter((LARGE_INTEGER*)&tail4);
    cout << "Col: " << (tail4 - head4) * 1000.0 / freq4 << "ms" << endl;
}
