#include <GL/glut.h>
#include <vector>
#include "Vector2.h"
#include "gl_canvas2d.h"
#include "Point.h"
#include "matrix.h"

class Car {
private:
    float centre_width, centre_height, angle, width, height, quality;
    float addx = 0., addy = 0., reward = 0., position = 0.;
    struct{ GLubyte red, green, blue; } pixel;
    Vector2 aux_center_car, center_car;
    std::vector<Point> points;

public:
    Car(float w, float h, float filter_quality, std::vector<Point> g_points){
        width = w;
        height = h;
        centre_width = w / 2;
        centre_height = h / 2;
        quality = float(0.1) / filter_quality;
        points = g_points;
        aux_center_car.set(0,1);
        aux_center_car.normalize();
        angle = get_start_angle();
    }

    float get_start_angle(){
        center_car = center_car.Bezier(0.01, points[0].p, points[1].p, points[2].p);
        center_car.normalize();
        return center_car.angulo(aux_center_car);
    }

    void step(std::vector<bool> action, float fps){
        if(action[0]){
            addy += (150/fps) * cos(angle);
            addx += (150/fps) * sin(angle);
        }
        if(action[1]){
            addy -= (150/fps) * cos(angle);
            addx -= (150/fps) * sin(angle);
        }
        if(action[2])
            angle -= 2/fps;
        if(action[3])
            angle += 2/fps;
    }

    void updatePoints(std::vector<Point> g_points){
        points = g_points;
    }

    float get_carx(){
        return points[0].p.x + addx;
    }

    float get_cary(){
        return points[0].p.y + addy;
    }

    float get_position(){
        return position;
    }

    std::vector<float> Render(bool show_lasers){
        float car_x = get_carx(), car_y = get_cary();
        CV::translate(car_x - centre_width, car_y - centre_height);
        glReadPixels(car_x, car_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
        position = (float(pixel.red)/255) * 100;
        CV::color(position/100, float(pixel.blue)/255, 1);
        float old_x, old_y, new_x, new_y;
        for(float y=0; y<height; y+=0.6)
            for(float x=0; x<width; x+=0.6){
                old_x = x - centre_width;
                old_y = y - centre_height;
                new_y = -old_x*sin(angle) + old_y*cos(angle);
                new_x = old_x*cos(angle) + old_y*sin(angle);
                new_y = centre_height - new_y;
                new_x = centre_width - new_x;
                CV::point(new_x, new_y);
            }
        if(!pixel.red && !pixel.green && !pixel.blue){
            addx = 0;
            addy = 0;
            center_car = center_car.Bezier(0.01, points[0].p, points[1].p, points[2].p);
            center_car.normalize();
            angle = center_car.angulo(aux_center_car);
        }

        CV::translate(car_x, car_y);
        std::vector<float> obs;
        float add_angle = 0.0, scale, distance;
        for(int i=0; i<8; i++){
            scale = 0.0;
            add_angle += PI_4;
            new_y = 100 * cos(angle+add_angle);
            new_x = 100 * sin(angle+add_angle);
            while(true){
                scale += quality;
                glReadPixels(car_x+scale*new_x, car_y+scale*new_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
                if (!pixel.red && !pixel.green && !pixel.blue){
                    distance = sqrt(float(pow(scale*new_x, 2) + pow(scale*new_y, 2))) - 20;
                    obs.push_back(distance);
                    if(show_lasers){
                        if(distance > 20)
                            CV::color(0.2,1,0.2);
                        else
                            CV::color(1,0.2,0.2);
                        CV::line(0,0,scale*new_x, scale*new_y);
                    }
                    break;
                }
            }
        }
        return obs;
    }
};