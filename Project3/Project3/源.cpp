#include<iostream>

using namespace std;

char a[20]= "3d6";
int num = 3;


double reg[5];
bool reg_use[5] = { 0,0,0,0,0 };
char sym[5] = { 'd','/','*','-','+' };
double res_f = 0.0;

int roll_dice(int face)
{
	return rand() % face + 1;
}

int request_reg()
{
	int i;
	for (i = 0; i < 5; i++)
	{
		if (reg_use[i]==0)
		{
			reg_use[i] = 1;
			return i;
		}
	}
	return -1;
}

double my_pow(double a, int k)
{
	int i = 0;
	double res = 1.0;
	if (k >= 0)
	{
		for (i = 0; i < k; i++)
		{
			res *= a;
		}
	}
	else
	{
		for (i = 0; i > k; i--)
		{
			res /= a;
		}
	}
	return res;
}

bool calculate(char *a0,int num0)
{
	char *p = a0, *q = a0, *r = a0, *s = a0, *dot = a0;
	int step = 0, i = 0, j = 0, k = 0, dotFlag = 0;
	double res = 0.0;
	while (q - a0 < num0)
	{
		if (*q == '(')
		{
			i = 0;
			r = q + 1;
			while ((i != 0 || *r != ')'))
			{
				if (r - a0 >= num0)
					return 1;
				if (*r == '(')
					i++;
				else if (*r == ')')
					i--;
				r++;
			}
			j = request_reg();
			if (j < 0)
				return 1;
			calculate(q + 1, r - q - 1);
			reg[j] = res_f;
			res_f = 0;
			p = q;
			while (p <= r)
			{
				*p = 'e' + j;
				p++;
			}
		}
		q++;
	}
	q = a0;
	p = a0;
	r = a0;
	while (true)
	{
		if (*q == sym[step])
		{
			r = q;
			s = q;
			while (r - a0 < num0)
			{
				r++;
				k = 0;
				for (i = step; i < 5; i++)
				{
					if (*r == sym[i])
						k = 1;
				}
				if (k == 1)
					break;
			}
			while (s > a0)
			{
				s--;
				k = 0;
				for (i = step; i < 5; i++)
				{
					if (*s == sym[i])
					{
						k = 1;
						s++;
					}
				}
				if (k == 1)
					break;
			}
			if (r - q <= 1)
				return 1;
			j = request_reg();
			if (j < 0)
				return 1;
			reg[j] = 0;
			if (*s >= '0'&&*s <= '9')
			{
				p = q;
				dot = q;
				dotFlag = 0;
				do
				{
					p--;
					if (*p == '.'&&dotFlag == 0)
					{
						dot = p;
						reg[j] *= my_pow(10, -(q - dot - 1));
						dotFlag = 1;
					}
					else if (*p == '.'&&dotFlag == 1)
						return 1;
					else
						reg[j] += (*p - '0')*my_pow(10, dot - p - 1);
				} while (p != s);
			}
			else if (s == q)
			{
				reg[j] = 1;
			}
			else
			{
				reg[j] = reg[*s - 'e'];
				reg_use[*s - 'e'] = 0;
			}
			if (*(r - 1) >= '0'&&*(r - 1) <= '9')
			{
				p = r;
				dot = r;
				dotFlag = 0;
				res = 0;
				do
				{
					p--;
					if (*p == '.'&&dotFlag == 0)
					{
						dot = p;
						res *= my_pow(10, -(r - dot - 1));
						dotFlag = 1;
					}
					else if (*p == '.'&&dotFlag == 1)
						return 1;
					else
						res += (*p - '0')*my_pow(10, dot - p - 1);
				} while (p != q + 1);
			}
			else
			{
				res = reg[*(r - 1) - 'e'];
				reg_use[*(r - 1) - 'e'] = 0;
			}
			int i0 = reg[j];
			switch (step)
			{
			case 0:
				reg[j] = 0;
				for (int i = 0; i < i0; i++)
				{
					reg[j] += roll_dice(res);
				}
				break;
			case 1:
				if (res == 0)
					return 1;
				reg[j] /= res;
				break;
			case 2:
				reg[j] *= res;
				break;
			case 3:
				reg[j] -= res;
				break;
			case 4:
				reg[j] += res;
				break;
			}
			p = s;
			while (p != r)
			{
				*p = j + 'e';
				p++;
			}
		}
		q++;
		if (q - a0 == num0)
		{
			step++;
			q = a0;
		}
		if (step == 5)
			break;
	}
	res_f = reg[*q - 'e'];
	reg_use[*q - 'e'] = 0;
	return 0;
	/*char *p = a, *q = a, *p0 = a, *dot = a;
	res = 0.0;
	bool dotFlag = 0;
	while (1)
	{
		while ((*q >= '0'&&*q <= '9') || *q == '.')
		{
			q++;
		}
		if (*q != '('&&q == p)
			return 1;
		else if (*q != '(')
		{
			p0 = q;
			dot = q;
			res = 0.0;
			do
			{
				p0--;
				if (*p0 == '.'&&dotFlag == 0)
				{
					dot = p0;
					res *= pow(10, -(q - dot - 1));
					dotFlag = 1;
				}
				else if (*p0 == '.'&&dotFlag == 1)
					return 1;
				else
					res += (*p0 - '0')*pow(10, dot - p0 - 1);
			} while (p0 != p);
	}*/
}

int main()
{
	double res;
	calculate(a, num);
	cout << res_f << endl;
	system("pause");
	return 0;
}