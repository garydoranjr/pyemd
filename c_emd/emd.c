/**************************************/
/* EMD - compute earth mover distance */
/**************************************/

#include <emd.h>
#include <math.h>

double emd(double a, double b) {
    return fabs(a - b);
}
