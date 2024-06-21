#include <iostream>

#include "cube_math.hpp"
#include "datatypes.hpp"
#include "render.hpp"

void render_frame(
        struct coordinate_3d terminal_dimensions,
        struct coordinate_3d triangle_vertices[12][3]
        )
{
    for (int x = 0; x < terminal_dimensions.x; x++)
    {
        for (int y = 0; y < terminal_dimensions.y; y++)
        {
            std::cout << get_pixel(triangle_vertices, x, y);
        }
        std::cout << '\n';
    }
}

char get_pixel(
        struct coordinate_3d triangle_vertices[12][3],
        int x,
        int y
        )
{
    double point_depth = 0;
    double depth_buffer = 0;
    char output_char = ' ';

    for (int triangle = 0; triangle < TRIANGLE_COUNT; triangle++)
    {

        point_depth = get_depth_buffer(triangle_vertices[triangle], x, y);
        
        if (in_triangle(triangle_vertices[triangle], x, y) && point_depth > depth_buffer)
        {
            output_char = '1';
        }
    }
    return output_char;
}

bool in_triangle(
        struct coordinate_3d p[3],
        int x,
        int y
        )
{
    struct coordinate_3d a = p[0];
    struct coordinate_3d b = p[1];
    struct coordinate_3d c = p[2];
   
    double alpha = ((b.y - c.y) * (x - c.x) + (c.x - b.x) * (y - c.y)) / 
        ((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
    double beta = ((c.y - a.y) * (x - c.x) + (a.x - c.x) * (y - c.y)) /
        ((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
    double gamma = 1.0f - alpha - beta;

    if (alpha >= 0 && beta >= 0 && gamma >= 0)
    {
        return true;
    }
    return false;
}

double get_depth_buffer(
        struct coordinate_3d p[3],
        int x,
        int y
        )
{
    // p is an array that represents a triangle (its named p because i dont wanna type out long variable names)
    double alpha;
    double beta;

    struct coordinate_3d a = p[0];
    struct coordinate_3d b = p[1];
    struct coordinate_3d c = p[2];

    alpha = ((c.x - a.x) * (y - a.y) - (c.y - a.y) * (x - a.x)) / ((b.y - a.y) * (c.x - a.x) - (b.x - a.x) * (c.y - a.y));
    beta = (x - a.y - alpha * (b.x - a.x)) / (c.x - a.x);

    return a.z + alpha * (b.z - a.z) + beta * (c.z - a.z);
}

                
