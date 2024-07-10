#pragma once

#define VERTEX_COUNT 8
#define AXIS_COUNT 3
#define TRIANGLE_COUNT 12
#define TRIANGLE_VERTICES 3
#define MAIN_VERTEX_COUNT 4

struct coord_3d
{
    double x;
    double y;
    double z;
};

struct rotation_matrix
{
    double x_rotate[AXIS_COUNT][AXIS_COUNT];
    double y_rotate[AXIS_COUNT][AXIS_COUNT];
    double z_rotate[AXIS_COUNT][AXIS_COUNT];
};

struct cube_arguments
{
    double size;
    struct coord_3d terminal_size;
    int refresh_rate;
    double x_angle;
    double y_angle;
    double z_angle;
};
