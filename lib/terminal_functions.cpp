#include <iostream>

#include "datatypes.hpp"

#ifdef _WIN32
    #include <windows.h>
#else 
    #include <sys/ioctl.h>
    #include <termios.h>
    #include <unistd.h>
#endif

struct coordinate_3d get_screen_size()
{
    struct coordinate_3d screen_size;
    
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    screen_size.y = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    screen_size.x = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

#else
    int demanded_fd = 1;
    struct winsize size_info;

    int err_code = ioctl(demanded_fd, TIOCGWINSZ, &size_info);
    if (0 != err_code)
    {
        std::cerr << "[WARN] could not execute ioctl call. Using default col/row bounds." << std::endl;
    }
    screen_size.y = size_info.ws_col;
    screen_size.x = size_info.ws_row;
#endif

    screen_size.z = screen_size.x + screen_size.y;

    return screen_size;
}
