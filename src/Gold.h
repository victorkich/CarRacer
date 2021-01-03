#ifndef GOLD_H
#define GOLD_H

class Gold {
public:
    float x, y;
    bool moving = false, visible = true;

    Gold(float _x, float _y, bool _visible) {
        x = _x;
        y = _y;
        visible = _visible;
    }

    void Update(float _x, float _y) {
        if (moving) {
            x += _x;
            y += _y;
        }
    }

    void updateStatus() {
        moving = not moving;
    }

    void Render(float a, float b) {
        if (visible){
            if (moving)
                CV::circleFill(x + a, y + b, 10, 20);
            else
                CV::circleFill(x, y, 10, 20);
        }
    }

    bool Colidiu(int mx, int my) {
        if (mx >= (x - 10) && mx <= (x + 10) && my >= (y - 10) && my <= (y + 10) && visible)
            return true;
        return false;
    }
};

#endif //GOLD_H