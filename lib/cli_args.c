#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datatypes.h"
#include "cli_args.h"


/*
    * temp comment to display structure of cube_arguments

    struct cube_arguments
    {
        int size;
        int terminal_size;
        int refresh_rate;
        double x_angle;
        double y_angle;
        double z_angle;
    };

*/


// returns the struct cube_arguments
struct cube_arguments parse_cli_args(int argc, char *argv[])
{
    // inputted_args keeps track of used args,
    // if one of its args is 0, it means there was no inputted arg
    struct cube_arguments cli_args = {};
    struct cube_arguments inputted_args = {};

    assign_cli_args(argc, argv, &cli_args, &inputted_args);
    
    // auto assigns variables that haven't been supplied in cli
    auto_assign_args(inputted_args, &cli_args);
    return cli_args;
}

// auto supplies values if they haven't been passed in cli
void auto_assign_args(struct cube_arguments inputted_args,
                      struct cube_arguments *p_cli_args)
{
    const int CUBE_SIZE = 20;
    const int REFRESH_RATE = 60;
    const double X = 1.0;
    const double Y = 1.0;
    const double Z = 1.0;

    if (inputted_args.size == 0) 
    {
        p_cli_args->size = CUBE_SIZE;
    }

    if (inputted_args.terminal_size == 0)
    {
        // TODO: GET TERMINAL SIZE
    }

    if (inputted_args.refresh_rate == 0)
    {
        p_cli_args->refresh_rate = REFRESH_RATE;
    }

    if (inputted_args.x_angle == 0)
    {
        p_cli_args->x_angle = X;
    }

    if (inputted_args.y_angle == 0)
    {
        p_cli_args->y_angle = Y;
    }

    if (inputted_args.z_angle == 0)
    {
        p_cli_args->z_angle = Z;
    }
}

void assign_cli_args(int argc, char *argv[],
                     struct cube_arguments *p_cli_args,
                     struct cube_arguments *p_inputted_args)
{
    // ignores arguments that aren't specified in c_ube.c
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--size") == 0) {
            p_cli_args->size = atoi(argv[i + 1]);
            p_inputted_args->size = 1;

        } else if (strcmp(argv[i], "--terminal") == 0) {
            p_cli_args->terminal_size = atoi(argv[i + 1]);
            p_inputted_args->terminal_size = 1;

        } else if (strcmp(argv[i], "--speed") == 0) {
            p_cli_args->refresh_rate = atoi(argv[i + 1]);
            p_inputted_args->refresh_rate = 1;

        } else if (strcmp(argv[i], "-x") == 0) {
            p_cli_args->x_angle = atof(argv[i + 1]);
            p_inputted_args->x_angle = 1;

        } else if (strcmp(argv[i], "-y") == 0) {
            p_cli_args->y_angle = atof(argv[i + 1]);
            p_inputted_args->y_angle = 1;

        } else if (strcmp(argv[i], "-z") == 0) {
            p_cli_args->z_angle = atof(argv[i + 1]);
            p_inputted_args->z_angle = 1;

        }
    }
}
