#include <stdio.h>
#include <stdbool.h>

#include "cube_math.h"
#include "render.h"
#include "datatypes.h"
#include "misc.h"

void render_frame(struct cube_arguments params,
                  struct coord_3d cube_vertices[VERTEX_COUNT],
                  struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES])
{
    struct coord_3d terminal = params.terminal_size;
    struct coord_3d normal_vectors[6] = {};

    get_normal_vectors(normal_vectors, triangles);
    scanf("%c");

    for (int x = 0; x < terminal.x; x++)
    {
        for (int y = 0; y < terminal.y; y++)
        {
            struct coord_3d p;
            p.x = x;
            p.y = y;

            printf("%c", return_char(p, cube_vertices, triangles, normal_vectors));
        }
        printf("\n");
    }
}

// determines which pixel to render
// helper funciton for render_frame
char return_char(struct coord_3d point,
                 struct coord_3d cube_vertices[VERTEX_COUNT],
                 struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES],
                 struct coord_3d normal_vectors[6])
{
    // if pixel isnt' on cube we return blank anyways
    char return_signal = ' ';
    char facemap[CUBE_FACES] = {'+', '-', '#', '@', '*', '.'};
    double depth_buffer = -10000000000000;
    bool in_triangle = false;
    double highest_tri_index = 0;
    struct coord_3d highest_triangle[TRIANGLE_VERTICES];

    for (int tri_index = 0; tri_index < TRIANGLE_COUNT; tri_index++)
    {
        struct coord_3d triangle_vertex;

        if (return_vertex(point, cube_vertices)) { return '\0'; }
        // keeps in_triangle true even if subsequent triangles aren't in triangle
        if (!in_triangle) { in_triangle = point_in_triangle(point, triangles[tri_index]); }

        // we want to render only what we can see - 
        double pixel_depth = get_pixel_depth(point, triangles[tri_index]);
        if (point_in_triangle(point, triangles[tri_index]) && pixel_depth > depth_buffer)
        {
            depth_buffer = pixel_depth;
            
            highest_tri_index = tri_index;
            return_signal = get_pixel(highest_triangle, facemap, normal_vectors);
        }
    }
    if (in_triangle)
    {
        return '0' + highest_tri_index;
    }
    return return_signal;
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
