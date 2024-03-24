#include "SDL2/SDL.h"

const int STARS_NUM = 32;

class Stars{
    public:
        int speed_x;
        int speed_y;

        bool color_sign;
        int color;
        int speed_color;

        int random;

        SDL_Point stars_list[STARS_NUM];

        Stars(int speed_x, int speed_y, int speed_color, int max_x, int max_y);
        void update(int min_x, int min_y, int max_x, int max_y);
        void render(SDL_Renderer *renderer);

        int rand();
};