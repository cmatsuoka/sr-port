#include <math.h>
#include "tun10.h"

void singen()
{
    int x;

    for (x = 0; x < 4096; x++) sinit[x] = sin(M_PI*(x)/128)*((1.0*x*3) / 128);
    for (x = 0; x < 2048; x++) cosit[x] = cos(M_PI*(x)/128)*((1.0*x*4) / 64);
}
