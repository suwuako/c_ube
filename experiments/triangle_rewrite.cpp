#include <iostream>
#include <math.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include  <sys/ioctl.h>
    #include <termios.h>
    #include <unistd.h>
#endif


const int triangle_offset = 20;
const char lightmap[] = {'#', '-'};

bool in_triangle(double* p_vertices, double* A, double* B, double* C, int x, int y);
double get_depth_buffer(double* A, double* B, double* C, int x, int y);
void get_screen_size(int* p_screen);
void render_frame(double* p_triangles, int terminal_x, int terminal_y);
void assign_coordinates(double* p_triangles, double* A, double* B, double* C, int triangle_iteration);
char get_pixel(double* p_triangles, int x, int y);

struct coordinate {
    double x;
    double y;
    double z;
};

int main()
{
    int screen_size[2];
    get_screen_size(&screen_size[0]);
    
    int terminal_x = screen_size[0];
    int terminal_y = screen_size[1];

    double centre_x = terminal_x / 2;
    double centre_y = terminal_y / 2;
    double centre_z = (terminal_x + terminal_y) / 2;

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

    std::cout << "dimensions: " << screen_size[0] << ", " << screen_size[1] << '\n';
    render_frame(p_triangles, terminal_x, terminal_y);
    
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

bool in_triangle(double* p_vertices, double* A, double* B, double* C, int x, int y)
{
    double A_x = *A;
    double A_y = *(A + 1);
    double B_x = *B;
    double B_y = *(B + 1);
    double C_x = *C;
    double C_y = *(C + 1);

    double alpha = ((B_y - C_y)*(x - C_x) + (C_x - B_x)*(y - C_y)) / ((B_y - C_y)*(A_x - C_x) + (C_x - B_x)*(A_y - C_y));
    double beta = ((C_y - A_y)*(x - C_x) + (A_x - C_x)*(y - C_y)) / ((B_y - C_y)*(A_x - C_x) + (C_x - B_x)*(A_y - C_y));
    double gamma = 1.0f - alpha - beta;

    if (alpha >= 0 && beta >= 0 && gamma >= 0 && (alpha + beta + gamma) <= 1)
    {
        return true;
    }
    return false;
}

void assign_coordinates(double* p_triangles, double* A, double* B, double* C, int triangle_iteration)
{
    for (int i = 0; i < 3; i++)
    {
        *(A + i) = *(p_triangles + i + 9 * triangle_iteration);
        *(B + i) = *(p_triangles + 3 + i + 9 * triangle_iteration);
        *(C + i) = *(p_triangles + 6 + i + 9 * triangle_iteration);
    }
}

char get_pixel(double* p_triangles, int x, int y)
{
    double A[3];
    double B[3];
    double C[3];

    double point_depth;
    double depth_buffer = 0;
    char output_char = ' ';
        
    for (int triangle = 0; triangle < 2; triangle++)
    {
        assign_coordinates(p_triangles, A, B, C, triangle);
        point_depth = get_depth_buffer(A, B, C, x, y);

        if (in_triangle(p_triangles, A, B, C, x, y) && point_depth > depth_buffer)
        {
            output_char = lightmap[triangle];
            depth_buffer = point_depth;
        }
    }
    return output_char;
}

void render_frame(double* p_triangles, int terminal_x, int terminal_y)
{
    for (int x = 0; x < terminal_x; x++)
    {
        for (int y = 0; y < terminal_y; y++)
        {
            std::cout << get_pixel(p_triangles, x, y);
        }
        std::cout << '\n';
    }
}

double get_depth_buffer(double* A, double* B, double* C, int x, int y)
{
    double alpha;
    double beta;

    double A_x = *A;
    double A_y = *(A + 1);
    double A_z = *(A + 2);

    double B_x = *B;
    double B_y = *(B + 1);
    double B_z = *(B + 2);

    double C_x = *C;
    double C_y = *(C + 1);
    double C_z = *(C + 2);

    alpha = ((C_x - A_x) * (y - A_y) - (C_y - A_y) * (x - A_x)) / ((B_y - A_y) * (C_x - A_x) - (B_x - A_x) * (C_y - A_y));
    beta = (x - A_x - alpha * (B_x - A_x)) / (C_x - A_x);

    return A_z + alpha * (B_z - A_z) + beta * (C_z - A_z);
}