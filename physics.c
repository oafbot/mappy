#include "game.hpp"
using namespace std;

// Physics::Physics(){
//     this->gravity = new Gravity(1, 0);
// }

Physics::Physics() : gravity(1, 0){
   // this->gravity = new Gravity(1, 0);
   // printf("hello world");
    this->gravity.gravity  = 0.98;
    this->gravity.buoyancy = 0.098*2;
    this->gravity.lift  = 0;
    this->gravity.speed = 0;
    this->gravity.delay = 0;
    // this->gravity.sprites.push_back(player);
    // vector<Sprite*>& temp(size,0);
    // this->sprites = temp;
}

Physics::Gravity::Gravity(float factor,  int delay/*, double ground*/){
    this->gravity  = 0.98;
    this->buoyancy = 0.098*2;
    this->lift  = 0;
    this->speed = 0;
    this->delay = delay ? delay : 0;
    // this->sprites.push_back(player);
    // vector<Sprite*>& sprites(0,);
    // this->sprites = sprites;
    // this->ground = ground;
    // 
}

// void Physics::Gravity::bind(Sprite* sprite){
//     // printf("%s\n", typeid(sprite).name());
//     this->sprites.push_back(sprite);
//     sprite->gravitate = true;
//     // return sprites.size();
// }

void Physics::Gravity::bind(Player* sprite){
    // printf("%s\n", typeid(sprite).name());
    this->sprites.push_back(sprite);
    sprite->gravitate = true;
    // return sprites.size();
}

void Physics::Gravity::update(){
    if(!game->PAUSED && this->delay==0){
        // printf("%f\n", this->gravity);
        this->speed += 0.98/8; //
        // sign = game.controls.direction == "left" ? -1 : 1;
        // printf("%s\n", typeid(this->sprites[0]).name());

        for(int i=0; i < this->sprites.size(); i++) {
            if(this->sprites[i]->fall){
                this->sprites[i]->y += this->speed;
            }
            // printf("%f\n", this->sprites[i]->y);

            if(this->sprites[i]->y < stage.bottom - (this->sprites[i]->height*2 + this->speed)){
                this->sprites[i]->y += this->speed;
                this->sprites[i]->fall = true;
                this->sprites[i]->render();
            }
            else{
                this->speed = 0;
                this->sprites[i]->fall = false;
            }
            this->lift = 0;
            // this->speed = this->sprites[i].x > stage.left && this->sprites[i].x < stage.right ? sign*this.momentum : 0;
        }
    }
    // else{
    //     // this->float(this->delay);
    // }
}

void Physics::Gravity::reset(){
    this->speed = 0;
    // this.x = sprite.x();
    // this.y = sprite.y();
}

