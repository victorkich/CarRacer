#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <math.h>

#include "Vector2.h"
#include "gl_canvas2d.h"
#include "Point.h"
#include "matrix.h"

class Road {
private:
    std::vector <Point> points;
    int n_points = 0, quality;
    float len, scale = 1, angle = 0;

public:
    Road(float length, int curve_quality) {
        quality = round((length / 10) * curve_quality);
        len = length;
    }

    void updatePoints(std::vector <Point> g_points, int g_npoints) {
        points = g_points;
        n_points = g_npoints;
    }

    float get_scale(){
        return scale;
    }

    void add_angle(float add){
        angle += 0.01745329*add;
    }

    void add_scale(float add){
        scale += add;
    }

    void add_len(float add){
        len += add;
    }

    float get_len(){
        return len;
    }

    void Render(float mouseX, float mouseY, bool finish) {
        if (n_points) {
            int number = n_points;
            if (not finish) {
                Point mouse(mouseX, mouseY, 1);
                points.push_back(mouse);
            } else
                number = n_points - 1;

            std::vector <Point> draw;
            float max_x = -10000, min_x = 10000, max_y = -10000, min_y = 10000;
            for (float t = 0; t < 0.05; t += 0.0001) {
                for (int i = number; i; i--)
                    for (int k = 0; k < i; k++)
                        points[k].p = points[k].p + (points[k + 1].p - points[k].p) * t;

                if(points[0].p.x > max_x)
                    max_x = points[0].p.x;
                if(points[0].p.y > max_y)
                    max_y = points[0].p.y;
                if(points[0].p.x < min_x)
                    min_x = points[0].p.x;
                if(points[0].p.y < min_y)
                    min_y = points[0].p.y;

                Point np(points[0].p.x, points[0].p.y, 1);
                draw.push_back(np);
            }

            float centre_width = (max_x-min_x) / 2;
            float centre_height = (max_y-min_y) / 2;

            float old_x, old_y, new_x, new_y;
            for (int i = 0; i < 500; i++) {
                CV::color(float(i) / 300, 1, 1);
                old_x = draw[i].p.x - centre_width;
                old_y = draw[i].p.y - centre_height;
                new_y = -old_x * sin(angle) + old_y * cos(angle);
                new_x = old_x * cos(angle) + old_y * sin(angle);
                new_y = centre_height + new_y;
                new_x = centre_width + new_x;
                CV::circleFill(new_x, new_y, len, quality);
                //CV::circleFill(draw[i].p.x, draw[i].p.y, len, quality);
            }
        }
    }
};
