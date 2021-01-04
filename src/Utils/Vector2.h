#ifndef __VECTOR_2_H__
#define __VECTOR_2_H__

#include <math.h>
#include <stdio.h>


class Vector2 {
public:
    float x, y;

    Vector2() {
        x = y = 0;
    }

    Vector2(float _x, float _y) {
        x = _x;
        y = _y;
    }

    void set(float _x, float _y) {
        x = _x;
        y = _y;
    }

    void normalize() {
        float norm = (float) sqrt(x * x + y * y);

        if (norm == 0.0) {
            printf("\n\nNormalize::Divisao por zero");
            x = 1;
            y = 1;
            return;
        }
        x /= norm;
        y /= norm;
    }

    Vector2 operator-(const Vector2 &v) {
        Vector2 aux(x - v.x, y - v.y);
        return (aux);
    }

    Vector2 operator+(const Vector2 &v) {
        Vector2 aux(x + v.x, y + v.y);
        return (aux);
    }

    Vector2 operator*(const Vector2 &v) {
        Vector2 aux(x * v.x, y * v.y);
        return (aux);
    }

    Vector2 operator*(const float i) {
        Vector2 aux(x * i, y * i);
        return (aux);
    }

    bool operator==(const Vector2 &v) {
        return (x == v.x && y == v.y);
    }

    float angulo(Vector2 v2) {
        return acos(x * v2.x + y * v2.y);
    }

    Vector2 Bezier(float t, Vector2 p1, Vector2 p2, Vector2 p3) {
        float x_t = (p2.x - p1.x) * (2 * t * (1 - t)) + (p3.x - p1.x) * pow(t, 3);
        float y_t = (p2.y - p1.y) * (2 * t * (1 - t)) + (p3.y - p1.y) * pow(t, 3);
        Vector2 ret(x_t, y_t);
        return ret;
    }
};

#endif
