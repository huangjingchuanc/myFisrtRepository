//#include<iostream>
#include"map.h"
#include<windows.h>
int main(void) {
	map m;
	
	m.showMap();
	while (1) {
		m.FindMine(m.GetMouseLeftEvent());
		
	}
	
	system("pause");
	return 0;
}