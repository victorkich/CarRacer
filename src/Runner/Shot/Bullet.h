#ifndef __BULLET_H__
#define __BULLET_H__

#include "../../OpenGL/gl_canvas2d.h"
#include "../../Utils/Vector2.h"
#include "math.h"


class Bullet {
private:
    Vector2 p;
    float angle;
    struct pix {
        GLubyte red, green, blue;
    } pixel;

public:
    bool inside = true;

    Bullet(Vector2 _p, float _angle) {
        p = _p;
        angle = _angle;
    }

    Bullet(float x, float y, float _angle) {
        p.set(x, y);
        angle = _angle;
    }

    void Render(float fps) {
        CV::translate(0, 0);

        float new_y = 100 * cos(angle);
        float new_x = 100 * sin(angle);

        p.x = p.x + 3 * (new_x / fps);
        p.y = p.y + 3 * (new_y / fps);

        glReadPixels(p.x, p.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
        if (!pixel.red && !pixel.green && !pixel.blue)
            inside = false;

        CV::color(0, 0, 0);
        CV::circleFill(p.x, p.y, 5, 10);
    }

    bool Colidiu(int mx, int my) {
        if (mx >= (p.x - 10) && mx <= (p.x + 10) && my >= (p.y - 10) && my <= (p.y + 10))
            return true;
        return false;
    }
};

#endif
