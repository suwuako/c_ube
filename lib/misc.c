#include <stdio.h>

#include "datatypes.h"

void print_cube_params(struct cube_arguments params)
{
    printf("\n=== c_ube arguments ===\n \n");
    printf("size: %lf\n", params.size);
    printf("terminal x: %lf\n", params.terminal_size.x);
    printf("terminal y: %lf\n", params.terminal_size.y);
    printf("terminal z: %lf\n", params.terminal_size.z);
    printf("speed: %d\n", params.refresh_rate);
    printf("x angle: %lf\n", params.x_angle);
    printf("y angle: %lf\n", params.y_angle);
    printf("z angle: %lf\n", params.z_angle);
}

void print_cube_vertices(struct coord_3d cube_vertices[VERTEX_COUNT])
{
    printf("\n \n \n=== c_ube vertices ===\n \n");
    for (int i = 0; i < VERTEX_COUNT; i++)
    {
        printf("(%lf, %lf, %lf)\n", cube_vertices[i].x, cube_vertices[i].y, cube_vertices[i].z);
    }
}

void print_triangles(struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES])
{
    printf("\n \n \n=== c_ube triangles ===\n \n");
    for (int i = 0; i < TRIANGLE_COUNT; i++)
    {
        printf("\nTriangle %d: ", i);
        for (int j = 0; j < TRIANGLE_VERTICES; j++)
        {
            printf("(%lf, %lf, %lf)", triangles[i][j].x, triangles[i][j].y, triangles[i][j].z);
            if (j != 2)
            {
                printf(", ");
            }
        }
    }

    printf("\n \n");
}
