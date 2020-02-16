#include "game.hpp"
using namespace std;

Enemy::Enemy() : Sprite(){
    array <array<int, SPRITE_SIZE>, BITMAP_SIZE> b = data.sprites;
    array<array<int, SPRITE_SIZE>, FRAMES>
    f0 = {b[26], b[27], b[28], b[27], b[26], b[27], b[28], b[27]}; // left, right

    define("left", f0);
    define("right", flip(f0));

    this->width  = DIM;
    this->height = DIM;

    this->gravitation = true;
    this->falling  = false;
    this->bouncing = false;
    this->direction = "left";
    this->state = "left";
    this->frame = 0;
    this->animated = true;
    // this->dead = false;
    this->collider = new Collider(this);
};

void Enemy::init(double x, double y){
    this->x = x;
    this->y = y;
    this->timestamp = SDL_GetTicks();
    collider->init(x+game.offset.x, y+game.offset.y, width, height);
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
    // if(state!="dead" && state!="spin"){
    //     if(this->falling && !game.controls.key_down){
    //         this->state = "drop";
    //     }
    //     else if(this->bouncing && !game.controls.key_down){
    //         this->state = "bound";
    //     }
    //     else{
    //         if(this->falling || this->bouncing){
    //             this->state = this->direction=="left" ? "hop-left" : "hop-right";
    //         }
    //         else{
    //             this->state = this->direction;
    //         }
    //     }
    // }
    // else if(state=="spin"){

    // }


    // if(game.scrolling && (game.offset.x>=64 || game.offset.x<=-128)){
    //     game.scrolling = false;
    // }
    // else if(x>game.center.x-64 && x+width<game.center.x+64){
    //      game.scrolling = true;
    // }
    collider->update(x+game.offset.x, y+game.offset.y);
    walk();
}

void Enemy::render(){
    SDL_Rect dest, src;
    // cout << (player.x + game.offset.x) - (game.offset.x + x) << endl;
    // cout << game.offset.x - 64 << endl;
    // cout << x + game.stage.width - (game.center.x - game.offset.x) << endl;
    // cout << game.offset.x + x + (game.center.x-game.offset.x) << endl;
    dest.x = game.offset.x + x; //+ (game.center.x-game.offset.x)-game.stage.width; //game.offset.x + x + (192);
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

int Enemy::index(double x, double y){ return 0; }
// int Enemy::adjacent(int direction){ return 0; }
int Enemy::adjacent(int direction, double x, double y){ return 0; }
bool Enemy::traverse(int direction){ return true; }
bool Enemy::traverse(int direction, double x, double y){ return true; }
void Enemy::align(){}
void Enemy::deaded(){}


void Enemy::move(){
    cout << direction << endl;
}

void Enemy::wander(){
    int r = rand() % 2;
    switch(r){
        case 0:
            direction = "left";
            move();
            break;
        case 1:
            direction = "right";
            move();
            break;
    }
}

void Enemy::pursue(){

}

void Enemy::decision(){

}

void Enemy::walk(){
    if(game.delay(2000, timestamp)){
        return;
    };
    wander();
    timestamp = SDL_GetTicks();
}

