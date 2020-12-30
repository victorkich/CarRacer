#ifndef __ES_CPP
#define __ES_CPP

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "matrix.h"
#include "Perceptron.cpp"

int argmax(QSMatrix<double> x) {
    int i_max = 0;
    unsigned rows = x.get_rows();
    for (unsigned i = 0; i < rows; i++) {
        if (x(i, 0) > x(i_max, 0)) {
            i_max = i;
        }
    }
}

class ES {
private:
    float noise_std = 0.01, lr = 0.001;

public:
    int evaluate(QSMatrix<double> obs) {
        QSMatrix<double> act_prob(4, 1, 0);
        act_prob = forward(obs);
        return argmax(act_prob);
    }

};

#endif