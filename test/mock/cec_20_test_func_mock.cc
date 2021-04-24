#include <math.h>

#define BOUNDARY_LOW -5.12
#define BOUNDARY_UP 5.12

double firstDejong(double *x, int d)
{
    double sum = 0;
    for(int i = 0; i < d; i++)
    {
        sum += pow(x[i],2);
    }
    return sum;
}

void cec20_test_func(double* x, double* f, int nx, int mx = 1, int func_num0 = 1)
{
    double res = firstDejong(x,nx);
    *f = res;
}