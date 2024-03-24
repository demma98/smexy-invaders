#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <list>
#include <stdio.h>

#include "ship.h"
#include "stars.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360

#define SHIP_MAX_SPEED 3

using namespace std;

void init(list<Stars> *stars, list<Ship> *ships, list<Bullet> *bullets, int ships_num, SDL_Texture *textures[]);
void init_level(list<Stars> *stars, list<Ship> *ships, int ships_number, SDL_Texture *textures[]);

void update(bool *running, list<Stars> *stars, list<Ship> *ships, list<Bullet> *bullets, int *ships_num, SDL_Texture *textures[]);
void update_frame(list<Stars> *stars, list<Ship> *ships, list<Bullet> *bullets, SDL_Texture *enemy_texture);
void render(SDL_Renderer *renderer, int color, list<Stars> *stars, list<Ship> *ships, list<Bullet> *bullets);

int main(){
    bool running;

    //open window
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("smexy invaders", 64, 64, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    //sprites
    SDL_Texture *player_texture = IMG_LoadTexture(renderer, "img/player.png");
    SDL_Texture *enemy_texture = IMG_LoadTexture(renderer, "img/enemy.png");

    int enemy_texture_t = 0;
    int enemy_texture_c = 0;
    SDL_Texture *enemy_textures[] = {IMG_LoadTexture(renderer, "img/enemy 1.png"), IMG_LoadTexture(renderer, "img/enemy 2.png"), IMG_LoadTexture(renderer, "img/enemy 3.png")};

    SDL_Texture *textures[] = {player_texture, enemy_textures[0]};

    uint32_t time_old = SDL_GetTicks(), time_new;

    list<Stars> stars;
    list<Ship> ships;
    list<Bullet> bullets;
    
    int ships_num = 1;

    int color = 0;
    SDL_SetRenderDrawColor(renderer, color, color, color, 255);

    init(&stars, &ships, &bullets, ships_num, textures);
    
    running = true;
    while(running){
        time_new = SDL_GetTicks();
        update(&running, &stars, &ships, &bullets, &ships_num, textures);
        if(time_new - time_old > 1000/60){
            time_old = SDL_GetTicks();
            update_frame(&stars, &ships, &bullets, enemy_texture);
            enemy_texture_t++;
            if(enemy_texture_t == 10){
                enemy_texture_c++;
                if(enemy_texture_c == 3)
                    enemy_texture_c = 0;
                enemy_texture = enemy_textures[enemy_texture_c];
                enemy_texture_t = 0;
            }
        }
        render(renderer, color, &stars, &ships, &bullets);
    }

    return 0;
}


void init(list<Stars> *stars, list<Ship> *ships, list<Bullet> *bullets, int ships_num, SDL_Texture *textures[]){
    stars->clear();
    Stars temp_stars = Stars(0, 2, 3, SCREEN_WIDTH, SCREEN_HEIGHT);
    stars->push_back(temp_stars);
    temp_stars = Stars(0, 3, 2, SCREEN_WIDTH, SCREEN_HEIGHT);
    stars->push_back(temp_stars);

    ships->clear();
    Ship temp_ship = Ship(SCREEN_WIDTH/2, (SCREEN_HEIGHT*3)/4, 0, 0, 0, textures[0]);
    ships->push_back(temp_ship);
    init_level(stars, ships, ships_num, textures);

    bullets->clear();
}


void init_level(list<Stars> *stars, list<Ship> *ships, int ships_num, SDL_Texture *textures[]){
    for(int i = 0; i < ships_num; i++){
        Ship temp_ship = Ship(SCREEN_WIDTH, 0, -2, 0, 1, textures[1]);
        temp_ship.y = stars->begin()->rand() % SCREEN_HEIGHT;
        if(temp_ship.y < 0)
            temp_ship.y *= -1;
        if(i % 2){
            temp_ship.speed_x *= -1;
            temp_ship.x -= SCREEN_WIDTH + temp_ship.hit.w;
        }
        ships->push_back(temp_ship);
    }
}

void update(bool *running, list<Stars> *stars, list<Ship> *ships, list<Bullet> *bullets, int *ships_num, SDL_Texture *textures[]){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                *running = false;
                break;
            case SDLK_r:
                init(stars, ships, bullets, *ships_num, textures);
            }
            break;
        case SDL_QUIT:
            *running = false;
            break;
        }
    }
    if(ships->begin()->team == 0)
        if(ships->size() == 1){
            *ships_num += 1;
            init_level(stars, ships, *ships_num, textures);
        }
}

