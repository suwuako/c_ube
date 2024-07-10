c_ube: c_ube.c
	gcc c_ube.c lib/cli_args.c lib/misc.c lib/cube_math.c -lm -o bin-c_ube.out
	./bin-c_ube.out
