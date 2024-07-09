#pragma once

#include "datatypes.h"

void create_cube_vertices(struct coord_3d cube_vertices[VERTEX_COUNT],
                          struct cube_arguments cube_parameters);

void set_singular_vertex(struct coord_3d cube_vertices[VERTEX_COUNT],
                         struct coord_3d terminal_center,
                         int cube_combination[AXIS_COUNT], double offset,
                         int vertex, int axis);

void update_cube_combination(int cube_combination[AXIS_COUNT]);
