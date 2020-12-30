#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include "gl_canvas2d.h"

//Classe de criacao da caixa de checagem
class Checkbox {
    float altura, largura, x, y;
    char label[100];
    bool active;

//Chamada de criacao
public:
    Checkbox(float _x, float _y, float _larg, float _alt, char *_label, bool act) {
        altura = _alt;
        largura = _larg;
        x = _x;
        y = _y;
        strcpy(label, _label);
        active = act;
    }

    //Funcao para atualizar a localizacao em torno do ponto inicial x e y
    void updateLocation(float _x, float _y) {
        x = _x;
        y = _y;
    }

    //Funcao para reescalonar a imagem dentro do canvas
    void updateScale(int w, int h, int a, int l) {
        x = w;
        y = h;
        altura = a;
        largura = l;
    }

    //Funcao para trocar o estado atual ativo/nao ativo
    void updateStatus() {
        active = !active;
    }

    //Funcao que retorna o estado atual
    bool getStatus() {
        return active;
    }

    //Funcao para renderizar a caixa de selecao, podendo também inserir duas linhas nas diagonais da caixa caso o status esteja ativado
    void Render() {
        CV::color(1, 1, 1);
        CV::rect(x, y, x + largura, y + altura);
        CV::color(1, 1, 1);
        CV::text(x + largura + 5, y + altura / 2 - 4, label); //escreve o label do botao mais ou menos ao centro.

        if (active) {
            CV::color(1, 1, 1);
            CV::line(x, y, x + largura, y + altura);
            CV::line(x, y + altura, x + largura, y);
        }
    }

    //recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
    bool Colidiu(int mx, int my) {
        if (mx >= x && mx <= (x + largura) && my >= y && my <= (y + altura)) {
            return true;
        }
        return false;
    }

};

#endif