#include <math.h>
#include <iostream>
#include <windows.h>

const int triangle_offset = 5;
const int triangle_count = 2;

void get_screen_size(int* p_screen);
void render_screen(double* triangle_vertices, int terminal_x, int terminal_y, int terminal_z);

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

	// debug message
	std::cout << "centres: (x: " << centre_x << "), (y: " << centre_y << "), (z: " << centre_z << ")\n";

	// arbitrarily create two triangles
	// array that contains an array of three coordinate points - a triangle
	double triangle_vertices[2][3][3] = {
		{
			{centre_x + triangle_offset, centre_y, centre_z}, 
			{centre_x, centre_y + triangle_offset, centre_z}, 
			{centre_x, centre_y, centre_z + triangle_offset}
		},
		{
			{centre_x, centre_y, centre_z}, 
			{centre_x + triangle_offset, centre_y, centre_z + triangle_offset},
			{centre_x + triangle_offset, centre_y + (triangle_offset / 2), centre_z + triangle_offset}
		}

	};
	double* p_triangle_vertices = &triangle_vertices[0][0][0];
  
  for (int triangle = 0; triangle < 2; triangle++)
  {
    for (int vertices = 0; vertices < 3; vertices++)
    {
      std::cout << '(';
      for (int coord = 0; coord < 3; coord++)
      {
        std::cout << triangle_vertices[triangle][vertices][coord] << ' ';
      }
      std::cout << ")\n";
    }
  }

  void render(p_triangle_vertices, terminal_x, terminal_y, terminal_z);
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

void render_screen(double* triangle_vertices, int terminal_x, int terminal_y, int terminal_z)
{
  
}
