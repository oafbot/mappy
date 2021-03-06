#include "game.hpp"
using namespace std;

Enemy::Enemy(int id) : Sprite(){
    array <array<int, SPRITE_SIZE>, BITMAP_SIZE> b = data.sprites;
    array<array<int, SPRITE_SIZE>, FRAMES>
    f0 = {b[26], b[27], b[28], b[27], b[26], b[27], b[28], b[27]}, // left, right
    f1 = {b[28], b[28], b[28], b[28], b[28], b[28], b[28], b[28]}, // hop
    f2 = {b[29], b[29], b[30], b[30], b[29], b[29], b[30], b[30]}, // bounce
    f3 = {b[31], b[31], b[31], b[31], b[31], b[31], b[31], b[31]}; // ko'ed

    define("left", f0);
    define("right", flip(f0));
    define("hop-left", f1);
    define("hop-right", flip(f1));
    define("drop", f2);
    define("bound", f2);
    define("ko-left", flip(f3));
    define("ko-right", f3);

    this->id = id;
    this->width  = DIM;
    this->height = DIM;

    this->gravitation = true;
    this->falling     = false;
    this->bouncing    = false;
    this->animated    = true;
    this->direction   = "left";
    this->state       = "left";
    this->type        = "enemy";
    this->frame       = 0;
    this->bounces     = -1;
    this->captured    = false;
    this->released    = false;
    this->speed       = ENEMY_SPEED;
    this->collider    = new Collider<Enemy>(this);
    this->cache       = &game.cache.enemy;
    this->blink       = false;
    this->blinkon     = {3,4,7,8};

    this->compile();
};

void Enemy::init(double x, double y){
    this->x = x;
    this->y = y;
    this->timestamp = SDL_GetTicks();
    this->active = true;

    collider->init(x + game.offset.x, y + game.offset.y, width, height);
}

void Enemy::draw(const array<int, SPRITE_SIZE> &bits){
    string color;
    SDL_Rect r;

    int w = SCALE;
    int row;
    int col;
    int bit;
    int alpha = 255;

    for(int i=0; i<SPRITE_SIZE; i++) {
        bit = bits[i];
        if(bit){
            color = data.palette[bit];
            SDL_Color c = hex2sdl(color);

            col = i % DIM;
            row = floor(i / DIM);

            r.x = w*col;
            r.y = w*row;
            r.w = w;
            r.h = w;
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, alpha);
            SDL_RenderFillRect(renderer, &r);
        }
    }
}

array<array<int, SPRITE_SIZE>, FRAMES>
Enemy::flip(array<array<int, SPRITE_SIZE>, FRAMES> frames){
    array<array<int, SPRITE_SIZE>, FRAMES> flipped;
    array<array<int, DIM>, SPRITE_SIZE/DIM> rows;
    array<int, SPRITE_SIZE> flat;
    int row;
    int col;

    for(int i=0; i < frames.size(); i++){
        for(int j=0; j < frames[i].size(); j++){
            col = j % DIM;
            row = floor(j / DIM);

            if(col==0){
                array<int, DIM> r;
                rows[row] = r;
            }
            rows[row][col] = frames[i][j];
        }

        for(int k=0; k<rows.size(); k++){
            reverse(rows[k].begin(), rows[k].end());
            for(int l=0; l<rows[k].size(); l++){
                flat[k*DIM+l] = rows[k][l];
            }
        }
        // printf("%lu\n", flat.size());
        flipped[i] = flat;
    }

    return flipped;
}

void Enemy::define(string name, array<array<int, SPRITE_SIZE>, FRAMES> frames){
    this->states.insert(make_pair(name.c_str(), frames));
}

