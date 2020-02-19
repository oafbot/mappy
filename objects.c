#include "game.hpp"
using namespace std;

GameObject::GameObject(){}

void GameObject::init(){
    int col, row;

    for(int i=0; i<group.size(); i++){
        if(i==0){
            col = group[i] % LEVEL_WIDTH;
            row = floor(group[i] / LEVEL_WIDTH);
            this->x = col*BYTE*SCALE;
            this->y = row*BYTE*SCALE;
        }
    }
}

void GameObject::define(string name, vector< array<array<int, TILE_SIZE>, FRAMES> > frames){
    this->states.insert(make_pair(name.c_str(), frames));
}

void GameObject::compile(){
    for(map< string, vector< array< array<int, TILE_SIZE>, FRAMES> > >::iterator it=states.begin(); it!=states.end(); ++it){
        string s = it->first;
        array<SDL_Texture*, FRAMES> images;

        for(int f=0; f<FRAMES; f++){
            SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width*SCALE, height*SCALE);
            SDL_SetRenderTarget(renderer, texture);

            for(int i=0; i<layout.size(); i++){
                draw(it->second[i][f], layout[i]);
            }

            images[f] = texture;
        }
        this->cache.insert(make_pair(s, images));
    }
    SDL_SetRenderTarget(renderer, NULL);
}

void GameObject::draw(const array<int, TILE_SIZE> &bits, Coordinates offset){
    string color;
    SDL_Rect r;

    int w = SCALE;
    int row;
    int col;
    int bit;
    int alpha = 255;

    for(int i=0; i<TILE_SIZE; i++) {
        bit = bits[i];
        if(bit){
            color = data.palette[bit];
            SDL_Color c = hex2sdl(color);

            col = i % BYTE;
            row = floor(i / BYTE);

            r.x = offset.x*SCALE + w*col;
            r.y = offset.y*SCALE + w*row;
            r.w = w;
            r.h = w;
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, alpha);
            SDL_RenderFillRect(renderer, &r);
        }
    }
}

array<int, TILE_SIZE> GameObject::flip(const array<int, TILE_SIZE> bits){
    array<int, TILE_SIZE> flipped;
    array<array<int, BYTE>, TILE_SIZE/BYTE> rows;
    array<int, TILE_SIZE> flat;
    int row;
    int col;

    for(int i=0; i < bits.size(); i++){
        col = i % BYTE;
        row = floor(i / BYTE);

        if(col==0){
            array<int, BYTE> r;
            rows[row] = r;
        }
        rows[row][col] = bits[i];
    }

    for(int k=0; k<rows.size(); k++){
        reverse(rows[k].begin(), rows[k].end());
        for(int l=0; l<rows[k].size(); l++){
            flat[k*BYTE+l] = rows[k][l];
        }
    }
    return flat;
}

void GameObject::assign(int index){
    group.push_back(index);
    group.push_back(index+1);
    group.push_back(index+2);
    cout << index << "\n";
    init();
}

void GameObject::reset(){
    this->frame = 5;
    this->animated = false;
    this->cycle = 0;
}

void GameObject::render(){
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
            if(loop==true || cycle<repeat){
                frame = 0;
                cycle++;
            }
            else{
                reset();
            }
        }
        else{
            frame++;
        }
    }
    SDL_RenderCopy(renderer, cache[state][frame], &src, &dest);
}

Trampoline::Trampoline(){
    this->state = "green";
    this->frame = 5;
    this->width = 3*BYTE;
    this->height = BYTE;
    this->assigned = false;
    this->animated = false;
    this->loop = false;
    this->repeat = 0;
    this->cycle = 0;
    this->bounces = 0;
    this->active = false;
    this->jumper = "";
    array<array<int, TILE_SIZE>, BITMAP_SIZE> d = data.objects;
    vector< array<array<int, TILE_SIZE>, FRAMES> > g;

    array<array<int, TILE_SIZE>, FRAMES>
    left   = {d[3], d[4], d[5], d[4], d[3], d[2], d[1], d[0]},
    center = {d[9], d[10], d[11], d[10], d[9], d[8], d[7], d[6]},
    right  = {flip(d[3]), flip(d[4]), flip(d[5]), flip(d[4]), flip(d[3]), flip(d[2]), flip(d[1]), flip(d[0])};

    Coordinates a = {0, 0}, b = {BYTE, 0}, c = {BYTE*2, 0};

    g.push_back(left);
    g.push_back(center);
    g.push_back(right);

    layout.push_back(a);
    layout.push_back(b);
    layout.push_back(c);

    define("green", g);
    define("yellow", changeColor(g, 37));
    define("orange", changeColor(g, 29));
    define("red",    changeColor(g, 30));
    define("broken", changeColor(g, 0));
}

void Trampoline::assign(int index){
    group.push_back(index);
    group.push_back(index+1);
    group.push_back(index+2);
    init();
}

void Trampoline::render(){
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
            bounce();
            if(loop==true || cycle<repeat){
                frame = 0;
                cycle++;
            }
            else{
                reset();
            }
        }
        else{
            frame++;
        }
    }

    SDL_RenderCopy(renderer, cache[state][frame], &src, &dest);
}

vector< array<array<int, TILE_SIZE>, FRAMES> >
Trampoline::changeColor(vector< array<array<int, TILE_SIZE>, FRAMES> > grouped, int color){
    vector< array<array<int, TILE_SIZE>, FRAMES> > replace;
    for(int i=0; i<grouped.size(); i++){
        array<array<int, TILE_SIZE>, FRAMES> array;
        for(int f=0; f<FRAMES; f++){
            for(int b=0; b<TILE_SIZE; b++){
                if(grouped[i][f][b]!=0){
                    array[f][b] = color;
                }else{
                    array[f][b] = 0;
                }
            }
        }
        replace.push_back(array);
    }
    return replace;
}

void Trampoline::init(){
    int col, row;
    for(int i=0; i<group.size(); i++){
        if(i==0){
            col = group[i] % LEVEL_WIDTH;
            row = floor(group[i] / LEVEL_WIDTH);
            this->x = col*BYTE*SCALE;
            this->y = (row-1)*BYTE*SCALE;
        }
    }
}

void Trampoline::reset(){
    this->cycle = 0;
    this->frame = 5;
    this->animated = false;
    this->active = false;
}

void Trampoline::bounce(){
    // cout<<jumper<<endl;
    if(jumper=="player" && bounces<=BOUNCES){
        bounces++;
    }
    switch(bounces){
        case 0:
            state = "green";
            break;
        case 1:
            state = "yellow";
            break;
        case 2:
            state = "orange";
            break;
        case 3:
            state = "red";
            break;
        case 4:
            state = "broken";
            break;
    }

    active = false;
}