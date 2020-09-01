#include "util.h"
#include <algorithm>

void util::hsvtorgb(float h, float s, float v, int *r, int *g, int *b)
{
    h = fmod(h, 360);
    s = s / 100.0;
    v = v / 100.0;
    float c = v * s;
    float x = c * (1 - abs(fmod(h/60.0, 2)-1));
    float m = v - c;

    if(h < 60) {
        *r = (c+m)*100;
        *g = (x+m)*100;
        *b = m*100;
    } else if(h < 120) {
        *r = (x+m)*100;
        *g = (c+m)*100;
        *b = m*100;
    } else if(h < 180) {
        *r = m*100;
        *g = (c+m)*100;
        *b = (x+m)*100;
    } else if(h < 240) {
        *r = m*100;
        *g = (x+m)*100;
        *b = (c+m)*100;
    } else if(h < 300) {
        *r = (x+m)*100;
        *g = m*100;
        *b = (c+m)*100;
    } else if(h < 360) {
        *r = (c+m)*100;
        *g = m*100;
        *b = (x+m)*100;
    }
}

void util::rgbtohsv(float r, float g, float b, int *h, int *s, int *v)
{
    r = r / 100;
    g = g / 100;
    b = b / 100;

    float cmax = std::max({r, g, b});
    float cmin = std::min({r, g, b});
    float delta = cmax - cmin;

    // calculate the hue
    if(delta == 0) {
        *h = 0;
    } else if(cmax == r) {
        *h = 60 * fmod((g-b)/delta, 6);
    } else if(cmax == g) {
        *h = 60 * ((b-r)/delta + 2);
    } else if(cmax == b) {
        *h = 60 * ((r-g)/delta + 4);
    }

    // calculate the saturation
    if(cmax == 0)
        *s = 0;
    else
        *s = delta / cmax * 100;

    // calculate the value
    *v = cmax * 100;
}

std::vector<float> util::logspace(const float &start, const float &stop, const uint &npoints) {
    std::vector<float> output;

    float delta = (stop - start) / npoints;

    float v = start;

    while (v < stop) {
        output.emplace_back(powf(10.0f, v));

        v += delta;
    }

    return output;
}
