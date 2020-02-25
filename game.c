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
    this->controls.lock = false;
    this->state="START_SCREEN";
}

void Game::update(){
    player.update();
    player.move();
    physics.update();

    for(int i=0; i<enemies.size(); i++){
        enemies[i].update();
    }

    if(collected.size()==10){
        COMPLETE = true;
        complete();
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
    for(int i=0; i<trampolines.size(); i++){
        if(trampolines[i].assigned){
            trampolines[i].render();
        }
    }
    for(int i=0; i<doors.size(); i++){
        if(doors[i].assigned)
            doors[i].render();
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
    if(delay()){
        return;
    };

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
    vector<int>::iterator it;

    for(int i=0; i<LEVEL_SIZE; i++){
        int key = data.interactive[level-1][i];
        it = find(skip.begin(), skip.end(), i);

        if(key>1 && it==skip.end()){
            switch(key){
                case TRAMPOLINE:
                    for(int t=0; t<trampolines.size(); t++){
                        if(!trampolines[t].assigned){
                            trampolines[t].assigned = true;
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
                            doors[d].assigned = true;
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
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024 ) < 0 ){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(2);
    }

    Mix_Music *music;
    music = Mix_LoadMUS("audio/theme.wav");
    Mix_PlayMusic(music, -1);
    // sounds["theme"] = new Audio(THEME, false);
    // device_id = SDL_OpenAudioDevice(NULL, 0, &(sounds["theme"]->spec), NULL, 0);

    // if(device_id == 0) {
    //     printf("Failed to open audio: %s\n", SDL_GetError());
    // }
    // SDL_PauseAudioDevice(device_id, 0);
    // sounds["theme"]->play();

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
        enemies.push_back(mewkie);
    }
}

int Game::trampoline(){
    for(int t=0; t<trampolines.size(); t++){
        if(trampolines[t].active){
            return t;
        }
    }
    return -1;
}

void Game::clear(){
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );
}

void Game::complete(){
    if(game.delay(2000, timestamp)){
        return;
    }
    collected.clear();
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
        doors[d].assigned = false;
        doors[d].open     = false;
        doors[d].state    = "closed";
        doors[d].group.clear();
    }

    for(int i=0; i<data.enemies[level-1]; i++){
        enemies[i].reset(data.spawn[i][0], data.spawn[i][1]);
    }

    setup();

    player.reset(580, 480);
    controls.lock = false;

    this->PAUSED    = false;
    this->RESET     = false;
    this->COMPLETE  = false;
}

void Game::restart(){
    clear();
    this->PAUSED    = false;
    this->RESET     = false;
    this->COMPLETE  = false;
    this->scrolling = true;
    this->offset.x  = -182;
    this->offset.y  = 0;
    this->pickup    = -1;

    player.reset(580, 480);
    controls.lock = false;

    for(int i=0; i<enemies.size(); i++){
        enemies[i].reset(data.spawn[i][0], data.spawn[i][1]);
    }

    for(int t=0; t<trampolines.size(); t++){
        trampolines[t].reset();
    }
}

void Game::reset(){
    clear();

    for(int t=0; t<trampolines.size(); t++){
        trampolines[t].assigned = false;
    }

    for(int d=0; d<doors.size(); d++){
        doors[d].assigned = false;
        doors[d].state    = "closed";
        doors[d].open     = false;
    }

    this->skip.clear();

    setup();

    clear();

    player.reset(580, 480);
    controls.lock   = false;

    this->level     = 1;
    this->PAUSED    = false;
    this->RESET     = false;
    this->COMPLETE  = false;
    this->offset.x  = -182;
    this->offset.y  = 0;
    this->scrolling = true;
    this->score     = 0;
    this->pickup    = -1;
    this->lives     = 3;
    this->factor    = 2;

    for(int i=0; i<data.enemies[level-1]; i++){
        enemies[i].reset(data.spawn[i][0], data.spawn[i][1]);
    }

}