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
    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );

    this->level     = 1;
    this->PAUSED    = false;
    this->RESET     = false;
    this->COMPLETE  = false;
    this->scrolling = true;
    this->offset.x  = -182;
    this->offset.y  = 0;
    this->score     = 0;
    this->pickup    = -1;
    this->factor    = 2;
    this->lives     = 3;
    this->tiers     = FLOORS;
    this->mapper    = * new Mapper();
    this->controls  = * new Control();
    this->sound     = * new Sound();
    this->timer     = * new Timer();
    this->text      = * new Text();
    this->controls.lock = false;
    this->state = "LOADING";
}

void Game::update(){
    if(mode==PLAY){
        if(collected.size()==10){
            complete();
        }
        else{
            if(!sound.music.playing() && music) playBGM();

            player.update();
            player.move();
            physics.update();
        }

        if(state!="BONUS_ROUND"){
            for(int i=0; i<enemies.size(); i++){
                if(!RESET && !COMPLETE){
                    enemies[i].update();
                }
            }

            goro.update();

            for(int i=0; i<doors.size(); i++){
                if(doors[i].type=="magic" && doors[i].wave.active){
                    doors[i].wave.update();
                }
            }
        }
        else{
            music = OFF;
            if(!sound.music.playing()){
                complete();
            }
        }
    }
    else if(state=="CLEAR_SCREEN"){
        interlude();
    }
    else if(state=="BONUS_RESULTS"){
        results();
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

    // goro.collider->debug();
    goro.render();

    for(int i=0; i<items.size(); i++){
        items[i].render();
    }
    for(int i=0; i<enemies.size(); i++){
        // enemies[i].collider->debug();
        enemies[i].render();
    }

    if(state=="BONUS_ROUND"){
        for(int i=0; i<balloons.size(); i++){
            balloons[i].render();
        }
    }
}

void Game::render(){
    if(!interval()){ return; }

    mapper.render("background");
    renderObjects();
    // player.collider->debug();
    player.render();
    mapper.render("foreground");
    points.render();

    if(state!="BONUS_ROUND")
        game.text.score(game.score);

    last_time = current_time;
}

void Game::loop(){
    controls.input();

    if(!PAUSED){
        update();

        if(!RESET && !COMPLETE){
            render();
        }
        else{
            last_time = current_time;
        }
    }
    else if(state=="LOADING"){
        if(timer.done(timeout)){
            state = "START_SCREEN";
        }
    }
    else if(state=="START_SCREEN"){
        demo.animate();
    }

    if(state=="START_DELAY"){
        if(timer.done(timeout)){
            state = isBonusRound(level) ? "BONUS_ROUND" : "RUNNING";
            timer.reset(timeout);

            this->mode  = PLAY;
            this->music = ON;
            PAUSED = false;
        }

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
                case BALLOON:
                    for(int b=0; b<balloons.size(); b++){
                        if(!balloons[b].assigned){
                            balloons[b].assign(i);

                            skip.push_back(i);
                            skip.push_back(i+1);
                            skip.push_back(i+LEVEL_WIDTH);
                            skip.push_back(i+LEVEL_WIDTH+1);
                            break;
                        }
                    }
                    break;
                case RADIO:
                case TV:
                case COMPUTER:
                case PAINTING:
                case SAFE:
                    for(int m=0; m<items.size(); m++){
                        if(items[m].id==key-7 && !items[m].assigned){
                            skip.push_back(i);
                            skip.push_back(i+1);
                            skip.push_back(i+LEVEL_WIDTH);
                            skip.push_back(i+LEVEL_WIDTH+1);

                            // Item item = Item(key-7);
                            items[m].assign(i);
                            break;
                        }
                    }
                    break;
            }
        }
    }

    skip.clear();
}

