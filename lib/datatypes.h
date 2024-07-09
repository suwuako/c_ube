#pragma once

#define VERTEX_COUNT 8



struct coord_3d
{
    double x;
    double y;
    double z;
};

struct cube_arguments
{
    int size;
    struct coord_3d terminal_size;
    int refresh_rate;
    double x_angle;
    double y_angle;
    double z_angle;
};
