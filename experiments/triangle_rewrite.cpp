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

int main()
{
    int screen_size[2];
    get_screen_size(&screen_size);

    double centre_x = screen_size[0] / 2;
    double centre_y = screen_size[1] / 2;
    double centre_z = (screen_size[0] + screen_size[1]) / 2;

    double triangles[2][3][3] = {
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

    double* p_triangles = &triangles[0][0][0];

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

}
