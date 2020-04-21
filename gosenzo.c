#include "game.hpp"
using namespace std;

Gosenzo::Gosenzo() : Enemy(11){
    array <array<int, SPRITE_SIZE>, BITMAP_SIZE> b = data.sprites;
    array<array<int, SPRITE_SIZE>, FRAMES>
    f0 = {b[38], b[39], b[40], b[39], b[38], b[39], b[40], b[39]}, // left, right
    f1 = {b[41], b[42], b[43], b[42], b[41], b[42], b[43], b[42]}; // up, down

    define("horizontal", f0);
    define("vertical", f1);

    this->width  = DIM;
    this->height = DIM;

    this->id          = 11;
    this->gravitation = false;
    this->falling     = false;
    this->bouncing    = false;
    this->animated    = true;
    this->direction   = "left";
    this->state       = "horizontal";
    this->type        = "gosenzo";
    this->mode        = "ROAM";
    this->frame       = 0;
    this->bounces     = -1;
    this->captured    = false;
    this->collider    = new Collider<Gosenzo>(this);
    this->cache       = &game.cache.gosenzo;

    this->compile();
    game.cache.reward = Draw::compile(data.items[16], DIM);


    collider->init(x + game.offset.x, y + game.offset.y, width, height);

    // collider->offset = 0;
};

void Gosenzo::update(){
    if(active){
        walk();
        collider->update(x+game.offset.x, y+game.offset.y);
    }
}

void Gosenzo::define(string name, array<array<int, SPRITE_SIZE>, FRAMES> frames){
    this->states.insert(make_pair(name.c_str(), frames));
}

void Gosenzo::draw(const array<int, SPRITE_SIZE> &bits){
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

void Gosenzo::compile(){
    if(!contains(game.cached, type)){
        for(map< string, array< array<int, SPRITE_SIZE>, FRAMES> >::iterator it=states.begin(); it!=states.end(); ++it){
            string s = it->first;
            array<SDL_Texture*, FRAMES> images;

            for(int f=0; f<FRAMES; f++){
                SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width*SCALE, height*SCALE);
                SDL_SetRenderTarget(renderer, texture);
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

                draw(it->second[f]);

                images[f] = texture;
            }
            cache->insert(make_pair(s, images));
        }
        SDL_SetRenderTarget(renderer, NULL);
        game.cached.push_back(type);
    }
}

void Gosenzo::render(){
    if(active){
        SDL_Rect dest, src;
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
        SDL_RenderCopy(renderer, (*cache)[state][frame], &src, &dest);
    }
}

// void Gosenzo::decision(){
//     // mode = "ROAM";
//     // collider->passthru = false;
//     string horiz, vert;

//     if(player.x < x){
//         horiz = "left";
//     }
//     else if(player.x > x){
//         horiz = "right";
//     }
//     else{
//         horiz = "none";
//     }

//     if(player.y < y){
//         vert = "up";
//     }
//     else if(player.y > y){
//         vert = "down";
//     }
//     else{
//         vert = "none";
//     }

//     if(vert=="up" && traverse(UP)){
//         direction = "up";
//         state = "vertical";

//     }
//     else if(vert=="down" && traverse(DOWN)){
//         direction = "down";
//         state = "vertical";

//     }
//     else{
//         direction = horiz;

//         if(direction=="left"){
//             if(traverse(LEFT)){
//                 state = "horizontal";
//             }
//             else{
//                 state = "horizontal";
//                 direction = "right";
//             }
//         }
//         else if(horiz=="right"){
//             if(traverse(RIGHT)){
//                 state = "horizontal";
//             }
//             else{
//                 state = "horizontal";
//                 direction = "left";
//             }
//         }
//     }
// }

void Gosenzo::decision(){
    // mode = "ROAM";
    // collider->passthru = false;
    if(adjacent(UP)!=1 && adjacent(UP, x, y-DIM*SCALE)!=1 && direction!="down" && y>160){
        direction = "up";
        state = "vertical";
    }
    else if(adjacent(DOWN)!=1 && adjacent(DOWN, x, y+DIM*SCALE)!=1 && direction!="up" && y<480){
        direction = "down";
        state = "vertical";
    }
    else if(adjacent(LEFT)!=1 /*&& adjacent(LEFT, x-DIM*SCALE, y)!=1*/ && direction!="right"){
        state = "horizontal";
        direction = "left";
    }

    else if(adjacent(RIGHT)!=1 /*&& adjacent(RIGHT, x+DIM*SCALE, y)!=1*/ && direction!="left"){
        state = "horizontal";
        direction = "right";
    }
}

void Gosenzo::move(){
    // cout << direction << endl;
    if(direction=="up" && adjacent(UP)!=1 && y>160){
        y -= BOSS_SPEED*2;
    }
    else if(direction=="down" && adjacent(DOWN)!=1 && y<480){
        y += BOSS_SPEED*2;
    }
    else if(direction=="left" && adjacent(LEFT)!=1){
        x -= BOSS_SPEED*2;
    }
    else if(direction=="right" && adjacent(RIGHT)!=1){
        x += BOSS_SPEED*2;
    }
    else{
        decision();
    }
}

void Gosenzo::fork(){
    int row  = LEVEL_HEIGHT-(index(x, y)/LEVEL_WIDTH);
    int pos  = (row/FLOOR_HEIGHT);
    int plyr = LEVEL_HEIGHT-(player.index(player.x, player.y)/LEVEL_WIDTH);

    if(plyr==row){
        if(player.x < x+game.offset.x+DIM*SCALE){
            direction = "left";
            state = "horizontal";
        }
        else if(player.x > x+game.offset.x-DIM*SCALE){
            direction = "right";
            state = "horizontal";
        }
    }
    else if(rand()%2){
        if(direction=="up" || direction=="down"){
            if(adjacent(LEFT)!=1 /*&& adjacent(LEFT, x-DIM*SCALE, y)!=1*/){
                state = "horizontal";
                direction = "left";
            }
            else if(adjacent(RIGHT)!=1 /*&& adjacent(RIGHT, x+DIM*SCALE, y)!=1*/){
                state = "horizontal";
                direction = "right";
            }
        }
        else{
            if(adjacent(UP)!=1 && adjacent(UP, x, y-DIM*SCALE)!=1){
                state = "vertical";
                direction = "up";
            }
            else if(adjacent(DOWN)!=1 && adjacent(DOWN, x, y+DIM*SCALE)!=1){
                state = "vertical";
                direction = "down";
            }
        }
    }
}

void Gosenzo::walk(){
    move();

    if(game.delay(1000, timestamp)){
        return;
    };

    fork();
    timestamp = SDL_GetTicks();
}

void Gosenzo::reset(){
    this->x = game.center.x;
    this->y = 35*BYTE;
    this->gravitation = false;
    this->falling     = false;
    this->bouncing    = false;
    this->direction   = "left";
    this->state       = "horizontal";
    this->type        = "gosenzo";
    this->frame       = 0;
    this->animated    = true;
    this->active      = false;
    this->bounces     = -1;
    this->collider->passthru = true;
    this->collider->update(x+game.offset.x, y+game.offset.y);
}