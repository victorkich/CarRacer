#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <vector>
#include <ctime>

#include "Point.h"
#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Checkbox.h"
#include "matrix.h"
#include "Car.cpp"
#include "Road.cpp"
#include "Fps.h"
#include "Score.h"
#include "Bonus.cpp"
#include "Botao.h"
#include "NN.cpp"


//variaveis globais para selecao do que sera exibido na canvas.
Checkbox *cb_cp = nullptr, *cb_ls = nullptr, *cb_train = nullptr;
Car *car = nullptr, *car_ia = nullptr;
Road *road = nullptr;
Fps *fps = nullptr;
Score *score = nullptr, *score_ia = nullptr;
Bonus *bonus = nullptr;
Botao *h_transpose = nullptr, *v_transpose = nullptr, *more_scale = nullptr, *less_scale = nullptr, *spin_left = nullptr, *spin_right = nullptr;
NN *nn = nullptr;

std::vector <Point> g_points;
std::vector<bool> g_actions(4, false), g_actions_ia(4, false);
QSMatrix<float> g_obs(1, 8, 0), ia_obs(1, 8, 0);
std::vector<bool> progress;

int g_npoints = 0;
bool g_finish = false, first_car_angle = true, point_local = true;
int screenWidth = 1280, screenHeight = 720; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY, mov_mouseX, mov_mouseY, dif_x, dif_y; //variaveis globais do mouse para poder exibir dentro da render().
clock_t timer;
float reward = 0, reward_ia = 0;
int lap = 0, lap_ia = 0;


