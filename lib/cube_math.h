#pragma once


#include <stdbool.h>

#include "datatypes.h"


// rotation functions
void rotate_vertices(struct coord_3d cube_vertices[VERTEX_COUNT],
                     struct cube_arguments cube_parameters);


struct rotation_matrix init_rotation_matrices(double x, double y, double z);

void multiply_rotation_matrices(struct coord_3d cube_vertices[VERTEX_COUNT],
                                struct rotation_matrix matrix);

void transform_vertices(struct coord_3d cube_vertices[VERTEX_COUNT],
                        struct coord_3d terminal, bool towards_origin);


// general vector math functions
struct coord_3d points_to_vector(struct coord_3d a, struct coord_3d b);
double get_vector_length(struct coord_3d vec);


// grouping vertices to 12 triangles
void group_vertices_to_triangles(struct coord_3d cube_vertices[VERTEX_COUNT],
                                 struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES],
                                 struct cube_arguments cube_parameters);

void get_main_vertices(struct coord_3d cube_vertices[VERTEX_COUNT],
                       struct coord_3d main_vertices[MAIN_VERTEX_COUNT],
                       int *main_vertex_index, double pythagorean_length);

// converts main vertices to triangles
void get_triangles_from_main_vertices(struct coord_3d main_vertices[MAIN_VERTEX_COUNT],
                                      struct coord_3d cube_vertices[VERTEX_COUNT],
                                      struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES],
                                      double cube_side_length);


// creating cube vertices
void create_cube_vertices(struct coord_3d cube_vertices[VERTEX_COUNT],
                          struct cube_arguments cube_parameters);

void set_singular_vertex(struct coord_3d cube_vertices[VERTEX_COUNT],
                         struct coord_3d terminal_center,
                         int cube_combination[AXIS_COUNT], double offset,
                         int vertex, int axis);

void update_cube_combination(int cube_combination[AXIS_COUNT]);
