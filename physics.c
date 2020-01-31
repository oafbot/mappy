#include "game.hpp"
using namespace std;

// Physics::Physics(){
//     this->gravity = new Gravity(1, 0);
// }

// Physics::Physics() : gravity(1, 0){
//    // this->gravity = new Gravity(1, 0);
//    // printf("hello world");
//     this->gravity.gravity  = 0.98;
//     this->gravity.buoyancy = 0.098*2;
//     this->gravity.lift  = 0;
//     this->gravity.speed = 0;
//     this->gravity.delay = 0;
//     // this->gravity.sprites.push_back(player);
//     // vector<Sprite*>& temp(size,0);
//     // this->sprites = temp;
// }

void Physics::update(){
    for(int i=0; i < this->dropable.size(); i++) {
        dropable[i].update();
    }
}

Gravity* Physics::gravity(float factor,  int delay){
    return new Gravity(factor, delay);
}

Gravity::Gravity(float factor,  int delay) : sprite(player){
    this->gravity  = 0.98*factor;
    this->buoyancy = 0.098*2*factor;
    this->lift  = 0;
    this->speed = 0;
    this->delay = delay ? delay : 0;
}

// void Physics::Gravity::bind(Sprite* sprite){
//     // printf("%s\n", typeid(sprite).name());
//     this->sprites.push_back(sprite);
//     sprite->gravitate = true;
//     // return sprites.size();
// }
void Gravity::bind(Player sprite){
    printf("%s\n", typeid(sprite).name());
    this->sprite = &sprite;
    printf("%s\n", typeid(this->sprite).name());
    sprite.gravitation = true;
}

void Gravity::bind(Sprite sprite){
    printf("%s\n", typeid(sprite).name());
    this->sprite = &sprite;
    printf("%s\n", typeid(this->sprite).name());
    sprite.gravitation = true;
}

void Gravity::update(){
    // printf("%s\n", typeid(this->sprite).name());
    if(!game->PAUSED && this->delay==0 && this->sprite->gravitation){
        this->speed += this->gravity;
        // sign = game.controls.direction == "left" ? -1 : 1;

        if(this->sprite->falling){
            this->sprite->y += this->speed;
        }

        if(this->sprite->y < stage.bottom - (this->sprite->height*2 + this->speed)){
            this->sprite->y += this->speed;
            this->sprite->falling = true;
            this->sprite->render();
        }
        else{
            this->speed = 0;
            this->sprite->falling = false;
        }
        this->lift = 0;
            // this->speed = this->sprites[i].x > stage.left && this->sprites[i].x < stage.right ? sign*this.momentum : 0;
    }
    // printf("%f\n", this->sprite->y);
    // else{
    //     // this->float(this->delay);
    // }
}

void Gravity::reset(){
    this->speed = 0;
    // this.x = sprite.x();
    // this.y = sprite.y();
}

