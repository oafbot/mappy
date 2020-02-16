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

    for(int i=0; i<enemies.size(); i++){
        enemies[i].update();
    }
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

bool Game::delay(int delay, Uint32 start){
    Uint32 current = SDL_GetTicks();

    if(current - start < delay){
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
    for(int i=0; i<enemies.size(); i++){
        enemies[i].render();
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
    Gravity<Player> g = physics.gravity(&player, 0.125, 0);
    physics.dropable.push_back(g);

    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);

    // SDL_RenderPresent(renderer);
    this->mapper.init();

    for(int i=0; i<BYTE; i++){
        Trampoline trampoline = Trampoline();
        trampoline.compile();
        objects["trampoline"].push_back(trampoline);
    }

    Enemy mewkie = * new Enemy();
    mewkie.compile();
    mewkie.init(770, 352);
    // mewkie.init(580, 480);
    enemies.push_back(mewkie);

    mewkie = * new Enemy();
    mewkie.compile();
    mewkie.init(80, 160);
    // mewkie.init(208, 224);
    mewkie.state = "right";
    enemies.push_back(mewkie);


    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );
    // for(int i=0; i<9; i++){
    //     Enemy mewkie = * new Enemy();
    //     mewkie.compile();
    //     mewkie.init(i*32, 10);
    //     enemies.push_back(mewkie);
    // }
}

int Game::trampoline(){
    for(int t=0; t<objects["trampoline"].size(); t++){
        if(objects["trampoline"][t].active){
            return t;
        }
    }
    return -1;
}