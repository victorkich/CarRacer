#include <GL/glut.h>
#include <vector>

#include "../../OpenGL/gl_canvas2d.h"
#include "../../Utils/Vector2.h"
#include "Bullet.h"


class Gun {
private:
    std::vector <Bullet> bullets;

public:
    Gun() {}

    void new_bullet(float x, float y, float angle) {
        Bullet bullet(x, y, angle);
        bullets.push_back(bullet);
    }

    bool Render(float car_x, float car_y, float fps) {
        bool fired = false;
        std::vector <Bullet> new_bullets;
        for (unsigned i = 0; i < bullets.size(); i++) {
            if (bullets[i].inside)
                new_bullets.push_back(bullets[i]);
            if (bullets[i].Colidiu(car_x, car_y))
                fired = true;
        }
        bullets = new_bullets;

        for (unsigned i = 0; i < bullets.size(); i++)
            bullets[i].Render(fps);

        return fired;
    }
};
