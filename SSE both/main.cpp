#include<iostream>
#include<Windows.h>
#include<emmintrin.h>
#include<fstream>
using namespace std;
const int N = 2000;
float A[N][N];

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
	 long long head2, tail2, freq2; // timers

	 // similar to CLOCKS_PER_SEC
	 QueryPerformanceFrequency((LARGE_INTEGER*)&freq2);
	 // start time
	 QueryPerformanceCounter((LARGE_INTEGER*)&head2);
	 SSEparallelLUboth();
	 QueryPerformanceCounter((LARGE_INTEGER*)&tail2);
	 cout << "Col: " << (tail2 - head2) * 1000.0 / freq2 << "ms" << endl;



}
