#include <math.h>
#include "tun10.h"

extern struct bc pcalc[138][64];

void ballgen()
{
    int z, a;

    for (z = 10; z < 148; z++) {
        for (a = 0; a < 64; a++) {
            pcalc[z - 10][a].x = 160 + (int)(sin(a * M_PI / 32) * (1.7 * z));
            pcalc[z - 10][a].y = 100 + (int)(cos(a * M_PI / 32) * z);
        }
    }
}
