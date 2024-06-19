#include <iostream>
#include <cmath>

#include "lib/datatypes.hpp"
#include "lib/terminal_functions.hpp"

// header guards for windows/linux
#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <termios.h>
    #include <unistd.h>
#endif

const int distance_between_closest_vertices = 30;
const double rotation_angle = M_PI/100;


int main(void)
{
    struct coordinate_2d terminal_dimensions = get_screen_size();
    std::cout << terminal_dimensions.x << terminal_dimensions.y << '\n';

    return 0;
}
