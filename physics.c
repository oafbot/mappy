#include "game.hpp"
using namespace std;

Physics::Physics(){}

void Physics::update(){
    player.gravity->update(&player);

    for(int i=0; i < game.enemies.size(); i++){
        game.enemies[i].gravity->update(&game.enemies[i]);
    }
}

Gravity* Physics::gravity(float factor,  int delay){
    return new Gravity(factor, delay);
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

template <class T>
bool Gravity::fallthru(T* sprite){
    int index = sprite->index(sprite->x, sprite->y)+LEVEL_WIDTH;

    for(int t=0; t<game.objects["trampoline"].size(); t++){
        vector<int> group = game.objects["trampoline"][t].group;

        if(find(begin(group), end(group), index) != end(group)){
            if(game.objects["trampoline"][t].bounces>=BOUNCES && sprite->type=="player"){
                speed = 0.5;
                sprite->frame = 0;
                sprite->state = "spin";
                game.objects["trampoline"][t].bounce();
                return true;
            }
        }
    }
    return sprite->traverse(DOWN, sprite->x, sprite->y + speed + gravity);
}

template <class T>
void Gravity::update(T* sprite){
    int tile, index;
    vector<int> group;

    if(!game.PAUSED && delay==0 && sprite->gravitation){
        double ground = game.stage.bottom - (sprite->height*SCALE);
        double s = sprite->y + speed + gravity;

        if(!sprite->bouncing && fallthru(sprite)){
            lift = 0;

            if(s < ground){
                speed += speed+gravity<max ? gravity : 0;
                sprite->y += round(speed);
                sprite->falling = true;
            }
            else{
                sprite->y = ground;
                sprite->falling = false;
                speed = 0;
            }
        }
        else{
            if(!sprite->bouncing){
                sprite->align();
                if(sprite->type=="player"){
                    for(int t=0; t<game.objects["trampoline"].size(); t++){
                        game.objects["trampoline"][t].reset();
                        game.objects["trampoline"][t].bounces = 0;
                        game.objects["trampoline"][t].state = "green";
                        game.objects["trampoline"][t].active = false;
                    }
                }
            }
            sprite->falling = false;
            speed = 0;
        }

        tile = sprite->adjacent(DOWN, sprite->x, sprite->y);

        if(tile==2){
            index = sprite->index(sprite->x, sprite->y)+LEVEL_WIDTH;

            for(int t=0; t<game.objects["trampoline"].size(); t++){
                group = game.objects["trampoline"][t].group;

                if(find(begin(group), end(group), index) != end(group)){
                    game.objects["trampoline"][t].frame = 0;
                    game.objects["trampoline"][t].animated = true;

                    if(game.objects["trampoline"][t].bounces<BOUNCES && !game.objects["trampoline"][t].active){
                        game.objects["trampoline"][t].active = true;
                        bound(sprite);
                    }
                    else if(game.objects["trampoline"][t].active){
                        // cout << "active" << endl;
                        // pass
                        sprite->bounce();
                    }
                    else{
                        sprite->deaded();
                    }
                }
                else{
                    // Not found
                }
            }
        }

        if(sprite->bouncing){
            if( game.delay() ){ return; }

            sprite->y -= lift;

            tile = sprite->adjacent(UP, sprite->x, sprite->y);

            if(tile!=0){
                lift = 0;
                sprite->state = "turn";
                sprite->falling = true;
                sprite->bouncing = false;
            }
        }
    }
}

void Gravity::reset(){
    this->speed = 0;
}

template <class T>
void Gravity::bound(T* sprite){
    sprite->falling = false;
    sprite->bouncing = true;

    if(sprite->type=="player")
        cout << sprite->x << endl;

    sprite->align(true);
    sprite->bounce(true);

    if(lift==0){
        lift = max*3;
    }

    // else{
    //     lift = lift>min ? lift - gravity : min;
    // }
};


Collider::Collider(Sprite* obj){
    this->object = obj;
}

void Collider::init(double x, double y, int w, int h){
    this->x = x;
    this->y = y;
    this->width  = w*SCALE;
    this->height = h*SCALE;
}

void Collider::update(double x, double y){
    this->x = x;//ffset ? this->object->x+game.offset.x : this->object->x;
    this->y = y;//offset ? this->object->y+game.offset.y : this->object->y;
}

bool Collider::check(Collider collider){
    // if(object->bouncing || object->falling || collider.object->falling || collider.object->bouncing){
    //     return false;
    // }
    double ax0 = x,
           ax1 = x + width,
           ay0 = y,
           ay1 = y + height,
           bx0 = collider.x,
           bx1 = collider.x + collider.width,
           by0 = collider.y,
           by1 = collider.y + collider.height;
    return !( ax1 < bx0 || ax0 > bx1 || ay0 > by1 || ay1 < by0 );
}