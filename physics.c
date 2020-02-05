#include "game.hpp"
using namespace std;

Physics::Physics(){}

void Physics::update(){
    for(int i=0; i < this->dropable.size(); i++) {
        dropable[i].update();
    }
}

Gravity Physics::gravity(float factor,  int delay){
    return * new Gravity(factor, delay);
}

Gravity::Gravity(float factor,  int delay){
    this->gravity  = 0.98*factor;
    this->buoyancy = 0.098*2*factor;
    this->lift  = 0;
    this->speed = 0;
    this->delay = delay ? delay : 0;
}

void Gravity::bind(Player* p){
    this->player = p;
    this->player->gravitation = true;
}

void Gravity::update(){
    if(!game.PAUSED && this->delay==0 && this->player->gravitation){
        // if(this->player->falling){
        //     this->player->y += this->speed;
        // }

        if(this->player->y + this->speed < game.stage.bottom - (this->player->height*SCALE)){
            this->speed += this->gravity;
            this->player->y += this->speed;
            this->player->falling = true;
            // this->player->render();
            // this->player->y = (this->player->y>game.stage.bottom-this->player->height*SCALE) ? game.stage.bottom-this->player->height*SCALE: this->player->y;
        }
        else{
            this->player->y = game.stage.bottom-this->player->height*SCALE;
            this->speed = 0;
            this->player->falling = false;
        }
        // this->lift = 0;
    }
}

void Gravity::reset(){
    this->speed = 0;
}