void update_frame(list<Stars> *stars, list<Ship> *ships, list<Bullet> *bullets, SDL_Texture *enemy_texture){

    for(list<Stars>::iterator i = stars->begin(); i != stars->end(); i++)
        i->update(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    list<Bullet>::iterator b = bullets->begin();
    while(b != bullets->end()){
        if(b->dead)
            b = bullets->erase(b);
        else{
            b->update(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            b++;
        }
    }

    SDL_Rect player_hit = ships->begin()->hit;
    const Uint8* state = SDL_GetKeyboardState(NULL);
    list<Ship>::iterator s = ships->begin();
    while(s != ships->end()){
        if(s->dead)
            s = ships->erase(s);
        else{
            for(list<Bullet>::iterator i = bullets->begin(); i != bullets->end() && !s->dead; i++){
                if(s->team != i->team){
                    int x = i->x;
                    int y1 = i->y;
                    int y2 = i->y + i->length;
                    if(SDL_IntersectRectAndLine(&s->hit, &x, &y1, &x, &y2)){
                            s->dead = true;
                            i->dead = true;
                        }
                }
            }

            s->update(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

            if(s->team){
                //enemy
                if(!s->speed_x && !s->speed_y){
                    s->speed_x = stars->begin()->rand() % SHIP_MAX_SPEED;
                    s->speed_y = stars->begin()->rand() % SHIP_MAX_SPEED;
                }
                if(s->x + s->hit.w == SCREEN_WIDTH)
                    if(s->speed_x > 0){
                        s->speed_x = stars->begin()->rand() % SHIP_MAX_SPEED;
                        if(s->speed_y == 0)
                            s->speed_y = stars->begin()->rand() % SHIP_MAX_SPEED;
                    }
                if(!s->x)
                    if(s->speed_x < 0){
                        s->speed_x = stars->begin()->rand() % SHIP_MAX_SPEED;
                        if(s->speed_y == 0)
                            s->speed_y = stars->begin()->rand() % SHIP_MAX_SPEED;
                    }
                if(s->y + s->hit.h == SCREEN_HEIGHT)
                    if(s->speed_y > 0){
                        s->speed_y = stars->begin()->rand() % SHIP_MAX_SPEED;
                        if(s->speed_x == 0)
                            s->speed_x = stars->begin()->rand() % SHIP_MAX_SPEED;
                    }
                if(!s->y)
                    if(s->speed_y < 0){
                        s->speed_y = stars->begin()->rand() % SHIP_MAX_SPEED;
                        if(s->speed_x == 0)
                            s->speed_x = stars->begin()->rand() % SHIP_MAX_SPEED;
                    }
                
                if(s->shoot_c == 0){
                    s->shoot_c = stars->begin()->rand() % 50;
                    if(s->shoot_c < 0)
                        s->shoot_c *= -1;
                    s->shoot_c += 20;
                }
                else if(s->shoot_c == 1){
                    Bullet temp_bullet = Bullet(s->x + (s->hit.w / 2), s->y, 0, 4, 1);
                    if(ships->begin()->y < s->y)
                        temp_bullet.speed_y *= -1;
                    bullets->push_back(temp_bullet);
                }

                if(ships->begin()->team == 0)
                    if(SDL_HasIntersection(&player_hit, &s->hit))
                        ships->begin()->dead = true;

                s->texture = enemy_texture;
            }
            else{
                //player
                if(state[SDL_SCANCODE_UP])
                    s->speed_y = -3;
                else if(state[SDL_SCANCODE_DOWN])
                    s->speed_y = 3;
                else
                    s->speed_y = 0;
                if(state[SDL_SCANCODE_LEFT])
                    s->speed_x = -3;
                else if(state[SDL_SCANCODE_RIGHT])
                    s->speed_x = 3;
                else
                    s->speed_x = 0;

                if(state[SDL_SCANCODE_SPACE])
                    if(s->shoot_c == 0){
                        Bullet temp_bullet = Bullet(s->x + (s->hit.w / 2), s->y, 0, -4.0, 0);
                        bullets->push_back(temp_bullet);
                        s->shoot_c = 45;
                    }
            }
        }
        s++;
    }
    
}

void render(SDL_Renderer *renderer, int color, list<Stars> *stars, list<Ship> *ships, list<Bullet> *bullets){
    SDL_SetRenderDrawColor(renderer, color, color, color, 255);
    SDL_RenderClear(renderer); //clear screen

    for(list<Stars>::iterator i = stars->begin(); i != stars->end(); i++){
        SDL_SetRenderDrawColor(renderer, i->color, i->color, i->color, 255);
        i->render(renderer);
    }

    for(list<Ship>::iterator i = ships->begin(); i != ships->end(); i++)
        i->render(renderer);

    for(list<Bullet>::iterator i = bullets->begin(); i != bullets->end(); i++)
        i->render(renderer);
    SDL_RenderPresent(renderer);
}