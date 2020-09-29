#pragma once
#include<graphics.h>

#define ROW 10
#define CLOS 10
#define NUM 10
typedef struct postion
{
	int x;
	int y;
}pos;

class map
{
public:
	explicit map();
	void initMap(int row,int clos);
	void setMinePos();
	void findMineCount();
	void MineCount(int r, int c);
	void showMap();
	void drawMap(int x,int y, COLORREF color= RGB(96, 172, 157));
	pos GetMouseLeftEvent();
	void FindMine(pos p);
	void numToChar(int x, wchar_t* const ch);
	void judgeMine(int row, int clos);
	void initMenu();
private:
	int Map[ROW+2][CLOS+2];
	wchar_t c[3];
	int countMine;
};

