#include <iostream>
#include <math.h>

#include "cube_math.hpp"
#include "datatypes.hpp"

void create_cube_vertices(
        struct coordinate_3d cube_vertices[8], 
        struct coordinate_3d terminal_dimensions, 
        int DISTANCE_BETWEEN_CLOSEST_VERTICES
        )
{
    struct coordinate_3d terminal_centre;

    terminal_centre.x = terminal_dimensions.x / 2;
    terminal_centre.y = terminal_dimensions.y / 2;
    terminal_centre.z = terminal_dimensions.z / 2;

    // since we are doing a combination of adding and subtracting DISTANCE_BETWEEN_CLOSEST_VERTICES from the centre,
    // we're just gonna end up with half the distance to adjust between vertices.
    double adjusted_distance = DISTANCE_BETWEEN_CLOSEST_VERTICES / 2;

    // all combinations of adding and subtacting DISTANCE_BETWEEN_CLOSEST_VERTICES from the centre creates the 8 vertices
    // i.e. 1 -1 -1 represents adding DISTANCE_BETWEEN_CLOSEST_VERTICES to terminal_centre.x
    // while subtacting from terminal_centre.y and terminal_centre.z
    int vertex_combination_tracker[3] = {1, 1, 1};

    std::cout << "Centre of 3d plane: {" << terminal_centre.x << ", " << terminal_centre.y << ", " << terminal_centre.z << "}\n";
    std::cout << "Length from centre of vertex: " << adjusted_distance << '\n';


    for (int vertex = 0; vertex < VERTEX_COUNT; vertex++)
    {
        cube_vertices[vertex].x = vertex_combination_tracker[0] * adjusted_distance + terminal_centre.x;
        cube_vertices[vertex].y = vertex_combination_tracker[1] * adjusted_distance + terminal_centre.y; 
        cube_vertices[vertex].z = vertex_combination_tracker[2] * adjusted_distance + terminal_centre.z; 

        int increment = 1;

        for (int i = 0; i < 3; i++)
        {
            if (increment == 1 && vertex_combination_tracker[i] == -1)
            {
                vertex_combination_tracker[i] = 1;
                    increment = 0;
            } else if (increment == 0 && vertex_combination_tracker[i] == 1) {
                vertex_combination_tracker[i] = 1;
            } else {
                vertex_combination_tracker[i] = -1;
            }
        }   
    }
}

struct coordinate_3d vectorise(struct coordinate_3d A, struct coordinate_3d B)
{
    struct coordinate_3d vector_AB = {};
    
    vector_AB.x = B.x - A.x;
    vector_AB.y = B.y - A.y;
    vector_AB.z = B.z - A.z;

    return vector_AB;
}

double get_vector_length(struct coordinate_3d AB)
{
    return sqrt(pow(AB.x, 2) + pow(AB.y, 2) + pow(AB.z, 2));
}

void create_origin_vertices(
        struct coordinate_3d cube_vertices[8],
        struct coordinate_3d origin_vertices[4],
        int DISTANCE_BETWEEN_CLOSEST_VERTICES
        )
{
    int origin_array_tracker = 1;

    // Since origin vertices are found by using EXACTLY the pythagorean length of 
    // DISTANCE_BETWEEN_CLOSEST_VERTICES, we save it first :)
    double pythagorean_length = sqrt(2 * pow(DISTANCE_BETWEEN_CLOSEST_VERTICES,2 ));

    for (int vertex = 0; vertex < VERTEX_COUNT; vertex++)
    {
        struct coordinate_3d vector_origin_to_vertex = vectorise(origin_vertices[0], cube_vertices[vertex]);
        double origin_to_vertex_length = get_vector_length(vector_origin_to_vertex);

        if (origin_to_vertex_length == pythagorean_length)
        {
            origin_vertices[origin_array_tracker] = cube_vertices[vertex];
            
            origin_array_tracker += 1;
        }
    }
}

void group_ajacent_points(
        struct coordinate_3d origin_point,
        struct coordinate_3d cube_vertices[VERTEX_COUNT],
        struct coordinate_3d ajacent_points[3],
        int DISTANCE_BETWEEN_CLOSEST_VERTICES
        )
{
    /*
     * given an origin point and 8 other vertex points, determine which 3 points lie within a given distance of said origin point
     */
    int ajacent_points_array_count = 0;

    for (int vertex = 0; vertex < VERTEX_COUNT; vertex++)
    {
        struct coordinate_3d origin_to_vector = vectorise(origin_point, cube_vertices[vertex]);
        double origin_to_vector_length = get_vector_length(origin_to_vector);
    
        if (origin_to_vector_length == DISTANCE_BETWEEN_CLOSEST_VERTICES)
        {
            ajacent_points[ajacent_points_array_count] = cube_vertices[vertex];
            ajacent_points_array_count++;
        }
    }
}


