#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>

#ifdef _MSC_VER
  #include <windows.h>
#else
  #include <sys/ioctl.h>
  #include <termios.h> // maybe don't use this one after all -V
  #include <unistd.h>
#endif

const int len_to_vertex = 10;
const char lightmap[] = {' ', '0'};
const double rotation_angle = 0;
const double rotation_increment = M_PI/100;

const double x_rotation_offset = 0;
const double y_rotation_offset = 0;
const double z_rotation_offset = 0;

const bool rotate_x = false;
const bool rotate_y = true;
const bool rotate_z = true;

void get_screen_size(int* p_screen);
void draw_cube_vertices(double* p_cube_vertices, int len_to_vertex, int terminal_x, int terminal_y, int terminal_z);
void test_vertex_render(int terminal_x, int terminal_y, double cube_vertices[8][3]);
void rotate_vertices(double angle, double cube_vertices[8][3], double* p_cube_vertices, int terminal_x, int terminal_y, int terminal_z);
void multiply_rotation_matrices(double* matrix, double* coord);
void loop(int terminal_x, int terminal_y, int terminal_z, double* p_cube_vertices, double cube_vertices[8][3]);
void group_triangles(double cube_vertices[8][3], double* p_triangles);

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

  std::cout << "\x1B[31m colour test \033[0m" << '\n';

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
  double cube_vertices[8][3];
  double* p_cube_vertices = &cube_vertices[0][0];

  draw_cube_vertices(p_cube_vertices, len_to_vertex, terminal_x, terminal_y, terminal_z);
  
  std::cout << "successfully drew vertexes! \n";

  /* 
    gotta create triangles! triangles will be a collection of 3 coordinates that make up one of the faces of the cube
    can end up messy since i will be storing triangles inside an array : example :
    {
      {
        {1, 1, 1}, {2, 2, 2}, {3, 3, 3}
      },
      {
        {...}, ... , {...}
      }
    }
    yuck!

    notes: we will always have 6 faces (of a cube), 12 triangles, 8 vertices. 3 vertices makes up a triangles.
  */
  double triangles[12][3][3];
  double* p_triangles = &triangles[0][0][0];

  group_triangles(cube_vertices, p_triangles);

  // tests
  loop(terminal_x, terminal_y, terminal_z, p_cube_vertices, cube_vertices);
  
  return 0;
}


void get_screen_size(int* p_screen)
{
  // create pointer variables to represent the two points of the array in screen_size[2]
  int* screen_row = p_screen;
  int* screen_col = (p_screen + 1);

	// get console width and height
	int columns = 150, rows = 35;
#ifdef _MSC_VER
	CONSOLE_SCREEN_BUFFER_INFO csbi;
  
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
  int demanded_fd = 1;
  struct winsize size_info;
  //struct termios attrib_struct;
  //columns = ioctl(); // tcgetattr(demanded_fd, &attrib_struct);
  int err_code = ioctl(demanded_fd, TIOCGWINSZ, &size_info); // &columns, &rows); // tcgetattr(demanded_fd, &attrib_struct);
  if (0 != err_code) {
    std::cerr << "[WARN] could not execute ioctl call. Using default col/row bounds." << std::endl;
  }
  columns = size_info.ws_col;
  rows = size_info.ws_row;
#endif
  
  *screen_row = rows;
  *screen_col = columns;
}


