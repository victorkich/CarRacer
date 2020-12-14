#ifndef __POINT_H__
#define __POINT_H__

#include "gl_canvas2d.h"
#include "Vector2.h"

class Point{
    float add_x=0, add_y=0;
    bool first = true;
public:
    Vector2 p;
    bool moving = false;
    
    Point(Vector2 p_, int pos)
    {
        p = p_;
        if (pos>0)
        {
            first = false;
        }
    }

    void Update()
    {
        if(moving)
        {
            p.x += add_x;
            p.y += add_y;
            add_x = 0;
            add_y = 0;
        }
    }

    void Render(float x, float y)
    {
        if(first)
        {
            CV::color(0, 0, 1);
        }else
        {
            CV::color(0, 1, 0);
        }

        if (moving)
        {
            add_x = x;
            add_y = y;
            CV::circleFill(p.x+x, p.y+y, 10, 20);
        }else
        {
            CV::circleFill(p.x, p.y, 10, 20);
        }
    }

    bool Colidiu(int mx, int my)
    {
        if( mx >= (p.x-10) && mx <= (p.x + 10) && my >= (p.y-10) && my <= (p.y + 10) )
        {
            return true;
        }
        return false;
    }
};

#endif

