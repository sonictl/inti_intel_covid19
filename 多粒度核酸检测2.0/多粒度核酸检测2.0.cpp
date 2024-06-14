#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<windows.h>
#include<graphics.h>
typedef struct Grade
{
	int k; //储存每组人数
	int s; //储存组数
}Grade;
Grade X[100];  //最多一百层
//存放最终组数
typedef struct End
{
	int k;
	int s;
}End;
End end[100];
double Min_E = 0xffffff;
int kl; //最终层数
int Jprice = 0;//检测费用
int Cprice = 0; //采集费用
double Wexpext(double q, int x, int L)    // k 上组人数 q 阴性概率  x 试验组人数 L 层数
{
	double qk = 0xffff;
	double retk = 0;
	double ret = 1;
	double E = 0;
	for (int i = 1; i <= L; i++)
	{
		retk += 1.0 / X[i].k;

		qk = pow(q, X[i].k);
		E += retk * qk * ret;
		ret *= (1 - qk);
	}
	E += (retk + 1) * ret;
	return E;
}
double minsw = 0xffff;
double w12expext(int L, double q, int k, int sum)
{
	double w = 0;
	int count = 0;
	if (sum % k > 1)return 0xfffff;
	do
	{
		sum -= k;
		count++;
	} while (sum >= k);
	w = k * count * Wexpext(q, k, L) + sum * Wexpext(q, sum, L);
	return w;
}
void Texpext(int L, double q, int k, double fminE)
{
	if (k <= 4)
	{
		if (fminE < Min_E)//当检测到该组人数为1时即为记录最小期望
		{
			if (k != 1)
			{
				X[L].k = 1;
				X[L].s = k;
				kl = L;
			}
			Min_E = fminE;
			for (int i = 1; i <= L; i++)
			{
				end[i].k = (int)(X[i].k + 0.9999);
				end[i].s = (int)X[i].s;
			}
			kl = L + 1;
			return;
		}
		return;
	}
	bool father = true;
	double E = 0; //记录期望
	//每组人数
	double ret = 1; ///储存表达式
	double retk = 0;
	double min = 0xfffff;
	int mink = 0;
	double minw1 = 0xfffff;
	double w1;
	double w2;
	int kx = k / 2;
	X[L].k = kx;
	X[L].s = (int)(k / X[L].k);
	if (k % 2 == 1)
	{
		X[L].k = kx;
		w2 = (k - 1) / 2 * Wexpext(q, (k - 1) / 2, L) + (k + 1) / 2 * Wexpext(q, (k + 1) / 2, L);
		w2 /= k;
	}
	else w2 = Wexpext(q, kx, L);
	minsw = 0xfffff;
	for (int i = 1; i < kx; i++)
	{
		w1 = w12expext(L, q, i, k) / k;
		if (w1 < minsw)
		{
			minsw = w1;
			mink = i;
		}
	}
	if (minsw - w2 < -0.00000000000000001)
	{
		X[L].k = mink;
		X[L].s = (int)((double)k / X[L].k + 0.99999);
		Texpext(L + 1, q, mink, minsw);
		return;
	}
	Texpext(L + 1, q, kx, w2);
}
double Owexpext(double k, double q)
{
	double qk = pow(q, k);
	double E = 1 / k * qk + (1 + 1 / k) * (1 - qk);
	return E;
}
void Oexpext(double q, int N)
{
	bool father = true;
	double w1; //方案1 期望
	double w2; //方案2 期望
	double min = 0xfffff;
	int mink = 0;
	int kx = (int)(double(N) / 2 + 0.999999);  //两组人数
	w2 = Owexpext(kx, q);
	for (int i = 2; i < kx; i++)
	{
		w1 = Owexpext(i, q);
		if (w1 < min)
		{
			min = w1;
			mink = i;
			X[1].k = i;
			X[1].s = (int)((double)N / i + 0.9999);
		}
	}
	if (min - w2 < 0)
	{
		Texpext(2, q, mink, min);
	}
	else Texpext(2, q, kx, w2);
}
void Arrows(int x, int y)
{
	int pts[] = { x, y,  x + 7, y + 5,  x + 7, y - 5 };
	setfillcolor(RGB(0, 0, 0));
	fillpolygon((POINT*)pts, 3);
}
void Buttonfront(int x, int y, char text[], int size)//按钮前
{

	setfillcolor(RGB(255, 255, 255));
	fillrectangle(x, y, x + size, y + size);
	settextstyle(20, 0, "宋体");
	int tx = x - textwidth(text) / 2;
	int ty = y - textheight(text) / 2;
	setbkmode(TRANSPARENT);
	settextcolor(RGB(0, 0, 0));
	outtextxy(tx, ty, text);
}
void Buttonend(int x, int y, char text[], int size)//按钮后
{

	setfillcolor(RGB(41, 131, 180));
	fillrectangle(x, y, x + size, y + size);
	settextstyle(20, 0, "宋体");
	int tx = x - textwidth(text) / 2;
	int ty = y - textheight(text) / 2;
	setbkmode(TRANSPARENT);
	settextcolor(RGB(0, 0, 0));
	outtextxy(tx, ty, text);
}
void Textbuttonend(int x, int y, const char text[])
{

	setfillcolor(RGB(255, 255, 255));
	fillrectangle(x, y, x + 80, y + 18);
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = 17;
	settextstyle(&f);
	settextstyle(18, 0, "宋体");
	setbkmode(TRANSPARENT);
	settextcolor(RGB(0, 0, 0));
	outtextxy(x - 90, y, text);

}
void Truebuttonedend(int x, int y, const char text[])
{

	setfillcolor(RGB(41, 131, 180));
	fillrectangle(x, y, x + 80, y + 22);
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = 17;
	settextstyle(&f);
	settextstyle(18, 0, "宋体");
	setbkmode(TRANSPARENT);
	settextcolor(RGB(0, 0, 0));
	outtextxy(x + 20, y + 3, text);

}
void Truebuttonedfront(int x, int y, const char text[])
{

	setfillcolor(RGB(255, 255, 255));
	fillrectangle(x, y, x + 80, y + 22);
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = 17;
	settextstyle(&f);
	settextstyle(18, 0, "宋体");
	setbkmode(TRANSPARENT);
	settextcolor(RGB(0, 0, 0));
	outtextxy(x + 20, y + 3, text);

}
void Sgrap(int* N, double* p)
{
	char Text[20];
	char Title[] = "多粒度核酸检测";
	setbkcolor(RGB(255, 255, 255));
	cleardevice();
	settextstyle(25, 0, "黑体");
	//画背景
	float H = 190;		// 色相
	float S = 1;		// 饱和度
	float L = 0.70f;		// 亮度
	for (int y = 0; y < 330; y++)
	{
		L += 0.0009f;
		setlinecolor(HSLtoRGB(H, S, L));
		line(0, y, 639, y);
	}

	//直线位置
	setlinecolor(RGB(0, 0, 0));
	line(0, 100, 400, 100);
	line(0, 270, 400, 270);
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = 25;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	settextcolor(RGB(0, 0, 0));
	char text2[] = "2.0";
	int x = 125 - textwidth(text2) / 2;
	outtextxy(35, 30, Title);
	f.lfHeight = 20;
	outtextxy(x, 55, text2);
	//输入框位置
	Textbuttonend(120, 120, "待检人数:");
	Textbuttonend(120, 155, "阳性概率:");
	Textbuttonend(120, 190, "检测费用:");
	Textbuttonend(120, 225, "采集费用:");
	Truebuttonedfront(85, 310, "确定");
	if (*N != 0) {

		sprintf_s(Text, ("%d"), *N);
		LOGFONT f;
		gettextstyle(&f);
		f.lfQuality = ANTIALIASED_QUALITY;
		f.lfHeight = 17;
		settextstyle(&f);
		settextstyle(18, 0, "宋体");
		setbkmode(TRANSPARENT);
		settextcolor(RGB(0, 0, 0));
		outtextxy(123, 120, Text);
	}
	if (*p != 0)
	{
		sprintf_s(Text, ("%lf"), *p);
		LOGFONT f;
		gettextstyle(&f);
		f.lfQuality = ANTIALIASED_QUALITY;
		f.lfHeight = 17;
		settextstyle(&f);
		settextstyle(18, 0, "宋体");
		setbkmode(TRANSPARENT);
		settextcolor(RGB(0, 0, 0));
		outtextxy(123, 156, Text);
	}
	if (Cprice != 0)
	{
		sprintf_s(Text, ("%d"), Cprice);
		LOGFONT f;
		gettextstyle(&f);
		f.lfQuality = ANTIALIASED_QUALITY;
		f.lfHeight = 17;
		settextstyle(&f);
		settextstyle(18, 0, "宋体");
		setbkmode(TRANSPARENT);
		settextcolor(RGB(0, 0, 0));
		outtextxy(123, 225, Text);
	}
	if (Jprice != 0)
	{
		sprintf_s(Text, ("%d"), Jprice);
		LOGFONT f;
		gettextstyle(&f);
		f.lfQuality = ANTIALIASED_QUALITY;
		f.lfHeight = 17;
		settextstyle(&f);
		settextstyle(18, 0, "宋体");
		setbkmode(TRANSPARENT);
		settextcolor(RGB(0, 0, 0));
		outtextxy(123, 190, Text);
	}

}
int Tchar(int number, char data[])
{
	int datanumber[20];
	int grad = 0;
	int tmp;
	for (int i = 0; i < number; i++)
	{
		tmp = data[i];
		if (tmp == 97 || tmp == 49)
		{
			datanumber[i] = 1;
			continue;
		}
		if (tmp == 98 || tmp == 50)
		{
			datanumber[i] = 2;
			continue;
		}
		if (tmp == 99 || tmp == 51)
		{
			datanumber[i] = 3;
			continue;
		}
		if (tmp == 100 || tmp == 52)
		{
			datanumber[i] = 4;
			continue;
		}
		if (tmp == 101 || tmp == 53)
		{
			datanumber[i] = 5;
			continue;
		}
		if (tmp == 102 || tmp == 54)
		{
			datanumber[i] = 6;
			continue;
		}
		if (tmp == 103 || tmp == 55)
		{
			datanumber[i] = 7;
			continue;
		}
		if (tmp == 104 || tmp == 56)
		{
			datanumber[i] = 8;
			continue;
		}
		if (tmp == 105 || tmp == 57)
		{
			datanumber[i] = 9;
			continue;
		}
		if (tmp == 96 || tmp == 48)
		{
			datanumber[i] = 0;
			continue;
		}
	}
	int count = 1;
	for (int i = 0; number != 0; i++)
	{
		grad += datanumber[--number] * count;
		count *= 10;
	}
	return grad;
}
char PPchar(int tmp)
{
	char tmp1 = 'a';
	if (tmp == 97 || tmp == 49)
	{
		tmp1 = '1';
		return tmp1;
	}
	if (tmp == 98 || tmp == 50)
	{
		tmp1 = '2';
		return tmp1;
	}
	if (tmp == 99 || tmp == 51)
	{
		tmp1 = '3';
		return tmp1;
	}
	if (tmp == 100 || tmp == 52)
	{
		tmp1 = '4';
		return tmp1;
	}
	if (tmp == 101 || tmp == 53)
	{
		tmp1 = '5';
		return tmp1;
	}
	if (tmp == 102 || tmp == 54)
	{
		tmp1 = '6';
		return tmp1;
	}
	if (tmp == 103 || tmp == 55)
	{
		tmp1 = '7';
		return tmp1;
	}
	if (tmp == 104 || tmp == 56)
	{
		tmp1 = '8';
		return tmp1;
	}
	if (tmp == 105 || tmp == 57)
	{
		tmp1 = '9';
		return tmp1;
	}
	if (tmp == 96 || tmp == 48)
	{
		tmp1 = '0';
		return tmp1;
	}
	if (tmp == 110 || tmp == -66)
	{
		tmp1 = '.';
		return tmp1;
	}
}
double Pchar(int number, char datas[20])
{
	char data2[20];
	for (int i = 2; i < number; i++)
	{
		data2[i - 2] = datas[i];
	}
	int tmp = Tchar(number - 2, data2);
	double grad = (double)tmp / pow(10.0, number - 2);
	return grad;
}
void OutN(double* pp, int* N)
{
	ExMessage p;
	char data[20];
	char Text[20];
	int number = 0;
	int Tdata = 0;
	do {

		getmessage(EM_KEY);
		getmessage(&p, EM_KEY);
		data[number++] = p.ch;
		if (number != 0 && p.ch == '\b') {
			number -= 2;
			Sgrap(N, pp);
		}
		data[number] = '\0';
		Tdata = Tchar(number, data);
		sprintf_s(Text, ("%d"), Tdata);
		LOGFONT f;
		gettextstyle(&f);
		f.lfQuality = ANTIALIASED_QUALITY;
		f.lfHeight = 17;
		settextstyle(&f);
		settextstyle(18, 0, "宋体");
		setbkmode(TRANSPARENT);
		settextcolor(RGB(0, 0, 0));
		outtextxy(123, 120, Text);
	} while (p.ch != '\r');
	data[--number] = '\0';
	*N = Tchar(number, data);
	Sgrap(N, pp);
}
void Outp(double* pp, int* N)
{
	ExMessage p;
	char data[20];
	char Text[20] = "\0";
	char Tmp[3];
	int number = 0;
	double Tdata = 0;
	do {

		getmessage(EM_KEY);
		getmessage(&p, EM_KEY);
		data[number++] = p.ch;
		if (number != 0 && p.ch == '\b') {
			number -= 2;
			Sgrap(N, pp);
		}
		data[number] = '\0';
		sprintf_s(Tmp, ("%c"), PPchar((int)p.ch));
		strcat_s(Text, 20, Tmp);
		LOGFONT f;
		gettextstyle(&f);
		f.lfQuality = ANTIALIASED_QUALITY;
		f.lfHeight = 17;
		settextstyle(&f);
		settextstyle(18, 0, "宋体");
		setbkmode(TRANSPARENT);
		settextcolor(RGB(0, 0, 0));
		outtextxy(123, 155, Text);
	} while (p.ch != '\r');
	data[--number] = '\0';
	*pp = Pchar(number, data);
	Sgrap(N, pp);
}
void OutC(double* pp, int* N)
{
	ExMessage p;
	char data[20];
	char Text[20];
	int number = 0;
	int Tdata = 0;
	do {
		getmessage(EM_KEY);
		getmessage(&p, EM_KEY);
		data[number++] = p.ch;
		if (number != 0 && p.ch == '\b') {
			number -= 2;
			Sgrap(N, pp);
		}
		data[number] = '\0';
		Tdata = Tchar(number, data);
		sprintf_s(Text, ("%d"), Tdata);
		LOGFONT f;
		gettextstyle(&f);
		f.lfQuality = ANTIALIASED_QUALITY;
		f.lfHeight = 17;
		settextstyle(&f);
		settextstyle(18, 0, "宋体");
		setbkmode(TRANSPARENT);
		settextcolor(RGB(0, 0, 0));
		outtextxy(123, 225, Text);
	} while (p.ch != '\r');
	data[--number] = '\0';
	Cprice = Tchar(number, data);
	Sgrap(N, pp);
}
void OutJ(double* pp, int* N)
{
	ExMessage p;
	char data[20];
	char Text[20];
	int number = 0;
	int Tdata = 0;
	do {

		getmessage(EM_KEY);
		getmessage(&p, EM_KEY);
		data[number++] = p.ch;
		if (number != 0 && p.ch == '\b') {
			number -= 2;
			Sgrap(N, pp);
		}
		data[number] = '\0';
		Tdata = Tchar(number, data);
		sprintf_s(Text, ("%d"), Tdata);
		LOGFONT f;
		gettextstyle(&f);
		f.lfQuality = ANTIALIASED_QUALITY;
		f.lfHeight = 17;
		settextstyle(&f);
		settextstyle(18, 0, "宋体");
		setbkmode(TRANSPARENT);
		settextcolor(RGB(0, 0, 0));
		outtextxy(123, 190, Text);
	} while (p.ch != '\r');
	data[--number] = '\0';
	Jprice = Tchar(number, data);
	Sgrap(N, pp);
}
void meau(int* N, double* p)
{
	while (1)
	{
		if (MouseHit())
		{
			MOUSEMSG pow = GetMouseMsg();
			switch (pow.uMsg)
			{
			case WM_LBUTTONDOWN:
				if (pow.x > 120 && pow.x < 200 && pow.y>120 && pow.y < 138)  //r
				{
					Sgrap(N, p);
					Arrows(210, 128);
					OutN(p, N);
				}
				if (pow.x > 120 && pow.x < 200 + 15 && pow.y>155 && pow.y < 173)   //p
				{
					Sgrap(N, p);
					Arrows(210, 165);
					Outp(p, N);
				}
				if (pow.x > 120 && pow.x < 200 + 15 && pow.y>190 && pow.y < 208)   //p
				{
					Sgrap(N, p);
					Arrows(210, 200);
					OutJ(p, N);
				}
				if (pow.x > 120 && pow.x < 200 + 15 && pow.y>225 && pow.y < 243)   //p
				{
					Sgrap(N, p);
					Arrows(210, 234);
					OutC(p, N);
				}
				if (pow.x > 85 && pow.x < 165 && pow.y>310 && pow.y < 338)
				{
					Truebuttonedend(85, 310, "确定");
					Sleep(300);
					Truebuttonedfront(85, 310, "确定");
					if (*N != 0 & *p != 0)
						return;
				}
				break;
			case WM_RBUTTONDOWN:
				closegraph();
				return;
				break;
			}
		}
	}
}
void Gradgraph(int N, double p)
{
	closegraph();
	char Title[50] = "待检人数：";
	char Title2[50] = "疾病概率：";
	char tmp[50];
	initgraph(400, 32 * kl + 75 + 30 + 30 + 30 + 30);
	setbkcolor(RGB(255, 255, 255));
	cleardevice();
	setlinecolor(RGB(0, 0, 0));
	line(0, 75, 400, 75);
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = 15;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	settextcolor(RGB(36, 116, 181));
	sprintf_s(tmp, ("%d"), N);
	strcat_s(Title, 50, tmp);
	outtextxy(10, 20, Title);
	settextcolor(RGB(237, 47, 106));
	sprintf_s(tmp, ("%lf"), p);
	strcat_s(Title2, 50, tmp);
	outtextxy(10, 50, Title2);
	char Td[30];
	int y = 85;
	int psum = N * p;
	if (psum <= 0)psum = 1;
	int sum = 0;    //检测次数总和
	sum += end[1].s;
	for (int i = 2; i < kl; i++)
		sum += end[i].s * psum;
	int csum = 0;    //采集次数总和
	csum += N;
	for (int i = 0; i < kl - 1; i++)
		csum += end[i].k * psum;
	//画标题
	int wid = getwidth() / 3;
	char Titale[3][15] = { "层数","组数","每组人数" };
	//画竖线 + 加标题
	for (int i = 1; i < 3; i++)
		line(i * wid, 75, i * wid, 32 * kl  + 75);

	for (int i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			settextcolor(RGB(163, 92, 143));
			break;
		case 1:
			settextcolor(RGB(52, 108, 156));
			break;
		case 2:
			settextcolor(RGB(235, 60, 112));
			break;
		}
		outtextxy(i * wid + wid / 2 - textwidth(Titale[i]) / 2, 75 + 32 / 2 - textheight(Titale[i]) / 2, Titale[i]);
	}
	//画横线
	for (int i = 1; i < kl; i++)
		line(0, 75 + 32 * i, getwidth(), 75 + 32 * i);
	for (int i = 1; i < kl; i++)
	{
		    settextcolor(RGB(163, 92, 143));
			char tm[8];
			sprintf_s(tm, ("%d"), i);
			outtextxy(0 * wid +wid/ 2 - textwidth(tm) / 2, 75+32/2 + 32 * i - textheight(tm) / 2, tm);
			settextcolor(RGB(52, 108, 156));
			sprintf_s(tm, ("%d"), end[i].s);
			outtextxy(1 * wid +wid/ 2 - textwidth(tm) / 2, 75 + 32 / 2 + 32 * i - textheight(tm) / 2, tm);
			settextcolor(RGB(235, 60, 112));
			sprintf_s(tm, ("%d"), end[i].k);
			outtextxy(2 * wid +wid/ 2 - textwidth(tm) / 2, 75 + 32 / 2 + 32 * i - textheight(tm) / 2, tm);
	}
	settextcolor(RGB(28, 20, 69));
	line(0, 32 * kl + 75, 400, 32 * kl  + 75);
	outtextxy(10, 32 * kl + 75 + 5, "共需检测：");
	sprintf_s(Td, ("%d"), sum);
	strcat_s(Td, sizeof(Td), "次");
	outtextxy(10 + textwidth("共需检测") + 5, 32 * kl + 75 + 6, Td);

	sprintf_s(Td, ("%d"), csum);
	strcat_s(Td, sizeof(Td), "次");
	outtextxy(200, 32 * kl + 75 + 5, "共需采集：");
	outtextxy(200 + textwidth("共需采集") + 5, 32 * kl + 75 + 6, Td);
	outtextxy(10, 32 * kl + 75 + 5 + 30, "单次检测费用：");

	sprintf_s(Td, ("%d"), Jprice);
	strcat_s(Td, sizeof(Td), "元");
	outtextxy(10 + textwidth("单次检测费用") + 5, 32 * kl + 75 + 6 + 30, Td);
	outtextxy(200, 32 * kl + 75 + 5 + 30, "单次采集费用：");
	sprintf_s(Td, ("%d"), Cprice);
	strcat_s(Td, sizeof(Td), "元");
	outtextxy(200 + textwidth("单次采集费用") + 5, 32 * kl + 75 + 6 + 30, Td);

	outtextxy(10, 32 * kl + 75 + 5 + 30 + 30, "检测总花费：");
	sprintf_s(Td, ("%d"), Jprice * sum);
	strcat_s(Td, sizeof(Td), "元");
	outtextxy(10 + textwidth("检测总花费") + 5, 32 * kl + 75 + 6 + 30 + 30, Td);
	outtextxy(200, 32 * kl + 75 + 5 + 30 + 30, "采集总花费：");
	sprintf_s(Td, ("%d"), Cprice * csum);
	strcat_s(Td, sizeof(Td), "元");
	outtextxy(200 + textwidth("采集总花费") + 5, 32 * kl + 75 + 6 + 30 + 30, Td);
	outtextxy(10, 32 * kl + 75 + 5 + 30 + 30 + 30, "总花费：");
	sprintf_s(Td, ("%d"), Jprice * sum + Cprice * csum);
	strcat_s(Td, sizeof(Td), "元");
	outtextxy(10 + textwidth("总花费") + 5, 32 * kl + 75 + 6 + 30 + 30 + 30, Td);

}
int main()
{
	initgraph(250, 370);
	int N = 0;
	double ql = 0;
	Sgrap(&N, &ql);
	meau(&N, &ql);
	Oexpext(1 - ql, N);
	Gradgraph(N, ql);
	while (1);
}
