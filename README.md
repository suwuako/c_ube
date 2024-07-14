# c_ube

a spinning cube implementation written in C

## demo
![](demo.mp4)

## command line flags

    * CLI ARGUMENTS
    *   --colour        / -c colour (0-256)                                 default: 232 (black)
    *   --size          / -S dictates the size (length side) of the cube    default: shortest terminal dimension / 2
    *   --terminal_x    / -X dictates the size of the terminal (x)          default: autodetected
    *   --terminal_y    / -Y dictates the size of the terminal (y)          default: autodetected
    *   --terminal_z    / -Z dictates the size of the terminal (z)          default: autodetected
    *   --speed         / -s terminal refresh rate (frames/sec)             default: 60
    *   -x              / -x x rotation angle amount                        default: 0.1
    *   -y              / -y y rotation angle amount                        default: 0.2
    *   -z              / -z z rotation angle amount                        default: 0.3

## todos
Currently working on:
 - [x] create file structure
 - [x] readin user input and put into function
 - [x] start parsing and assigning arguments
 - [x] create auto assignment for uninputted args
 - [x] update auto assignment to include terminal sizes for both win/linux
 - [x] create cube vertices
 - [x] convert cube_vertices to triangles
 - [x] Render triangles
 - [x] Rotate triangles
 - [x] frame rate sleep things
 - [x] shorthand flags
 - [x] have triangles display different characters based off cube face
 - [x] support depth shading (not that feasible anymore...) nvm i did it lol

## installation / compilation

run `make` and you can run, rename or move `bin-c_ube.out` into `/usr/bin/`
