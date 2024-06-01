#include <iostream>
#include <windows.h>

const int triangle_offset = 10;

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

void render(double* triangles)
{
	/*
		rendering: we need to be able to determine if an arbitrary point is within the three points that make up a triangle

		let's start with a simpler case - how do we determine if a point lies on the line between two points?
		let's label the points A and B, forming the vector AB.
	   */
}

int main(void)
{
	int screen_size[2];
	int terminal_x;
	int terminal_y;
	int terminal_z;

	get_screen_size(screen_size);

	// terminal_z is set so that it isn't the smallest parameter
	terminal_x = screen_size[0];
	terminal_y = screen_size[1];
	terminal_z = terminal_x + terminal_y;

	double centre_x = terminal_x / 2;
	double centre_y = terminal_y / 2;
	double centre_z = terminal_z / 2;

	// arbitrarily create two triangles
	// array that contains an array of three coordinate points - a triangle
	double triangles[2][3][3] = {
		{
			{centre_x + triangle_offset, centre_y, centre_z}, 
			{centre_x, centre_y + triangle_offset, centre_z}, 
			{centre_x, centre_y, centre_z + triangle_offset}
		},
		{
			{centre_x, centre_y, centre_z}, 
			{centre_x + triangle_offset, centre_y, centre_z + triangle_offset},
			{centre_x + triangle_offset, centre_y + triangle_offset / 2, centre_z + triangle_offset}
		}

	};
	double* p_triangles = triangles;

	// test render
	std::cout << triangles;

	render(p_triangles);
	return 0;
}