void Enemy::update(){
    if(active){
        if(captured){
            if(state == "right"){
                x -= WAVE_SPEED;
                collider->passthru = true;
            }
            else if(state == "left"){
                x += WAVE_SPEED;
                collider->passthru = true;
            }
        }
        else if(state!="ko-left" && state!="ko-right"){
            if(this->falling && state!="hop-right" && state!="hop-left"){
                this->state = "drop";
                this->collider->passthru = true;
            }
            else if(this->bouncing && state!="hop-right" && state!="hop-left"){
                this->state = "bound";
                this->collider->passthru = true;
            }
            else if(this->falling || this->bouncing){
                this->collider->passthru = true;
            }
            else if(state=="hop-left" || state=="hop-right"){
                this->collider->passthru = true;
            }
            else if(state!="hop-left" && state!="hop-right"){
                this->state = this->direction;
                this->collider->passthru = false;
            }

            walk();
            spaceout();
            collider->update(x+game.offset.x, y+game.offset.y);
        }
        else{

            if(state=="ko-left" && x>slide && adjacent(DOWN_LEFT)!=EMPTY && adjacent(DOWN_LEFT)!=TRAMPOLINE){
                x -= ENEMY_SPEED*2;
            }
            else if(state=="ko-right" && x<slide && adjacent(DOWN_RIGHT)!=EMPTY && adjacent(DOWN_RIGHT)!=TRAMPOLINE){
                x += ENEMY_SPEED*2;
            }

            this->collider->passthru = true;

            if(game.delay(3500, timestamp)){
                return;
            }

            blink = true;

            if(game.delay(5000, timestamp)){
                return;
            }

            if(state=="ko-left"||state=="ko-right"){
                respawn();
            }
        }
    }
    else if(released){
        if(game.timer.done(regenerate)){
            respawn();
        }
        else{
            // x = game.center.x -(game.stage.width - (game.stage.width+game.offset.x+OFFSET)) - width*SCALE/2;
            y = 100;
        }
    }
}

void Enemy::spaceout(){
    if((state=="left" && !adjacent(DOWN_RIGHT)) || (state=="right" && !adjacent(DOWN_LEFT))){
        for(int i=0; i<game.enemies.size(); i++){
            if(id!=game.enemies[i].id){
                if(collider->check(game.enemies[i].collider) && direction==game.enemies[i].direction){
                    x = direction=="right" ? x-width*SCALE : x+width*SCALE;
                }
            }
        }
    }
    else if((state=="bound" || state=="drop") && !adjacent(DOWN)){
        for(int i=0; i<game.enemies.size(); i++){
            if(id!=game.enemies[i].id){
                if(collider->check(game.enemies[i].collider) && state==game.enemies[i].state){
                    y = state=="drop" ? y-height*SCALE : y+height*SCALE;
                }
            }
        }
    }
}

void Enemy::render(){
    if(active){
        SDL_Rect dest, src;

        // dest.x = game.center.x + game.stage.width - (1024 - x) + game.offset.x;
        dest.x = game.offset.x + x;
        dest.y = game.offset.y + y;

        dest.w = width*SCALE;
        dest.h = height*SCALE;

        src.x = 0;
        src.y = 0;
        src.w = width*SCALE;
        src.h = height*SCALE;

        if(animated){
            if( frame >= FRAME_COUNT ){
                frame = 0;
            }
            else{
                frame++;
            }
        }
        SDL_SetTextureBlendMode((*cache)[state][frame], SDL_BLENDMODE_BLEND);
        if(!blink || (blink && contains(blinkon, frame) ))
            SDL_RenderCopy(renderer, (*cache)[state][frame], &src, &dest);
    }
}

void Enemy::compile(){
    if(!contains(game.cached, type)){
        for(map< string, array< array<int, SPRITE_SIZE>, FRAMES> >::iterator it=states.begin(); it!=states.end(); ++it){
            string s = it->first;
            array<SDL_Texture*, FRAMES> images;
            // it->second[f].size()
            for(int f=0; f<FRAMES; f++){
                SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width*SCALE, height*SCALE);
                SDL_SetRenderTarget(renderer, texture);
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

                // for(int i=0; i<layout.size(); i++){
                draw(it->second[f]);
                // }

                images[f] = texture;
                // throw exception();
                // SDL_SetRenderTarget(renderer, NULL);
            }
            cache->insert(make_pair(s, images));
        }
        SDL_SetRenderTarget(renderer, NULL);
        game.cached.push_back(type);
    }
}

// bool Enemy::collision(Collider<Player> complement){return true;}

void Enemy::deaded(){}

void Enemy::knockedout(int direction, double door_x){
    // int tile;

    if(direction==LEFT){
        state = "ko-right";
        collider->passthru = true;
        // tile = adjacent(DOWN_LEFT, door_x + DIM*SCALE*3, y); //data.interactive[game.level-1][index(door_x + DIM*SCALE*3, y)];
        // slide = tile!=EMPTY && tile!=TRAMPOLINE ? door_x + DIM*SCALE*3 : door_x + DIM*SCALE;
        slide = door_x + DIM*SCALE*3;
    }
    else if(direction==RIGHT){
        state = "ko-left";
        collider->passthru = true;
        // tile = adjacent(DOWN_RIGHT, door_x - DIM*SCALE*3, y); //data.interactive[game.level-1][index(door_x - DIM*SCALE*3, y)];
        // slide = tile!=EMPTY && tile!=TRAMPOLINE ? door_x - DIM*SCALE*3 : door_x - DIM*SCALE;
        slide = door_x - DIM*SCALE*3;
    }
    timestamp = SDL_GetTicks();
}

