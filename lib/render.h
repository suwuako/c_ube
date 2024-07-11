#pragma once


#include <stdbool.h>

#include "datatypes.h"


void render_frame(struct cube_arguments params,
                  struct coord_3d cube_vertices[VERTEX_COUNT],
                  struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES]);

char return_char(struct coord_3d point,
                 struct coord_3d cube_vertices[VERTEX_COUNT],
                 struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES]);

bool point_in_triangle(struct coord_3d p,
                       struct coord_3d triangle[TRIANGLE_VERTICES]);
