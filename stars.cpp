#include "stars.h"

using namespace std;

Stars :: Stars(int speed_x, int speed_y, int speed_color, int max_x, int max_y){
    this->speed_x = speed_x;
    this->speed_y = speed_y;

    this->speed_color = speed_color;

    for(SDL_Point *i = stars_list; i - stars_list < STARS_NUM; i++){
        for(int j = 0; j < i->x % 32; j++)
            rand();

        i->x = rand() % max_x;

        for(int j = 0; j < i->y % 32; j++)
            rand();

        i->y = rand() % max_y;
    }

    color = 0;
}

void Stars :: update(int min_x, int min_y, int max_x, int max_y){
    if(color <= 0)
        color_sign = true;
    else if(color >= 255)
        color_sign = false;
    
    if(color_sign)
        color += speed_color;
    else
        color -= speed_color;

    for(SDL_Point *i = stars_list; i - stars_list < STARS_NUM; i++){
        bool x_test = false, y_test = false;;
        i->x += speed_x;
        if(i->x < min_x){
            i->x += min_x;
            x_test = true;
        }
        else if(i->x > max_x){
            i->x -= max_x;
            x_test = true;
        }
        if(x_test)
            i->y = rand() % max_y;
        
        i->y += speed_y;
        if(i->y < min_y){
            i->y += min_y;
            y_test = true;
        }
        else if(i->y > max_y){
            i->y -= max_y;
            y_test = true;
        }
        if(y_test)
            i->x = rand() % max_x;
        
    }
}

void Stars :: render(SDL_Renderer *renderer){
    for(SDL_Point *i = stars_list; i - stars_list < STARS_NUM; i++)
        SDL_RenderDrawPoint(renderer, i->x, i->y);
}

int Stars :: rand(){
    random *= 5;
    random++;
    return random;
}