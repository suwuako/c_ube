#include <iostream>
#include <windows.h>


const char lightmap[] = {' ', '0'};

int main(void)
{
	// get console width and height
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;
  
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  
	std::cout << "columns : " << columns << '\n' << "rows : " << rows << '\n';

	// create coordinates for cube vertecies in 3d space
	int cube_vertexes[8][3];
	int* p_cube_vertexes = &cube_vertexes[0][0];

	std::cout << p_cube_vertexes;
	

	return 0;
}