void Enemy::respawn(){
    // cout << "respawn" << endl;
    game.timer.reset(regenerate);
    // cout << game.center.x + game.offset.x/2 + OFFSET + width*SCALE << endl;
    this->x = game.center.x + game.offset.x/2 + OFFSET/* + width*SCALE*/;
    this->y = 95;
    this->frame = 0;
    this->active   = true;
    this->falling  = true;
    this->bouncing = false;
    this->animated = true;
    this->captured = false;
    this->released = false;
    this->blink    = false;
    this->gravitation = true;
    this->direction = x>game.center.x ? "left" : "right";
    this->state = direction=="left" ? "left" : "right";
    this->collider->update(x+game.offset.x, y+game.offset.y);
    this->collider->passthru = false;
}

void Enemy::ledge(){
    if(adjacent(DOWN_LEFT)==0 || adjacent(DOWN_RIGHT)==0){
        this->collider->passthru = true;
    }
}

int Enemy::index(){
    // printf("%f %f\n", x, y);
    // printf("%f\n", game.offset.x);
    int tile = BYTE*SCALE;
    double diff = game.stage.width-(LEVEL_WIDTH*tile);
    double xpos;
    xpos = x + (width/2)*SCALE;

    int col = (int)floor(xpos/tile);
    int row = (int)floor((y+height)/tile);
    return col + row*LEVEL_WIDTH;
}

int Enemy::index(double x, double y){
    // printf("%f %f\n", x, y);
    // printf("%f\n", game.offset.x);
    int tile = BYTE*SCALE;
    double diff = game.stage.width-(LEVEL_WIDTH*tile);
    double xpos;
    xpos = x + (width/2)*SCALE;

    int col = (int)floor(xpos/tile);
    int row = (int)floor((y+height)/tile);
    return col + row*LEVEL_WIDTH;
}

int Enemy::adjacent(int direction){
    int a, shift;

    int i = index(x, y);
    shift = LEVEL_WIDTH;

    switch(direction){
        case UP:
            a = data.interactive[game.level-1][i-shift];
            break;
        case DOWN:
            a = data.interactive[game.level-1][i+shift];
            break;
        case RIGHT:
            a = data.interactive[game.level-1][i+1];
            break;
        case LEFT:
            a = data.interactive[game.level-1][i-1];
            break;
        case DOWN_LEFT:
            a = data.interactive[game.level-1][i-1+shift];
            break;
        case DOWN_RIGHT:
            a = data.interactive[game.level-1][i+1+shift];
            break;
    }
    return a;
}

int Enemy::adjacent(int direction, double _x, double _y){
    int a, shift;

    int i = index(_x, _y);
    shift = LEVEL_WIDTH;

    switch(direction){
        case UP:
            a = data.interactive[game.level-1][i-shift];
            break;
        case DOWN:
            a = data.interactive[game.level-1][i+shift];
            break;
        case RIGHT:
            a = data.interactive[game.level-1][i+1];
            break;
        case LEFT:
            a = data.interactive[game.level-1][i-1];
            break;
        case DOWN_LEFT:
            a = data.interactive[game.level-1][i-1+shift];
            break;
        case DOWN_RIGHT:
            a = data.interactive[game.level-1][i+1+shift];
            break;
        case UP_LEFT:
            a = data.interactive[game.level-1][i-1-shift];
            break;
        case UP_RIGHT:
            a = data.interactive[game.level-1][i+1-shift];
            break;
    }
    return a;
}

bool Enemy::aligned(){
    int i = index(x,y);
    int col = i % LEVEL_WIDTH;
    return col%17==2;
}

void Enemy::align(){
    if(((int)y)%LONG!=0){
        y = (int)(floor(y)/LONG)*LONG;
    }
}

void Enemy::align(bool horiz){
    x = (trampoline.group[1]%LEVEL_WIDTH)*BYTE*SCALE + (BYTE*SCALE/2) - (width*SCALE/2);
}

