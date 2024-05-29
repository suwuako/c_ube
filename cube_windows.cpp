#include <iostream>
#include <cmath>
#include <windows.h>

const int len_to_vertex = 10;
const char lightmap[] = {' ', '0'};

void get_screen_size(int* p_screen);
void draw_cube_vertices(int* p_cube_vertices, int len_to_vertex, int terminal_x, int terminal_y, int terminal_z);
void test_vertex_render(int terminal_x, int terminal_y, int cube_vertices[8][3]);
void rotate_vertices(double angle, int cube_vertices[8][3], int* p_cube_vertices, int terminal_x, int terminal_y, int terminal_z);

struct point 
{
	int x;
	int y;
	int z;
};

struct rotation_matrix
{
	double row_1[3];
	double row_2[3];
	double row_3[3];
};

point rotate_vertex(rotation_matrix matrix, point);

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
	   	where if *p == 1; *(p+1) == 2 and *(p+2) == 3.
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
	
	std::cout << "successfully drew vertexes! \n";

	for (int vertex = 0; vertex < 8; vertex++)
	{
		std::cout << "{";
		for (int coord = 0; coord < 3; coord++)
		{
			std::cout << cube_vertices[vertex][coord] << ' ';
		}
		std::cout << "} \n";
	}

	// tests
	test_vertex_render(terminal_x, terminal_y, cube_vertices);
	
	double rotation = M_PI / 10;
	rotate_vertices(rotation, cube_vertices, p_cube_vertices, terminal_x, terminal_y, terminal_z);

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
	/*
		I'm using a weird hacky way to draw the vertices - it probably would be better to use some form of matrix multiplication but its done this way:
		since we have an centre, if we add/subtract a constant integer value in each of the x-y-z we end up with a vertex. if we want all 8 vertices, 
		we can use a system similar to binary where 0s represents subtract and 1s represent addition - we start with 000 and that means subtract 
		pythag_len from all three x, y and z centres and then we increment to 001, meaning we subract from x and y but add to z. iterating will yield us
		the pattern 000, 001, 010, 011, 100, 101, 110, 111 which are our 8 vertices!
	*/
	int centre_x = terminal_x / 2;
	int centre_y = terminal_y / 2;
	int centre_z = terminal_z / 2;

	// this is the system where the offsets are used - we start with {0,0,0}
	int offset_system[3] = {0, 0, 0};

	int pythag_len = sqrt((pow(len_to_vertex, 2)) / 3);

	std::cout << "Center of 3d plane : {" << centre_x << ", " << centre_y << ", " << centre_z << "} \n";
	std::cout << "Length from centre of vertex : " << len_to_vertex << '\n';
	std::cout << "Pythagorean length : " << pythag_len << '\n';

	// draw the coordinates of the 8 vertices 

	// interates over each vertex
	for (int vertex{ 0 }; vertex < 8; vertex++)
	{
		// sets coordinate value for each vertex
		for (int j = 0; j < 3; j++)
		{
			if (j == 0)
			{
				// x coordinate
				if (offset_system[2] == 0)
				{
					// 3 * vertex iterates over the number of vertexes in the array (i.e. 3 * 1 -> cube_vertices[1][0])
					*(p_cube_vertices + (3 * vertex) + j) = centre_x - pythag_len;
				} else {
					*(p_cube_vertices + (3 * vertex) + j) = centre_x + pythag_len;
				}
	

			} else if (j == 1) {
				// y coordinate
				if (offset_system[1] == 0)
				{
					*(p_cube_vertices + (3 * vertex) + j) = centre_y - pythag_len;
				} else {
					*(p_cube_vertices + (3 * vertex) + j) = centre_y + pythag_len;
				}

			} else if (j == 2) {
				// z coordinate
				if (offset_system[0] == 0)
				{
					*(p_cube_vertices + (3 * vertex) + j) = centre_z - pythag_len;
				} else {
					*(p_cube_vertices + (3 * vertex) + j) = centre_z + pythag_len;
				}

			} else {
				//catch error
				std::cout << "unknown error, coordinate out of bounds. \n";
			}			
		}
		//increment offset_system
		int increment = 1;

		for (int i = 0; i < 3; i++)
		{
			if (increment == 1 && offset_system[i] == 0)
			{
				offset_system[i] = 1;
				increment = 0;
			} else if (increment == 0 && offset_system[i] == 1) {
				offset_system[i] = 1;
			} else {
				offset_system[i] = 0;
			}
		}
		
		std::cout << "offset_system orientation : ";

		for (int i = 0; i < 3; i++)
		{
			std::cout << offset_system[i];
		}
		std::cout << '\n';

	}
}


