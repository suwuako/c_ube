#include <iostream>
#include <windows.h>
#include <math.h>

const int triangle_offset = 10;
const int triangle_count = 2;

void get_screen_size(int* p_screen)
{
	// create pointer variables to represent the two points of the array in screen_size[2]
	int* screen_row = p_screen;
	int* screen_col = (p_screen + 1);

	// get console width and height
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;
  
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  
	*screen_row = rows;
	*screen_col = columns;
}

void assign_coords(int tri, double* triangles, double* A, double* B, double* C)
{
	for (int vertex = 0; vertex < 3; vertex++)
	{
		for (int coord = 0; coord < 3; coord++)
		{
			switch (vertex)
			{
				case 0:
					*(A + coord) = *(triangles + 9 * tri + vertex * 3 + coord);
					break;
				case 1:
					*(B + coord) = *(triangles + 9 * tri + vertex * 3 + coord);
					break;
				case 2:
					*(C + coord) = *(triangles + 9 * tri + vertex * 3 + coord);
					break;
				default:
					std::cout << "something broke";
			}
		}
	}
}

void assign_triangle_vectors(double* A, double* B, double* C, double* vec_AB, double* vec_AC, double* vec_BC)
{
	for(int i = 0; i < 3; i++)
	{
		*(vec_AB + i) = *(B + i) - *(A + i);
		*(vec_AC + i) = *(C + i) - *(A + i);
		*(vec_BC + i) = *(C + i) - *(B + i);
	}
}


void assign_p_vectors(double* A, double* B, double* C, double* P, double* vec_PA, double* vec_PB, double* vec_PC)
{
	for (int i = 0; i < 3; i++)
	{
		*(vec_PA + i) = *(A + i) - *(P + i);
		*(vec_PB + i) = *(B + i) - *(P + i);
		*(vec_PC + i) = *(C + i) - *(P + i);
	}
}

double dot_product_2d(double* vec_A, double* vec_B)
{
    double result;
    for (int i = 0; i < 2; i++)
    {
        result += *(vec_A + i) * *(vec_B + i);
    }
    return result;
}

double vector_len_2d(double* vec_AB)
{
    double len;
    len = sqrt(pow(*vec_AB, 2) + pow(*(vec_AB + 1), 2));

    return len;
}

void orthogonal_projection(double* base_vector, double* secondary_vector, double* output_vector)
{
    // refer to orthogonal_projection.png
    // orth_vec = secondary - (proj (base) secondary)
    //          = secondary - (base dot secondary)/ |secondary|^2 * secondary
    // need dot product, vector multiplication

    // will be dealt in 2d
    double projection_len;
    double orthogonal_vec[2];

    projection_len = dot_product_2d(base_vector, secondary_vector);
    projection_len /= abs( pow(*(base_vector), 2) + pow(*(base_vector + 1), 2) );

    for (int i = 0; i < 2; i++)
    {
        orthogonal_vec[i] = *(secondary_vector + i) - projection_len * *(base_vector + i);
        *(output_vector + i) = orthogonal_vec[i];
    }
}

double area_2d(double* vec_AB, double* vec_AC)
{
    double vec_orth[2];
    orthogonal_projection(vec_AB, vec_AC, vec_orth);

	double area;
    double base_len = vector_len_2d(vec_AB);
    double height_len = vector_len_2d(vec_orth);
   
    area = base_len * height_len / 2;

	return abs(area);
	
}

bool p_in_triangle(double* vec_PA, double* vec_PB, double* vec_PC, double* vec_AB, double* vec_AC, double* vec_BC)
{
    double total_area = area_2d(vec_AB, vec_AC);
    
    double vec_AP[2];
    vec_AP[0] = -vec_PA[0];
    vec_AP[1] = -vec_PA[1];

    double vec_BP[2];
    vec_BP[0] = -vec_PB[0];
    vec_BP[1] = -vec_PB[1];

    double area_PCA = area_2d(vec_AC, vec_AP);
    double area_PAB = area_2d(vec_AB, vec_AP);
    double area_PBC = area_2d(vec_BC, vec_BP);

    double check = area_PCA + area_PAB + area_PBC;
        std::cout << check << ' ' << total_area << '\n';
    if (abs(check - total_area) <= 500)
    {
        return true;
    }

    return false;
}

