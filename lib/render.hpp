#pragma once

void render_frame(
        struct coordinate_3d terminal_dimensions,
        struct coordinate_3d triangle_vertices[12][3]
        );

char get_pixel(
        struct coordinate_3d triangle_vertices[12][3],
        int x,
        int y
        );

double get_depth_buffer(
        struct coordinate_3d p[3],
        int x,
        int y
        );

bool in_triangle(
        struct coordinate_3d p[3],
        int x,
        int y
        );
