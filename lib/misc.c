#include <stdio.h>

#include "datatypes.h"

void print_cube_params(struct cube_arguments params)
{
    printf("size: %d\n", params.size);
    printf("terminal: %d\n", params.terminal_size);
    printf("speed: %d\n", params.refresh_rate);
    printf("x angle: %lf\n", params.x_angle);
    printf("y angle: %lf\n", params.y_angle);
    printf("z angle: %lf\n", params.z_angle);
}
