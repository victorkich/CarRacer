//
// Created by victorkich on 12/15/20.
//

#ifndef CARRACER_VECTOR3_H
#define CARRACER_VECTOR3_H


class Vector3 {
public:
    float x, y, z;

    Vector3()
    {
        x = y = z =0;
    }

    Vector3(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    void set(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
};


#endif //CARRACER_VECTOR3_H