char return_pixel(int x, int y, double* triangles, int triangle_count, int terminal_x, int terminal_y, int terminal_z)
{
	double A[3];
	double B[3];
	double C[3];
	double P[3] = {(double) x,(double) y, 0};
	
	double vec_AB[3];
	double vec_AC[3];
	double vec_BC[3];

	double vec_PA[3];
	double vec_PB[3];
	double vec_PC[3];

	for (int tri = 0; tri < triangle_count; tri++)
	{
		assign_coords(tri, triangles, A, B, C);		
		assign_triangle_vectors(A, B, C, vec_AB, vec_AC, vec_BC);
		assign_p_vectors(A, B, C, P, vec_PA, vec_PB, vec_PC);	
        
        if (p_in_triangle(vec_PA, vec_PB, vec_PC, vec_AB, vec_AC, vec_BC))
        {
            return '-';
        }
        /*
        std::cout << "(" << vec_PA[0] << ", " << vec_PA[1] << ")\n";
        std::cout << "(" << vec_PB[0] << ", " << vec_PB[1] << ")\n";
        std::cout << "(" << vec_PC[0] << ", " << vec_PC[1] << ")\n";
        p_in_triangle(vec_PA, vec_PB, vec_PC, vec_AB, vec_AC, vec_BC);
        std::cout << "(" << vec_AB[0] << ", " << vec_AB[1] << ")\n";
        std::cout << "(" << vec_AC[0] << ", " << vec_AC[1] << ")\n";
        std::cout << area_2d(vec_AB, vec_AC) << '\n';

        std::cout << '\n';
        */
    }

	return ' ';
}

void render(double* triangles, int triangle_count, int terminal_x, int terminal_y, int terminal_z)
{
	/*
		rendering: we need to be able to determine if an arbitrary point is within the three points that make up a triangle

	 	see docs/checking-for-p-in-triangle.png  
	 	
		given a point P and triangle ABC, if the sum of |(PA X PB)/2 + (PB X PC)/2 + (PC X PA)/2| = ABC_Area then P is within ABC, otherwise it is outside of the triangle.
		it also passes edge cases where it lies on vector PA or lies on point A. 
	 */
	
		
	for (int x = 0; x < terminal_x; x++)
	{
		for (int y = 0; y < terminal_y; y++)
		{
            std::cout << return_pixel(x, y, triangles, triangle_count, terminal_x, terminal_y, terminal_z);
		}
		std::cout << '\n';
	}

}

int main(void)
{
	int screen_size[2];
	int terminal_x;
	int terminal_y;
	int terminal_z;

	get_screen_size(screen_size);

	// terminal_z is set so that it isn't the smallest parameter
	terminal_x = screen_size[0];
	terminal_y = screen_size[1];
	terminal_z = terminal_x + terminal_y;

	double centre_x = terminal_x / 2;
	double centre_y = terminal_y / 2;
	double centre_z = terminal_z / 2;

	// debug message
	std::cout << "centres: (x: " << centre_x << "), (y: " << centre_y << "), (z: " << centre_z << ")\n";

	// arbitrarily create two triangles
	// array that contains an array of three coordinate points - a triangle
	double triangles[2][3][3] = {
		{
			{centre_x + triangle_offset, centre_y, centre_z}, 
			{centre_x, centre_y + triangle_offset, centre_z}, 
			{centre_x, centre_y, centre_z + triangle_offset}
		},
		{
			{centre_x, centre_y, centre_z}, 
			{centre_x + triangle_offset, centre_y, centre_z + triangle_offset},
			{centre_x + triangle_offset, centre_y + (triangle_offset / 2), centre_z + triangle_offset}
		}

	};
	double* p_triangles = &triangles[0][0][0];


	render(p_triangles, triangle_count, terminal_x, terminal_y, terminal_z);
	return 0;
}
