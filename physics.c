#include "game.hpp"
using namespace std;

Physics::Physics(){}

void Physics::update(){
    gravitation.player->update();

    // for(int i=0; i < gravitation.enemies.size(); i++) {
    //     gravitation.enemies[i]->update();
    // }
}

template <class T>
Gravity<T> Physics::gravity(T* s, float factor,  int delay){
    return * new Gravity<Player>(s, factor, delay);
}
// Gravity Physics::gravity(Player s, float factor,  int delay){
//     return * new Gravity(s, factor, delay);
// }

template<class T>
Gravity<T>::Gravity(T* s, float factor,  int delay): sprite(s){
    cout << typeid(s).name() << endl;
    cout << s->x << ", " << s->y << endl;
    this->gravity  = /*0.98**/factor;
    this->buoyancy = 0.098*2*factor;
    this->lift  = 0;
    this->speed = 0;
    this->delay = delay ? delay : 0;
    this->min = SCALE;
    this->max = BYTE;
    this->type = "player";
    // this->sprite = s;
    // this->sprite = unique_ptr<Player>( dynamic_cast<Player*>(s) );
    // this->sprite = dynamic_cast<Player *>(s);
    this->sprite->gravitation = true;
    cout << typeid(sprite).name() << endl;
    cout << sprite->index(sprite->x, sprite->y) << endl;
}

// void Gravity::bind(Player* s){
//     this->sprite = s;
//     this->sprite->gravitation = true;
// }
template <class T>
bool Gravity<T>::fallthru(){
    int index = sprite->index(sprite->x, sprite->y)+LEVEL_WIDTH;

    for(int t=0; t<game.objects["trampoline"].size(); t++){
        vector<int> group = game.objects["trampoline"][t].group;
        // When the element is not found, std::find returns the end of the range
        if(find(begin(group), end(group), index) != end(group)){
            if(game.objects["trampoline"][t].bounces>=BOUNCES){
                speed = 0.5;
                sprite->frame = 0;
                sprite->state = "spin";
                game.objects["trampoline"][t].bounce();
                return true;
            }
        }
    }
    // cout << sprite->x << " " << sprite->y << " " << speed << " " <<  gravity <<endl;
    return sprite->traverse(DOWN, sprite->x, sprite->y + speed + gravity);
}

template <class T>
void Gravity<T>::update(){
    int tile, index;
    vector<int> group;

    if(!game.PAUSED && delay==0 && sprite->gravitation){
        double ground = game.stage.bottom - (sprite->height*SCALE);
        double s = sprite->y + speed + gravity;

        if(!sprite->bouncing && fallthru()){
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
                for(int t=0; t<game.objects["trampoline"].size(); t++){
                    game.objects["trampoline"][t].reset();
                    game.objects["trampoline"][t].bounces = 0;
                    game.objects["trampoline"][t].state = "green";
                    game.objects["trampoline"][t].active = false;
                }
            }
            sprite->falling = false;
            speed = 0;
        }

        tile = sprite->adjacent(DOWN, sprite->x, sprite->y);

        // if(tile != 0){
        //     cout << tile << endl;
        // }

        if(tile==2){
            index = sprite->index(sprite->x, sprite->y)+LEVEL_WIDTH;

            for(int t=0; t<game.objects["trampoline"].size(); t++){
                group = game.objects["trampoline"][t].group;
                // When the element is not found, std::find returns the end of the range
                if(find(begin(group), end(group), index) != end(group)){
                    //cout << distance(group, find(begin(group), end(group), index)) << endl;
                    game.objects["trampoline"][t].frame = 0;
                    game.objects["trampoline"][t].animated = true;
                    // cout << game.objects["trampoline"][t].bounces << endl;
                    if(game.objects["trampoline"][t].bounces<BOUNCES && !game.objects["trampoline"][t].active){
                        game.objects["trampoline"][t].active = true;
                        bound();
                    }
                    else if(game.objects["trampoline"][t].active){
                        // cout << "active" << endl;
                        // pass
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

template <class T>
void Gravity<T>::reset(){
    this->speed = 0;
}

template <class T>
void Gravity<T>::bound(){
    sprite->falling = false;
    sprite->bouncing = true;

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