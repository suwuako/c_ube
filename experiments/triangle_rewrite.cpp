#include <iostream>
#include <math.h>

bool in_triangle(double* p_vertices, int x, int y);

int main()
{
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

    return 0;
}


bool in_triangle(double* p_vertices, int x, int y)
{

}
