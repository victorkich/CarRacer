#ifndef __POINT_H__
#define __POINT_H__

#include "../OpenGL/gl_canvas2d.h"
#include "Vector2.h"


class Point {
private:
    bool first = false;

public:
    Vector2 p;
    bool moving = false;

    Point() {}

    Point(Vector2 p_, int pos) {
        p = p_;
        if (not pos)
            first = true;
    }

    Point(float x, float y, int pos) {
        p.set(x, y);
        if (not pos)
            first = true;
    }

    void set(float x, float y) {
        p.set(x, y);
    }

    void Update(float x, float y) {
        if (moving) {
            p.x += x;
            p.y += y;
        }
    }

    void Render(float x, float y) {
        if (first)
            CV::color(0, 0, 1);
        else
            CV::color(0, 1, 0);

        if (moving)
            CV::circleFill(p.x + x, p.y + y, 10, 20);
        else
            CV::circleFill(p.x, p.y, 10, 20);
    }

    bool Colidiu(int mx, int my) {
        if (mx >= (p.x - 10) && mx <= (p.x + 10) && my >= (p.y - 10) && my <= (p.y + 10))
            return true;
        return false;
    }
};

#endif
