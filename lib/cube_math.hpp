#pragma once

#include "datatypes.hpp"

void create_cube_vertices(
        struct coordinate_3d cube_vertices[8],
        struct coordinate_3d terminal_dimensions,
        int DISTANCE_BETWEEN_CLOSEST_VERTICES
        );

void group_vertices_to_triangles(
        struct coordinate_3d triangle_vertices[12][3],
        struct coordinate_3d cube_vertices[8],
        int DISTANCE_BETWEEN_CLOSEST_VERTICES
        );

void create_origin_vertices(
        struct coordinate_3d cube_vertices[8],
        struct coordinate_3d origin_vertices[4],
        int DISTANCE_BETWEEN_CLOSEST_VERTICES
        );

void create_origin_vertices(
        struct coordinate_3d cube_vertices[8],
        struct coordinate_3d origin_vertices[4],
        int DISTANCE_BETWEEN_CLOSEST_VERTICES
        );

void rotate(
        struct coordinate_3d cube_vertices[8],
        struct coordinate_3d terminal_dimensions,
        double angle
        );

void multiply_matrices(
        struct coordinate_3d point,
        double rotation_matrix[3][3]
        );

struct coordinate_3d vectorise(struct coordinate_3d A, struct coordinate_3d B);

double get_vector_length(struct coordinate_3d AB);
