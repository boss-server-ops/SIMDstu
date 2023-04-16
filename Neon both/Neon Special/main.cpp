#include<iostream>
#include<fstream>
#include<cmath>
#include<string>
#include<arm_neon.h>
#include<sys/time.h>
using namespace std;
int shouxiang(int integernum, int* beixiaoyuanmatrix, int beixiaoyuannum)
{
	int biaoshi = integernum * 32;//记录遍历到的编号

	for (int j = integernum - 1; j >= 0; j--)
	{
		for (int k = 31; k >= 0; k--)
		{
			biaoshi--;
			/*cout << "j" << j << "k" << k << endl;*/
			bool bit = (beixiaoyuanmatrix[j] >> k) & 1;
			if (bit == 1)
			{
				return biaoshi;
			};

		}

	}
	return 0;


}
int daoru(ifstream& infile, int** b, int matrixcolnum, int integernum, int** shengge, int beixiaoyuannum)
{
	int i = 0;
	static int hang = matrixcolnum - 1;
	int colnum;
	char c;
	int integerindex;
	int bitindex;
	int cishu = 5;
	for (int j = 0; j < 5; j++)
	{
		for (int k = 0; k < integernum; k++)
		{

			b[j][k] = 0;
		}

	}
	while (cishu > 0)
	{
		cishu--;
		int pos = infile.tellg();
		infile >> colnum;
		/*cout << colnum << endl;*/
		if (colnum == hang)//首项符合要求，导入数据
		{
			infile.seekg(pos);
			while (1)
			{
				/*cout << "shouxiang" << shouxiang(integernum, b[1], 5) << endl;*/
				infile >> colnum;

				integerindex = colnum / 32;
				bitindex = colnum % 32;
				int bitnum = 1 << bitindex;
				b[i][integerindex] |= bitnum;
				infile.get(c);
				if (infile.peek() == '\n')
				{
					infile.ignore();
					i++;
					hang--;
					break;
				}
			}

		}
		else
		{
			/*for (int k = 0; k < beixiaoyuannum; k++)
			{
				if (colnum == shouxiang(integernum, shengge[k], beixiaoyuannum))
				{
					b[i] = shengge[k];
					i++;
					hang--;
					continue;
				}
			}*/
			infile.seekg(pos);
			for (int j = 0; j < integernum; j++)
			{
				b[i][j] = 0;

			}
			/*cout << "lai" << endl;*/
			i++;
			hang--;

		}


		if (infile.peek() == EOF)
		{
			return 0;
		}


	}
	/*for (int j = 0; j < 5; j++)
	{
		cout << shouxiang(integernum, b[j], 5)<<' ';
	}
	cout << endl;*/
	return 1;
}

