#ifndef __PERCEPTRON_CPP
#define __PERCEPTRON_CPP

#include <stdio.h> 
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "matrix.h"

QSMatrix<double> Wh(20, 10, 0.5);
QSMatrix<double> bh(10, 1, 0.5);
QSMatrix<double> Wo(4, 10, 0.5);
QSMatrix<double> bo(4, 1, 0.5);
float eta = 0.01;

QSMatrix<double> sh(10, 1, 0);
QSMatrix<double> zh(10, 1, 0);
QSMatrix<double> so(4, 1, 0);
QSMatrix<double> zo(4, 1, 0);

QSMatrix<double> sigmoid(QSMatrix<double> x)
{
    unsigned rows = x.get_rows();
    unsigned cols = x.get_cols();

    QSMatrix<double> result(rows, cols, 0.0);

    for (unsigned i=0; i<rows; i++) {
        for (unsigned j=0; j<cols; j++) {
            result(i,j) = 1.0/(1.0 + exp(-x(i,j)));
        }
    }
    return result;
}

QSMatrix<double> forward(QSMatrix<double> x)
{
    sh = (Wh*x) + bh;
    zh = sigmoid(sh);
    so = (Wo*zh) + bo;
    zo = sigmoid(so);
    return zo;
}

void train(QSMatrix<double> X, QSMatrix<double> Y)
{
    QSMatrix<double> y = forward(X);
    QSMatrix<double> od = (y - Y) * zo * ((zo*-1.0) + 1.0);
    QSMatrix<double> dh = (Wo.transpose() * od) * zh * ((zh*-1.0) + 1);
    Wo = Wo + (od*zh.transpose())*-eta;
    bo = bo + (od*-eta);
    Wh = Wh + (dh*X.transpose())*-eta;
    bh = bh + (dh*eta);
}

#endif