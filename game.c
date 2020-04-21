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

    this->level     = START_LEVEL;
    this->PAUSED    = false;
    this->RESET     = false;
    this->COMPLETE  = false;
    this->perfect   = false;
    this->bonus     = false;
    this->scrolling = true;
    this->offset.x  = -182;
    this->offset.y  = 0;
    this->score     = 0;
    this->hiscore   = 0;
    this->pickup    = -1;
    this->factor    = 2;
    this->lives     = 3;
    this->overtime  = 0;
    this->round     = 0;
    this->lifeup    = 20000;
    this->tiers     = FLOORS;
    this->mapper    = * new Mapper();
    this->controls  = * new Control();
    this->sound     = * new Sound();
    this->timer     = * new Timer();
    this->text      = * new Text();
    this->controls.lock = false;
    this->state = "LOADING";
    this->ms_per_frame = 100;  // animate at 10 fps
}

void Game::update(){
    clock();

    if(state=="GAME_OVER"){
        gameover();
    }
    else if(state=="RESTART"){
        if(timer.done(timeout)){
            timer.reset(timeout);
            reset();
        }
    }
    else if(mode==PLAY){
        if(state!="BONUS_ROUND" && collected.size()==10){
            complete();
        }
        else if(lives<=0 && !GAME_OVER){
            if(state!="GAME_OVER"){
                timer.reset(timeout);
                timeout = timer.start(2000);
            }
            state = "GAME_OVER";
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

            for(int b=0; b<bells.size(); b++){
                if(bells[b].assigned) bells[b].update();
            }

            if(time>TIME_LIMIT){
                overlimit();
            }
            else if(time>OVERTIME){
                speedup();
            }
        }
        else{
            music = OFF;
            if(!sound.music.playing()){
                complete();
            }

            if(bonus_balloon.collected){
                goro.y -= SPEED*SCALE;
                goro.balloons();
            }
        }

        for(int t=0; t<trampolines.size(); t++){
            trampolines[t].update();
        }


        if(score>lifeup){
            lifeup = lifeup==20000 ? 70000 : lifeup+70000;
            lives += 1;
            cout << "next life: " << lifeup << endl;
        }

        if(score>hiscore){
            hiscore = score;
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

void Game::warning(){
    if(hurry>stage.left-DIM*SCALE){
        hurry -= SPEED*SCALE*8;
        Draw::render(cache.hurry, hurry, center.y-DIM*SCALE);
    }
}

void Game::speedup(){
    if(!overtime){
        hurry = stage.right+DIM*SCALE;
        overtime += 1;
        for(int i=0; i<enemies.size(); i++){
            // enemies[i].speed = enemies[i].speed + (0.5*overtime);
            enemies[i].speed = ENEMY_SPEED*1.8;
        }

        enemies[7].reset(data.spawn[7][0], data.spawn[7][1]);
        enemies[8].reset(data.spawn[8][0], data.spawn[8][1]);

        stopBGM();
        sound.effects.play("hurryx1", 7);
    }
    else if(!sound.music.playing() && !sound.effects.playing(7) && !player.dead && !COMPLETE){
        music = ON;
        playBGM();
    }
}

void Game::overlimit(){
    if(overtime<2){
        overtime += 1;
        hurry = stage.right+DIM*SCALE;

        stopBGM();
        sound.effects.play("hurryx2", 7);
        gosenzo.active = true;
        gosenzo.direction = gosenzo.x+offset.x<player.x ? "right" : "left";
    }
    else if(!sound.music.playing() && !sound.effects.playing(7) && !player.dead && !COMPLETE){
        music = ON;
        playBGM();
    }

    gosenzo.update();
}

void Game::clock(){
    current_time = SDL_GetTicks();
    time = current_time - start_time - delta;
}

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

    goro.render();
    gosenzo.render();
    // goro.collider->debug();
    // gosenzo.collider->debug();

    for(int i=0; i<items.size(); i++){
        items[i].render();
    }

    for(int i=0; i<bells.size(); i++){
        // bells[i].collider->debug();
        bells[i].render();
    }

    for(int i=0; i<enemies.size(); i++){
        // enemies[i].collider->debug();
        enemies[i].render();
    }

    if(state=="BONUS_ROUND"){
        for(int i=0; i<balloons.size(); i++){
            balloons[i].render();
        }
        bonus_balloon.render();
    }
}

void Game::renderLives(){
    for(int i=0; i<lives; i++){
        Draw::render(cache.badge, 0+BYTE+i*DIM*SCALE, stage.height-BYTE-DIM*SCALE);
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
    renderLives();

    if(state!="BONUS_ROUND"){
        text.score(game.score);
        text.hiscore();
    }

    if(GAME_OVER){
        Draw::render(cache.gameover, center.x-32, center.y-DIM*SCALE/2);
    }

    if(overtime>0){
        warning();
    }

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

void Game::gameover(){

    if(!interval()){ return; }

    if(timer.done(timeout) && !GAME_OVER){
        state = "RESTART";
        GAME_OVER = true;

        timer.reset(timeout);
        timeout = timer.start(10000);

        if(!sound.effects.playing(4))
            sound.effects.play("gameover", 4);

        EM_ASM( GameOver() );
    }
}

void Game::setup(){
    int key;
    bool found;

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
                case BONUS_BALLOON:
                    bonus_balloon.assign(i);
                    skip.push_back(i);
                    skip.push_back(i+1);
                    skip.push_back(i+2);
                    skip.push_back(i+3);
                    skip.push_back(i+LEVEL_WIDTH);
                    skip.push_back(i+LEVEL_WIDTH+1);
                    skip.push_back(i+LEVEL_WIDTH+2);
                    skip.push_back(i+LEVEL_WIDTH+3);
                    skip.push_back(i+LEVEL_WIDTH*2);
                    skip.push_back(i+LEVEL_WIDTH*2+1);
                    skip.push_back(i+LEVEL_WIDTH*2+2);
                    skip.push_back(i+LEVEL_WIDTH*2+3);
                    skip.push_back(i+LEVEL_WIDTH*3);
                    skip.push_back(i+LEVEL_WIDTH*3+1);
                    skip.push_back(i+LEVEL_WIDTH*3+2);
                    skip.push_back(i+LEVEL_WIDTH*3+3);
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
                case BELL:
                    for(int b=0; b<bells.size(); b++){
                        if(!bells[b].assigned){
                            skip.push_back(i);
                            skip.push_back(i+1);
                            skip.push_back(i+LEVEL_WIDTH);
                            skip.push_back(i+LEVEL_WIDTH+1);

                            bells[b].assign(i);
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
    if(state=="START_SCREEN"){
        sound.init();
        sound.music.load("theme", AUDIO_THEME);
        sound.music.load("bonus", AUDIO_BONUS);
        sound.music.load("fast",  AUDIO_FAST);
        sound.music.load("fanfare", AUDIO_FANFARE);
        sound.music.load("results", AUDIO_RESULTS);
        sound.effects.load("gameover", AUDIO_GAME_OVER);
        sound.effects.load("dead",  AUDIO_DEAD);
        sound.effects.load("clear", AUDIO_CLEAR);
        sound.effects.load("item",  AUDIO_ITEM);
        sound.effects.load("wave",  AUDIO_WAVE);
        sound.effects.load("bell",  AUDIO_BELL);
        sound.effects.load("start", AUDIO_START);
        sound.effects.load("trampoline", AUDIO_JUMP);
        sound.effects.load("hurryx1", AUDIO_HURRY1);
        sound.effects.load("hurryx2", AUDIO_HURRY2);

        sound.effects.play("start");
        this->PAUSED = true;

        if(!isBonusRound(level)){
            player.reset(580, 480);
            goro.reset(770, 160);
        }
        else{
            player.reset(620, 160);
            goro.reset(32, 450);
            goro.gravitation = false;
            goro.state = "bound";
        }

        state = "START_DELAY";
        timer.reset(timeout);
        timeout = timer.start(1000);

        mode  = PLAY;
        music = OFF;
        playBGM();
        controls.lock = false;
        start_time = SDL_GetTicks();
    }
}

void Game::init(int w, int h){
    SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );

    text.init({WHITE,RED,GREEN,BLUE,YELLOW,ORANGE,PINK}, w, h);

    this->PAUSED = true;
    controls.lock = true;

    this->stage = * new Stage(w, h);
    this->center.x = this->stage.width/2;
    this->center.y = this->stage.height/2;
    this->stage.bottom -= DIM*SCALE;

    demo = * new Demo();
    demo.start();

    points = * new Points();

    physics = * new Physics();

    player = * new Player();
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

    for(int i=0; i<2; i++){
        bells.push_back(* new Bell());
    }

    for(int b=0; b<16; b++){
        balloons.push_back(* new Balloon(false));
    }

    bonus_balloon = * new Balloon(true);

    for(int i=0; i<9; i++){
        Enemy mewkie = * new Enemy(i);
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

    gosenzo = * new Gosenzo();
    gosenzo.init(center.x, 35*BYTE);
    gosenzo.active = false;

    array<array<int, SPRITE_SIZE>,1> badge = {data.items[17]};
    cache.badge = Draw::compile(badge, 1, DIM);
    Draw::clear();

    array<array<int, SPRITE_SIZE>,2> go = {data.items[26], data.items[27]};
    cache.gameover = Draw::compile(go, 2, DIM);
    Draw::clear();

    array<array<int, SPRITE_SIZE>,2> hry = {data.items[28], data.items[29]};
    cache.hurry = Draw::compile(hry, 2, DIM);
    hurry = stage.right+DIM*SCALE;
    Draw::clear();


    timeout = timer.start(3000);
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
    if(state=="RUNNING" || state=="BONUS_ROUND"){
        PAUSED = !PAUSED;
        if(PAUSED){
            sound.music.pause();
            sound.effects.pause();
            pause_time = SDL_GetTicks();
        }
        else{
            sound.music.resume();
            sound.effects.resume();
            current_time = SDL_GetTicks();
            delta += current_time - pause_time;
        }
    }
}

void Game::playBGM(){
    if(state!="BONUS_ROUND" && music){
        if(overtime<1){
            sound.music.loop("theme");
        }
        else{
            sound.music.loop("fast");
        }
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

    double HEADING = 6;

    text.center("earnings", PINK, HEADING);

    if(state!="BONUS_RESULTS"){
        clear();
        state = "BONUS_RESULTS";
        sound.music.play("results");


        bonus = contains(collected, 2000);

        if(collected.size()>15 && bonus){
            perfect = true;
            score += 5000;
        }

        timestamp = SDL_GetTicks();
    }
    else{
        int balloons = bonus ? collected.size()-1 : collected.size();

        if(!delay(2000, timestamp)){
            mapper.draw(data.items[15], {center.x-10*BYTE*SCALE, (HEADING+4)*BYTE*SCALE});
            text.render("200 * ", WHITE, {game.center.x-7*BYTE*SCALE, (HEADING+5)*BYTE*SCALE});
        }
        if(!delay(2500, timestamp)){
            text.render(to_string(balloons), YELLOW, {game.center.x-BYTE*SCALE, (HEADING+5)*BYTE*SCALE});
            text.render(" = ", WHITE, {game.center.x+2*BYTE*SCALE,(HEADING+5)*BYTE*SCALE});
        }
        if(!delay(2750, timestamp)){
            text.indent(to_string(200*balloons), RED, 4, {game.center.x+6*BYTE*SCALE, (HEADING+5)*BYTE*SCALE});
        }

        if(!delay(4000, timestamp)){
            text.render("2000 * ", WHITE, {game.center.x-7*BYTE*SCALE, (HEADING+10)*BYTE*SCALE});
        }
        if(!delay(4500, timestamp)){
            text.render((bonus ? " 1" : " 0"), YELLOW, {game.center.x-BYTE*SCALE, (HEADING+10)*BYTE*SCALE});
            text.render(" = ", WHITE, {game.center.x+2*BYTE*SCALE, (HEADING+10)*BYTE*SCALE});
        }
        if(!delay(4750, timestamp)){
            text.indent((bonus ? "2000" : "000"), BLUE, 4, {game.center.x+6*BYTE*SCALE, (HEADING+10)*BYTE*SCALE});
        }

        if(!delay(6000, timestamp)){
            text.render("perfect game = ", WHITE, {game.center.x-10*BYTE*SCALE, (HEADING+15)*BYTE*SCALE});
        }
        if(!delay(6500, timestamp)){
            text.indent((perfect ? "5000" : "000"), YELLOW, 4, {game.center.x+6*BYTE*SCALE, (HEADING+15)*BYTE*SCALE});
        }

        if(!delay(8000, timestamp)){
            text.render("total", ORANGE, {game.center.x-3*BYTE*SCALE, (HEADING+23)*BYTE*SCALE});
        }
        if(!delay(8500, timestamp)){
            text.render(" = ", WHITE, {game.center.x+2*BYTE*SCALE, (HEADING+23)*BYTE*SCALE});
        }
        if(!delay(8750, timestamp)){
            text.indent(to_string(200*balloons + (bonus ? 2000 : 0) + (perfect ? 5000 : 0)), GREEN, 4,
                        {game.center.x+6*BYTE*SCALE, (HEADING+23)*BYTE*SCALE});
        }
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
        if(state!="CLEAR_SCREEN"){
            clear();
            state = "CLEAR_SCREEN";
            text.center("round " + to_string(round*16 + level + 1), 2, 18);
            timer.reset(timeout);
            timeout = timer.start(500);
        }
        else if(state=="CLEAR_SCREEN" && timer.done(timeout)){
            timer.reset(timeout);
            timeout = timer.start(500);
            restage();
        }
    }
}

void Game::restage(){
    int enemy_count;

    if(state=="CLEAR_SCREEN"){
        this->offset.x  = -182;
        this->offset.y  = 0;
        this->scrolling = true;
        this->pickup    = -1;
        this->factor    = 2;
        this->perfect   = false;
        this->bonus     = false;
        this->overtime  = 0;
        this->delta = 0;

        if(level==16)
            round += 1;
        level = level==16 ? 1 : level + 1;

        if(!isBonusRound(level)){
            state="RUNNING";
            if(level>3) tiers = FLOORS + 1;
        }
        else{
            state="BONUS_ROUND";

            for(int i=0; i<balloons.size(); i++){
                balloons[i].reset();
            }

            bonus_balloon.reset();
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

        for(int i=0; i<bells.size(); i++){
            bells[i].reset();
        }

        for(int i=0; i<enemies.size(); i++){
            enemies[i].deactivate();
        }

        enemy_count = round<1 ? data.enemies[level-1] : 7;

        for(int i=0; i<enemy_count; i++){
            enemies[i].reset(data.spawn[i][0], data.spawn[i][1]);
        }

        setup();

        points.reset();
        collected.clear();

        if(state=="BONUS_ROUND"){
            player.reset(620, 160);
            goro.reset(32, 450);
            goro.gravitation = false;
            goro.state = "bound";
            gosenzo.reset();
        }
        else{
            player.reset(580, 480);
            goro.reset(770, 160);
            gosenzo.reset();
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
        this->time = 0;
        this->start_time = SDL_GetTicks();
    }
    else{
        restage();
    }
}

void Game::restart(){
    clear();
    PAUSED = true;
    overtime = 0;
    time = 0;

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

    int enemy_count = round<1 ? data.enemies[level-1] : 7;

    for(int i=0; i<enemy_count; i++){
        enemies[i].reset(data.spawn[i][0], data.spawn[i][1]);
    }

    for(int t=0; t<trampolines.size(); t++){
        trampolines[t].reset();
    }

    points.reset();

    goro.reset(770, 160);
    gosenzo.reset();

    controls.lock = false;

    this->PAUSED    = false;
    this->RESET     = false;
    this->COMPLETE  = false;
    this->scrolling = true;
    this->offset.x  = -182;
    this->offset.y  = 0;
    this->pickup    = -1;

    this->mode  = PLAY;
    this->music = ON;
    this->delta = 0;
    this->start_time = SDL_GetTicks();
}

void Game::reset(){
    timeout = 0;
    level = 0;
    round = 0;
    overtime = 0;
    time = 0;
    delta = 0;
    state = "CLEAR_SCREEN";
    collected.clear();
    sound.effects.stop();
    clear();
    stopBGM();
    restage();
    PAUSED = true;
    perfect = false;
    bonus = false;
    lives = 3;
    score = 0;
    mode = DISPLAY;
    demo.start();
    state = "START_SCREEN";
    GAME_OVER = false;

    EM_ASM( ResetButton() );
}