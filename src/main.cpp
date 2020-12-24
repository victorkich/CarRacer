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
#include "Vector3.h"
#include "Fps.h"


//variaveis globais para selecao do que sera exibido na canvas.
Checkbox *cb_cp = nullptr, *cb_ls=nullptr, *cb_train= nullptr;
std::vector<Point> g_points;
std::vector<bool> g_actions(4,false), g_actions_ia(4,false);
std::vector<float> g_obs, ia_obs;
Car *car = nullptr, *car_ia = nullptr;
Road *road = nullptr;
Fps *fps = nullptr;
int g_npoints = 0;
bool g_finish = false, first_car_angle = true, point_local = true;
int screenWidth = 1280, screenHeight = 720; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY, mov_mouseX, mov_mouseY; //variaveis globais do mouse para poder exibir dentro da render().
clock_t timer;

void render()
{
    timer = clock();
    glClearColor(0, 0, 0, 0);

    cb_cp->Render();
    cb_ls->Render();
    cb_train->Render();

    road->updatePoints(g_points, g_npoints);
    road->Render(float(mouseX), float(mouseY));

    CV::translate(0, 0);
    if(g_npoints > 0 && cb_cp->getStatus()){
       for(int x=0; x<g_npoints; x++){
          g_points[x].Render(float(mouseX - mov_mouseX), float(mouseY - mov_mouseY));
       }
    }

    if(g_finish){
        if(first_car_angle){
            car = new Car(20, 40, 4, g_points);  // width, height, filter quality(1-10), points
            car_ia = new Car(20, 40, 4, g_points);
            first_car_angle = false;
        }
        car_ia->updatePoints(g_points);
        car->updatePoints(g_points);
        car->step(g_actions, fps->get_fps());
        car_ia->step(g_actions_ia, fps->get_fps());
        ia_obs = car_ia->Render(cb_ls->getStatus());
        g_obs = car->Render(cb_ls->getStatus());
    }

    fps->Render(timer);
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   //printf("\nTecla: %d" , key);

   if(key == 201)
       g_actions[0] = true; // forward
   if(key == 203)
       g_actions[1] = true; // backward
   if(key == 200)
       g_actions[2] = true; // right
   if(key == 202)
       g_actions[3] = true; // left
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   //printf("\nLiberou: %d" , key);

    if(key == 201)
        g_actions[0] = false; // forward
    if(key == 203)
        g_actions[1] = false; // backward
    if(key == 200)
        g_actions[2] = false; // right
    if(key == 202)
        g_actions[3] = false; // left
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   if( button == 0){
      if (state == 0){
         if( cb_cp->Colidiu(x, y) )//Atualiza o status da checkbox
            cb_cp->updateStatus();
         else if( cb_ls->Colidiu(x, y) )//Atualiza o status da checkbox
            cb_ls->updateStatus();
         else if( cb_train->Colidiu(x, y) )
             cb_train->updateStatus();
         else{
            for (int p=g_npoints-1; p>=0; p--){
               if (g_points[0].Colidiu(mouseX, mouseY) && g_points[g_npoints-1].Colidiu(mouseX, mouseY)){
                  point_local = false;
                  mov_mouseX = mouseX;
                  mov_mouseY = mouseY;
                  g_points[0].moving = true;
                  g_points[g_npoints-1].moving = true;
                  break;
               }else if(g_points[p].Colidiu(mouseX, mouseY)){
                  point_local = false;
                  mov_mouseX = mouseX;
                  mov_mouseY = mouseY;
                  g_points[p].moving = true;
                  break;
               }
            }
            if(point_local && !g_finish){
               Vector2 new_xy ((float)mouseX, (float)mouseY);
               Point new_point (new_xy, g_npoints);
               g_points.push_back(new_point);
               g_npoints++;
            }
         }
      }else if(state == 1){
         for (int p=g_npoints-1; p>=0; p--){
            g_points[p].Update();
            g_points[p].moving = false;
         }
         point_local = true;
      }
   }
   if( button == 2 ){
      if( state == 0 ){
         if(g_points[0].Colidiu(mouseX, mouseY) && !g_finish && g_npoints%2 == 0){
            Point new_point (g_points[0].p, 0);
            g_points.push_back(new_point);
            g_npoints++;
            g_finish = true;
         }
      }
   }
}

int main(){
   CV::init(&screenWidth, &screenHeight, "CarRacer");

   //Cria a inicializa duas caixas de selecao
   cb_cp = new Checkbox(1000, 650, 40, 40, "Show Control Points", true);
   cb_ls = new Checkbox(1000, 600, 40, 40, "Show Laser Scanner", true);
   cb_train = new Checkbox(1000, 550, 40, 40, "Train Neural Network", false);
   fps = new Fps(30, 690);

   road = new Road(40, 5);
   CV::run();
}
