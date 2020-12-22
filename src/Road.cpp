#include <GL/glut.h>
#include <vector>
#include "Vector2.h"
#include "gl_canvas2d.h"
#include "Point.h"
#include "matrix.h"

class Road {
private:
    std::vector<Point> points;
    int n_points = 0, quality;
    float len;

public:
    Road(float length, int curve_quality){
        quality = round((length/10) * curve_quality);
        len = length;
    }

    void updatePoints(std::vector<Point> g_points, int g_npoints) {
        points = g_points;
        n_points = g_npoints;
    }

    void Render(float mouseX, float mouseY){
        if(n_points > 2){
            Vector2 vetor;
            for(int x=2; x<n_points; x+=2)
            {
                CV::color(1,1,1);
                CV::translate(points[x-2].p.x, points[x-2].p.y);
                for(float t=0; t<1; t+=0.005)
                {
                    vetor = vetor.Bezier(t, points[x-2].p, points[x-1].p, points[x].p);
                    CV::point(vetor.x, vetor.y);
                    CV::circleFill(vetor.x, vetor.y, len, quality);
                }
            }
            if (n_points%2 == 0){
                CV::color(1,1,1);
                CV::translate(points[n_points-2].p.x, points[n_points-2].p.y);
                for(float t=0; t<1; t+=0.005)
                {
                    vetor.set(mouseX, mouseY);
                    vetor = vetor.Bezier(t, points[n_points-2].p, points[n_points-1].p, vetor);
                    CV::point(vetor.x, vetor.y);
                    CV::circleFill(vetor.x, vetor.y, len, quality);
                }
            }
        }
    }
};
