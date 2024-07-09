#include <stdio.h>
#include <math.h>

#include "cube_math.h"
#include "datatypes.h"

// creates the 8 coordinates of the cube
void create_cube_vertices(struct coord_3d cube_vertices[VERTEX_COUNT],
                          struct cube_arguments cube_parameters)
{
    /* Creating cube vertices
         * assume we have a point in space. to create a cube, we first
         * move across the x-axis, y-axis and then z-axis finally. 
         * Combine this with a combination of moving positively (+x) (-x)
         * across all the axises and we can make the vertices of a cube. 
         * We can accomplish this by having a 'binary' like system where
         * we start off with {0, 0, 0}, where 0 means subract and 1 means
         * add. After each iteration, we increment the last digit by 1. 
         * This is in base 2.
    */
    int cube_combination[AXIS_COUNT] = {0, 0, 0};
    double offset = cube_parameters.size / 2;
    struct coord_3d terminal_centers;

    terminal_centers.x = cube_parameters.terminal_size.x / 2;
    terminal_centers.y = cube_parameters.terminal_size.y / 2;
    terminal_centers.z = cube_parameters.terminal_size.z / 2;

    for (int vertex = 0; vertex < VERTEX_COUNT; vertex++)
    {
        for (int axis = 0; axis < AXIS_COUNT; axis++)
        {
            set_singular_vertex(cube_vertices, terminal_centers, cube_combination, offset, vertex, axis);

        }
        update_cube_combination(cube_combination);
    }
}

// helper function for create_cube_vertices
// updates cube_combination to be point to a different axis
void update_cube_combination(int cube_combination[AXIS_COUNT])
{
    for (int i = 0; i < AXIS_COUNT; i++)
    {
        if (cube_combination[i] == 0)
        {
            cube_combination[i] = 1;
            return;
        } else if (cube_combination[i] == 1) {
            cube_combination[i] = 0;
        }
    }
}

// helper function for create_cube_vertices
// sets vertex according to cube_combination
void set_singular_vertex(struct coord_3d cube_vertices[VERTEX_COUNT],
                         struct coord_3d terminal_center,
                         int cube_combination[AXIS_COUNT], double offset,
                         int vertex, int axis)
{
    if (cube_combination[axis] == 0)
    {
        offset = -offset;
    }

    switch (axis)
    {
        case 0:
            cube_vertices[vertex].x = offset + terminal_center.x;
            break;
        case 1:
            cube_vertices[vertex].y = offset + terminal_center.y;
            break;
        case 2:
            cube_vertices[vertex].z = offset + terminal_center.z;
            break;
    }
}
