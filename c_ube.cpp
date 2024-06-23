#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>

#include "lib/datatypes.hpp"
#include "lib/terminal_functions.hpp"
#include "lib/cube_math.hpp"
#include "lib/render.hpp"

// header guards for windows/linux
#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <termios.h>
    #include <unistd.h>
#endif

const int frame_rate = 2000;
const int DISTANCE_BETWEEN_CLOSEST_VERTICES = 30;
const double ROTATION_ANGLE = M_PI/100;


int main(void)
{
    struct coordinate_3d terminal_dimensions = get_screen_size();
    
    std::cout << "terminal_x: " << terminal_dimensions.x << '\n';
    std::cout << "terminal_y: " << terminal_dimensions.y << '\n';
    std::cout << "terminal_z: " << terminal_dimensions.z << '\n';
    std::cout << '\n';

    struct coordinate_3d cube_vertices[8] = {};
    struct coordinate_3d *p_cube_vertices = &cube_vertices[0];
    create_cube_vertices(cube_vertices, terminal_dimensions, DISTANCE_BETWEEN_CLOSEST_VERTICES);

    // print vertices
    std::cout << "Vertices:\n";
    for (int vertex = 0; vertex < VERTEX_COUNT; vertex++)
    {
        std::cout << cube_vertices[vertex].x << ' ' << cube_vertices[vertex].y << ' ' << cube_vertices[vertex].z << '\n';
    }
    
    // a triangle is composed of three vertices that we store
    // each vertex is a 3d coordinate
    struct coordinate_3d triangle_vertices[12][3] = {};

    // print triangles
    for (int triangle = 0; triangle < TRIANGLE_COUNT; triangle++)
    {
        printf("Triangle %d\n", triangle);
        for (int coordinate = 0; coordinate < 3; coordinate++)
        {
            printf("%lf %lf %lf\n", triangle_vertices[triangle][coordinate].x, triangle_vertices[triangle][coordinate].y, triangle_vertices[triangle][coordinate].z);
        }
        std::cout << '\n';
    }

    while (1)
    {
        group_vertices_to_triangles(triangle_vertices, cube_vertices, DISTANCE_BETWEEN_CLOSEST_VERTICES);

        render_frame(terminal_dimensions, triangle_vertices);
        //rotate(p_cube_vertices, cube_vertices, terminal_dimensions, ROTATION_ANGLE);


#ifdef _WIN32
        Sleep(frame_rate);

#else
        std::chrono::milliseconds timespan(frame_rate);
        std::this_thread::sleep_for(timespan);
#endif
    }

    return 0;
}
