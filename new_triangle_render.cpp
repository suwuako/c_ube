#include <math.h>
#include <iostream>
#include <windows.h>

const int triangle_offset = 10;
const int triangle_count = 2;

void get_screen_size(int* p_screen);
void render_screen(double* p_triangle_vertices, int terminal_x, int terminal_y, int terminal_z);
void assign_vertices(double* p_triangle_vertices, double* A, double* B, double* C);
void assign_vectors(double* A, double* B, double* C, double* AB, double* AC);
char return_pixel(double* p_triangle_vertices, int terminal_x, int terminal_y, int terminal_z);
bool in_triangle(int x, int y, double AB[3], double AC[3], double A[3], double B[3], double C[3]);


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
			{centre_x + triangle_offset, centre_y + triangle_offset, centre_z + triangle_offset},
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

  render_screen(p_triangle_vertices, terminal_x, terminal_y, terminal_z);
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


void assign_vertices(double* p_triangle_vertices, double* A, double* B, double* C, int which_triangle)
{
  for (int triangle = 0; triangle < triangle_count; triangle++)
  {
    for (int vertex = 0; vertex < 3; vertex++)
    {
      for (int coordinate = 0; coordinate < 3; coordinate++)
      {
        if (vertex == 0)
        {
          *(A + coordinate) = *(p_triangle_vertices + which_triangle * 9 + 3 * vertex + coordinate);
        } else if (vertex == 1)
        {
          *(B + coordinate) = *(p_triangle_vertices + which_triangle * 9 + 3 * vertex + coordinate);
        } else {
          *(C + coordinate) = *(p_triangle_vertices + which_triangle * 9 + 3 * vertex + coordinate);
        }
      }
    }
  }
}


void assign_vectors(double* A, double* B, double* C, double* AB, double* AC)
{
  for (int i = 0; i < 2; i++)
  {
    *(AB+i) = *(B+i) - *(A+i);
    *(AC+i) = *(C+i) - *(A+i);
  }
}


bool in_triangle(int x, int y, double AB[3], double AC[3], double A[3], double B[3], double C[3])
{
  double A_x = A[0];
  double A_y = A[1];

  double AB_x = AB[0];
  double AB_y = AB[1];
  double AC_x = AC[0];
  double AC_y = AC[1];

  double w_1;
  double w_2;

  w_1 = (A_x * (AC_y) + (y - A_y) * AC_x - x * (AC_y)) / (AB_y * AC_x - AB_x * AC_y);
  w_2 = (y - A_y - w_1 * AB_y) / AC_y;


  if (w_1 >= 0 && w_2 >= 0 && (w_1 + w_2 <= 1) )
  {
    return true;
  }
  return false;

}


char return_pixel(double* p_triangle_vertices, int x, int y)
{
  /*
    std::cout << A[0] << ' ' << A[1] << ' ' << A[2] << '\n';
    std::cout << B[0] << ' ' << B[1] << ' ' << B[2] << '\n';
    std::cout << C[0] << ' ' << C[1] << ' ' << C[2] << '\n';
 */ 

  double A[3];
  double B[3];
  double C[3];

  double AB[3];
  double AC[3];


  for (int triangle = 0; triangle < triangle_count; triangle++)
  {
    assign_vertices(p_triangle_vertices, A, B, C, triangle);
    assign_vectors(A, B, C, AB, AC);

    if (in_triangle(x, y, AB,  AC, A, B, C))
    {
      return '#';
    } else {
      return ' ';
    }
  }
}


void render_screen(double* p_triangle_vertices, int terminal_x, int terminal_y, int terminal_z)
{
  for (int x = 1; x < terminal_x + 1; x++)
  {
    for (int y = 1; y < terminal_y; y++)
    {
      std::cout << return_pixel(p_triangle_vertices, x, y);
    }
    std::cout << '\n';
  }
}