void group_vertices_to_triangles(
        struct coordinate_3d triangle_vertices[12][3],
        struct coordinate_3d cube_vertices[8],
        int DISTANCE_BETWEEN_CLOSEST_VERTICES
        )
{
    /*
     * acts as a wrapper for other actions on vertices to group cube vertices into triangles
     */

    // find origin vertices
    struct coordinate_3d origin_vertices[4] = {};
    
    // pick arbitrary vertex as origin vertex
    origin_vertices[0] = cube_vertices[0];
    create_origin_vertices(cube_vertices, origin_vertices, DISTANCE_BETWEEN_CLOSEST_VERTICES);

    std::cout << "\nCreating origin vertices...\n";
    std::cout << origin_vertices[0].x << ' ' << origin_vertices[0].y << ' ' << origin_vertices[0].z << '\n';
    std::cout << origin_vertices[1].x << ' ' << origin_vertices[1].y << ' ' << origin_vertices[1].z << '\n';
    std::cout << origin_vertices[2].x << ' ' << origin_vertices[2].y << ' ' << origin_vertices[2].z << '\n';
    std::cout << origin_vertices[3].x << ' ' << origin_vertices[3].y << ' ' << origin_vertices[3].z << "\n\n";

    int triangle_vertex_count = 0;
    // for each origin vertex, find ajacent vertices and group two ajacent vertices and origin vertex into a triangle
    for (int origin_vertex = 0; origin_vertex < ORIGIN_VERTEX_COUNT; origin_vertex++)
    {
        struct coordinate_3d ajacent_points[3] = {};
        group_ajacent_points(origin_vertices[origin_vertex], cube_vertices, ajacent_points, DISTANCE_BETWEEN_CLOSEST_VERTICES);

        for (int i = 0; i < 3; i++)
        {
            if (i == 0)
            {
                triangle_vertices[triangle_vertex_count][0] = origin_vertices[origin_vertex];
                triangle_vertices[triangle_vertex_count][1] = ajacent_points[0];
                triangle_vertices[triangle_vertex_count][2] = ajacent_points[1];
            } else if (i == 1) {
                triangle_vertices[triangle_vertex_count][0] = origin_vertices[origin_vertex];
                triangle_vertices[triangle_vertex_count][1] = ajacent_points[1];
                triangle_vertices[triangle_vertex_count][2] = ajacent_points[2];
            } else if (i == 2) {
                triangle_vertices[triangle_vertex_count][0] = origin_vertices[origin_vertex];
                triangle_vertices[triangle_vertex_count][1] = ajacent_points[0];
                triangle_vertices[triangle_vertex_count][2] = ajacent_points[2];
            }
            triangle_vertex_count++;
        }
    }
}

void rotate(
        struct coordinate_3d cube_vertices[8],
        struct coordinate_3d terminal_dimensions,
        double angle
        )
{
    struct coordinate_3d terminal_centre;

    terminal_centre.x = terminal_dimensions.x / 2;
    terminal_centre.y = terminal_dimensions.y / 2;
    terminal_centre.z = terminal_dimensions.z / 2;
    
    double z_rotation[3][3] = 
    {
        {cos(angle),  -sin(angle),    0},
        {sin(angle),  cos(angle),     0},
        {0,           0,              1}
    };

    double y_rotation[3][3] = 
    {
        {cos(angle),    0,          sin(angle)  },
        {0,             1,          0           },
        {-sin(angle),   0,          cos(angle)  }
    };

    double x_rotation[3][3] = 
    {
        {1,             0,          0},
        {0,             cos(angle), -sin(angle)},
        {0,             sin(angle), cos(angle)}
    };
    
    
    for (int vertex = 0; vertex < 8; vertex++)
    {
        cube_vertices[vertex].x -= terminal_centre.x;
        cube_vertices[vertex].y -= terminal_centre.z;
        cube_vertices[vertex].z -= terminal_centre.y;

        
    }
}