void Game::start(){
    sound.init();
    sound.music.load("theme", AUDIO_THEME);
    sound.music.load("bonus", AUDIO_BONUS);
    sound.music.load("fanfare", AUDIO_FANFARE);
    sound.music.load("results", AUDIO_RESULTS);
    sound.effects.load("dead",  AUDIO_DEAD);
    sound.effects.load("clear", AUDIO_CLEAR);
    sound.effects.load("item",  AUDIO_ITEM);
    sound.effects.load("wave",  AUDIO_WAVE);
    sound.effects.load("start", AUDIO_START);
    sound.effects.load("trampoline", AUDIO_JUMP);

    sound.effects.play("start");
    state = "START_DELAY";
    timer.reset(timeout);
    timeout = timer.start(1000);

    this->PAUSED = true;

    if(!isBonusRound(level)){
        player.reset(580, 480);
        goro.reset(770, 160);
    }
    else{
        player.reset(620, 160);
        // goro.reset(770, 160);
    }

    this->mode  = PLAY;
    this->music = OFF;
    this->playBGM();
}

void Game::init(int w, int h){
    SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );

    text.init({WHITE,RED,GREEN,BLUE,YELLOW,ORANGE,PINK}, w, h);

    this->PAUSED = true;

    this->stage = * new Stage(w, h);
    this->center.x = this->stage.width/2;
    this->center.y = this->stage.height/2;
    this->stage.bottom -= DIM*SCALE;

    demo = * new Demo();
    demo.start();

    points = * new Points();

    physics = * new Physics();
    player.gravity = physics.gravity(0.125, 0);
    player.gravitation = true;

    mapper.init();

    for(int i=0; i<12; i++){
        Trampoline trampoline = * new Trampoline();
        trampolines.push_back(trampoline);
    }

    for(int i=0; i<4; i++){
        for(int j=0; j<6; j++){
            Door door = * new Door(i, i*4+j);
            doors.push_back(door);
        }
    }

    for(int i=0; i<5; i++){
        for(int j=0; j<2; j++){
            items.push_back(* new Item(i));
        }
    }

    for(int b=0; b<16; b++){
        balloons.push_back(* new Balloon());
    }

    for(int i=0; i<9; i++){
        Enemy mewkie = * new Enemy();
        mewkie.init(data.spawn[i][0], data.spawn[i][1]);
        mewkie.gravity = physics.gravity(0.125, 0);
        mewkie.id = i;

        if(i>=data.enemies[level-1]){
            mewkie.deactivate();
        }

        enemies.push_back(mewkie);
    }

    goro = * new Boss();
    goro.init(0,0);
    goro.gravity = physics.gravity(0.125, 0);
    goro.active = true;

    timeout = timer.start(1500);

    // for(int i=0; i<cached.size(); i++){ cout << cached[i] << " "; }
    // cout<<endl;
    // cout<< state << endl;
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

void Game::playBGM(){
    if(state!="BONUS_ROUND"){
        if(music) sound.music.loop("theme");
    }
    else{
        if(music) sound.music.play("bonus");
    }
}

void Game::stopBGM(){
    sound.music.stop();
    music = OFF;
}

void Game::clear(){
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );
}

bool Game::isBonusRound(int lvl){
    return (lvl==3 || lvl==7 || lvl==11 || lvl==15);
}

void Game::complete(){
    controls.lock = true;

    if(state!="BONUS_ROUND"){
        if(!COMPLETE){
            COMPLETE = true;
            stopBGM();
            sound.effects.stop();
            sound.effects.play("clear", 8);
        }

        else if(!sound.effects.playing(8)){
            clear();
        }

        if(game.delay(2000, timestamp)){
            return;
        }

        collected.clear();

        timer.reset(timeout);
        timeout = timer.start(800);

        interlude();
    }
    else{
        COMPLETE = true;

        if(sound.music.playing()){
            stopBGM();
        }

        timer.reset(timeout);
        timeout = timer.start(10000);
        results();
    }
}

