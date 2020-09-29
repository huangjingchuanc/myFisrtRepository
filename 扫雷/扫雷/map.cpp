#define _CRT_SECURE_NO_WARNINGS
#include "map.h"
#include<time.h>
#include<stdlib.h>
#include<stdio.h>


#define BLOCK_SIZE 50
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 500

map::map() 
{
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	
	
	initMap(ROW + 2, CLOS + 2);
	setMinePos();
	findMineCount();
	countMine = 0;
}
//��ʼ����ͼ
void map::initMap(int row, int clos)
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < clos; j++) {
			Map[i][j] = 0;
		}
	}
}

//���õ���λ��
void map::setMinePos()
{
	srand(time(0));
	int i = NUM;
	while (i) {
		int row = rand() % ROW + 1;
		int clos = rand() % CLOS + 1;
		//��������жϷ�ֹ�ظ��������
		if (Map[row][clos] != -1) {
			Map[row][clos] = -1;
			i--;
		}
	}
}

//����ÿ�����׵�����
void map::findMineCount()
{
	for (int i = 1; i <=ROW; i++) {
		for (int j = 1; j <=CLOS; j++) {

			if (Map[i][j] == 0) {
				MineCount(i, j);
			}
		}
	}
}


//�����ǰ�㲻���ף��Ͷ��������Χ���б���
void map::MineCount(int r, int c)
{
	int count = 0;
	for (int i = r - 1; i <= r + 1; i++) {
		for (int j = c - 1; j <= c + 1; j++) {
			if ((i != r || j != c) && Map[i][j] == -1) {
				count++;
			}
				
		}
	}
	Map[r][c] = count;
}

//��ʾ��ͼ
void map::showMap()
{
	for (int i = 0; i <ROW; i++) {
		for (int j = 0; j <CLOS; j++) {
			drawMap(j*BLOCK_SIZE, i*BLOCK_SIZE);
			
		}
	}
	initMenu();
}

//���Ƶ�ͼ
void map::drawMap(int x=0, int y=0,COLORREF color)
{
	setfillcolor(color);
	fillrectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
}

//��ȡ���ʱ�䷢����λ��
pos map::GetMouseLeftEvent()
{
	pos p = {0,0};
	MOUSEMSG msg;
	while (1) {
		msg = GetMouseMsg();
		if (msg.mkLButton == true) {
			p.x = msg.x;
			p.y = msg.y;
			return p;
		}
	}
	return p;
}

//����Χ����ʱ���ж���Χ�ķ����������Χ�Ƿ�����
void map::judgeMine(int row, int clos) {
	int x,y;
	for (int i = row-1; i < row+1; i++) {
		for (int j = clos-1; j < clos+1; j++) {
			if (Map[i][j]!=-2) {
				//�����ǰ���飬������-2����С��0��������
				drawMap(x=(j-1) * BLOCK_SIZE, y=(i - 1)* BLOCK_SIZE, RED);
				if (Map[i][j] < 0) {
					c[0] = L'��';
					c[1] = L'\0';
				}else {
					numToChar(Map[i][j], c);
				}
				outtextxy(x + 15, y + 15, c);
				Map[i][j] = -2;
			}
		}
	}
}

//��ʼ���˵�
void map::initMenu()
{
	int x = ROW * BLOCK_SIZE;
	int y = CLOS * BLOCK_SIZE;
	setfillcolor(COLORREF(RGB(171, 91, 236)));
	fillrectangle(x,0,WINDOW_WIDTH,y);
	
	RECT r = { ROW * BLOCK_SIZE + 3,0,WINDOW_WIDTH,50 };
	settextcolor(RGB(0, 255, 0));
	drawtext(L"�÷֣�", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	numToChar(countMine, c);
	r.top += 50;
	r.bottom += 50;
	drawtext(c, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}
void map::FindMine(pos p)
{
	//��ͼ���к���
	int clos = p.x / BLOCK_SIZE;
	int row = p.y / BLOCK_SIZE;

	//�¼������ķ���λ��
	int x = clos * BLOCK_SIZE;
	int y = row * BLOCK_SIZE;
	
	//�жϷ��������������е�����
	switch (Map[row + 1][clos + 1]) {
	case -1://���������
	{
		c[0] = L'��';
		c[1] = L'\0';
		drawMap(x, y, BLUE);
		outtextxy(x + 15, y + 15, c);
		Sleep(200);
		drawMap(x, y);
		countMine++;
	}
		break;
	case 0://������ķ�����Χû����
	{
		numToChar(Map[row + 1][clos + 1], c);
		drawMap(x, y, RED);
		outtextxy(x + 15, y + 15, c);
	}
		break;
	default://û�е㵽�ף�����Χ���ײ��ų�
	{
		//��ʾ�¼����������׵�����,������������Ϊ-2
		numToChar(Map[row + 1][clos + 1], c);
		drawMap(x, y, RED);
		outtextxy(x + 15, y + 15, c);
		Map[row + 1][clos + 1] = -2;

		//����������Χ�ķ������ݣ��������ų�
		for (int i = row; i < row + 2; i++) {
			for (int j = clos; j < clos + 2; j++) {
				if (i != row + 1 || j != clos + 1) {
					judgeMine(i, j);
				}
			}
		}
	}
	}
	
}

void map::numToChar(int x, wchar_t* const ch)
{
	if (x == 0) {
		ch[0] = 48;
		ch[1] = '\0';
		return;
	}

	int i = 0;
	while (x) {
		char c = x % 10;
		ch[i++] = c + 48;
		x /= 10;
	}

	ch[i] = '\0';
	i--;

	int j = 0;
	wchar_t c;
	
	while (j != i) {
		c = ch[j];
		ch[j] = ch[i];
		ch[i] = c;
		j++;
		i--;
	}
}