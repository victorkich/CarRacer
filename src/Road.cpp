#include <GL/glut.h>
#include <stdio.h>
#include <vector>

#include "Vector2.h"
#include "gl_canvas2d.h"
#include "Point.h"
#include "matrix.h"

class Road {
private:
    std::vector <Point> points;
    int n_points = 0, quality;
    float len;

public:
    Road(float length, int curve_quality) {
        quality = round((length / 10) * curve_quality);
        len = length;
    }

    void updatePoints(std::vector <Point> g_points, int g_npoints) {
        points = g_points;
        n_points = g_npoints;
    }

    void Render(float mouseX, float mouseY, bool finish) {
        if (n_points) {
            int number = n_points;
            if (not finish) {
                Point mouse(mouseX, mouseY, 1);
                points.push_back(mouse);
            } else
                number = n_points - 1;
            for (float t = 0; t < 0.05; t += 0.0001) {
                for (int i = number; i; i--)
                    for (int k = 0; k < i; k++)
                        points[k].p = points[k].p + (points[k + 1].p - points[k].p) * t;

                CV::color(t * 50, 1, 1);
                CV::circleFill(points[0].p.x, points[0].p.y, len, quality);
            }
        }
    }
};
