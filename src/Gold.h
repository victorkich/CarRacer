#ifndef GOLD_H
#define GOLD_H

class Gold {
public:
    float x, y, add_x = 0, add_y = 0;
    bool visible = true, moving = false;

    Gold(float _x, float _y, bool _visible) {
        x = _x;
        y = _y;
        visible = _visible;
    }

    void Update(float _x, float _y) {
        if (moving) {
            x += _x;
            y += _y;
            add_x = 0;
            add_y = 0;
        }
    }

    void Render(float _x, float _y) {
        CV::color(0.48, 0.4, 0.12);
        if (moving) {
            add_x = _x;
            add_y = _y;
            CV::circleFill(x + _x, y + _y, 10, 20);
        } else
            CV::circleFill(x, y, 10, 20);
    }

    bool Colidiu(int mx, int my) {
        if (mx >= (x - 10) && mx <= (x + 10) && my >= (y - 10) && my <= (y + 10))
            return true;
        return false;
    }
};

#endif //GOLD_H