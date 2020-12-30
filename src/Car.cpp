#include <GL/glut.h>
#include <vector>
#include <stdio.h>
#include <math.h>

#include "Vector2.h"
#include "gl_canvas2d.h"
#include "Point.h"
#include "matrix.h"


class Car {
private:
    float centre_width, centre_height, angle, width, height, quality;
    float addx = 0., addy = 0., position = 0., speed = 0.;
    int lap = 0, score = 0;
    struct pix { GLubyte red, green, blue;} pixel;
    Vector2 aux_center_car, center_car;
    std::vector<Point> points;
    std::vector<bool> progress;
    std::vector<float> obs;

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
        for(int i=0; i<10; i++)
            progress.push_back(false);
        for(int i=0; i<8; i++)
            obs.push_back(0.0);
    }

    float get_start_angle(){
        center_car = center_car.Bezier(0.01, points[0].p, points[1].p, points[2].p);
        center_car.normalize();
        return center_car.angulo(aux_center_car);
    }

    void step(std::vector<bool> action, float fps){
        addy += ((150/fps)*cos(angle)) * tanh(speed*0.02);
        addx += ((150/fps)*sin(angle)) * tanh(speed*0.02);
        if(action[0]){
            speed++;
        }
        if(action[1]){
            speed--;
        }
        if(not action[0] && not action[1]){
            if(speed > 0)
                speed -= 0.05 * speed;
            else if(speed < 0)
                speed -= 0.05 * speed;
        }
        if(action[2])
            angle -= 2/fps;
        if(action[3])
            angle += 2/fps;
    }

    void updatePoints(std::vector<Point> g_points){
        points = g_points;

        float car_x = get_carx(), car_y = get_cary();
        CV::translate(car_x - centre_width, car_y - centre_height);
        glReadPixels(car_x, car_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
        if(!pixel.red && !pixel.green && !pixel.blue){
            addx = 0;
            addy = 0;
            speed = 0;
            center_car = center_car.Bezier(0.01, points[0].p, points[1].p, points[2].p);
            center_car.normalize();
            angle = center_car.angulo(aux_center_car);
        }
        position = (float(pixel.red)/255) * 100;
        updateProgress();
        CV::color(0, 0, 1);
        char str[16];
        sprintf(str, "%d", int(position));
        CV::text(30, 30, str);
    }

    int get_speed(){
        return round(tanh(speed*0.02)*200);
    }

    void updateProgress(){
        int index = 0;
        for(int i=0; i<10; i++)
            if (progress[i])
                index++;
        if (10*(index+1) >= position && position >= index*10)
            progress[index] = true;

        bool prog = true;
        for (float x : progress)
            if(!x){
                prog = false;
                break;
            }
        if(prog) {
            for (int i=0; i<10; i++)
                progress[i] = false;
            lap++;
            score++;
        }
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

    std::vector<bool> get_progress(){
        return progress;
    }

    int get_lap(){
        return lap;
    }

    int get_score(){
        return score;
    }

    std::vector<float> Render(bool show_lasers){
        float car_x = get_carx(), car_y = get_cary();
        CV::translate(car_x - centre_width, car_y - centre_height);
        CV::color(0, 0, 1);
        float old_x, old_y, new_x, new_y;
        for(float y=0; y<height; y+=0.7)
            for(float x=0; x<width; x+=0.7){
                old_x = x - centre_width;
                old_y = y - centre_height;
                new_y = -old_x*sin(angle) + old_y*cos(angle);
                new_x = old_x*cos(angle) + old_y*sin(angle);
                new_y = centre_height - new_y;
                new_x = centre_width - new_x;
                CV::point(new_x, new_y);
            }

        CV::translate(car_x, car_y);
        float add_angle = 0., scale, distance;
        for(int i=0; i<8; i++){
            scale = 0.;
            add_angle += PI_4;
            new_y = 100 * cos(angle+add_angle);
            new_x = 100 * sin(angle+add_angle);
            while(true){
                scale += quality;
                glReadPixels(car_x+scale*new_x, car_y+scale*new_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
                if (!pixel.red && !pixel.green && !pixel.blue){
                    distance = sqrt(float(pow(scale*new_x, 2) + pow(scale*new_y, 2))) - 20;
                    obs[i] = distance;
                    if(show_lasers){
                        if(distance > 20)
                            CV::color(0,1,0.2);
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