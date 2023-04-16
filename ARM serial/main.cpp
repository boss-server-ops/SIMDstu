#include<iostream>
#include<sys/time.h>
#include<arm_neon.h>
#include<fstream>
using namespace std;
const int N = 2000;
float A[N][N];

void SSEparallelLUboth() {
	for (int k = 0; k < N; k++) {
		for (int j = k + 1; j < N; j++) {
			A[k][j] = A[k][j] / A[k][k];
		}
		A[k][k] = 1.0;
		for (int i = k+1; i < N; i++) {
			for (int j = k+1; j < N; j++) {
				A[i][j] = A[i][j] - A[i][k] * A[k][j];
			}
			A[i][k] = 0;
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
