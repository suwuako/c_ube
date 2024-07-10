#include <stdio.h>
#include <stdbool.h>

#include "render.h"
#include "datatypes.h"

void render_frame(struct cube_arguments params,
                  struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES])
{
    struct coord_3d terminal = params.terminal_size;

    for (int x = 0; x < terminal.x; x++)
    {
        for (int y = 0; y < terminal.y; y++)
        {
            printf("%c", return_char(x, y, triangles));
        }
        printf("\n");
    }
}

char return_char(int x, int y, 
                 struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES])
{
    for (int tri_index = 0; tri_index < TRIANGLE_COUNT; tri_index++)
    {
        struct coord_3d triangle[TRIANGLE_VERTICES] = {};

        for (int i = 0; i < TRIANGLE_VERTICES; i++)
        {
            triangle[i] = triangles[tri_index][i];
            if (x == (int)triangle[i].x && y == (int)triangle[i].y)
            {
                return '#';
            }
        }

        
    }
    return ' ';
}

bool point_in_triangle(int x, int y, 
                       struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES])
{
    
}
