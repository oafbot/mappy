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

bool Gravity::fallthru(){
    int index = player->index(player->x, player->y)+LEVEL_WIDTH;

    for(int t=0; t<game.objects["trampoline"].size(); t++){
        vector<int> group = game.objects["trampoline"][t].group;
        // When the element is not found, std::find returns the end of the range
        if(find(begin(group), end(group), index) != end(group)){
            if(game.objects["trampoline"][t].bounces>=BOUNCES){
                speed = 0.5;
                player->frame = 0;
                player->state = "spin";
                game.objects["trampoline"][t].bounce();
                return true;
            }
        }
    }
    return player->traverse(DOWN, player->x, player->y + speed + gravity);
}

void Gravity::update(){
    int tile, index;
    vector<int> group;

    if(!game.PAUSED && delay==0 && player->gravitation){
        double ground = game.stage.bottom - (player->height*SCALE);
        double s = player->y + speed + gravity;

        if(!player->bouncing && fallthru()){
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
                for(int t=0; t<game.objects["trampoline"].size(); t++){
                    game.objects["trampoline"][t].reset();
                    game.objects["trampoline"][t].bounces = 0;
                    game.objects["trampoline"][t].state = "green";
                    game.objects["trampoline"][t].active = false;
                }
            }
            player->falling = false;
            speed = 0;
        }

        tile = player->adjacent(DOWN, player->x, player->y);

        if(tile==2){
            index = player->index(player->x, player->y)+LEVEL_WIDTH;

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
                        player->deaded();
                    }
                }
                else{
                    // Not found
                }
            }
        }

        if(player->bouncing){
            if( game.delay() ){
                return;
            }

            int t = game.trampoline();
            if(t>=0){
                if(game.objects["trampoline"][t].active){
                    game.objects["trampoline"][t].bounce();
                }
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

    if(lift==0){
        lift = max*3;
    }
    // else{
    //     lift = lift>min ? lift - gravity : min;
    // }
};