#include <vector>
#include "gl_canvas2d.h"
#include "Gold.h"


class Bonus {
private:
    float x, y, altura, largura;
    std::vector <Gold> player1, player2;

public:
    Bonus(float _x, float _y, float _altura, float _largura) {
        altura = _altura;
        largura = _largura;
        x = _x;
        y = _y;
        Gold p1(_x + (largura / 3), y + (altura / 2) - 10, true);
        player1.push_back(p1);
        Gold p2(_x + (largura / 3) * 2, y + (altura / 2) - 10, true);
        player2.push_back(p2);
    }

    void Render(float mouseX, float mouseY) {
        CV::color(1, 1, 1);
        CV::rect(x, y, x + largura, y + altura);
        CV::text(x + (largura / 2) - 25, y + altura + 10, "Bonus");
        CV::text(x + (largura / 3) - 10, y + (altura / 5) * 4 - 10, "P1");
        CV::text(x + (largura / 3) * 2 - 10, y + (altura / 5) * 4 - 10, "IA");
        CV::color(0.48, 0.4, 0.12);
        for (Gold g : player1)
            g.Render(mouseX, mouseY);
        for (Gold g : player2)
            g.Render(mouseX, mouseY);
    }
};
