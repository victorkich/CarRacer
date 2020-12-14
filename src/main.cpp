#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "Point.h"
#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Checkbox.h"


//variavel global para selecao do que sera exibido na canvas.
Checkbox *cb_cp = NULL, *cb_ls=NULL;
std::vector<Point> g_points;
int g_npoints = 0;
int opcao  = 50;
double angle = 0;
float c=0;
float addy = 0, addx = 0;
float cont=0;
bool g_finish = false, first_car_angle = true;
bool point_local = true;
bool frente = false, tras = false, direita = false, esquerda = false;
int screenWidth = 1280, screenHeight = 720; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY, mov_mouseX, mov_mouseY; //variaveis globais do mouse para poder exibir dentro da render().
struct{ GLubyte red, green, blue; } pixel;

Vector2 avaliaBezier(float t, Vector2 p1, Vector2 p2, Vector2 p3)
{
    float x_t = (p2.x-p1.x)*(2*t*(1-t)) + (p3.x-p1.x)*pow(t, 3);
    float y_t = (p2.y-p1.y)*(2*t*(1-t)) + (p3.y-p1.y)*pow(t, 3);
    Vector2 ret (x_t, y_t);
    return ret;
}

void render()
{
   glClearColor(0, 0, 0, 0);
   CV::color(1, 1, 1);
   cb_cp->Render();
   cb_ls->Render();

   if(g_npoints > 2){
      Vector2 vetor, vetor2, vetor3;
      for(int x=2; x<g_npoints; x+=2)
      {
         CV::color(1,1,1);
         CV::translate(g_points[x-2].p.x, g_points[x-2].p.y);
         for(float t=0; t<1; t+=0.001)
         {
            vetor = avaliaBezier(t, g_points[x-2].p, g_points[x-1].p, g_points[x].p);
            CV::point(vetor.x, vetor.y);
            CV::circleFill(vetor.x, vetor.y, 40, 20);
         }
      }
      if (g_npoints%2 == 0){
         Vector2 mouse;
         CV::color(1,1,1);
         CV::translate(g_points[g_npoints-2].p.x, g_points[g_npoints-2].p.y);
         for(float t=0; t<1; t+=0.001)
         {
            mouse.set(mouseX, mouseY);
            vetor = avaliaBezier(t, g_points[g_npoints-2].p, g_points[g_npoints-1].p, mouse);
            CV::point(vetor.x, vetor.y);
            CV::circleFill(vetor.x, vetor.y, 40, 20);
         }
      }
   }

   CV::translate(0, 0);
   CV::color(0.5, 1, 0.5);
   if(g_npoints > 0 && cb_cp->getStatus()){
      for(int x=0; x<g_npoints; x++)
      {
         g_points[x].Render(mouseX - mov_mouseX, mouseY - mov_mouseY);
      }
   }

   if(g_finish){
      if(first_car_angle)
      {
         Vector2 aux_center_car (0, 1);
         aux_center_car.normalize();
         Vector2 center_car = avaliaBezier(0.001, g_points[0].p, g_points[1].p, g_points[2].p);
         center_car.normalize();
         angle = center_car.angulo(aux_center_car);
         first_car_angle = false;
      }
      if(frente)
      {
         addy += 2*cos(angle);
         addx += 2*sin(angle);
      }
      if(tras)
      {
         addy -= 2*cos(angle);
         addx -= 2*sin(angle);
      }
      if(direita)
      {
         angle -= 0.04;
      }
      if(esquerda)
      {
         angle += 0.04;
      }

      int new_centre_height = round(((40)/2)-1);
      int new_centre_width = round(((20)/2)-1);

      CV::translate(g_points[0].p.x - new_centre_width, g_points[0].p.y - new_centre_height);
      glReadPixels(g_points[0].p.x+addx, g_points[0].p.y+addy, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
      CV::color(0.5, 0.5, 0);
      for(float y=0; y<40; y+=0.6)
      for(float x=0; x<20; x+=0.6)
      {
         float old_x = x-new_centre_width-1;
         float old_y = y-new_centre_height-1;

         int new_y = round(-old_x*sin(angle)+old_y*cos(angle));
         int new_x = round(old_x*cos(angle)+old_y*sin(angle));

         new_y = new_centre_height-new_y;
         new_x = new_centre_width-new_x;

         CV::point(new_x+addx, new_y+addy);
      }
      if(!pixel.red && !pixel.green && !pixel.blue)
      {
         addx = 0;
         addy = 0;
         Vector2 aux_center_car (0, 1);
         aux_center_car.normalize();
         Vector2 center_car = avaliaBezier(0.001, g_points[0].p, g_points[1].p, g_points[2].p);
         center_car.normalize();
         angle = center_car.angulo(aux_center_car);
      }

      CV::translate(g_points[0].p.x+addx, g_points[0].p.y+addy);
      std::vector<float> obs;
      float add_angle = 0.0, new_x, new_y, scale, distance;
      for(int i=0; i<8; i++)
      {
         scale = 0.0;
         add_angle += 0.785;
         new_y = 300*cos(angle+add_angle);
         new_x = 300*sin(angle+add_angle);
         while(true)
         {
            scale += 0.02;
            glReadPixels(g_points[0].p.x+addx+(scale*new_x), g_points[0].p.y+addy+(scale*new_y), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
            if (!pixel.red && !pixel.green && !pixel.blue)
            {
               distance = sqrt(pow(scale*new_x, 2) + pow(scale*new_y, 2))-20;
               obs.push_back(distance);
               if(cb_ls->getStatus())
               {
                  if(distance > 20)
                     CV::color(0.2,1,0.2);
                  else
                     CV::color(1,0.2,0.2);
                  CV::line(0,0,(scale*new_x), (scale*new_y));
               }
               break;
            }
         }
      }
   }
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   //printf("\nTecla: %d" , key);

   if(key==201)
   {
    frente = true;
   }
   if(key==203)
   {
    tras = true;
   }
   if(key==200)
   {
    direita = true;
   }
   if(key==202)
   {
    esquerda = true;
   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   //printf("\nLiberou: %d" , key);

   if(key==201)
   {
    frente = false;
   }
   if(key==203)
   {
    tras = false;
   }
   if(key==200)
   {
    direita = false;
   }
   if(key==202)
   {
    esquerda = false;
   }
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   if( button == 0)
   {
      if (state == 0)
      {
         if( cb_cp->Colidiu(x, y) )//Atualiza o status da checkbox
         {
            cb_cp->updateStatus();
         }else if( cb_ls->Colidiu(x, y) )//Atualiza o status da checkbox
         {
            cb_ls->updateStatus();
         }else
         {
            for (int p=g_npoints-1; p>=0; p--)
            {
               if (g_points[0].Colidiu(mouseX, mouseY) && g_points[g_npoints-1].Colidiu(mouseX, mouseY))
               {
                  point_local = false;
                  mov_mouseX = mouseX;
                  mov_mouseY = mouseY;
                  g_points[0].moving = true;
                  g_points[g_npoints-1].moving = true;
                  break;
               }else if(g_points[p].Colidiu(mouseX, mouseY))
               {
                  point_local = false;
                  mov_mouseX = mouseX;
                  mov_mouseY = mouseY;
                  g_points[p].moving = true;
                  break;
               }
            }
            if(point_local && !g_finish)
            {
               Vector2 new_xy (mouseX, mouseY);
               Point new_point (new_xy, g_npoints);
               g_points.push_back(new_point);
               g_npoints++;
            }
         }
      }else if(state == 1)
      {
         for (int p=g_npoints-1; p>=0; p--)
         {
            g_points[p].Update();
            g_points[p].moving = false;
         }
         point_local = true;
      }
   }
   if( button == 2 )
   {
      if( state == 0 )
      {
         if(g_points[0].Colidiu(mouseX, mouseY) && !g_finish && g_npoints%2 == 0)
         {
            Point new_point (g_points[0].p, 0);
            g_points.push_back(new_point);
            g_npoints++;
            g_finish = true;
         }
      }
   }
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "CarRacer");

   //Cria a inicializa a caixa de selecao
   cb_cp = new Checkbox(1000, 650, 40, 40, "Show control points", true);
   cb_ls = new Checkbox(1000, 600, 40, 40, "Show laser scanner", true);

   CV::run();
}
