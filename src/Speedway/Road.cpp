#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <math.h>

#include "../Utils/Vector2.h"
#include "../OpenGL/gl_canvas2d.h"
#include "../Utils/Point.h"
#include "../Utils/matrix.h"


class Road {
private:
    std::vector <Point> points;
    int n_points = 0, quality;
    float len, scale = 1, angle = 0.01745329 * 180;

public:
    Road(float length, int curve_quality) {
        quality = round((length / 10) * curve_quality);
        len = length;
    }

    void updatePoints(std::vector <Point> g_points, int g_npoints) {
        points = g_points;
        n_points = g_npoints;
    }

    float get_scale() {
        return scale;
    }

    void add_angle(float add) {
        angle = add;
    }

    void add_scale(float add) {
        scale = add;
    }

    void add_len(float add) {
        len += add;
    }

    float get_len() {
        return len;
    }

    std::vector <Point>
    Render(float mouseX, float mouseY, bool finish, std::vector <Point> g_points, bool rotated, bool vt, bool ht,
           bool scaled) {
        if (n_points) {
            int number = n_points;
            if (not finish) {
                Point mouse(mouseX, mouseY, 1);
                points.push_back(mouse);
            } else
                number = n_points - 1;

            float max_x = -10000, min_x = 10000, max_y = -10000, min_y = 10000;
            for (float t = 0; t < 0.05; t += 0.0001) {
                for (int i = number; i; i--)
                    for (int k = 0; k < i; k++)
                        points[k].p = points[k].p + (points[k + 1].p - points[k].p) * t;

                if (rotated) {
                    if (points[0].p.x > max_x)
                        max_x = points[0].p.x;
                    if (points[0].p.y > max_y)
                        max_y = points[0].p.y;
                    if (points[0].p.x < min_x)
                        min_x = points[0].p.x;
                    if (points[0].p.y < min_y)
                        min_y = points[0].p.y;
                }
                CV::color(t * 50, 1, 1);
                CV::circleFill(points[0].p.x, points[0].p.y, len, quality);
            }

            float centre_width = (max_x - min_x) / 2 + min_x;
            float centre_height = (max_y - min_y) / 2 + min_y;

            if (scaled) {
                max_x = -10000;
                max_y = -10000;
                for (unsigned i = 0; i < g_points.size(); i++) {
                    if (g_points[i].p.x > max_x)
                        max_x = g_points[i].p.x;
                    if (g_points[i].p.y > max_y)
                        max_y = g_points[i].p.y;
                }
                float back_x = (max_x * scale - max_x) / 1.5;
                float back_y = (max_y * scale - max_y) / 1.5;
                for (unsigned i = 0; i < g_points.size(); i++) {
                    g_points[i].p.x = g_points[i].p.x * scale - back_x;
                    g_points[i].p.y = g_points[i].p.y * scale - back_y;
                }
            }

            if (vt) {
                max_y = -10000;
                min_y = 10000;
                for (unsigned i = 0; i < g_points.size(); i++) {
                    if (g_points[i].p.y > max_y)
                        max_y = g_points[i].p.y;
                    if (g_points[i].p.y < min_y)
                        min_y = g_points[i].p.y;
                }
                for (unsigned i = 0; i < g_points.size(); i++)
                    g_points[i].p.y = abs(max_y - g_points[i].p.y) + min_y;
            }

            if (ht) {
                max_x = -10000;
                min_x = 10000;
                for (unsigned i = 0; i < g_points.size(); i++) {
                    if (g_points[i].p.x > max_x)
                        max_x = g_points[i].p.x;
                    if (g_points[i].p.x < min_x)
                        min_x = g_points[i].p.x;
                }
                for (unsigned i = 0; i < g_points.size(); i++)
                    g_points[i].p.x = abs(max_x - g_points[i].p.x) + min_x;
            }

            if (rotated) {
                float old_x, old_y, new_x, new_y;
                for (unsigned i = 0; i < g_points.size(); i++) {
                    old_x = g_points[i].p.x - centre_width;
                    old_y = g_points[i].p.y - centre_height;
                    new_y = -old_x * sin(angle) + old_y * cos(angle);
                    new_x = old_x * cos(angle) + old_y * sin(angle);
                    new_y = centre_height - new_y;
                    new_x = centre_width - new_x;
                    g_points[i].p.x = new_x;
                    g_points[i].p.y = new_y;
                }
            }
        }
        return g_points;
    }
};
