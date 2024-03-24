#include "SDL2/SDL.h"

class Ship{
    public:
        SDL_Rect hit;
        double x;
        double y;
        double speed_x;
        double speed_y;

        int team;
        int shoot_c;

        bool dead;
        int temp;

        SDL_Texture *texture;

        Ship(double x, double y, double speed_x, double speed_y, int team, SDL_Texture *texture);
        void update(int min_x, int min_y, int max_x, int max_y);
        void render(SDL_Renderer *renderer);
};

class Bullet{
    public:
        double x;
        double y;
        double speed_x;
        double speed_y;

        int color;
        bool color_sign;

        int team;

        int length;

        bool dead;

        Bullet(double x, double y, double speed_x, double speed_y, int team);
        void update(int min_x, int min_y, int max_x, int max_y);
        void render(SDL_Renderer *renderer);
};