#ifndef __PERCEPTRON_CPP
#define __PERCEPTRON_CPP

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "matrix.h"


class Perceptron {
private:
    QSMatrix<float> *Wh, *bh, *Wo, *bo, *sh, *zh, *so, *zo;
    float eta;

public:
    Perceptron(float _eta) {
        Wh = new QSMatrix<float>(20, 10, (rand()%100)/100-0.5);
        bh = new QSMatrix<float>(20, 1, (rand()%100)/100-0.5);
        sh = new QSMatrix<float>(20, 1, 0);
        zh = new QSMatrix<float>(20, 1, 0);

        Wo = new QSMatrix<float>(4, 20, (rand()%100)/100-0.5);
        bo = new QSMatrix<float>(4, 1, (rand()%100)/100-0.5);
        so = new QSMatrix<float>(4, 1, 0);
        zo = new QSMatrix<float>(4, 1, 0);

        eta = _eta;
    }

    QSMatrix<float> sigmoid(QSMatrix<float> x) {
        unsigned rows = x.get_rows();
        unsigned cols = x.get_cols();

        QSMatrix<float> result(rows, cols, 0.0);

        for (unsigned i = 0; i < rows; i++) {
            for (unsigned j = 0; j < cols; j++) {
                result.modify(i, j, 1.0 / (1.0 + exp(-x(i, j))));
            }
        }
        return result;
    }

    QSMatrix<float> forward(QSMatrix<float> x) {
        *sh = (*Wh * x) + *bh;
        *zh = sigmoid(*sh);
        *so = (*Wo * *zh) + *bo;
        *zo = sigmoid(*so);
        return *zo;
    }

    void train(QSMatrix<float> y, QSMatrix<float> Y) {
        QSMatrix<float> od = (y - Y) * *zo * ((*zo * -1.0) + 1.0);
        QSMatrix<float> dh = (Wo->transpose() * od) * *zh * ((*zh * -1.0) + 1);
        *Wo = *Wo - (od * zh->transpose()) * eta;
        *bo = *bo - (od * eta);
        *Wh = *Wh - (dh * y.transpose()) * eta;
        *bh = *bh - (dh * eta);
    }
};

#endif
