#include <iostream>
#include <math.h>
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
int main()
{
    double vec1[2] = {
        0, 5
    };

    double vec2[2] = {
        10, 5
    };

    double orth[2];
    
    double area;

    orthogonal_projection(vec1, vec2, orth);
    std::cout << orth[0] << ' ' << orth[1] << '\n';

    area = area_2d(vec1, vec2);
    
    std::cout << area << '\n';
    return 0;
}
