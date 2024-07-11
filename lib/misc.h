#pragma once


void sync_refresh_rate(struct cube_arguments params);
void print_cube_params(struct cube_arguments params);
void print_cube_vertices(struct coord_3d cube_vertices[VERTEX_COUNT]);
void print_triangles(struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES]);
