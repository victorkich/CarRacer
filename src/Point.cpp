#include "gl_canvas2d.h"
#include "Point.h"

Point::Point(Vector2 p_)
{
    Point::p = p_;
}

void Point::Render()
{
    CV::color(0, 1, 0);
    CV::circle(Point::p, 10, 10);
}


bool Point::Colidiu(int mx, int my)
{
    if( mx >= Point::p.x && mx <= (Point::p.x + 5) && my >= Point::p.y && my <= (Point::p.y + 5) )
    {
        return true;
    }
    return false;
}
