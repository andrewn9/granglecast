#pragma once

#include <math.h>

class Vector2 
{
public:
    float x;
    float y;

    Vector2(float _x, float _y) : x(_x), y(_y) {}

    Vector2 normal()
    {
        float magnitude = sqrt(x * x + y * y);
        Vector2* t = new Vector2(x / magnitude, y / magnitude);
        return *t;
    }

    bool operator == (Vector2 other)
    {
        return (x == other.x && y == other.y);
    }

    Vector2 operator * (float n)  
    {
        return {x * n, y * n};
    }

    float magnitude()
    {
        return sqrt(x * x + y * y);
    }
};