void test_vertex_render(int terminal_x, int terminal_y, int cube_vertices[8][3])
{
	// test render
	for (int x = 0; x < terminal_x; x++)
	{
		for (int y = 0; y < terminal_y; y++)
		{
			bool pixel_unrendered = false;
			for (int vertex = 0; vertex < 8; vertex++)
			{
				if (cube_vertices[vertex][0] == x && cube_vertices[vertex][1] == y)
				{
					std::cout << '0';
					pixel_unrendered = true;
				}
			}

			if (!pixel_unrendered)
			{
				std::cout << ' ';
			}
		}
		std::cout << '\n';
	}
}


void rotate_vertices(double angle, int cube_vertices[8][3], int* p_cube_vertices, int terminal_x, int terminal_y, int terminal_z)
{
	/* 
	   we're gonna want to give it a roll, pitch and yaw (rotate on all three axies)
	   there are two ways we can go about this:
	   
	   1) we can use the standard rotation matrices that rotate about the x, y and z axies which means that we need to 
		first transform the offset (subtract each coordinate depending if its x, y or z by the center point, apply
		the matrix and then rescale it back onto our coordinate plane)
	   2) or we can try and find a rotation matrix that does it around an arbitrary axis lol
	*/

	// 30/5/2024 (3:51 am) - method 1 seems easier and maybe it doesn't seem all that hacky, might as well try it.

	int centre_x = terminal_x / 2;
	int centre_y = terminal_y / 2;
	int centre_z = terminal_z / 2;
	
	// translate vertices to a cube with the origin at its centre
	int cube_vertices_remap[8][3]; 

	for (int vertex = 0; vertex < 8; vertex++)
	{
		for (int coord = 0; coord < 3; coord++)
		{
			if (coord == 0)
			{
				cube_vertices_remap[vertex][coord] = cube_vertices[vertex][coord] - centre_x;
			} else if (coord == 1) {
				cube_vertices_remap[vertex][coord] = cube_vertices[vertex][coord] - centre_y;
			} else {
				cube_vertices_remap[vertex][coord] = cube_vertices[vertex][coord] - centre_z;
			}
		}
	}

	// apply rotation matrices
	/*
		rotate on x-axis:
		{
			{1,	0,	0	},
			{0,	cos(a),	-sin(a)	},
			{0,	sin(a), cos(a)	}
		}

		rotate on y-axis:
		{
			{cos(b),	0,	sin(b)	},
			{0,		1,	0	},
			{-sin(b),	0,	cos(b)	}
		}

		rotate on z-axis:
		{
			{cos(c),	-sin(c),	0},
			{sin(c),	cos(c), 	0},
			{0,		0,		1}
		}
		
	 */
	double angle_x;
	double angle_y;
	double angle_z;

	double x_array[3][3] =
		{
			{1,	0,		0},
			{0,	cos(angle_x),	-sin(angle_x)},
			{0,	sin(angle_x),	cos(angle_x)}
		};

	double y_array[3][3] = 
		{
			{cos(angle_y),	0,	sin(angle_y)	},
			{0,		1,	0	},
			{-sin(angle_y),	0,	cos(angle_y)	}
		};

	double z_array[3][3] = 
		{
			{cos(angle_z),	-sin(angle_z),	0},
			{sin(angle_z),	cos(angle_z), 	0},
			{0,		0,		1}
		};


	rotation_matrix x_rotation;
	rotation_matrix y_rotation;
	rotation_matrix z_rotation;

	
	// translate back to original centre point

	// write to coordinate pointer
}
