#include "game.hpp"
using namespace std;

Stage::Stage(int w, int h){
    this->left   = 0;
    this->right  = w;
    this->top    = 0;
    this->bottom = h;
    this->width  = w;
    this->height = h;
}

Game::Game() : stage(0,0){
    this->level = 1;
    this->PAUSED = false;
    this->controls = * new Control();
    this->mapper   = * new Mapper();
}

void Game::update(){
    player.update();
    player.move();
    physics.update();
}

void Game::render(){
    int ms;

    current_time = SDL_GetTicks();
    ms = current_time - last_time;

    if( ms < ms_per_frame ){
        return;
    }

    mapper.render();
    player.render();

    last_time = current_time;
}

void Game::init(int w, int h){
    this->stage = * new Stage(w, h);

    physics = * new Physics();
    Gravity g = physics.gravity(0.125, 0);/*0.125*/
    g.bind(&player);
    physics.dropable.push_back(g);

    SDL_Init( SDL_INIT_VIDEO );
    SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );
    // SDL_RenderPresent(renderer);
    this->mapper.init();
}