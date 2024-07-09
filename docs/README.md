# c_ube layout

## lib/

- datatypes.h
    - holds all structs, #defines and constant values
    - structs:
        - coord_3d - general 3d struct that holds (x, y, z)
        - cube_arguments - holds args required to create a cube
    - consts:
        - VERTEX_COUNT - verticies of a cube
        - AXIS_COUNT - axes of a 3d space (x, y, z)

- cube_math.c
    - handles general cube math
    - create_cube_vertices creates the 8 vertices of a cube

- cli_args.c
    - parses, handles and sets cube arguments
    - user input from cli is read through parse_cli_args
    - any unspecified argument is set in auto_assign_args

- misc.c
    - handles any random misc operation
    - print_cube_params 
    - print_cube_vertices 
