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
    this->level     = 1;
    this->PAUSED    = false;
    this->RESET     = false;
    this->COMPLETE  = false;
    this->scrolling = true;
    this->offset.x  = -182;
    this->offset.y  = 0;
    this->tiers     = 6;
    this->score     = 0;
    this->pickup    = -1;
    this->factor    = 2;
    this->lives     = 3;
    this->mapper    = * new Mapper();
    this->controls  = * new Control();
    this->sound     = * new Sound();
    this->timer     = * new Timer();
    this->controls.lock = false;
    this->state = "START_SCREEN";
}

void Game::update(){
    player.update();
    player.move();
    physics.update();

    for(int i=0; i<enemies.size(); i++){
        enemies[i].update();
    }

    if(collected.size()==10){
        complete();
    }

    for(int i=0; i<doors.size(); i++){
        if(doors[i].type=="magic" && doors[i].wave.active){
            doors[i].wave.update();
        }
    }
}

bool Game::interval(){
    int ms;

    current_time = SDL_GetTicks();
    ms = current_time - last_time;

    if( ms < ms_per_frame )
        return false;
    return true;
}

bool Game::delay(int delay, Uint32 start){
    Uint32 current = SDL_GetTicks();

    if(current - start < delay){
        return true;
    }
    return false;
}

// void Game::clock(){
//     timestamp = SDL_GetTicks();
// }

void Game::renderObjects(){
    for(int i=0; i<trampolines.size(); i++){
        if(trampolines[i].assigned){
            trampolines[i].render();
        }
    }
    for(int i=0; i<doors.size(); i++){
        doors[i].render();
        if(doors[i].type=="magic"){
            doors[i].wave.render();
            // doors[i].wave.collider->debug();
        }
    }
    for(int i=0; i<items.size(); i++){
        items[i].render();
    }
    for(int i=0; i<enemies.size(); i++){
        // enemies[i].collider->debug();
        enemies[i].render();
    }
}

void Game::render(){
    if(!interval()){ return; }

    mapper.render();
    renderObjects();
    // player.collider->debug();
    player.render();

    last_time = current_time;
}

void Game::loop(){
    controls.input();

    if(!PAUSED){
        update();

        if(!RESET && !COMPLETE)
            render();
    }
}

void Game::setup(){
    int key;
    bool found;
    // vector<int>::iterator it;

    for(int i=0; i<LEVEL_SIZE; i++){
        key = data.interactive[level-1][i];
        found = find(skip.begin(), skip.end(), i)!=skip.end();

        if(key>1 && !found){
            switch(key){
                case TRAMPOLINE:
                    for(int t=0; t<trampolines.size(); t++){
                        if(!trampolines[t].assigned){
                            trampolines[t].assign(i);

                            skip.push_back(i);
                            skip.push_back(i+1);
                            skip.push_back(i+2);
                            break;
                        }
                    }
                    break;
                case DOOR_LEFT:
                case DOOR_RIGHT:
                case MAGIC_DOOR_L:
                case MAGIC_DOOR_R:
                    for(int d=0; d<doors.size(); d++){
                        if(!doors[d].assigned && doors[d].id==key-3){
                            doors[d].assign(i);
                            skip.push_back(i);
                            break;
                        }
                    }
                    break;
                case RADIO:
                case TV:
                case COMPUTER:
                case PAINTING:
                case SAFE:
                    skip.push_back(i);
                    skip.push_back(i+1);
                    skip.push_back(i+LEVEL_WIDTH);
                    skip.push_back(i+LEVEL_WIDTH+1);

                    Item item = Item(key-7);
                    item.assign(i);
                    items.push_back(item);
                    break;
            }
        }
    }

    skip.clear();
}

void Game::start(){
    sound.init();
    sound.music.load("theme", AUDIO_THEME);
    sound.effects.load("dead", AUDIO_DEAD);
    sound.effects.load("clear", AUDIO_CLEAR);
    sound.effects.load("item", AUDIO_ITEM);
    sound.effects.load("wave", AUDIO_WAVE);
    sound.effects.load("trampoline", AUDIO_JUMP);

    sound.music.loop("theme");

    this->PAUSED = false;
    this->state="RUNNING";
}

void Game::init(int w, int h){
    this->stage = * new Stage(w, h);
    this->center.x = this->stage.width/2;
    this->center.y = this->stage.height/2;

    this->stage.bottom -= DIM*SCALE;

    physics = * new Physics();
    player.gravity = physics.gravity(0.125, 0);
    player.gravitation = true;

    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );
    SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);

    this->PAUSED = true;
    this->mapper.init();

    for(int i=0; i<8; i++){
        Trampoline trampoline = * new Trampoline();
        trampolines.push_back(trampoline);
    }

    for(int i=0; i<4; i++){
        for(int j=0; j<6; j++){
            Door door = * new Door(i);
            doors.push_back(door);
        }
    }

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );

    for(int i=0; i<data.enemies[level-1]; i++){
        Enemy mewkie = * new Enemy();
        mewkie.init(data.spawn[i][0], data.spawn[i][1]);
        mewkie.gravity = physics.gravity(0.125, 0);
        mewkie.id = i;
        enemies.push_back(mewkie);
    }

    for(int i=0; i<cached.size(); i++){ cout << cached[i] << " "; }
    cout<<endl;
}

int Game::trampoline(){
    for(int t=0; t<trampolines.size(); t++){
        if(trampolines[t].active){
            return t;
        }
    }
    return -1;
}

void Game::pause(){
    PAUSED = !PAUSED;
    if(PAUSED){
        sound.music.pause();
        sound.effects.pause();
    }
    else{
        sound.music.resume();
        sound.effects.resume();
    }
}

void Game::clear(){
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );
}

void Game::complete(){
    sound.music.stop();

    if(!COMPLETE)
        sound.effects.play("clear");

    COMPLETE = true;

    if(game.delay(2000, timestamp)){
        return;
    }

    collected.clear();
    // timeout = timer.start(2000);
    restage();
}

void Game::restage(){
    clear();

    this->PAUSED    = true;
    this->level    += 1;
    this->offset.x  = -182;
    this->offset.y  = 0;
    this->scrolling = true;
    this->pickup    = -1;
    this->factor    = 2;

    for(int t=0; t<trampolines.size(); t++){
        trampolines[t].assigned = false;
        trampolines[t].reset();
        trampolines[t].group.clear();
    }

    for(int d=0; d<doors.size(); d++){
        doors[d].cleanup();
    }

    for(int i=0; i<data.enemies[level-1]; i++){
        enemies[i].reset(data.spawn[i][0], data.spawn[i][1]);
    }

    setup();

    player.reset(580, 480);
    controls.lock = false;

    this->PAUSED   = false;
    this->RESET    = false;
    this->COMPLETE = false;

    game.sound.music.loop("theme");
}

void Game::restart(){
    clear();

    player.reset(580, 480);
    controls.lock = false;

    for(int i=0; i<enemies.size(); i++){
        enemies[i].reset(data.spawn[i][0], data.spawn[i][1]);
    }

    for(int t=0; t<trampolines.size(); t++){
        trampolines[t].reset();
    }

    this->PAUSED    = false;
    this->RESET     = false;
    this->COMPLETE  = false;
    this->scrolling = true;
    this->offset.x  = -182;
    this->offset.y  = 0;
    this->pickup    = -1;

    game.sound.music.loop("theme");
}

void Game::reset(){
    level = 0;
    restage();
}