bool Enemy::traverse(int direction){
    int tile = adjacent(direction);

    if(tile==0){
        if(direction==RIGHT || direction==LEFT){
            int shift = direction==RIGHT ? 1 : -1;

            for(int i=0; i<game.doors.size(); i++){
                if((contains(game.doors[i].group, index(x, y)+shift+LEVEL_WIDTH) ||
                    contains(game.doors[i].group, index(x, y)+shift+LEVEL_WIDTH*2)) && !game.doors[i].open){
                    return false;
                }
            }
        }
        return true;
    }
    if(tile>=RADIO && tile<=BELL){
        return true;
    }
    if(tile>=DOOR_LEFT && tile<=MAGIC_DOOR_R){
        switch(direction){
            case RIGHT:
                for(int i=0; i<game.doors.size(); i++){
                    if(contains(game.doors[i].group, index(x, y)+1) && game.doors[i].open){
                        return true;
                    }
                }
                break;
            case LEFT:
                for(int i=0; i<game.doors.size(); i++){
                    if(contains(game.doors[i].group, index(x, y)-1) && game.doors[i].open){
                        return true;
                    }
                }
                break;
        }
    }
    return false;
}

bool Enemy::traverse(int direction, double x, double y){
    int tile = adjacent(direction, x, y);

    if(tile==0){
        if(direction==RIGHT || direction==LEFT){
            int shift = direction==RIGHT ? 1 : -1;

            for(int i=0; i<game.doors.size(); i++){
                if((contains(game.doors[i].group, index(x, y)+shift+LEVEL_WIDTH) ||
                    contains(game.doors[i].group, index(x, y)+shift+LEVEL_WIDTH*2)) && !game.doors[i].open){
                    return false;
                }
            }
        }
        return true;
    }
    if(tile>=RADIO && tile<=BELL){
        return true;
    }
    if(tile>=DOOR_LEFT && tile<=MAGIC_DOOR_R){
        switch(direction){
            case RIGHT:
                for(int i=0; i<game.doors.size(); i++){
                    if(contains(game.doors[i].group, index(x, y)+1) && game.doors[i].open){
                        return true;
                    }
                }
                break;
            case LEFT:
                for(int i=0; i<game.doors.size(); i++){
                    if(contains(game.doors[i].group, index(x, y)-1) && game.doors[i].open){
                        return true;
                    }
                }
                break;
        }
    }
    return false;
}

void Enemy::opendoor(int direction){
    int ix = direction==LEFT ? index(x, y)-1 : index(x, y)+1;

    for(int i=0; i<game.doors.size(); i++){
        if(contains(game.doors[i].group, ix)){
            game.doors[i].operate();
            break;
        }
    }
}

void Enemy::move(){
    int tile;

    if(direction=="left"){
        tile = adjacent(LEFT);

        if(traverse(LEFT)){
            state = "left";
            x -= speed;
        }
        else if(rand()%2 && (tile==DOOR_LEFT || tile==DOOR_RIGHT)){
            opendoor(LEFT);
        }
        else{
            state = "right";
            direction = "right";
            x += speed;
        }
    }
    else if(direction=="right"){
        tile = adjacent(RIGHT);

        if(traverse(RIGHT)){
            state = "right";
            x += speed;
        }
        else if(rand()%2 && (tile==DOOR_LEFT || tile==DOOR_RIGHT)){
            opendoor(RIGHT);
        }
        else{
            state = "left";
            direction = "left";
            x -= speed;
        }
    }
}

void Enemy::hop(int i){
    y = i<25 ? y - 0.25 : y + 0.25;
    x = direction=="right" ? x + 0.25 : x - 0.25;
    collider->passthru = true;
}

void Enemy::hopoff(){
    bouncing = false;
    falling = false;
    collider->passthru = true;

    if(x>800){
        state = "hop-left";
        direction = "left";
    }
    else if(x<100){
        state = "hop-right";
        direction = "right";
    }

    if(!game.interval()){ return; };

    if(traverse(DOWN)){
        if(direction=="left" && traverse(LEFT)){
            for(int i=0; i<48; i++){
                hop(i);
            }
        }
        else if(direction=="right" && traverse(RIGHT)){
            for(int i=0; i<48; i++){
                hop(i);
            }
        }
    }
    else{
        bouncing = false;
        falling = false;
        move();
    }
    bounces = -1;
}

void Enemy::wander(){
    int r;

    if(state=="bound" || state=="drop" || state=="turn"){
        if(x<100){
            state = "hop-right";
            direction = "right";
            hopoff();
        }
        else if(x>800){
            state = "hop-left";
            direction = "left";
            hopoff();
        }
        else{
            r = rand()%2;
            switch(r){
                case 0:
                    state = "hop-left";
                    direction = "left";
                    hopoff();
                    break;
                case 1:
                    state = "hop-right";
                    direction = "right";
                    hopoff();
                    break;
            }
        }
    }
    // else{
    //     r = rand() % 5;
    //     switch(r){
    //         case 0:
    //             direction = "left";
    //             state = "left";
    //             break;
    //         case 1:
    //             direction = "right";
    //             state = "right";
    //             break;
    //         default:
    //             break;
    //     }
    // }
}

