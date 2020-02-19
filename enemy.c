#include "game.hpp"
using namespace std;

Enemy::Enemy() : Sprite(){
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
    define("dead", f3);

    this->width  = DIM;
    this->height = DIM;

    this->gravitation = true;
    this->falling  = false;
    this->bouncing = false;
    this->direction = "left";
    this->state = "left";
    this->type = "enemy";
    this->frame = 0;
    this->animated = true;
    // this->dead = false;
    this->collider = new Collider(this);
    this->bounces = -1;
};

void Enemy::init(double x, double y){
    this->x = x;
    this->y = y;
    this->timestamp = SDL_GetTicks();

    this->absolute.x = x + (game.stage.full_width - game.offset.x);
    this->absolute.y = y + game.offset.y;

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
    if(state!="dead"){
        if(this->falling && state!="hop-right" && state!="hop-left"){
            this->state = "drop";
        }
        else if(this->bouncing && state!="hop-right" && state!="hop-left"){
            this->state = "bound";
        }
        // else if(this->falling || this->bouncing){
        //         this->state = this->direction=="left" ? "hop-left" : "hop-right";
        // }
        else if(state!="hop-left" && state!="hop-right"){
            this->state = this->direction;
        }
    }
    // cout << state << endl;
    walk();
    collider->update(x+4+game.offset.x, y+game.offset.y);
}

void Enemy::render(){
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
    SDL_SetTextureBlendMode(cache[state][frame], SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, cache[state][frame], &src, &dest);
}

void Enemy::compile(){
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
        this->cache.insert(make_pair(s, images));
    }
    SDL_SetRenderTarget(renderer, NULL);
}

bool Enemy::collision(Collider complement){

    return true;
}

// int Enemy::index(double x, double y){ return 0; }
// int Enemy::adjacent(int direction){ return 0; }
// int Enemy::adjacent(int direction, double x, double y){ return 0; }
// bool Enemy::traverse(int direction){ return true; }
// bool Enemy::traverse(int direction, double x, double y){ return true; }
// void Enemy::align(){}
void Enemy::deaded(){}

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

int Enemy::adjacent(int direction, double _x, double _y){
    int a, shift;
    // _x = _x==NULL ? x+(width/2)*SCALE  : _x;
    // _y = _y==NULL ? y+(height/2)*SCALE : _y;

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
    int i = index(x,y);
    int col = i % LEVEL_WIDTH;

    if(col%17<2 && col%17!=0){
        x = ( col + (col%17)-1  )*BYTE*SCALE  + width;
    }
    else{
        // cout << col%17 << endl;
        x = ( col + 1 - (col%17) )*BYTE*SCALE + width;
    }
}

bool Enemy::traverse(int direction){
    int tile = adjacent(direction, NULL, NULL);
    if(tile==0){
        return true;
    }
    return false;
}

bool Enemy::traverse(int direction, double x, double y){
    int tile = adjacent(direction, x, y);
    if(tile==0){
        return true;
    }
    return false;
}

void Enemy::move(){
    // cout << direction << endl;
    if(direction=="left"){
        if(traverse(LEFT)){
            state = "left";
            x -= ENEMY_SPEED;
        }else{
            state = "right";
            direction = "right";
            x += ENEMY_SPEED;
        }
    }
    else if(direction=="right"){
        if(traverse(RIGHT)){
            state = "right";
            x += ENEMY_SPEED;
        }else{
            state = "left";
            direction = "left";
            x -= ENEMY_SPEED;
        }
    }
}

void Enemy::hop(int i){
    y = i<25 ? y - 0.25 : y + 0.25;
    x = direction=="right" ? x + 0.25 : x - 0.25;
}

void Enemy::hopoff(){
    bouncing = false;
    falling = false;

    if(game.delay()){ return; };

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

    if(state=="bound" || state=="drop"){
        if(x<150){
            state = "hop-right";
            direction = "right";
            hopoff();
        }
        else if(x>600){
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
    //     r = rand() % 2;
    //     switch(r){
    //         case 0:
    //             direction = "left";
    //             state = "left";
    //             break;
    //         case 1:
    //             direction = "right";
    //             state = "right";
    //             break;
    //     }
    // }

}

void Enemy::pursue(){

}

void Enemy::decision(){
    wander();
}

void Enemy::bounce(){
    if(bounces==-1){
        tier = rand()%game.tiers;
        bounces = rand()%3;
    }
    else{
        if(game.delay()){
            return;
        }
        if(bounces>0){
            bounces--;
        }
    }
}

void Enemy::trampoline(){
    int row = index(x, y)/LEVEL_HEIGHT;
    int pos = game.tiers-((row-10)/game.tiers);

    if(pos==tier && bounces<=0){
        wander();
    }
}

void Enemy::walk(){
    if(state!="bound" && state!="drop" && state!="hop-left" && state!="hop-right"){
        move();
    }
    else if(state=="bound"){
        trampoline();
    }
    else if(state=="drop"){
        if(direction=="left" && !aligned()){
             x -= ENEMY_SPEED;
        }
        else if(direction=="right" && !aligned()){
            x += ENEMY_SPEED;
        }
    }
    else if(state=="hop-left" || state=="hop-right"){
        hopoff();
    }

    if(game.delay(3000, timestamp)){
        return;
    };

    // cout << "bouncing: " << bouncing << endl;
    // cout << "falling: "  << falling << endl;

    if(state!="bound" && state!="drop" && state!="hop-right" && state!="hop-left"){
        decision();
    }

    timestamp = SDL_GetTicks();
}