void render() {
    // Set timer to calculate the FPS
    timer = clock();
    glClearColor(0, 0, 0, 0);
    // Calculate moved distance
    dif_x = mouseX - mov_mouseX;
    dif_y = mouseY - mov_mouseY;
    // Checkbox renders
    cb_cp->Render();
    cb_ls->Render();
    cb_train->Render();
    // Button renders
    h_transpose->Render();
    v_transpose->Render();
    more_scale->Render();
    less_scale->Render();
    spin_left->Render();
    spin_right->Render();
    // Road render
    road->updatePoints(g_points, g_npoints);
    road->Render(float(mouseX), float(mouseY), g_finish);

    CV::translate(0, 0);
    if (g_npoints && cb_cp->getStatus())
        for (Point p : g_points)
            p.Render(dif_x, dif_y);

    if (g_finish) {
        if (first_car_angle) {
            car = new Car(20, 40, 2, g_points);  // width, height, filter quality(1-10), points
            car_ia = new Car(20, 40, 2, g_points);
            first_car_angle = false;
        }
        car_ia->updatePoints(g_points);
        car->updatePoints(g_points);
        car->step(g_actions, fps->get_fps());
        g_obs = car->Render(cb_ls->getStatus());
        ia_obs = car_ia->Render(cb_ls->getStatus());

        if (cb_train->getStatus()) {
            g_actions_ia = nn->evaluate(ia_obs);
            car_ia->step(g_actions_ia, fps->get_fps());
        }

        reward = bonus->getReward(car->get_carx(), car->get_cary(), true);
        car->addReward(reward);
        reward_ia = bonus->getReward(car_ia->get_carx(), car_ia->get_cary(), false);
        car_ia->addReward(reward_ia);

        if (lap < car->get_lap()){
            bonus->reset(true);
            lap = car->get_lap();
        }
        if (lap_ia < car_ia->get_lap()){
            bonus->reset(false);
            lap_ia = car_ia->get_lap();
        }

        CV::translate(0, 0);
        score->Render(car->get_lap(), car->get_score(), car->get_speed());
        score_ia->Render(car_ia->get_lap(), car_ia->get_score(), car_ia->get_speed());
        //progress = car->get_progress();
        //printf("\n%s %s %s %s %s %s %s %s %s %s", progress[0] ? "true" : "false", progress[1] ? "true" : "false", progress[2] ?"true" : "false", progress[3] ? "true" : "false", progress[4] ? "true" : "false",
        //       progress[5] ? "true" : "false", progress[6] ? "true" : "false", progress[7] ?"true" : "false", progress[8] ? "true" : "false", progress[9] ? "true" : "false");
    }

    // Bonus panel render
    bonus->Render(dif_x, dif_y);
    // FPS render
    fps->Render(timer);
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key) {
    //printf("\nTecla: %d" , key);

    if (key == 201)
        g_actions[0] = true; // forward
    if (key == 203)
        g_actions[1] = true; // backward
    if (key == 200)
        g_actions[2] = true; // right
    if (key == 202)
        g_actions[3] = true; // left
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key) {
    //printf("\nLiberou: %d" , key);

    if (key == 201)
        g_actions[0] = false; // forward
    if (key == 203)
        g_actions[1] = false; // backward
    if (key == 200)
        g_actions[2] = false; // right
    if (key == 202)
        g_actions[3] = false; // left
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y) {
    mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
    mouseY = y;

    //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

    if (button == 0) {
        if (state == 0) {
            if (cb_cp->Colidiu(x, y))//Atualiza o status da checkbox
                cb_cp->updateStatus();
            else if (cb_ls->Colidiu(x, y))//Atualiza o status da checkbox
                cb_ls->updateStatus();
            else if (cb_train->Colidiu(x, y))
                cb_train->updateStatus();
            else if (h_transpose->Colidiu(x, y))
                h_transpose->updateStatus();
            else if (v_transpose->Colidiu(x, y))
                v_transpose->updateStatus();
            else if (more_scale->Colidiu(x, y))
                more_scale->updateStatus();
            else if (less_scale->Colidiu(x, y))
                less_scale->updateStatus();
            else if (spin_left->Colidiu(x, y))
                spin_left->updateStatus();
            else if (spin_right->Colidiu(x, y))
                spin_right->updateStatus();
            else if (bonus->Colidiu(x, y)) {
                mov_mouseX = mouseX;
                mov_mouseY = mouseY;
            } else {
                for (int p = g_npoints - 1; p >= 0; p--) {
                    if (g_points[0].Colidiu(x, y) && g_points[g_npoints - 1].Colidiu(x, y)) {
                        point_local = false;
                        mov_mouseX = x;
                        mov_mouseY = y;
                        g_points[0].moving = true;
                        g_points[g_npoints - 1].moving = true;
                        break;
                    } else if (g_points[p].Colidiu(x, y)) {
                        point_local = false;
                        mov_mouseX = x;
                        mov_mouseY = y;
                        g_points[p].moving = true;
                        break;
                    }
                }
                if (point_local && !g_finish) {
                    Point new_point(float(x), float(y), g_npoints);
                    g_points.push_back(new_point);
                    g_npoints++;
                }
            }
        } else if (state == 1) {
            if (h_transpose->Colidiu(x, y))
                h_transpose->updateStatus();
            else if (v_transpose->Colidiu(x, y))
                v_transpose->updateStatus();
            else if (more_scale->Colidiu(x, y))
                more_scale->updateStatus();
            else if (less_scale->Colidiu(x, y))
                less_scale->updateStatus();

            bonus->stopMoving(float(x - mov_mouseX), float(y - mov_mouseY));

            for (int p = g_npoints - 1; p >= 0; p--) {
                g_points[p].Update(float(x - mov_mouseX), float(y - mov_mouseY));
                g_points[p].moving = false;
            }
            point_local = true;
        }
    }
    if (button == 2) {
        if (state == 0) {
            if (g_points[0].Colidiu(x, y) && !g_finish) {
                Point new_point(g_points[0].p, 0);
                g_points.push_back(new_point);
                g_npoints++;
                g_finish = true;
            }
        }
    }
}

int main() {
    CV::init(&screenWidth, &screenHeight, "CarRacer");

    cb_cp = new Checkbox(1000, 650, 40, 40, "Show Control Points", true);
    cb_ls = new Checkbox(1000, 600, 40, 40, "Show Laser Scanner", true);
    cb_train = new Checkbox(1000, 550, 40, 40, "Train Neural Network", false);
    score = new Score(1000, 50, 70, 110, "Player: 1");
    score_ia = new Score(1130, 50, 70, 110, "Player: 2");
    road = new Road(40, 5);
    bonus = new Bonus(1050, 400, 70, 150);
    fps = new Fps(30, 690);

    h_transpose = new Botao(1050, 350, 150, 40, " H Transpose");
    v_transpose = new Botao(1050, 300, 150, 40, " V Transpose");
    more_scale = new Botao(1050, 250, 70, 40, "Size +");
    less_scale = new Botao(1130, 250, 70, 40, "Size -");
    spin_left = new Botao(1050, 200, 70, 40, "Spin -");
    spin_right = new Botao(1130, 200, 70, 40, "Spin +");

    nn = new NN(0.0001);

    CV::run();
}
