#include <iostream>
#include <windows.h>

const int len_to_vertex = 10;
const char lightmap[] = {' ', '0'};

void get_screen_size(int* p_screen);
void draw_cube_vertices(int* p_cube_vertices, int len_to_vertex, int terminal_x, int terminal_y, int terminal_z);

int main(void)
{
	// set console width & height
	// p_screen is used as a pointer to store the height and width from a function
	int screen_size[2];
	int* p_screen_size = &screen_size[0];

	get_screen_size(p_screen_size);

	int terminal_x = screen_size[0];
	int terminal_y = screen_size[1];
	int terminal_z = terminal_x + terminal_y;
	
	std::cout << "terminal_x : " << terminal_x << '\n';
	std::cout << "terminal_y : " << terminal_y << '\n';
	std::cout << "terminal_z : " << terminal_z << '\n';

	// create coordinates for cube vertecies in 3d space
	/*
	   	{
			{1,2,3},
			{4,5,6},
			{7,8,9},
			  ...
		}
	*/
	int cube_vertices[8][3];
	int* p_cube_vertices = &cube_vertices[0][0];

	draw_cube_vertices(p_cube_vertices, len_to_vertex, terminal_x, terminal_y, terminal_z);

	return 0;
}

void get_screen_size(int* p_screen)
{
	// create pointer variables to represent the two points of the array in screen_size[2]
	int* screen_row = p_screen;
	int* screen_col = (p_screen + 1);

	// get console width and height
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;
  
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  
	*screen_row = rows;
	*screen_col = columns;
}

void draw_cube_vertices(int* p_cube_vertices, int len_to_vertex, int terminal_x, int terminal_y, int terminal_z)
{
	int center_x = terminal_x / 2;
	int center_y = terminal_y / 2;
	int center_z = terminal_z / 2;

	std::cout << "Center of 3d plane : {" << center_x << ", " << center_y << ", " << center_z << "} \n";
	std::cout << "Length from center of vertex : " << len_to_vertex << '\n';

	// 
}
