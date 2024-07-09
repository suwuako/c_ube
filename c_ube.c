#include <stdio.h>
#include <stdlib.h>

#include "lib/cli_args.h"
#include "lib/datatypes.h"
#include "lib/misc.h"

/*
    * CLI ARGUMENTS
    *   --size          / -s dictates the size (length side) of the cube 
    *   --terminal      / -t dictates the size of the terminal
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
    * - [ ] create auto assignment for uninputted args
    *
    * TODO:
    * if invalid CLI args are passed we default to normal args 
    * (i.e. --size rahhh) then we default to normal size
    *
    * General TODOS:
    * - [ ] Complete cli parsing
    * - [ ] Create cube vertices
    * - [ ] Link cube vertices into triangles
    * - [ ] Render triangles
    * - [ ] Rotate triangles
*/


int main(int argc, char *argv[])
{
    // all arguments (explicitly defined in terminal or not)
    // are defined in lib/cli_args.c
    struct cube_arguments cube_parameters = {};
    cube_parameters = parse_cli_args(argc, argv);

    print_cube_params(cube_parameters);
}
