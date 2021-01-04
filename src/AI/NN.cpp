#ifndef __NN_CPP
#define __NN_CPP

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "../Utils/matrix.h"
#include "Perceptron.cpp"


class NN {
private:
    Perceptron *p;
    float len;

public:
    NN(float eta){
        p = new Perceptron(eta);
    }

    int argmax(QSMatrix<float> x) {
        int i_max = 0;
        unsigned rows = x.get_rows();
        for (unsigned i = 0; i < rows; i++) {
            if (x(i, 0) > x(i_max, 0)) {
                i_max = i;
            }
        }
        return i_max;
    }

    int argmin(QSMatrix<float> x) {
        int i_min = 0;
        int rows = x.get_rows()-2;
        for (int i = 0; i < rows; i++) {
            if (x(i, 0) < x(i_min, 0)) {
                i_min = i;
            }
        }
        return i_min;
    }

    std::vector<bool> evaluate(QSMatrix<float> obs, float _len) {
        len = _len;
        QSMatrix<float> act_prob(4, 1, 0);
        act_prob = p->forward(obs);
        int max = argmax(act_prob);
        for (int i=0; i<4; i++){
            if (i==max)
                act_prob.modify(i, 0, 1);
            else
                act_prob.modify(i, 0, 0);
        }
        p->train(act_prob, expected_action(obs));
        std::vector<bool> act(4, false);
        act[max] = true;
        return act;
    }

    QSMatrix<float> expected_action(QSMatrix<float> obs){
        QSMatrix<float> result(4, 1, 0);
        int min = argmin(obs);

        if (obs(min, 0) > (len/2)) {
            result.modify(0, 0, 1);
        }else{
            switch (min) {
                case 0:
                    result.modify(2, 0, 1);
                    break;
                case 1:
                    result.modify(2, 0, 1);
                    break;
                case 2:
                    result.modify(0, 0, 1);
                    break;
                case 3:
                    result.modify(0, 0, 1);
                    break;
                case 4:
                    result.modify(0, 0, 1);
                    break;
                case 5:
                    result.modify(3, 0, 1);
                    break;
                case 6:
                    result.modify(1, 0, 1);
                    break;
                case 7:
                    result.modify(3, 0, 1);
                    break;
            }
        }
        return result;
    }
};

#endif