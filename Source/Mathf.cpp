#include "Mathf.h"
#include <stdlib.h>

float Mathf::Lerp(float a, float b, float t)
{
    return a * (1.0f - t) + (b * t);
}

float Mathf::RandomRange(float min, float max)
{
    //return rand() % (int)max + (int)min;

    float value = static_cast<float>(rand()) / RAND_MAX;

    return min + (max - min) * value;
}