void Game::results(){
    mode = DISPLAY;

    if(state!="BONUS_RESULTS"){
        clear();
        state = "BONUS_RESULTS";
        sound.music.play("results");
    }

    if(timer.done(timeout)){
        state = "CLEAR_SCREEN";
        timer.reset(timeout);
        restage();
    }
};

void Game::interlude(){
    mode = DISPLAY;

    if(isBonusRound(level+1)){
        if(state!="CLEAR_SCREEN"){
            clear();
            sound.music.play("fanfare");
            text.center("bonus round", 13, 7);
            text.center("sting  s", 2, 14);
            text.center("before   ends", 21, 17);
            mapper.draw(data.items[15], {center.x + BYTE*SCALE, 13*BYTE*SCALE});
            mapper.draw(data.items[14], {center.x, 17*BYTE*SCALE - BYTE});
            state = "CLEAR_SCREEN";
        }
        else if(state=="CLEAR_SCREEN" && !sound.music.playing()){
            timer.reset(timeout);
            timeout = timer.start(500);
            restage();
        }
    }
    else{
        clear();
        state = "CLEAR_SCREEN";
        timer.reset(timeout);
        timeout = timer.start(500);
        restage();
    }
}

void Game::restage(){
    if(state=="CLEAR_SCREEN"){
        this->level    += 1;
        this->offset.x  = -182;
        this->offset.y  = 0;
        this->scrolling = true;
        this->pickup    = -1;
        this->factor    = 2;

        if(!isBonusRound(level)){
            state="RUNNING";
            if(level>3) tiers = FLOORS + 1;
        }
        else{
            state="BONUS_ROUND";

            for(int i=0; i<balloons.size(); i++){
                balloons[i].reset();
            }
        }

        for(int t=0; t<trampolines.size(); t++){
            trampolines[t].assigned = false;
            trampolines[t].reset();
            trampolines[t].group.clear();
        }

        for(int d=0; d<doors.size(); d++){
            doors[d].cleanup();
        }

        for(int i=0; i<items.size(); i++){
            items[i].reset();
        }

        for(int i=0; i<enemies.size(); i++){
            enemies[i].deactivate();
        }

        for(int i=0; i<data.enemies[level-1]; i++){
            enemies[i].reset(data.spawn[i][0], data.spawn[i][1]);
        }

        setup();

        points.reset();

        if(state=="BONUS_ROUND"){
            player.reset(620, 160);
        }
        else{
            player.reset(580, 480);
            goro.reset(770, 160);
        }

        controls.lock = false;
    }

    if(timer.done(timeout)){
        timer.reset(timeout);
        this->PAUSED   = false;
        this->RESET    = false;
        this->COMPLETE = false;

        this->mode = PLAY;
        this->music = ON;
    }
    else{
        restage();
    }
}

void Game::restart(){
    clear();
    sound.effects.stop();
    player.reset(580, 480);

    for(int d=0; d<doors.size(); d++){
        if(doors[d].id+3==MAGIC_DOOR_R || doors[d].id+3==MAGIC_DOOR_L){
            doors[d].wave.cleanup();
        }
    }

    for(int i=0; i<enemies.size(); i++){
        enemies[i].deactivate();
    }

    for(int i=0; i<data.enemies[level-1]; i++){
        enemies[i].reset(data.spawn[i][0], data.spawn[i][1]);
    }

    for(int t=0; t<trampolines.size(); t++){
        trampolines[t].reset();
    }

    points.reset();

    goro.reset(770, 160);

    controls.lock = false;

    this->PAUSED    = false;
    this->RESET     = false;
    this->COMPLETE  = false;
    this->scrolling = true;
    this->offset.x  = -182;
    this->offset.y  = 0;
    this->pickup    = -1;

    this->mode = PLAY;
    this->music = ON;
}

void Game::reset(){
    stopBGM();
    level = 0;
    clear();
    collected.clear();
    state = "CLEAR_SCREEN";
    restage();
    PAUSED = true;
    mode = DISPLAY;
    demo.start();
    state = "START_SCREEN";
}