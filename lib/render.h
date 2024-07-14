#pragma once


#include <stdbool.h>

#include "datatypes.h"


void render_frame(struct cube_arguments params,
                  struct coord_3d cube_vertices[VERTEX_COUNT],
                  struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES]);

void print_char(struct cube_arguments params, struct coord_3d point,
                 struct coord_3d cube_vertices[VERTEX_COUNT],
                 struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES],
                 struct coord_3d normal_vectors[6], double max_depth);

char get_pixel(struct coord_3d highest_triangle[TRIANGLE_VERTICES],
               char facemap[CUBE_FACES],
               struct coord_3d normal_vectors[6]);

double get_max_depth(struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES],
                     struct coord_3d terminal);

int get_pixel_shading(double max_depth, double depth,
                      struct cube_arguments params);

bool return_vertex(struct coord_3d point,
                   struct coord_3d cube_vertices[VERTEX_COUNT]);

bool point_in_triangle(struct coord_3d p,
                       struct coord_3d triangle[TRIANGLE_VERTICES]);

