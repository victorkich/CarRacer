#ifndef __SCORE_H__
#define __SCORE_H__

#include "../OpenGL/gl_canvas2d.h"


class Score {
    float x, y, altura, largura;
    char player[100];

public:
    Score(float _x, float _y, float _altura, float _largura, char *_player) {
        altura = _altura;
        largura = _largura;
        x = _x;
        y = _y;
        strcpy(player, _player);
    }

    void Render(int _lap, int _score, int _speed) {
        char str[32];
        CV::color(1, 1, 1);
        CV::rect(x, y, x + largura, y + altura);
        CV::text(x + 5, y + (altura / 5) * 4, player);
        sprintf(str, "Score: %d", _score);
        CV::text(x + 5, y + (altura / 5) * 3, str);
        sprintf(str, "Lap: %d", _lap);
        CV::text(x + 5, y + (altura / 5) * 2, str);
        sprintf(str, "Speed: %d", _speed);
        CV::text(x + 5, y + altura / 5, str);
    }
};

#endif
