#ifndef COLORFUNCTIONS_H
#define COLORFUNCTIONS_H

#include <math.h>
#include <vector>

namespace util
{
    void hsvtorgb(float h, float s, float v, int *r, int *g, int *b);
    void rgbtohsv(float r, float g, float b, int *h, int *s, int *v);
    std::vector<float> logspace(const float &start, const float &stop, const unsigned int &npoints);
}

#endif // COLORFUNCTIONS_H
