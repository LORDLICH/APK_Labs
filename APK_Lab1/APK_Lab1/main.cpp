#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <time.h>


using namespace std;
void main()
{
	while (true)
	{
		long double a, b, step, x, f;
		clock_t start, finish;
		float delay;
		int flag = 0, three = 3;

		cout << "Enter min value(greater than 0): " << endl;
		cin >> a;

		cout << "Enter max value: " << endl;
		cin >> b;

		cout << "Enter step: " << endl;
		cin >> step;

		x = a;
		f = 0;

		start = clock();
		_asm {
			finit
			fld b
			fld x
		}

		while (x < b) {
			_asm {
				fld x
				fsin
				fld x
				fyl2x
				fimul three

				fadd f
				fstp f

				fadd step
				fst x
			}
		}
		_asm {
			fwait
		}
		finish = clock();


		delay = (float)(finish - start) / CLK_TCK;
		cout << "Time ASM is " << delay << " seconds" << endl;
		cout << "F(x) = " << f << endl;

		x = a;
		f = 0;

		start = clock();
		while (x < b) {
			float temp = 3 * log2l(x) * sin(x);
			x += step;
			f += temp;
		}
		finish = clock();

		delay = (float)(finish - start) / CLK_TCK;
		cout << "Time C is " << delay << " seconds" << endl;
		cout << "F(x) = " << f << endl;
		

		cout << "Stop programm?\n1-Yes\n0-No" << endl;
		cin >> flag;
		if (flag) return;
		system("CLS");
	}
}