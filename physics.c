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
    this->gravity  = /*0.98**/factor;
    this->buoyancy = 0.098*2*factor;
    this->lift  = 0;
    this->speed = 0;
    this->delay = delay ? delay : 0;
    this->min = SCALE;
    this->max = BYTE;
}

void Gravity::bind(Player* p){
    this->player = p;
    this->player->gravitation = true;
}

void Gravity::update(){
    int tile;
    if(!game.PAUSED && delay==0 && player->gravitation){
        double ground = game.stage.bottom - (player->height*SCALE);
        double s = player->y + speed + gravity;

        if(!player->bouncing && player->traverse(DOWN, player->x, s)){
            lift = 0;

            if(s < ground){
                speed += speed+gravity<max ? gravity : 0;
                player->y += round(speed);
                player->falling = true;
                // printf("%f\n", player->y);
            }
            else{
                player->y = ground;
                player->falling = false;
                speed = 0;
            }
        }
        else{
            if(!player->bouncing){
                player->align();
            }
            player->falling = false;
            speed = 0;
        }

        tile = player->adjacent(DOWN, player->x, player->y);

        if(tile==2){
            bound();
        }

        if(player->bouncing){
            if( game.delay() ){
                return;
            }

            player->y -= lift;

            tile = player->adjacent(UP, player->x, player->y);

            if(tile!=0){
                lift = 0;
                player->falling = true;
                player->bouncing = false;
                player->state = "drop";
            }
        }
    }
}

void Gravity::reset(){
    this->speed = 0;
}

void Gravity::bound(){
    player->falling = false;
    player->bouncing = true;
    // player->state = "bound";

    if(lift==0){
        lift = max*3;
    }
    // else{
    //     lift = lift>min ? lift - gravity : min;
    // }
};