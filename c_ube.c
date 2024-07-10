#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "lib/cli_args.h"
#include "lib/datatypes.h"
#include "lib/misc.h"
#include "lib/cube_math.h"
#include "lib/render.h"

/*
    * CLI ARGUMENTS
    *   --size          / -s dictates the size (length side) of the cube 
    *   --terminal_x      / -X dictates the size of the terminal (x)
    *   --terminal_y      / -Y dictates the size of the terminal (x)
    *   --speed         / -S terminal refresh rate (frames/sec)
    *   -x              / -x x rotation angle amount
    *   -y              / -y y rotation angle amount
    *   -z              / -z z rotation angle amount
 */

/*
    * Currently working on:
    * - [x] create file structure
    * - [x] readin user input and put into function
    * - [x] start parsing and assigning arguments
    * - [x] create auto assignment for uninputted args
    * - [x] update auto assignment to include terminal sizes for both win/linux
    * - [x] create cube vertices
    * - [x] convert cube_vertices to triangles
    * - [x] Render triangles
    * - [ ] Rotate triangles
    *
    * TODO:
    * if invalid CLI args are passed we default to normal args 
    * (i.e. --size rahhh) then we default to normal size
    *
    * General TODOS:
    * - [x] Complete cli parsing
    * - [x] Create cube vertices
    * - [x] Link cube vertices into triangles
    * - [x] Render triangles
    * - [ ] Rotate triangles
    * - [ ] have triangles display different characters based off cube face
    * - [ ] support depth shading
*/


int main(int argc, char *argv[])
{
    // all arguments (explicitly defined in terminal or not)
    // are defined in lib/cli_args.c
    struct cube_arguments cube_parameters;
    cube_parameters = parse_cli_args(argc, argv);
    print_cube_params(cube_parameters);

    struct coord_3d cube_vertices[VERTEX_COUNT];
    create_cube_vertices(cube_vertices, cube_parameters);
    print_cube_vertices(cube_vertices);

    struct coord_3d triangles[TRIANGLE_COUNT][TRIANGLE_VERTICES];
    group_vertices_to_triangles(cube_vertices, triangles, cube_parameters);
    print_triangles(triangles);

    printf("Press enter to continue;\n");
    scanf("%c");
    while (true)
    {
        render_frame(cube_parameters, triangles);
        rotate_vertices(cube_vertices, cube_parameters);
        group_vertices_to_triangles(cube_vertices, triangles, cube_parameters);
    }
    return 0;
}
