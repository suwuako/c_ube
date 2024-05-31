#include <iostream>

void multiply_matrices(int* matrix, int* coord)
{
	// apply arrays to local scope
	int rotation_matrix[3][3];
	int rotation_coord[3];
	int new_coord[3];


	for (int i = 0; i < 3; i++)
	{
		rotation_coord[i] = *(coord + i);
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{	
			rotation_matrix[i][j] = *(matrix + 3*i + j);
		}
	}

	// rotation matrices work by having the rotation matrix multiply by coordinate 
	for (int i = 0; i < 3; i++)
	{
		int sum{0};
		for (int j = 0; j < 3; j++)
		{
			sum += rotation_matrix[i][j] * rotation_coord[j];
		}
		new_coord[i] = sum;
	}
	
	// rewrite coordinate to p_coord
	for (int i = 0; i < 3; i++)
	{
		*(coord + i) = new_coord[i];
	}
}

int main(void)
{
	int matrix_1[3][3] = {
		{1, 2, 3}, 
		{4, 5, 6},
		{7, 8, 9}
	};

	int coord[3] = {5, 5, 8};

	int* p_matrix = &matrix_1[0][0];
	int* p_coord = &coord[0];

	multiply_matrices(p_matrix, p_coord);

	// print that goober out
	std::cout << '(';
	for (int i{ 0 }; i < 3; i++)
	{
		std::cout << coord[i] << ' ';
	}
	std::cout << ")\n";
	return 0;
}
