#include<iostream>
#include<Windows.h>
#include <immintrin.h>
#include<fstream>
using namespace std;
const int N = 2000;
float A[N][N];

void AVXparallelLUsecond()//¶þÖØÑ­»·
{
    __m256_u vt, va, vb, vc;
	for (int k = 0; k < N; k++)
	{

		float tmp[8]={A[k][k],A[k][k],A[k][k],A[k][k],A[k][k],A[k][k],A[k][k],A[k][k]};
        vt=_mm256_loadu_ps(tmp);
        int j=0;
		for (j = k + 1; j + 8 <= N; j += 8)
		{
			va = _mm256_loadu_ps(&A[k][j]);
			va = _mm256_div_ps(va, vt);
			_mm256_storeu_ps(&A[k][j], va);
		}
		while (j < N) {
			A[k][j] = A[k][j] / A[k][k];
			j++;
		}
		A[k][k] = 1.0;
		for (int i = k + 1; i < N; i++)
        {
			for (int j = k + 1; j < N; j++)
            {
				A[i][j] = A[i][j] - A[i][k] * A[k][j];
			}
			A[i][k] = 0;
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
			infile >> A[i][j];

		}
	}
    infile.close();
    cout<<A[0][1]<<endl;
    long long head4, tail4, freq4; // timers

    // similar to CLOCKS_PER_SEC
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq4);
    // start time
    QueryPerformanceCounter((LARGE_INTEGER*)&head4);
    AVXparallelLUsecond();
    QueryPerformanceCounter((LARGE_INTEGER*)&tail4);
    cout << "Col: " << (tail4 - head4) * 1000.0 / freq4 << "ms" << endl;
}

