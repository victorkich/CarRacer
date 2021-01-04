#ifndef CARRACER_FPS_H
#define CARRACER_FPS_H

#include <ctime>

class Fps {
private:
    float x, y, fps = 0.;
    char frame[16];

public:
    Fps(float _x, float _y) {
        x = _x;
        y = _y;
    }

    float get_fps() {
        return fps;
    }

    void Render(clock_t timer) {
        CV::translate(0, 0);
        CV::color(0.2, 1, 0.2);
        fps = float(1 / (float(clock() - timer) / (float) CLOCKS_PER_SEC));
        sprintf(frame, "FPS: %d", int(fps));
        CV::text(x, y, frame);
    }
};

#endif //CARRACER_FPS_H
