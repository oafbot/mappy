#include "game.hpp"
using namespace std;

Stage::Stage(int w, int h){
    this->left   = 0;
    this->right  = w;
    this->top    = 0;
    this->bottom = h;
    this->width  = w;
    this->height = h;
    this->full_width = w+128;
    this->full_height = h;
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
    this->tiers = 6;
    this->score = 0;
    this->pickup = -1;
    this->factor = 2;
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
    for(int i=0; i<items.size(); i++){
        items[i].render();
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
    vector<int>::iterator it;

    for(int i=0; i<LEVEL_SIZE; i++){
        int key = data.interactive[level-1][i];

        it = find(skip.begin(), skip.end(), i);

        switch(key){
            case TRAMPOLINE:
                if(it==skip.end()){
                    skip.push_back(i);
                    skip.push_back(i+1);
                    skip.push_back(i+2);

                    for(int t=0; t<objects["trampoline"].size(); t++){
                        if(!objects["trampoline"][t].assigned){
                            objects["trampoline"][t].assigned = true;
                            objects["trampoline"][t].assign(i);
                            break;
                        }
                    }
                }
                break;
            case RADIO:
            case TV:
            case COMPUTER:
            case PAINTING:
            case SAFE:
                if(it==skip.end()){
                    skip.push_back(i);
                    skip.push_back(i+1);
                    skip.push_back(i+LEVEL_WIDTH);
                    skip.push_back(i+LEVEL_WIDTH+1);

                    Item item = Item(key-7);
                    item.assign(i);
                    items.push_back(item);
                }
                break;
        }
    }
}

void Game::init(int w, int h){
    this->stage = * new Stage(w, h);
    this->center.x = this->stage.width/2;
    this->center.y = this->stage.height/2;

    this->stage.bottom -= DIM*SCALE;

    physics = * new Physics();
    player.gravity = physics.gravity(0.125, 0);
    player.gravitation = true;

    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);

    this->mapper.init();

    for(int i=0; i<BYTE; i++){
        Trampoline trampoline = Trampoline();
        objects["trampoline"].push_back(trampoline);
    }

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );

    for(int i=0; i<data.enemies[level-1]; i++){
        Enemy mewkie = * new Enemy();
        mewkie.init(data.spawn[i][0], data.spawn[i][1]);
        mewkie.gravity = physics.gravity(0.125, 0);
        enemies.push_back(mewkie);
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