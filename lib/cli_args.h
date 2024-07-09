#pragma once

struct cube_arguments parse_cli_args(int argc, char *argv[]);

void assign_cli_args(int argc, char *argv[],
                     struct cube_arguments *p_cli_args,
                     struct cube_arguments *p_inputted_args);

void auto_assign_args(struct cube_arguments inputted_args,
                      struct cube_arguments *p_cli_args);
