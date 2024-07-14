# c_ube

a spinning cube implementation written in C

## command line flags

    *   --size          / -s dictates the size (length side) of the cube 
    *   --terminal_x    / -X dictates the size of the terminal (x)
    *   --terminal_y    / -Y dictates the size of the terminal (x)
    *   --speed         / -S terminal refresh rate (frames/sec)
    *   -x              / -x x rotation angle amount
    *   -y              / -y y rotation angle amount
    *   -z              / -z z rotation angle amount

## todos

    * Currently working on:
    * - [x] create file structure
    * - [x] readin user input and put into function
    * - [x] start parsing and assigning arguments
    * - [x] create auto assignment for uninputted args
    * - [x] update auto assignment to include terminal sizes for both win/linux
    * - [x] create cube vertices
    * - [x] convert cube_vertices to triangles
    * - [x] Render triangles
    * - [x] Rotate triangles
    * - [x] frame rate sleep things
    * - [x] shorthand flags
    * - [x] have triangles display different characters based off cube face
    * - [x] support depth shading (not that feasible anymore...)

## installation / compilation

run `make` and you can run, rename or move `bin-c_ube.out` into `/usr/bin/`
