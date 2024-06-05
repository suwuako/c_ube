#include <iostream>
#include <math.h>


const int triangle_offset = 5;
const int triangle_count = 2;



int main()
{
  double centre_x = 35;
  double centre_y = 140; 
  double centre_z = 175;

  double triangle_vertices[2][3][3] = 
		{
			{centre_x + triangle_offset, centre_y, centre_z}, 
			{centre_x, centre_y + triangle_offset, centre_z}, 
			{centre_x, centre_y, centre_z + triangle_offset}
		};

  double point[2] = {1, 2};
  
  std::cout << true << '\n';

}

bool point_in_triangle(double* vec_AB, double* vec_AC, int x, int y)
{
  double alpha;
  double beta;

  alpha = ( ( x * *(vec_AC+1) ) - ( y * *(vec_AC) ) ) / ( ( *(vec_AB) * *(vec_AC + 1) ) - ( *(vec_AB + 1) * *(vec_AC) ) );
  beta = (y - alpha * *(vec_AB + 1)) / *(vec_AC + 1);

  if (alpha >= 0 && beta >= 0 && (alpha + beta) <= 1)
  {
    return true;
  }
  return false;
}
