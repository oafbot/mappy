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
    // this->timer    =   Timer();
    this->offset.x = -182;
    this->offset.y = 0;
    this->scrolling = true;
    this->controls.lock = false;
}

void Game::update(){
    player.update();
    player.move();
    physics.update();
}

bool Game::delay(){
    int ms;

    current_time = SDL_GetTicks();
    ms = current_time - last_time;

    if( ms < ms_per_frame ){
        return true;
    }
    return false;
}

void Game::renderObjects(){
    for(int i=0; i<objects["trampoline"].size(); i++){
        if(objects["trampoline"][i].assigned){
            objects["trampoline"][i].render();
            // cout << objects["trampoline"][i]->frame << "\n";
            // cout << objects["trampoline"][i]->state << "\n";
        }
    }
}

void Game::render(){
    if(delay()){
        return;
    };

    mapper.render();
    // objects.render();
    renderObjects();
    player.render();

    last_time = current_time;
}

void Game::setup(){
    for(int i=0; i<LEVEL_SIZE; i++) {
        int key = data.interactive[level-1][i];
        if(key==2){
            for(int t=0; t<objects["trampoline"].size(); t++){
                if(!objects["trampoline"][t].assigned){
                    objects["trampoline"][t].assigned = true;
                    objects["trampoline"][t].assign(i);
                    break;
                }
            }
            i = i + 2;
        }
    }
}

void Game::init(int w, int h){
    this->stage = * new Stage(w, h);
    this->center.x = this->stage.width/2;
    this->center.y = this->stage.height/2;

    this->stage.bottom -= DIM*SCALE;

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

    for(int i=0; i<BYTE; i++){
        Trampoline trampoline = Trampoline();
        trampoline.compile();
        objects["trampoline"].push_back(trampoline);
    }
}

int Game::trampoline(){
    for(int t=0; t<objects["trampoline"].size(); t++){
        if(objects["trampoline"][t].active){
            return t;
        }
    }
    return -1;
}