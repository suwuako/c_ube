#include <iostream>

#include "datatypes.hpp"

#ifdef _WIN32
    #include <windows.h>
#else 
    #include <sys/ioctl.h>
    #include <termios.h>
    #include <unistd.h>
#endif

struct coordinate_2d get_screen_size()
{
    struct coordinate_2d screen_size;
    screen_size.x = 20.0;
    screen_size.y = 30.0;
    return screen_size;
}
