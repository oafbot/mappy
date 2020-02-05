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
};

void Player::init(double x, double y){
    this->x = x-this->width/2;
    this->y = y-this->height/2;
    this->data = sprites;
}

void Player::move(){
    if( game.controls.left_key_down ) {
        if(this->x>game.stage.left){
            this->x -= SPEED;
        }
        else{
            this->x = game.stage.left;
        }
    }

    if( game.controls.right_key_down ) {
        if(this->x<game.stage.right-this->width*2){
            this->x += SPEED;
        }
        else{
            this->x = game.stage.right-this->width*2;
        }
    }
}

// void Player::position(double x, double:y){

// }

void Player::draw(const array<array<int, SPRITE_SIZE>, FRAMES> &data){
    string color;
    SDL_Rect r;

    int w = SCALE;
    int row;
    int col;
    int bit;
    int alpha;

    for(int i=0; i < data[0].size(); i++) {
        bit = data[current_frame][i];
        if(bit){
            alpha = palette[bit]=="NULL" ? 0 : 255;
            color = palette[bit]=="NULL" ? "#000000" : palette[bit];
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
    // printf("%s\n", name.c_str());
    // printf("%s\n", typeid(frames).name());
}

void Player::update(){
    if(this->falling){
        this->state = "drop";
    }
    else{
        this->state = this->direction;
    }
}

void Player::render(){
    // int ms;

    // current_time = SDL_GetTicks();
    // ms = current_time - last_time;

    // if( ms < ms_per_frame ){
    //     return;
    // }

    // if(this->falling){
    //     this->x = (this->x<0) ? 0 : this->x;
    //     this->x = (this->x>game.stage.right-DIM) ? game.stage.right-DIM*4 : this->x;
    //     this->y = (this->y<0) ? 0 : this->y;
        
    // }else{

    // }

    if(game.controls.key_down){
        if( current_frame >= FRAME_COUNT ) {
            // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            // SDL_RenderClear( renderer );
            current_frame = 0;
        }
        else{
            current_frame++;
        }
    }
    // printf("%s\n", this->state.c_str());
    this->draw(this->states[this->state]);
    this->frame = current_frame;

    // last_time = current_time;
}