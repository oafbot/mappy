#include "game.hpp"
using namespace std;

Player::Player() : Sprite(){
    this->width  = DIM;
    this->height = DIM;

    this->gravitation = true;
    this->falling  = false;
    this->bouncing = false;
    this->direction = "left";
    this->state = "left";
    this->frame = 0;
    this->dead = false;
};

void Player::init(double x, double y){
    this->x = x-this->width/2;
    this->y = y-this->height/2;
    // this->data = data.sprites;
}

// void Player::stopBounce(){
//     bouncing = false;
// };

void Player::move(){
    if( game.controls.left_key_down ) {
        if(traverse(LEFT)){
            if(this->x>game.stage.left){
                if(!game.scrolling){
                    this->x -= SPEED;
                }
                else{
                    game.offset.x += SPEED;
                }
            }
            else{
                this->x = game.stage.left;
            }
            if(bouncing){
                // state = "hop-left";
                if(game.delay()){
                    return;
                };
                bouncing = false;
            }
            // else if(falling){
            //     state = "hop-left";
            // }
        }
    }

    if( game.controls.right_key_down ) {
        if(traverse(RIGHT)){
            if(this->x<game.stage.right-this->width*2){
                if(!game.scrolling){
                    this->x += SPEED;
                }
                else{
                    game.offset.x -= SPEED;
                }
            }
            else{
                this->x = game.stage.right-this->width*2;
            }
            if(bouncing){
                // state = "hop-right";
                if(game.delay()){
                    return;
                };
                bouncing = false;
            }
            // else if(falling){
            //     state = "hop-right";
            // }
        }
    }
}

bool Player::traverse(int direction){
    int tile = adjacent(direction, NULL, NULL);
    if(tile==0){
        return true;
    }
    return false;
}

bool Player::traverse(int direction, double x, double y){
    int tile = adjacent(direction, x, y);
    if(tile==0){
        return true;
    }
    return false;
}

array<double, 2> Player::position(){
    array<double, 2> coord;
    return coord;
}

int Player::index(double x, double y){
    // printf("%f %f\n", x, y);
    // printf("%f\n", game.offset.x);
    int tile = BYTE*SCALE;
    double diff = game.stage.width-(LEVEL_WIDTH*tile);
    double xpos;
    xpos = x + (width/2)*SCALE - game.offset.x;

    int col = (int)floor(xpos/tile);
    int row = (int)floor((y+height-game.offset.y)/tile);
    return col + row*LEVEL_WIDTH;
}

int Player::adjacent(int direction, double _x, double _y){
    int a, shift;
    _x = _x==NULL ? x+(width/2)*SCALE - game.offset.x : _x;
    _y = _y==NULL ? y+(height/2)*SCALE - game.offset.y: _y;

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

void Player::align(){
    if(((int)y)%LONG!=0){
        y = (int)(floor(y)/LONG)*LONG;
    }
}

void Player::draw(const array<array<int, SPRITE_SIZE>, FRAMES> &bits){
    string color;
    SDL_Rect r;

    int w = SCALE;
    int row;
    int col;
    int bit;
    int alpha;

    for(int i=0; i < bits[0].size(); i++) {
        bit = bits[frame][i];
        if(bit){
            alpha = data.palette[bit]=="NULL" ? 0 : 255;
            color = data.palette[bit]=="NULL" ? "#000000" : data.palette[bit];
            SDL_Color c = hex2sdl(color);

            col = i % DIM;
            row = floor(i / DIM);

            r.x = this->x + w*col;
            r.y = this->y + w*row;
            r.w = w;
            r.h = w;
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, alpha);
            SDL_RenderFillRect(renderer, &r );
        }
    }
}

array<array<int, SPRITE_SIZE>, FRAMES> Player::flip(array<array<int, SPRITE_SIZE>, FRAMES> frames){
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

void Player::define(string name, array<array<int, SPRITE_SIZE>, FRAMES> frames){
    this->states.insert(make_pair(name.c_str(), frames));
}

void Player::update(){
    if(state!="dead" && state!="spin"){
        if(this->falling && !game.controls.key_down){
            this->state = "drop";
        }
        else if(this->bouncing && !game.controls.key_down){
            this->state = "bound";
        }
        else{
            if(this->falling || this->bouncing){
                this->state = this->direction=="left" ? "hop-left" : "hop-right";
            }
            else{
                this->state = this->direction;
            }
        }
    }
    else if(state=="spin"){

    }


    if(game.scrolling && (game.offset.x>=64 || game.offset.x<=-128)){
        game.scrolling = false;
    }
    else if(x>game.center.x-64 && x+width<game.center.x+64){
         game.scrolling = true;
    }
}

void Player::render(){
    if(game.controls.key_down || state=="spin" || state=="dead"){
        if( frame >= FRAME_COUNT ) {
            if(state=="spin"){
                state = "dead";
            }
            else{
                frame = 0;
            }
        }
        else{
            frame++;
        }
    }
    this->draw(this->states[this->state]);
}

void Player::deaded(){
    state = "spin";
    bouncing = false;
    falling = false;
    dead = true;
}