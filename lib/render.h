#pragma once


#include <stdbool.h>

#include "datatypes.h"


void render_frame(struct cube_arguments params,
                  struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES]);

char return_char(int x, int y, 
                 struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES]);

bool point_in_triangle(int x, int y, 
                       struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES]);
