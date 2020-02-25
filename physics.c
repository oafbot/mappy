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

    for(int t=0; t<game.trampolines.size(); t++){
        vector<int> group = game.trampolines[t].group;

        if(find(begin(group), end(group), index) != end(group)){
            game.trampolines[t].bounce();

            if(game.trampolines[t].bounces>=BOUNCES && sprite->type=="player"){
                speed = 0.5;
                sprite->frame = 0;
                sprite->state = "spin";
                return true;
            }
        }
    }
    return sprite->traverse(DOWN);
}

template <class T>
void Gravity::update(T* sprite){
    int tile, index;
    vector<int> group;

    if(!game.PAUSED && delay==0 && sprite->gravitation){
        double ground = game.stage.bottom - (sprite->height*SCALE);
        double s = sprite->y + speed + gravity;

        if(!sprite->bouncing && fallthru(sprite)){
            // Falling
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
            // Not Falling
            if(!sprite->bouncing){
                sprite->align();

                if(sprite->type=="player"){
                    for(int t=0; t<game.trampolines.size(); t++){
                        game.trampolines[t].reset();
                        game.trampolines[t].bounces = 0;
                        game.trampolines[t].state   = "green";
                        game.trampolines[t].active  = false;
                        game.trampolines[t].jumper  = "";
                    }
                }
            }
            sprite->falling = false;
            speed = 0;
        }

        tile = sprite->adjacent(DOWN);

        if(tile==2){
            // Bouncing
            index = sprite->index(sprite->x, sprite->y)+LEVEL_WIDTH;

            for(int t=0; t<game.trampolines.size(); t++){
                group = game.trampolines[t].group;

                if(find(begin(group), end(group), index) != end(group)){
                    game.trampolines[t].frame = 0;
                    game.trampolines[t].animated = true;
                    game.trampolines[t].jumper = sprite->type;

                    if(game.trampolines[t].bounces<BOUNCES && !game.trampolines[t].active){
                        game.trampolines[t].active = true;
                        bound(sprite);
                    }
                    else if(game.trampolines[t].active){
                        // cout << "active" << endl;
                        // pass
                    }
                    else if(sprite->type=="player"){
                        sprite->deaded();
                    }
                    else if(sprite->type=="enemy"){
                        game.trampolines[t].active = true;
                        bound(sprite);
                    }

                }
            }
        }

        if(sprite->bouncing){
            if( game.delay() ){ return; }

            sprite->y -= lift;

            tile = sprite->adjacent(UP);

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

    // if(sprite->type=="player") cout << sprite->x << endl;

    sprite->align(true);
    sprite->bounce();

    if(lift==0){
        lift = max*2;
    }
};


template <class T>
Collider<T>::Collider(T* obj){
    this->object = obj;
}

template <class T>
void Collider<T>::init(double x, double y, int w, int h){
    this->offset = w/8;
    this->x = x + offset;
    this->y = y;
    this->width  = w*SCALE-(offset*4);
    this->height = h*SCALE;
    this->passthru = false;
}

template <class T>
void Collider<T>::update(double x, double y){
    this->x = x + offset;
    this->y = y;
}

template <class T>
void Collider<T>::debug(){
    SDL_Rect r;
    r.x = x + offset;
    r.y = y;
    r.w = width;
    r.h = height;
    if(passthru)
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    else
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &r);
}

template <class T>
template <class T2>
bool Collider<T>::check(Collider<T2>* collider){
    if(this->passthru || collider->passthru){
        return false;
    }
    // if(object->state=="hop-right" || collider.object->state=="hop-right" ||
    //    object->state=="hop-left"  || collider.object->state=="hop-left"  ||
    //    object->falling            || collider.object->falling            ||
    //    object->bouncing           || collider.object->bouncing           ||
    //    object->passthru           || collider.object->passthru           ){
    //     return false;
    // }

    // cout << object->passthru << collider.object->passthru<<endl;
    double ax0 = x,
           ax1 = x + width,
           ay0 = y,
           ay1 = y + height,
           bx0 = collider->x,
           bx1 = collider->x + collider->width,
           by0 = collider->y,
           by1 = collider->y + collider->height;
    return !( ax1 < bx0 || ax0 > bx1 || ay0 > by1 || ay1 < by0 );
}