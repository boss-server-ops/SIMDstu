#include<iostream>
#include<sys/time.h>
#include<arm_neon.h>
#include<fstream>
using namespace std;
const int N = 2000;
float A[N][N];

void SSEparallelLUboth() {
	float32x4_t vt, va, vb, vc;
	for (int k = 0; k < N; k++)
	{

		vt = vld1q_dup_f32(&A[k][k]);
		int j;
		for (j = k + 1; j + 4 <= N; j += 4)
		{
			va = vld1q_f32(&A[k][j]);
			va = vdivq_f32(va, vt);
			vst1q_f32(&A[k][j], va);
		}
		while (j < N) {
			A[k][j] = A[k][j] / A[k][k];
			j++;
		}
		A[k][k] = 1.0;
		for (int i = k + 1; i < N; i++)
		{


			vt = vld1q_dup_f32(&A[i][k]);
			int j = 0;
			for (j = k + 1; j + 4 <= N; j += 4)
			{
				va = vld1q_f32(&A[k][j]);
				vb = vld1q_f32(&A[i][j]);
				vc = vmulq_f32(vt, va);
				vb = vsubq_f32(vb, vc);
				vst1q_f32(&A[i][j], vb);
			}
			while (j < N) {
				A[i][j] = A[i][j] - A[i][k] * A[k][j];
				j++;
			}
			A[i][k] = 0.0f;

		}
	}
}

int main()
{

    ifstream infile("example.txt");
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

	 timeval start,finish;


	//开始计时检测
    gettimeofday(&start,NULL);// Start Time
    SSEparallelLUboth();
	gettimeofday(&finish,NULL);// End Time
    cout<<((finish.tv_sec-start.tv_sec)*1000000.0+finish.tv_usec-start.tv_usec)/1000.0<<endl;


}
