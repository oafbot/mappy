#include "game.hpp"
using namespace std;

Sprite::Sprite(){
    this->x = x-this->width/2;
    this->y = y-this->height/2;

    this->width  = DIM;
    this->height = DIM;

    this->gravitation = true;
    this->falling  = false;
    this->bouncing = false;
};


// void Sprite::render(){
//     printf("Sprite.render\n");
// }

void Sprite::render(){
    string color;
    SDL_Rect r;

    int w = SCALE;
    int row;
    int col;
    int bit;
    int alpha;
    int ms;

    current_time = SDL_GetTicks();
    ms = current_time - last_time;

    if( ms < ms_per_frame ){
        return;
    }

    if(game.controls.key_down){
        if( current_frame >= FRAME_COUNT ) {
            // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            // SDL_RenderClear( renderer );
            current_frame = 0;
        }
        else{
            current_frame++;
        }
    }

    this->frame = current_frame;

    last_time = current_time;

    for(int i=0; i < sprites[0].size(); i++) {
        bit = current_frame%2 ? sprites[1][i] : sprites[0][i];

        alpha = palette[bit]=="NULL" ? 0 : 255;
        color = palette[bit]=="NULL" ? "#000000" : palette[bit];
        SDL_Color c = hex2sdl(color);

        col = i % DIM;
        row = floor(i / DIM);

        r.x = this->x + w*col;
        r.y = this->y + w*row;
        r.w = w;
        r.h = w;
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, alpha);
        SDL_RenderFillRect(renderer, &r );
    }
}