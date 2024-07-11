#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "cube_math.h"
#include "datatypes.h"


/*
    * ROTATION MATRIES
*/
// rotates the 8 points of the cube
void rotate_vertices(struct coord_3d cube_vertices[VERTEX_COUNT],
                     struct cube_arguments cube_parameters)
{
    double x_angle = cube_parameters.x_angle;
    double y_angle = cube_parameters.y_angle;
    double z_angle = cube_parameters.z_angle;
    struct coord_3d terminal = cube_parameters.terminal_size;
    struct rotation_matrix rotate = init_rotation_matrices(x_angle, y_angle, z_angle);

    // transform each point based back to its origin
    transform_vertices(cube_vertices, terminal, true);
    multiply_rotation_matrices(cube_vertices, rotate);
    transform_vertices(cube_vertices, terminal, false);
}

// multiplies rotation matrices
// helper function for rotation_vertices
void multiply_rotation_matrices(struct coord_3d cube_vertices[VERTEX_COUNT],
                                struct rotation_matrix matrix)
{
    for (int vertex = 0; vertex < VERTEX_COUNT; vertex++)
    {
        struct coord_3d rotated_point;

        rotated_point = multiply_matrix(matrix.x_rotate, cube_vertices[vertex]);
        rotated_point = multiply_matrix(matrix.y_rotate, rotated_point);
        rotated_point = multiply_matrix(matrix.z_rotate, rotated_point);

        cube_vertices[vertex].x = rotated_point.x;
        cube_vertices[vertex].y = rotated_point.y;
        cube_vertices[vertex].z = rotated_point.z;

    }
}

struct coord_3d multiply_matrix(double matrix[3][3],
                                struct coord_3d point)
{
    /*
     * | a b c |     | x |   | ax + by + cz |
     * | d e f |  x  | y | = | dx + ey + fz |
     * | g h i |     | z |   | gx + hy + iz |
     */

    struct coord_3d return_point;
    double point_arr[3];
    double new_point[3];

    point_arr[0] = point.x;
    point_arr[1] = point.y;
    point_arr[2] = point.z;

    for (int row = 0; row < 3; row++)
    {
        double sum = 0;
        for (int col = 0; col < 3; col++)
        {
            sum += matrix[row][col] * point_arr[col];
        }
        new_point[row] = sum;
    }

    return_point.x = new_point[0];
    return_point.y = new_point[1];
    return_point.z = new_point[2];

    return return_point;
}

// transforms vertices to the origin (0,0,0) or away (centre_x, centre_y, centre_z)
// towards_origin = true -> 0, 0, 0; towards_origin = false -> centres
void transform_vertices(struct coord_3d cube_vertices[VERTEX_COUNT],
                        struct coord_3d terminal, bool towards_origin)
{
    struct coord_3d center;
    center.x = terminal.x / 2;
    center.y = terminal.y / 2;
    center.z = terminal.z / 2;

    for (int i = 0; i < VERTEX_COUNT; i++)
    {
        if (towards_origin)
        {
            cube_vertices[i].x -= center.x;
            cube_vertices[i].y -= center.y;
            cube_vertices[i].z -= center.z;
        } else {
            cube_vertices[i].x += center.x;
            cube_vertices[i].y += center.y;
            cube_vertices[i].z += center.z;
        }
    }
}

struct rotation_matrix init_rotation_matrices(double x, double y, double z)
{
    struct rotation_matrix matrix;
    double x_rotate[3][3] = {
        {1,             0,          0},
        {0,             cos(x), -sin(x)},
        {0,             sin(x), cos(x)}
    };

    double y_rotate[3][3] = {
        {cos(y),    0,          sin(y)  },
        {0,             1,          0           },
        {-sin(y),   0,          cos(y)  }
    };

    double z_rotate[3][3] = {
        {cos(z),  -sin(z),    0},
        {sin(z),  cos(z),     0},
        {0,           0,              1}
    };

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            matrix.x_rotate[i][j] = x_rotate[i][j];
            matrix.y_rotate[i][j] = y_rotate[i][j];
            matrix.z_rotate[i][j] = z_rotate[i][j];
        }
    }
    
    return matrix;
}

/*
    * VECTOR MATH STUFF
*/
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

    length = sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
    return length;
}


/*
    * TRIANGLE SECTION
*/
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
    const double pythagorean_length = sqrt(2 * cube_parameters.size * cube_parameters.size);
    struct coord_3d main_vertices[MAIN_VERTEX_COUNT] = {};
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

            // weird ass bug where something something floating points don't equal
            if (abs(cube_side_length - length) < 0.01)
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

        // weird ass bug where something something floating points don't equal
        if (abs(vec_length - pythagorean_length) < 0.5)
        {
            main_vertices[*p_main_vertex_index] = cube_vertices[i];
            *p_main_vertex_index += 1;
        }
    }
}

/*
    * CUBE VERTEX
*/
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