void Enemy::pursue(){
    if(player.x < x && (x+game.offset.x)-player.x>DIM){
    // if(player.x < x && x-player.x>DIM){
        direction = "left";
        state = "left";
    }
    else if(player.x > x && player.x-(x+game.offset.x)>DIM){
    // else if(player.x > x && player.x-x>DIM){
        direction = "right";
        state = "right";
    }
}

void Enemy::decision(){
    int r = rand()%6;

    if(r && player.state!="drop" && player.state!="bound"){
        pursue();
    }
    else{
        wander();
    }
}

void Enemy::bounce(){
    falling = false;
    bouncing = true;

    if(bounces==-1){
        int t = trampoline.tier();
        tier = t + rand()%(game.tiers-t);
        bounces = rand()%3;
    }
    else{
        if(!game.interval()){
            return;
        }
        if(bounces>=0){
            bounces--;
        }
    }
}

void Enemy::jump(){
    int row  = LEVEL_HEIGHT-(index(x, y)/LEVEL_WIDTH);
    int pos  = (row/FLOOR_HEIGHT);
    int plyr = LEVEL_HEIGHT-(player.index(player.x, player.y)/LEVEL_WIDTH);

    if(bounces<0){
        wander();
    }
    else if(pos==tier && bounces==0){
        wander();
    }
    else if(type!="boss" && row==plyr){
        if(player.x < x+game.offset.x){
            direction = "left";
            state = "hop-left";
        }
        else if(player.x >= x+game.offset.x){
            direction = "right";
            state = "hop-right";
        }
        hopoff();
    }
    // else{
    //     if(x<100){
    //         state = "hop-right";
    //         direction = "right";
    //         hopoff();
    //     }
    //     else if(x>800){
    //         state = "hop-left";
    //         direction = "left";
    //         hopoff();
    //     }
    // }
}

void Enemy::walk(){
    if(state!="bound" && state!="drop" && state!="hop-left" && state!="hop-right"){
        move();
    }
    else if(state=="bound"){
        jump();
    }
    else if(state=="drop"){
        if(direction=="left" && !aligned()){
            x -= speed;
        }
        else if(direction=="right" && !aligned()){
            x += speed;
        }
    }
    else if(state=="hop-left" || state=="hop-right"){
        hopoff();
    }

    if(game.delay(3000, timestamp)){
        return;
    };

    if(state=="bound" || state=="drop" || state=="turn"){
        decision();
    }

    timestamp = SDL_GetTicks();
}

void Enemy::reset(double x, double y){
    this->x = x;
    this->y = y;
    this->gravitation = true;
    this->falling     = false;
    this->bouncing    = false;
    this->direction   = "left";
    this->state       = "left";
    this->type        = "enemy";
    this->frame       = 0;
    this->animated    = true;
    this->active      = true;
    this->blink       = false;
    this->released    = false;
    this->captured    = false;
    this->bounces     = -1;
    this->collider->passthru = false;
    this->collider->update(x+game.offset.x, y+game.offset.y);

    if(game.round){
        speed = ENEMY_SPEED*1.8;
    }
    else if(game.level<3){
        speed = ENEMY_SPEED;
    }
    else if(game.level<7){
        speed = ENEMY_SPEED*1.25;
    }
    else if(game.level<11){
        speed = ENEMY_SPEED*1.5;
    }
    else if(game.level<15){
        speed = ENEMY_SPEED*1.75;
    }
    else{
        speed = ENEMY_SPEED*1.8;
    }
}

void Enemy::deactivate(){
    active = false;
    collider->passthru = true;
    x = -1;
    y = -1;
}

void Enemy::capture(int direction){
    if(direction==LEFT){
        state = "right";
        animated = false;
        captured = true;
        collider->passthru = true;
        gravitation = false;
    }
    else{
        state = "left";
        animated = false;
        captured = true;
        collider->passthru = true;
        gravitation = false;
    }
}

void Enemy::release(int delay){
    // cout << " release " << type <<endl;
    game.timer.reset(regenerate);
    regenerate = game.timer.start(delay*500);
    captured = false;
    released = true;
    active = false;
}