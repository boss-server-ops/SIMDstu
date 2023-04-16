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
		int j=k+1;
        __m128 *ptr = (__m128*)&A[k][j];

        __m128 *aligned_ptr = (__m128*)(((unsigned long long)&A[k][j] + 15) & ~15);

        int n_aligned = ((unsigned long long)aligned_ptr - (unsigned long long)ptr)/4;
//        cout<<n_aligned<<endl;
		for(int j=k+1;j<n_aligned+k+1;j++)
        {
            A[k][j] = A[k][j] / A[k][k];

        }
		for (j = n_aligned+k+1; j + 4 <= N; j += 4)
		{
//            cout<<&A[k][j]<<endl;
			va = _mm_load_ps(&A[k][j]);
			va = _mm_div_ps(va, vt);
			_mm_store_ps(&A[k][j], va);
		}
		while (j < N) {
			A[k][j] = A[k][j] / A[k][k];
			j++;
		}
		A[k][k] = 1.0;
		int i = k+1;
		j = k+1;
		 __m128 *ptr2 = (__m128*)&A[i][j];

        __m128 *aligned_ptr2 = (__m128*)(((unsigned long long)&A[i][j] + 15) & ~15);

        int n_aligned2 = ((unsigned long long)aligned_ptr2 - (unsigned long long)ptr2)/4;

         __m128 *ptr3 = (__m128*)&A[k][j];

        __m128 *aligned_ptr3 = (__m128*)(((unsigned long long)&A[k][j] + 15) & ~15);

        int n_aligned3 = ((unsigned long long)aligned_ptr3 - (unsigned long long)ptr3)/4;
//        cout<<n_aligned2<<endl;
//        cout<<n_aligned3<<endl;
        for (int i = k+1; i < n_aligned2+k+1; i++) {
			for (int j = k+1; j < n_aligned3+k+1; j++) {
				A[i][j] = A[i][j] - A[i][k] * A[k][j];
			}
			A[i][k] = 0;
		}

		for (int i = n_aligned2+k+1; i < N; i++)
		{


			vt = _mm_set_ps1(A[i][k]);
			int j = 0;
			for (j = n_aligned3+k+1; j + 4 <= N; j += 4)
			{
				va = _mm_load_ps(&A[k][j]);
				vb = _mm_load_ps(&A[i][j]);
				vc = _mm_mul_ps(vt, va);
				vb = _mm_sub_ps(vb, vc);
				_mm_store_ps(&A[i][j], vb);
			}
			while (j < N) {
				A[i][j] = A[i][j] - A[i][k] * A[k][j];
				j++;
			}
			A[i][k] = 0;

		}
//    for (int i = k + 1; i < N; i++)
//		{
//
//
//			vt = _mm_set_ps1(A[i][k]);
//			int j = 0;
//			for (j = k + 1; j + 4 <= N; j += 4)
//			{
//				va = _mm_loadu_ps(&A[k][j]);
//				vb = _mm_loadu_ps(&A[i][j]);
//				vc = _mm_mul_ps(vt, va);
//				vb = _mm_sub_ps(vb, vc);
//				_mm_storeu_ps(&A[i][j], vb);
//			}
//			while (j < N) {
//				A[i][j] = A[i][j] - A[i][k] * A[k][j];
//				j++;
//			}
//			A[i][k] = 0;
//
//		}
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
