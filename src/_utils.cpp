#include "_utils.h"
#include <cmath>

float degToRad(float a) { 
    return a * M_PI / 180.0; 
}

float fixAng(float a) {
    if (a > 359)
        a -= 360;
    if (a < 0)
        a += 360;
    return a;
}

float distance(float ax, float ay, float bx, float by, float ang) {
  return cos(degToRad(ang)) * (bx - ax) - sin(degToRad(ang)) * (by - ay);
}
