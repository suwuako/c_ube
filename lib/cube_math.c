#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cube_math.h"
#include "datatypes.h"

// converts two points into a vector in the form AB
struct coord_3d points_to_vector(struct coord_3d a, struct coord_3d b)
{
    // we are creating vector AB (b - a);
    struct coord_3d vector;
    vector.x = b.x - a.x;
    vector.y = b.y - a.y;
    vector.z = b.z - a.z;

    return vector;
}

// gets the length of a vector
double get_vector_length(struct coord_3d vec)
{
    double length;

    length = sqrt(abs(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2)));
    return length;
}

// groups the 8 vertices of the cube into
// the 12 triangles that make up our cube
void group_vertices_to_triangles(struct coord_3d cube_vertices[VERTEX_COUNT],
                                 struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES],
                                 struct cube_arguments cube_parameters)
{
    /* Grouping vertices into triangles
        * Fortunately, theres a very easy way (and geometric) way to group
        * the vertices of a cube into 12 triangles that make up a cube. 
        * 
        * Consider picking a random vertex of a cube at random. Connecting
        * to that vertex are three adacent vertices that make up the edge 
        * of a cube. connecting any two of those three vertices with our chosen
        * vertex creates our cube. If we then search for vertices that lie with
        * exactly the pythagorean length of the edges of the square, we can see 
        * that the triangles created by the four "main" vertices generates our 12
        * required triangles.
    */
    const double pythagorean_length = sqrt(1.0 * pow(cube_parameters.size, 2));
    struct coord_3d main_vertices[MAIN_VERTEX_COUNT];
    int main_vertex_index = 1;

    // arbitrarily picking a random point to be one of our "main" vertices
    main_vertices[0] = cube_vertices[0];
    get_main_vertices(cube_vertices, main_vertices, &main_vertex_index, pythagorean_length);
    get_triangles_from_main_vertices(main_vertices, cube_vertices, triangles, cube_parameters.size * 1.0);

}

// helper function for group_vertices_to_triangles
// using our main vertices, we expand outwards and group
// alterative vertices into triangles
void get_triangles_from_main_vertices(struct coord_3d main_vertices[MAIN_VERTEX_COUNT],
                                      struct coord_3d cube_vertices[VERTEX_COUNT],
                                      struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES],
                                      double cube_side_length)
{
    int triangle_index = 0;

    // iterates through each main vertex
    for (int main_vertex = 0; main_vertex < MAIN_VERTEX_COUNT; main_vertex++)
    {
        struct coord_3d current_vertex = main_vertices[main_vertex];
        struct coord_3d ajacent_vertices[TRIANGLE_VERTICES] = {};
        int ajacent_vertex_index = 0;

        // gets all ajacent vertices of each vertex
        for (int vertex = 0; vertex < VERTEX_COUNT; vertex++)
        {
            struct coord_3d point = cube_vertices[vertex];
            struct coord_3d main_vec = points_to_vector(current_vertex, point);
            double length = get_vector_length(main_vec);

            if (cube_side_length == length)
            {
                ajacent_vertices[ajacent_vertex_index] = cube_vertices[vertex];
                ajacent_vertex_index++;
            }
        }

        // assigns combinations of ajacent vertices to triangles
        for (int ajacent_vertex = 0; ajacent_vertex < TRIANGLE_VERTICES; ajacent_vertex++)
        {
            // first coordinate is always the main vertex
            triangles[triangle_index][0] = current_vertex;

            int next_index = ajacent_vertex + 1;
            if (next_index > 2)
            {
                next_index = 0;
            }


            triangles[triangle_index][1] = ajacent_vertices[ajacent_vertex];
            triangles[triangle_index][2] = ajacent_vertices[next_index];

            triangle_index += 1;
        }
    }
}

// helper funciton for group_vertices_to_triangles
// gets our main vertices (explained in function above)
void get_main_vertices(struct coord_3d cube_vertices[VERTEX_COUNT],
                       struct coord_3d main_vertices[MAIN_VERTEX_COUNT],
                       int *p_main_vertex_index, double pythagorean_length)
{
    for (int i = 0; i < VERTEX_COUNT; i++)
    {
        struct coord_3d vec_main_to_point = {};
        double vec_length;

        vec_main_to_point = points_to_vector(main_vertices[0], cube_vertices[i]);
        vec_length = get_vector_length(vec_main_to_point);


        if (vec_length == pythagorean_length)
        {
            main_vertices[*p_main_vertex_index] = cube_vertices[i];
            *p_main_vertex_index += 1;
        }
    }
}

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
        /*
        // debug version without terminal_centers
        case 0:
            cube_vertices[vertex].x = offset;
            break;
        case 1:
            cube_vertices[vertex].y = offset;
            break;
        case 2:
            cube_vertices[vertex].z = offset;
            break;
        */

        // working copy
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