void draw_cube_vertices(double* p_cube_vertices, int len_to_vertex, int terminal_x, int terminal_y, int terminal_z)
{
  /*
    I'm using a weird hacky way to draw the vertices - it probably would be better to use some form of matrix multiplication but its done this way:
    since we have an centre, if we add/subtract a constant integer value in each of the x-y-z we end up with a vertex. if we want all 8 vertices, 
    we can use a system similar to binary where 0s represents subtract and 1s represent addition - we start with 000 and that means subtract 
    pythag_len from all three x, y and z centres and then we increment to 001, meaning we subract from x and y but add to z. iterating will yield us
    the pattern 000, 001, 010, 011, 100, 101, 110, 111 which are our 8 vertices!
   */
  double centre_x = terminal_x / 2;
  double centre_y = terminal_y / 2;
  double centre_z = terminal_z / 2;

  // this is the system where the offsets are used - we start with {0,0,0}
  int offset_system[3] = {0, 0, 0};

  double pythag_len = sqrt((pow(len_to_vertex, 2)) / 3);

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


void test_vertex_render(int terminal_x, int terminal_y, double cube_vertices[8][3])
{
  // test render

  /*
    made the mistake of storing coordinates and vertices as ints instead of doubles - causing it to round down... will probably have to write code here that
    turns the double into int!
   */

	std::string colours[8] = {"\x1B[31m", "\x1B[32m", "\x1B[33m","\x1B[34m", "\x1B[35m", "\x1B[36m", "\x1B[37m", "\x1B[38m"};

	for (int x = 0; x < terminal_x; x++)
	{
		for (int y = 0; y < terminal_y; y++)
		{
			bool pixel_unrendered = true;
			for (int vertex = 0; vertex < 8; vertex++)
			{
				if ((int) cube_vertices[vertex][0] == x && (int) cube_vertices[vertex][1] == y)
				{
					std::cout << colours[vertex] << "0" << "\033[0m";
					pixel_unrendered = false;
				}
			}
      if (pixel_unrendered)
      {
        std::cout << ' ';
      }
    }
    std::cout << '\n';
  }
}


void rotate_vertices(double angle, double cube_vertices[8][3], double* p_cube_vertices, int terminal_x, int terminal_y, int terminal_z)
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

  double centre_x = terminal_x / 2;
  double centre_y = terminal_y / 2;
  double centre_z = terminal_z / 2;
  
  // translate vertices to a cube with the origin at its centre
  double cube_vertices_remap[8][3]; 

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
      {1,  0,  0  },
      {0,  cos(a),  -sin(a)  },
      {0,  sin(a), cos(a)  }
    }

    rotate on y-axis:
    {
      {cos(b),  0,  sin(b)  },
      {0,    1,  0  },
      {-sin(b),  0,  cos(b)  }
    }

    rotate on z-axis:
    {
      {cos(c),  -sin(c),  0},
      {sin(c),  cos(c),   0},
      {0,    0,    1}
    }
    
   */
  double angle_x = angle + x_rotation_offset;
  double angle_y = angle + y_rotation_offset;
  double angle_z = angle + z_rotation_offset;

  double x_array[3][3] =
    {
      {1,  0,    0},
      {0,  cos(angle_x),  -sin(angle_x)},
      {0,  sin(angle_x),  cos(angle_x)}
    };

  double y_array[3][3] = 
    {
      {cos(angle_y),  0,  sin(angle_y)  },
      {0,    1,  0  },
      {-sin(angle_y),  0,  cos(angle_y)  }
    };

  double z_array[3][3] = 
    {
      {cos(angle_z),  -sin(angle_z),  0},
      {sin(angle_z),  cos(angle_z),   0},
      {0,    0,    1}
    };

  
  for (int vertex = 0; vertex < 8; vertex++)
  {
    // flags at beginning for customisation if we want certain rotations or not
    if (rotate_x)
    {
      multiply_rotation_matrices(&x_array[0][0], &cube_vertices_remap[vertex][0]);
    }
    
    if(rotate_y)
    {
      multiply_rotation_matrices(&y_array[0][0], &cube_vertices_remap[vertex][0]);
    }
    
    if (rotate_z)
    {
      multiply_rotation_matrices(&z_array[0][0], &cube_vertices_remap[vertex][0]);
    }
    

    for (int j = 0; j < 3; j++)
    {
      std::cout << cube_vertices_remap[vertex][j] << ' ';
    }
    std::cout << '\n';
  }

  
  // translate back to original centre point
  // kinda hacky i just recopied the code lol maybe i should use a function for transforming coordinates too...
  for (int vertex = 0; vertex < 8; vertex++)
  {
    for (int coord = 0; coord < 3; coord++)
    {
      if (coord == 0)
      {
        cube_vertices_remap[vertex][coord] +=  centre_x;
      } else if (coord == 1) {
        cube_vertices_remap[vertex][coord] += centre_y;
      } else {
        cube_vertices_remap[vertex][coord] += centre_z;
      }
    }
  }

  // write to coordinate pointer
  // lots of for loops here can probably simplify it down
  for (int vertex = 0; vertex < 8; vertex++)
  {
    for (int coord = 0; coord < 3; coord++)
    {  
      *(p_cube_vertices + coord + 3 * vertex) = cube_vertices_remap[vertex][coord];
    }
  }
}

void multiply_rotation_matrices(double* matrix, double* coord)
{
  // apply arrays to local scope
  double rotation_matrix[3][3];
  double rotation_coord[3];
  double new_coord[3];


  for (int i = 0; i < 3; i++)
  {
    rotation_coord[i] = *(coord + i);
  }

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {  
      rotation_matrix[i][j] = *(matrix + 3*i + j);
    }
  }

  // rotation matrices work by having the rotation matrix multiply by coordinate 
  for (int i = 0; i < 3; i++)
  {
    double sum{0};
    for (int j = 0; j < 3; j++)
    {
      sum += rotation_matrix[i][j] * rotation_coord[j];
    }
    new_coord[i] = sum;
  }
  
  // rewrite coordinate to p_coord
  for (int i = 0; i < 3; i++)
  {
    *(coord + i) = new_coord[i];
  }
}

void loop(int terminal_x, int terminal_y, int terminal_z, double* p_cube_vertices, double cube_vertices[8][3])
{
  /*
  test_vertex_render(terminal_x, terminal_y, cube_vertices);

  double rotation = rotation_angle;
  
  while (true)
  {
    rotate_vertices(rotation, cube_vertices, p_cube_vertices, terminal_x, terminal_y, terminal_z);

    test_vertex_render(terminal_x, terminal_y, cube_vertices);
    rotation += rotation_increment;  
    Sleep(1000);
  }
  */
  double rotation = rotation_increment; // rotation_angle;

  while (true)
  {
    rotate_vertices(rotation, cube_vertices, p_cube_vertices, terminal_x, terminal_y, terminal_z);

    test_vertex_render(terminal_x, terminal_y, cube_vertices);
    //rotation += rotation_increment; 
    //std::cout << "growing rotation by " << rotation_increment << " to get " << rotation << std::endl;

#ifdef _MSC_VER
    Sleep(1000);
#else
    sleep(1);
#endif
  }
}

void group_triangles(double cube_vertices[8][3], double* p_triangles)
{
  /*
    to start grouping triangles, we pick two random vertices. these two vertices cannot exceed the length of pythag_len (shortest length between two vertices)
    we then find a vertex that is parallel to the vector created by our first two vertices (using cross product) and also is the length of pythag_len. this will
    be our three vertices that makes up a triangle. we then take the third and first vertex and do the same, ignoring the second vertex.

    each vertex can only be part of three triangles
   */

  double pythag_len = sqrt((pow(len_to_vertex, 2)) / 3);

}
