#include "ship.h"

using namespace std;

Ship :: Ship (double x, double y, double speed_x, double speed_y, int team, SDL_Texture *texture) {
    this->x = x;
    this->y = y;
    this->speed_x = speed_x;
    this->speed_y = speed_y;

    this->team = team;

    this->texture = texture;
    SDL_QueryTexture(this->texture, NULL, NULL, &hit.w, &hit.h);
    hit.x = x;
    hit.y = y;

    shoot_c = 0;
    dead = false;
}

void Ship :: update(int min_x, int min_y, int max_x, int max_y){
    x += speed_x;
    y += speed_y;

    if(x < min_x)
        x = min_x;
    else if(x > max_x - hit.w)
        x = max_x - hit.w;
    
    if(y < min_y)
        y = min_y;
    else if(y > max_y - hit.h)
        y = max_y - hit.h;
    
    if(shoot_c > 0)
        shoot_c--;
}

void Ship :: render(SDL_Renderer *renderer){
    hit.x = (int) x;
    hit.y = (int) y;
    SDL_RenderCopy(renderer, texture, NULL, &hit);
}


Bullet :: Bullet(double x, double y, double speed_x, double speed_y, int team){
    this->x = x;
    this->y = y;
    this->speed_x = speed_x;
    this->speed_y = speed_y;
    this->team = team;

    length = 10;
    color = 192;
    dead = false;
}

void Bullet :: update(int min_x, int min_y, int max_x, int max_y){
    x += speed_x;
    y += speed_y;

    if(color <= 0)
        color_sign = true;
    else if(color >= 255)
        color_sign = false;
    
    if(color_sign)
        color += 2;
    else
        color -= 2;
    
    dead = x < min_x || x > max_x || y + length < min_y || y > max_y;
}

void Bullet :: render(SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(renderer, 255, 255, color, 255);
    SDL_RenderDrawLine(renderer, x, y, x, y + length);
}