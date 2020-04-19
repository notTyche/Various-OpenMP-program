#ifndef GRAPHIC_H
#define GRAPHIC_H
#include <string>
#include <iostream>
#include <ctime>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <omp.h>
//#define NUM_THREADS 8
using namespace std;
double t_Start;
double t_Stop;
class GamingOfLife{
        
private:
int** matrix;
ALLEGRO_BITMAP* bmp;
ALLEGRO_BITMAP* buffer;
ALLEGRO_DISPLAY *display;
int scale_w;
int scale_h;
int scale_x;
int scale_y;
bool swap;
int population;
int change_color;
int pixel;
int screenWidth; 
int screenHeight;
int x;
int y;

public:
GamingOfLife(const int &scale_w, const int &scale_h, const int &scale_x, const int &scale_y, ALLEGRO_BITMAP *buffer, ALLEGRO_DISPLAY *display, int m, int n): population(0), change_color(0), pixel(18)
{
    x=m;
    y=n;
    screenHeight=pixel*y;
    screenWidth=pixel*x;
    al_init_primitives_addon();
    srand(time(NULL));
    int cont=0;
    matrix = new int*[screenHeight];
    
    for(int i=0;i<screenHeight;i++)
        matrix[i]=new int[screenWidth];

    #pragma omp parallel for schedule(dynamic)
    for (int i=0;i<y;i++)
        for (int j=0;j<x;j++)
        {
            if(cont!=(x*y)/2){
                matrix[i][j]=1; //set this for worst case (test)
                //matrix[i][j]=rand()%2;
            }
            else{
                matrix[i][j]=0;
            }
            if(matrix[i][j])
                cont++;
        }

    this->scale_h = scale_h;
    this->scale_w = scale_w;
    this->scale_x = scale_x;
    this->scale_y = scale_y;
    this->buffer = buffer;
    this->display = display;
}

void nextChild() 
{ 
    int** next_matrix = new int*[screenHeight];
    for(int i=0;i<screenHeight;i++)
        next_matrix[i]=new int[screenWidth];

    t_Start = omp_get_wtime();		
    #pragma omp parallel for schedule(static)
    for (int l = 1; l < y - 1; l++) 
    {
        for (int m = 1; m < x - 1; m++) 
        { 
            int alive_neighbours = 0;
            for (int i = -1; i <= 1; i++) 
                for (int j = -1; j <= 1; j++) 
                    alive_neighbours += matrix[l + i][m + j];

            alive_neighbours -= matrix[l][m];

            if ((matrix[l][m] == 1) && (alive_neighbours < 2)) 
                next_matrix[l][m] = 0;   
            else if ((matrix[l][m] == 1) && (alive_neighbours > 3)) 
                next_matrix[l][m] = 0;   
            else if ((matrix[l][m] == 0) && (alive_neighbours == 3)) 
                next_matrix[l][m] = 1; 
            else
                next_matrix[l][m] = matrix[l][m]; 
        }
    }

    t_Stop = omp_get_wtime() - t_Start;		
    //cout << "Parallel Execution Time: " << t_Stop << " seconds." << endl;
    cout<<t_Stop;

    for(int i=0;i<x;i++)
        delete[] matrix[i];

    delete[] matrix;
    matrix=next_matrix; 
}

void showMap ()
{   
    al_set_target_bitmap(buffer);
    al_clear_to_color(al_map_rgb( 0x42, 0x42, 0x42));

    nextChild();
    drawMap();
    drawPopulation();

    al_set_target_backbuffer(this->display);
    al_draw_scaled_bitmap(buffer, 0, 0,(x*pixel) ,(y*pixel), scale_x, scale_y, scale_w, scale_h, 0);
}

void drawMap ()
{
        population=0;
        change_color=0;

        for(int i=0;i<y;i++)
        for(int j=0;j<x;j++){
            if(matrix[i][j]==1 && change_color==0){
                al_draw_filled_rectangle(j*pixel-8,i*pixel-8,j*pixel+8,i*pixel+8,al_map_rgb(0x29,0x62,0xFF));
                change_color++;
                population++;
            }
            else if(matrix[i][j]==1){
                al_draw_filled_rectangle(j*pixel-8,i*pixel-8,j*pixel+8,i*pixel+8,al_map_rgb(0xFF,0x02,0x66));
                change_color=0;
                population++;
            }
        }
}

void drawPopulation (){
    al_draw_textf(al_create_builtin_font(), al_map_rgb(0,0,0) , 780 , 10 , ALLEGRO_ALIGN_CENTRE, "Population: %d" , population);
    al_flip_display();
}

~GamingOfLife(){}

};
#endif