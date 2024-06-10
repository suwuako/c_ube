#include <iostream>
#include <math.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include  <sys/ioctl.h>
    #include <termios.h>
    #include <unistd.h>
#endif


const int triangle_offset = 10;

bool in_triangle(double* p_vertices, int x, int y);
void get_screen_size(int* p_screen);

struct coordinate {
    double x;
    double y;
    double z;
};

int main()
{
    int screen_size[2];
    get_screen_size(&screen_size[0]);

    double centre_x = screen_size[0] / 2;
    double centre_y = screen_size[1] / 2;
    double centre_z = (screen_size[0] + screen_size[1]) / 2;

    double triangles[3][2] = {
        {35, 70},
        {20, 40},
        {50, 20}
    };
        
    double* p_triangles = &triangles[0][0];

    std::cout << "dimensions: " << screen_size[0] << ", " << screen_size[1] << '\n';

    for (int x = 0; x < screen_size[0]; x++)
    {
        for (int y = 0; y < screen_size[1]; y++)
        {
            std::cout << in_triangle(p_triangles, x, y);
        }
        std::cout << '\n';
    }
    return 0;
}


void get_screen_size(int* p_screen)
{
    // create pointer variables to represent the two points of the array in screen_size[2]
    int* screen_row = p_screen;
    int* screen_col = (p_screen + 1);

	// get console width and height
	int columns = 150, rows = 35;

#ifdef _WIN32
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

bool in_triangle(double* p_vertices, int x, int y)
{
    double A_x = *p_vertices;
    double A_y = *(p_vertices + 1);
    double B_x = *(p_vertices + 2);
    double B_y = *(p_vertices + 3);
    double C_x = *(p_vertices + 4);
    double C_y = *(p_vertices + 5);

    double alpha = ((B_y - C_y)*(x - C_x) + (C_x - B_x)*(y - C_y)) / ((B_y - C_y)*(A_x - C_x) + (C_x - B_x)*(A_y - C_y));
    double beta = ((C_y - A_y)*(x - C_x) + (A_x - C_x)*(y - C_y)) / ((B_y - C_y)*(A_x - C_x) + (C_x - B_x)*(A_y - C_y));
    double gamma = 1.0f - alpha - beta;

    if (alpha >= 0 && beta >= 0 && gamma >= 0 && (alpha + beta + gamma) <= 1)
    {
        return true;
    }
    return false;

    return false;
}
