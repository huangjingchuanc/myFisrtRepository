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
//初始化地图
void map::initMap(int row, int clos)
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < clos; j++) {
			Map[i][j] = 0;
		}
	}
}

//设置地雷位置
void map::setMinePos()
{
	srand(time(0));
	int i = NUM;
	while (i) {
		int row = rand() % ROW + 1;
		int clos = rand() % CLOS + 1;
		//这个条件判断防止重复的随机数
		if (Map[row][clos] != -1) {
			Map[row][clos] = -1;
			i--;
		}
	}
}

//查找每个点雷的数量
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


//如果当前点不是雷，就对这个点周围进行遍历
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

//显示地图
void map::showMap()
{
	for (int i = 0; i <ROW; i++) {
		for (int j = 0; j <CLOS; j++) {
			drawMap(j*BLOCK_SIZE, i*BLOCK_SIZE);
			
		}
	}
	initMenu();
}

//绘制地图
void map::drawMap(int x=0, int y=0,COLORREF color)
{
	setfillcolor(color);
	fillrectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
}

//获取鼠标时间发生的位置
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

//当周围有雷时，判断周围的方块和他的周围是否有雷
void map::judgeMine(int row, int clos) {
	int x,y;
	for (int i = row-1; i < row+1; i++) {
		for (int j = clos-1; j < clos+1; j++) {
			if (Map[i][j]!=-2) {
				//如果当前方块，不等于-2而且小于0，就有雷
				drawMap(x=(j-1) * BLOCK_SIZE, y=(i - 1)* BLOCK_SIZE, RED);
				if (Map[i][j] < 0) {
					c[0] = L'雷';
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

//初始化菜单
void map::initMenu()
{
	int x = ROW * BLOCK_SIZE;
	int y = CLOS * BLOCK_SIZE;
	setfillcolor(COLORREF(RGB(171, 91, 236)));
	fillrectangle(x,0,WINDOW_WIDTH,y);
	
	RECT r = { ROW * BLOCK_SIZE + 3,0,WINDOW_WIDTH,50 };
	settextcolor(RGB(0, 255, 0));
	drawtext(L"得分：", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	numToChar(countMine, c);
	r.top += 50;
	r.bottom += 50;
	drawtext(c, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}
void map::FindMine(pos p)
{
	//地图的行和列
	int clos = p.x / BLOCK_SIZE;
	int row = p.y / BLOCK_SIZE;

	//事件产生的方块位置
	int x = clos * BLOCK_SIZE;
	int y = row * BLOCK_SIZE;
	
	//判断方块在数据数组中的数据
	switch (Map[row + 1][clos + 1]) {
	case -1://点击到了雷
	{
		c[0] = L'雷';
		c[1] = L'\0';
		drawMap(x, y, BLUE);
		outtextxy(x + 15, y + 15, c);
		Sleep(200);
		drawMap(x, y);
		countMine++;
	}
		break;
	case 0://点击到的方块周围没有雷
	{
		numToChar(Map[row + 1][clos + 1], c);
		drawMap(x, y, RED);
		outtextxy(x + 15, y + 15, c);
	}
		break;
	default://没有点到雷，但周围有雷并排除
	{
		//显示事件发生方块雷的数量,并将数据设置为-2
		numToChar(Map[row + 1][clos + 1], c);
		drawMap(x, y, RED);
		outtextxy(x + 15, y + 15, c);
		Map[row + 1][clos + 1] = -2;

		//遍历方块周围的方块数据，并将雷排除
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