int main()
{
    timeval start, finish;


	//开始计时检测
	gettimeofday(&start, NULL);// Start Time
	int matrixcolnum;
	int xiaoyuanzinum;
	int beixiaoyuannum;
	matrixcolnum = 130;
	xiaoyuanzinum = 22;
	beixiaoyuannum = 8;
	cout << "矩阵列数" << matrixcolnum << "消元子" << xiaoyuanzinum << "被消元行" << beixiaoyuannum << endl;

	int integernum = ceil(matrixcolnum / 32.0);

	int** shengge = new int* [beixiaoyuannum];//用来记录被消元行升格后的消元子

	for (int i = 0; i < beixiaoyuannum; i++)
	{
		shengge[i] = new int[integernum];
	}
	for (int i = 0; i < beixiaoyuannum; i++)
	{
		for (int j = 0; j < integernum; j++)
		{
			shengge[i][j] = 0;
		}
	}
	int** beixiaoyuanmatrix = new int* [beixiaoyuannum];
	bool* biaoji = new bool[beixiaoyuannum];
	for (int i = 0; i < beixiaoyuannum; i++)
	{
		biaoji[i] = 0;
	}
	for (int i = 0; i < beixiaoyuannum; i++)
	{
		beixiaoyuanmatrix[i] = new int[integernum];
	}
	for (int i = 0; i < beixiaoyuannum; i++)
	{
		for (int j = 0; j < integernum; j++)
		{
			beixiaoyuanmatrix[i][j] = 0;
		}
	}
	ifstream infile("被消元行.txt");
	int index = 0;//记录读到了第几行
	int colnum;//读到的数据
	int integerindex;
	int bitindex;
	char c;
	while (1)
	{


		infile >> colnum;
		/*cout << colnum<<' ';*/
		integerindex = colnum / 32;
		bitindex = colnum % 32;
		int bitnum = 1 << bitindex;
		beixiaoyuanmatrix[index][integerindex] |= bitnum;
		infile.get(c);
		if (infile.peek() == '\n')
		{
			infile.ignore();
			cout << endl;
			index++;
		}
		if (infile.peek() == EOF)
		{
			break;
		}
	}
	infile.close();

//	for (int i = 0; i < beixiaoyuannum; i++)
//	{
//		for (int j = integernum - 1; j >= 0; j--)
//		{
//			for (int k = 31; k >= 0; k--)
//			{
//				bool bit = (beixiaoyuanmatrix[i][j] >> k) & 1;
//				cout << bit;
//			}
//
//		}
//		cout << endl;
//	}
	/*cout << "xiaoyuanzi" << endl;*/
	ifstream infile2("消元子.txt");
	int** xiaoyuanzimatrix = new int* [xiaoyuanzinum];
	for (int i = 0; i < 5; i++)
	{
		xiaoyuanzimatrix[i] = new int[integernum];
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < integernum; j++)
		{
			xiaoyuanzimatrix[i][j] = 0;
		}
	}


	/*daoru(infile2,xiaoyuanzimatrix, matrixcolnum,integernum,shengge,beixiaoyuannum);*/
	int lastshouxiang = matrixcolnum - 5;//记录此位置之前的消元子的最后一个非0首项




	while (daoru(infile2, xiaoyuanzimatrix, matrixcolnum, integernum, shengge, beixiaoyuannum))
	{

		/*cout << "beixiaoyuan" <<beixiaoyuannum<< endl;*/

		for (int i = 0; i < beixiaoyuannum; i++)
		{
			if (biaoji[i] == 1)
			{
				continue;
			}

			for (int j = 0; j < 5; j++)
			{
				int xyzsx = shouxiang(integernum, xiaoyuanzimatrix[j], 5);//消元子首项
				for (int k = 0; k < beixiaoyuannum; k++)
				{
					if (biaoji[k] == 1 && k != i && shouxiang(integernum, beixiaoyuanmatrix[i], beixiaoyuannum) == shouxiang(integernum, beixiaoyuanmatrix[k], beixiaoyuannum))
					{
						int32x4_t vt, va;
						int t = 0;
						for (; t+4 <= integernum; t+=4)
						{
							va = vld1q_s32(&beixiaoyuanmatrix[i][t]);
							vt = vld1q_s32(&beixiaoyuanmatrix[k][t]);
							va = veorq_s32(va, vt);
							vst1q_s32(&beixiaoyuanmatrix[i][t], va);
						}
						beixiaoyuanmatrix[i][t] ^= beixiaoyuanmatrix[k][t];
					}
				}
				if (shouxiang(integernum, beixiaoyuanmatrix[i], beixiaoyuannum) == xyzsx)
				{
					int k = 0;
					int32x4_t vt, va;
					for (; k+4 <= integernum; k+=4)
					{

						va = vld1q_s32(&beixiaoyuanmatrix[i][k]);
						vt = vld1q_s32(&xiaoyuanzimatrix[j][k]);
						va = veorq_s32(va, vt);
						vst1q_s32(&beixiaoyuanmatrix[i][k], va);

					}
					beixiaoyuanmatrix[i][k] ^= xiaoyuanzimatrix[j][k];
				}
				else if (xyzsx == 0)
				{
					for (int k = 0; k < beixiaoyuannum; k++)
					{
						if (biaoji[k] == 1 && k != i && shouxiang(integernum, beixiaoyuanmatrix[i], beixiaoyuannum) == shouxiang(integernum, beixiaoyuanmatrix[k], beixiaoyuannum))
						{
							int32x4_t vt, va;
							int t = 0;
							for (; t + 4 <= integernum; t += 4)
							{
								va = vld1q_s32(&beixiaoyuanmatrix[i][t]);
								vt = vld1q_s32(&beixiaoyuanmatrix[k][t]);
								va = veorq_s32(va, vt);
								vst1q_s32(&beixiaoyuanmatrix[i][t], va);

							}
							beixiaoyuanmatrix[i][t] ^= beixiaoyuanmatrix[k][t];
						}
					}
				}
			}

			if (shouxiang(integernum, beixiaoyuanmatrix[i], beixiaoyuannum) > lastshouxiang)
			{
				biaoji[i] = 1;
				/*cout << shouxiang(integernum, beixiaoyuanmatrix[i], beixiaoyuannum) <<"    "<< lastshouxiang <<"   " << "biaoji" << shouxiang(integernum, beixiaoyuanmatrix[i], beixiaoyuannum) << endl;*/
			}

		}
		lastshouxiang -= 5;


	}

//	for (int i = 0; i < 5; i++)
//	{
//		for (int j = integernum - 1; j >= 0; j--)
//		{
//			for (int k = 31; k >= 0; k--)
//			{
//				bool bit = (xiaoyuanzimatrix[i][j] >> k) & 1;
//				cout << bit;
//			}
//
//		}
//		cout << endl;
//	}

	cout << "消元结果" << endl;
	for (int i = 0; i < beixiaoyuannum; i++)
	{
		/*for (int j = integernum - 1; j >= 0; j--)
		{
			for (int k = 31; k >= 0; k--)
			{
				bool bit = (beixiaoyuanmatrix[i][j] >> k) & 1;
				cout << bit;
			}

		}
		cout << endl;*/
		cout << shouxiang(integernum, beixiaoyuanmatrix[i], beixiaoyuannum) << endl;
	}
	infile2.close();
	delete[]xiaoyuanzimatrix;
	delete[]biaoji;
	delete[]beixiaoyuanmatrix;
	gettimeofday(&finish, NULL);// End Time
	cout << ((finish.tv_sec - start.tv_sec) * 1000000.0 + finish.tv_usec - start.tv_usec) / 1000.0 << endl;

}
