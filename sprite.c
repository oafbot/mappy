#include "game.hpp"
using namespace std;

Sprite::Sprite(int x, int y){
    this->x = x-this->width/2;
    this->y = y-this->height/2;

    this->width = DIM;
    this->height = DIM;
};


void Sprite::render(){
    printf("Sprite.render\n");
}