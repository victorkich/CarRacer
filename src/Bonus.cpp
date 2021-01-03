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
        Gold p1(_x + (largura / 3), _y + (altura / 2) - 10, true);
        player1.push_back(p1);
        Gold p2(_x + (largura / 3) * 2, _y + (altura / 2) - 10, true);
        player2.push_back(p2);
    }

    float getReward(float cx, float cy, bool type) {
        float reward = 0;
        if (type) {
            for (int i = 0; i < player1.size(); i++)
                if (player1[i].x >= cx - 10 && player1[i].x <= cx + 10 && player1[i].y >= cy - 10 &&
                    player1[i].y <= cy + 10 && player1[i].visible) {
                    player1[i].visible = false;
                    reward++;
                }
        } else
            for (int i = 0; i < player2.size(); i++)
                if (player2[i].x >= cx - 10 && player2[i].x <= cx + 10 && player2[i].y >= cy - 10 &&
                    player2[i].y <= cy + 10 && player2[i].visible) {
                    player2[i].visible = false;
                    reward++;
                }

        return reward;
    }

    void reset(bool type) {
        if (type){
            for (int i = 0; i < player1.size(); i++)
                player1[i].visible = true;
        }else
            for (int i = 0; i < player2.size(); i++)
                player2[i].visible = true;
    }

    void Render(float mouseX, float mouseY) {
        bool new_gold = true;
        for (int i = 0; i < player1.size(); i++)
            if (player1[i].x >= x && player1[i].x <= x + largura && player1[i].y >= y && player1[i].y <= y + altura)
                new_gold = false;

        if (new_gold) {
            Gold p1(x + (largura / 3), y + (altura / 2) - 10, true);
            player1.push_back(p1);
        } else
            new_gold = true;

        for (int i = 0; i < player2.size(); i++)
            if (player2[i].x >= x && player2[i].x <= x + largura && player1[i].y >= y && player2[i].y <= y + altura)
                new_gold = false;

        if (new_gold) {
            Gold p2(x + (largura / 3) * 2, y + (altura / 2) - 10, true);
            player2.push_back(p2);
        }

        CV::color(1, 1, 1);
        CV::rect(x, y, x + largura, y + altura);
        CV::text(x + (largura / 2) - 25, y + altura + 10, "Bonus");
        CV::text(x + (largura / 3) - 10, y + (altura / 5) * 4 - 10, "P1");
        CV::text(x + (largura / 3) * 2 - 10, y + (altura / 5) * 4 - 10, "IA");
        CV::color(0.48, 0.4, 0.12);
        for (Gold g : player1)
            g.Render(mouseX, mouseY);
        CV::color(0.68, 0.6, 0.32);
        for (Gold g : player2)
            g.Render(mouseX, mouseY);
    }

    void stopMoving(float x, float y) {
        for (int i = 0; i < player1.size(); i++) {
            player1[i].Update(x, y);
            player1[i].moving = false;
        }
        for (int i = 0; i < player2.size(); i++) {
            player2[i].Update(x, y);
            player2[i].moving = false;
        }
    }

    bool Colidiu(int mx, int my) {
        for (int i = 0; i < player1.size(); i++)
            if (player1[i].Colidiu(mx, my)) {
                player1[i].moving = true;
                return true;
            }
        for (int i = 0; i < player2.size(); i++)
            if (player2[i].Colidiu(mx, my)) {
                player2[i].moving = true;
                return true;
            }
        return false;
    }
};
