#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cube_math.h"
#include "render.h"
#include "datatypes.h"
#include "misc.h"


double get_max_depth(struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES],
                     struct coord_3d terminal)
{
    double greatest_depth = 0;
    for (int x = 0; x < terminal.x; x++)
    {
        for (int y = 0; y < terminal.y; y++)
        {
            for (int tri = 0; tri < TRIANGLE_COUNT; tri++)
            {
                struct coord_3d p;
                p.x = x;
                p.y = y;

                double depth = get_pixel_depth(p, triangles[tri]);
                if (depth > greatest_depth && point_in_triangle(p, triangles[tri]))
                {
                    greatest_depth = depth;
                }
            }
        }
    }

    return greatest_depth;
}
void render_frame(struct cube_arguments params,
                  struct coord_3d cube_vertices[VERTEX_COUNT],
                  struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES])
{
    struct coord_3d terminal = params.terminal_size;
    struct coord_3d normal_vectors[6] = {};
    double max_depth = get_max_depth(triangles, terminal);

    get_normal_vectors(normal_vectors, triangles);

    // optimisations required to render at larger resolutions without screen splitting
    for (int x = 0; x < terminal.x; x++)
    {
        for (int y = 0; y < terminal.y; y++)
        {
            struct coord_3d p;
            p.x = x;
            p.y = y;

            print_char(params, p, cube_vertices, triangles, normal_vectors, max_depth);
        }
        printf("\n");
    }
}


// determines which pixel to render
// helper funciton for render_frame
void print_char(struct cube_arguments params, struct coord_3d point,
                 struct coord_3d cube_vertices[VERTEX_COUNT],
                 struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES],
                 struct coord_3d normal_vectors[6], double max_depth)
{
    // if pixel isnt' on cube we return blank anyways
    char pixel = ' ';
    char facemap[CUBE_FACES] = {'+', '-', '#', '|', '/', '?'};

    bool in_triangle = false;
    double depth_buffer = 0;
    struct coord_3d highest_triangle[TRIANGLE_VERTICES];


    for (int tri_index = 0; tri_index < TRIANGLE_COUNT; tri_index++)
    {
        if (return_vertex(point, cube_vertices)) { return; }
        // keeps in_triangle true even if subsequent triangles aren't in triangle
        if (!in_triangle) { in_triangle = point_in_triangle(point, triangles[tri_index]); }

        // we want to render only what we can see - 
        double pixel_depth = get_pixel_depth(point, triangles[tri_index]);
        if (point_in_triangle(point, triangles[tri_index]) && pixel_depth > depth_buffer)
        {
            depth_buffer = pixel_depth;
            
            highest_triangle[0] = triangles[tri_index][0];
            highest_triangle[1] = triangles[tri_index][1];
            highest_triangle[2] = triangles[tri_index][2];
        }
    }

    if (in_triangle)
    {
        pixel = get_pixel(highest_triangle, facemap, normal_vectors);
    }

    printf("\033[38;5;%dm%c\033[0m", get_pixel_shading(max_depth, depth_buffer, params), pixel);
}

int get_pixel_shading(double max_depth, double depth,
                      struct cube_arguments params)
{
    int black = 232;
    int white = 255; // subtracting from 256 will result in darker tones

    struct coord_3d terminal = params.terminal_size;
    double side_length = params.size;

    double adjusted_depth = (depth);
    double increment = max_depth / 25;
    double current = increment;

    for (int i = 0; i < 24; i++)
    {
        if (current > adjusted_depth)
        {
            return black + i;
        }
        current += increment;
    }

    return white;
}

char get_pixel(struct coord_3d highest_triangle[TRIANGLE_VERTICES],
               char facemap[CUBE_FACES],
               struct coord_3d normal_vectors[6])
{
    for (int i = 0; i < 6; i++)
    {
        struct coord_3d normal = cross_product(highest_triangle);

        if (equal_vectors(normal_vectors[i], normal))
        {
            return facemap[i];
        }
    }
    return 'i';
}


// returns the vertex if point lies on vertex
bool return_vertex(struct coord_3d point,
                   struct coord_3d cube_vertices[VERTEX_COUNT])
{
    for (int i = 0; i < VERTEX_COUNT; i++)
    {
        // probably better to do abs(point - cube_vertex) < 0.1
        // prints vertices of cube
        if (point.x == (int)cube_vertices[i].x && point.y == (int)cube_vertices[i].y)
        {
            printf("\033[0;31m%d\033[0m", i);
            return true;
        }
    }
    return false;
}


// uses barycentric coordinates to determine if a point is within a triangle or not
// this function only takes in one triangle, so a point *may* lie within the cube,
// but not specific triangles.
bool point_in_triangle(struct coord_3d p,
                       struct coord_3d triangle[TRIANGLE_VERTICES])
{
    /* point in triangle (barycentric coordinates)
        * given a triangle ABC, assuming that vectors AB and AC aren't parallel,
        * we can express any point (P) in 3d space as: [P = OA + s * AB + t * AC]
        * where s and t are scalars to scale AB and AC. Now, if 0 <= s <= 1 (same for t),
        * the point is within the parallelogram created when you flip A across points B and C.
        * thus, to check if the point is within said triangle, [s + t <= 1].
    */
    struct coord_3d a = triangle[0];
    struct coord_3d b = triangle[1];
    struct coord_3d c = triangle[2];

    /*
    double s = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / 
        ((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
    double t = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) /
        ((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
        */
    double s = ((c.x - a.x) * (p.y - a.y) - (c.y - a.y) * (p.x - a.x)) / 
        ((b.y - a.y) * (c.x - a.x) - (b.x - a.x) * (c.y - a.y));
    double t = (p.x - a.x - s * (b.x - a.x)) / (c.x - a.x);
    if ((s + t) <= 1 && s > 0 && t > 0)
    {
        // printf("%lf + %lf = %lf     ", s, t, s+t);
        return true;
    }
    return false;
}
