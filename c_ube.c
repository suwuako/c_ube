#include <stdio.h>
#include <stdlib.h>

#include "lib/cli_args.h"
#include "lib/datatypes.h"
#include "lib/misc.h"
#include "lib/cube_math.h"

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
    * - [ ] convert cube_vertices to triangles
    *
    * TODO:
    * if invalid CLI args are passed we default to normal args 
    * (i.e. --size rahhh) then we default to normal size
    *
    * General TODOS:
    * - [x] Complete cli parsing
    * - [x] Create cube vertices
    * - [ ] Link cube vertices into triangles
    * - [ ] Render triangles
    * - [ ] Rotate triangles
    * - [ ] have triangles display different characters based off cube face
    * - [ ] support depth shading
*/


int main(int argc, char *argv[])
{
    struct coord_3d cube_vertices[VERTEX_COUNT] = {};
    // all arguments (explicitly defined in terminal or not)
    // are defined in lib/cli_args.c
    struct cube_arguments cube_parameters = {};
    cube_parameters = parse_cli_args(argc, argv);
    print_cube_params(cube_parameters);

    create_cube_vertices(cube_vertices, cube_parameters);
    print_cube_vertices(cube_vertices);
}
