#include <stdio.h>
#include <stdbool.h>

#include "render.h"
#include "datatypes.h"
#include "misc.h"

void render_frame(struct cube_arguments params,
                  struct coord_3d cube_vertices[VERTEX_COUNT],
                  struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES])
{
    struct coord_3d terminal = params.terminal_size;

    for (int x = 0; x < terminal.x; x++)
    {
        for (int y = 0; y < terminal.y; y++)
        {
            struct coord_3d p;
            p.x = x;
            p.y = y;

            printf("%c", return_char(p, cube_vertices, triangles));
        }
        printf("\n");
    }
}

// determines which pixel to render
// helper funciton for render_frame
char return_char(struct coord_3d point,
                 struct coord_3d cube_vertices[VERTEX_COUNT],
                 struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES])
{
    for (int tri_index = 0; tri_index < TRIANGLE_COUNT; tri_index++)
    {
        struct coord_3d triangle_vertex;

        for (int i = 0; i < VERTEX_COUNT; i++)
        {
            if (point.x == (int)cube_vertices[i].x && point.y == (int)cube_vertices[i].y)
            {
                printf("\033[0;31m%d\033[0m", i);
                return '\0';
            }
        }

        for (int i = 0; i < TRIANGLE_VERTICES; i++)
        {
            triangle_vertex = triangles[tri_index][i];
            //printf("\n%d %lf %lf %lf\n",i, triangle[i].x, triangle[i].y, triangle[i].z);
            if (point.x == (int)triangle_vertex.x && point.y == (int)triangle_vertex.y)
            {
                return 'x';
            }

        }

        if (point_in_triangle(point, triangles[tri_index]))
        {
            char return_signal = '0' + tri_index;
            return return_signal;
        }
    }
    return ' ';
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

    double s = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / 
        ((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
    double t = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) /
        ((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
    /*
    double s = ((c.x - a.x) * (p.y - a.y) - (c.y - a.y) * (p.x - a.x)) / 
        ((b.y - a.y) * (c.x - a.x) - (b.x - a.x) * (c.y - a.y));
    double t = (p.x - a.x - s * (b.x - a.x)) / (c.x - a.x);
    */
    if ((s + t) <= 1 && s > 0 && t > 0)
    {
        // printf("%lf + %lf = %lf     ", s, t, s+t);
        return true;
    }
    return false;
}