#include <iostream>
#include <Windows.h>
#include <random>
#include <time.h>

using namespace std;
void main(void)
{
	clock_t start, finish;
	float delay;
	std::random_device generator;
	std::uniform_int_distribution<short> distribution(1, 8);
	int iterations = 1000000;
	short int temp;

	short int matrix_a[16][16], resultArray[16], k = 5;
	int res = 0;

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			matrix_a[i][j] = distribution(generator);
		}
		resultArray[i] = 0;
	}

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			cout << matrix_a[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;

	start = clock();
	for (int z = 0; z < iterations; z++)
	{
		for (int i = 0; i < 16; i++)
		{
			temp = 0;
			for (int j = 0; j < 16; j++)
			{
				temp += matrix_a[i][j];
			}
			resultArray[i] = temp;
		}
	}
	finish = clock();
	delay = (float)(finish - start) / CLK_TCK;

	cout << "Time C is " << delay << " seconds" << endl;
	cout << "Result array: ";
	for (int i = 0; i < 16; i++)
	{
		cout << resultArray[i] << ' ';
	}
	cout << endl << endl;

	for (int i = 0; i < 16; i++)
	{
		resultArray[i] = 0;
	}

	start = clock();
	for (int i = 0; i < iterations; i++)
	{
		int cnt = 16;
		_asm
		{
			pusha
			xor esi, esi
			xor edi, edi
			mov edx, cnt
		loop1 :
			xor ax, ax
			mov ecx, cnt
		loop2 :
			add ax, matrix_a[esi]
			add esi, 2
			dec ecx
			jnz loop2
			mov resultArray[edi], ax
			add edi, 2
			dec edx
			jne loop1
			popa
		}
	}
	finish = clock();
	delay = (float)(finish - start) / CLK_TCK;

	cout << "Time ASM is " << delay << " seconds" << endl;
	cout << "Result array: ";
	for (int i = 0; i < 16; i++)
	{
		cout << resultArray[i] << ' ';
	}
	cout << endl << endl;

	for (int i = 0; i < 16; i++)
	{
		resultArray[i] = 0;
	}

	start = clock();
	for (int i = 0; i < iterations; i++)
	{
		short cnt = 16;
		_asm
		{
			pusha
			xor esi, esi
			xor edi, edi
		loop3:
			mov ecx, 4
			pxor mm7, mm7
		loop4:
			movq mm0, matrix_a[esi]
			paddd mm7, mm0
			add esi, 8
			dec ecx
			jnz loop4
			punpckhdq mm1, mm7	
			punpckldq mm2, mm7
			paddd mm1, mm2
			punpckhwd mm3, mm1
			punpckldq mm4, mm3
			paddd mm3, mm4
			punpckhwd mm3, mm5
			punpckhdq mm3, mm5
			movd eax, mm3
			mov resultArray[edi], ax
			add edi, 2
			dec cnt
			jnz loop3
			emms
			popa
		}
	}
	finish = clock();
	delay = (float)(finish - start) / CLK_TCK;

	cout << "Time ASM with MMX is " << delay << " seconds" << endl;
	cout << "Result array: ";
	for (int i = 0; i < 16; i++)
	{
		cout << resultArray[i] << ' ';
	}
	cout << endl << endl;
	
	

	system("pause");
}