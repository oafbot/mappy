#include "game.hpp"
using namespace std;

Player::Player() : Sprite(){
    this->width  = DIM;
    this->height = DIM;

    this->gravitation = true;
    this->falling  = false;
    this->bouncing = false;
    this->animated = true;
    this->direction = "left";
    this->state = "left";
    this->type = "player";
    this->frame = 0;
    this->dead = false;
    this->repeat = 2;
    this->cycle = 0;
    this->bounces = 0;
    this->timestamp = SDL_GetTicks();
    this->collider = new Collider<Player>(this);
    this->cache = &game.cache.player;

    array <array<int, SPRITE_SIZE>, BITMAP_SIZE> b = data.sprites;
    array<array<int, SPRITE_SIZE>, FRAMES>
    f0 = {b[0], b[1], b[0], b[1], b[0], b[1], b[0], b[1]}, // left, right
    f1 = {b[2], b[2], b[3], b[3], b[2], b[2], b[3], b[3]}, // drop
    f2 = {b[4], b[4], b[4], b[4], b[4], b[4], b[4], b[4]}, // bound
    f3 = {b[5], b[5], b[5], b[5], b[5], b[5], b[5], b[5]}, // hop
    f5 = {b[7], b[8], b[9], b[10], b[11], b[12], b[13], b[14]}, // spin
    f6 = {b[15], b[15], b[16], b[16], b[15], b[15], b[16], b[16]}; // dead

    define("left", f0);
    define("right", flip(f0));
    define("drop", f1);
    define("bound", f1);
    define("turn", f2);
    define("hop-left", f3);
    define("hop-right", flip(f3));
    define("spin", f5);
    define("dead", f6);

    compile();
};

void Player::init(double x, double y){
    this->x = x;
    this->y = y;
    this->collider->init(x, y, width, height);
}

void Player::reset(double x, double y){
    this->gravitation = true;
    this->falling  = false;
    this->bouncing = false;
    this->direction = "left";
    this->state = "left";
    this->type = "player";
    this->frame = 0;
    this->dead = false;
    this->repeat = 2;
    this->cycle = 0;
    this->bounces = 0;
    this->collider->passthru = true;
    this->x = x;
    this->y = y;
}

void Player::compile(){
    // if(!contains(game.cached, type)){
        for(map< string, array< array<int, SPRITE_SIZE>, FRAMES> >::iterator it=states.begin(); it!=states.end(); ++it){
            string s = it->first;
            array<SDL_Texture*, FRAMES> images;

            for(int f=0; f<FRAMES; f++){
                SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width*SCALE, height*SCALE);
                SDL_SetRenderTarget(renderer, texture);
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

                draw(it->second[f]);

                images[f] = texture;
            }
            cache->insert(make_pair(s, images));
        }
        SDL_SetRenderTarget(renderer, NULL);
        game.cached.push_back(type);
    // }
}

void Player::move(){
    if( game.controls.left_key_down && !game.controls.lock) {
        if(traverse(LEFT)){
            if(this->x>game.stage.left){
                if(!game.scrolling){
                    this->x -= SPEED;
                }
                else{
                    game.offset.x += SPEED;
                }
            }
            else{
                this->x = game.stage.left;
            }
            if(bouncing){
                if( !game.interval() ){ return; }
                bouncing = false;
            }
        }
        else if(bouncing){
            if( !game.interval() ){ return; }
            bouncing = false;
            falling = true;
        }
    }

    if( game.controls.right_key_down && !game.controls.lock) {
        if(traverse(RIGHT)){
            if(this->x<game.stage.right-this->width*2){
                if(!game.scrolling){
                    this->x += SPEED;
                }
                else{
                    game.offset.x -= SPEED;
                }
            }
            else{
                this->x = game.stage.right-this->width*2;
            }
            if(bouncing){
                if( !game.interval() ){ return; }
                bouncing = false;
            }
        }
        else if(bouncing){
            if( !game.interval() ){ return; }
            bouncing = false;
            falling = true;
        }
    }
}

bool Player::traverse(int direction){
    int tile = adjacent(direction);
    if(tile==0){
        if(direction==RIGHT || direction==LEFT){
            int shift = direction==RIGHT ? 1 : -1;

            for(int i=0; i<game.doors.size(); i++){
                if((contains(game.doors[i].group, index(x, y)+shift+LEVEL_WIDTH) ||
                    contains(game.doors[i].group, index(x, y)+shift+LEVEL_WIDTH*2)) && !game.doors[i].open){
                    return false;
                }
            }
        }
        return true;
    }

    if(tile>=RADIO && tile<=BONUS_BALLOON){
        return true;
    }

    if(tile>=DOOR_LEFT && tile<=MAGIC_DOOR_R){
        switch(direction){
            case RIGHT:
                for(int i=0; i<game.doors.size(); i++){
                    if(contains(game.doors[i].group, index(x, y)+1) && game.doors[i].open){
                        return true;
                    }
                }
                break;
            case LEFT:
                for(int i=0; i<game.doors.size(); i++){
                    if(contains(game.doors[i].group, index(x, y)-1) && game.doors[i].open){
                        return true;
                    }
                }
                break;
        }
    }

    return false;
}

bool Player::traverse(int direction, double x, double y){
    int tile = adjacent(direction, x, y);
    if(tile==0){
        int shift = direction==RIGHT ? 1 : -1;

        for(int i=0; i<game.doors.size(); i++){
            if((contains(game.doors[i].group, index(x, y)+shift+LEVEL_WIDTH) ||
                contains(game.doors[i].group, index(x, y)+shift+LEVEL_WIDTH*2)) && !game.doors[i].open){
                return false;
            }
        }

        return true;
    }

    if(tile>=RADIO && tile<=BONUS_BALLOON){
        return true;
    }

    if(tile>=DOOR_LEFT && tile<=MAGIC_DOOR_R){
        switch(direction){
            case RIGHT:
                for(int i=0; i<game.doors.size(); i++){
                    if(contains(game.doors[i].group, index(x, y)+1) && game.doors[i].open){
                        return true;
                    }
                }
                break;
            case LEFT:
                for(int i=0; i<game.doors.size(); i++){
                    // cout << (contains(game.doors[i].group, index(x, y)-1) && game.doors[i].open) << endl;
                    if(contains(game.doors[i].group, index(x, y)-1) && game.doors[i].open){
                        return true;
                    }
                }
                break;
        }
    }
    else{

    }

    return false;
}

void Player::ledge(){
    if(adjacent(DOWN_LEFT)!=1 || adjacent(DOWN_RIGHT)!=1){
        this->collider->passthru = true;
    }
    else{
        this->collider->passthru = false;
    }
}

int Player::index(double x, double y){
    int tile = BYTE*SCALE;
    double diff = game.stage.width-(LEVEL_WIDTH*tile);
    double xpos;
    xpos = x + (width/2)*SCALE - game.offset.x;

    int col = (int)floor(xpos/tile);
    int row = (int)floor((y+height-game.offset.y)/tile);
    return col + row*LEVEL_WIDTH;
}

int Player::index(){
    int tile = BYTE*SCALE;
    double diff = game.stage.width-(LEVEL_WIDTH*tile);
    double xpos;
    xpos = x + (width/2)*SCALE - game.offset.x;

    int col = (int)floor(xpos/tile);
    int row = (int)floor((y+height-game.offset.y)/tile);
    return col + row*LEVEL_WIDTH;
}

int Player::adjacent(int direction){
    int a, shift;

    int i = index(x, y);
    shift = LEVEL_WIDTH;

    switch(direction){
        case UP:
            a = data.interactive[game.level-1][i-shift];
            break;
        case DOWN:
            a = data.interactive[game.level-1][i+shift];
            break;
        case RIGHT:
            a = data.interactive[game.level-1][i+1];
            break;
        case LEFT:
            a = data.interactive[game.level-1][i-1];
            break;
        case DOWN_LEFT:
            a = data.interactive[game.level-1][i-1+shift];
            break;
        case DOWN_RIGHT:
            a = data.interactive[game.level-1][i+1+shift];
            break;
    }
    return a;
}

int Player::adjacent(int direction, double _x, double _y){
    int a, shift;

    int i = index(_x, _y);
    shift = LEVEL_WIDTH;

    switch(direction){
        case UP:
            a = data.interactive[game.level-1][i-shift];
            break;
        case DOWN:
            a = data.interactive[game.level-1][i+shift];
            break;
        case RIGHT:
            a = data.interactive[game.level-1][i+1];
            break;
        case LEFT:
            a = data.interactive[game.level-1][i-1];
            break;
        case DOWN_LEFT:
            a = data.interactive[game.level-1][i-1+shift];
            break;
        case DOWN_RIGHT:
            a = data.interactive[game.level-1][i+1+shift];
            break;
    }
    return a;
}

void Player::align(){
    if(((int)y)%LONG!=0){
        y = (int)(floor(y)/LONG)*LONG;
    }
}

void Player::align(bool horiz){
    // if(((int)x)%LONG!=0){
    //     x = (int)(floor(x)/LONG)*LONG;
    // }
}

void Player::draw(const array<int, SPRITE_SIZE> &bits){
    string color;
    SDL_Rect r;
    int row, col, bit,
    w = SCALE,
    alpha = 255;

    for(int i=0; i<SPRITE_SIZE; i++) {
        bit = bits[i];
        if(bit){
            color = data.palette[bit];
            SDL_Color c = hex2sdl(color);

            col = i % DIM;
            row = floor(i / DIM);

            r.x = w*col;
            r.y = w*row;
            r.w = w;
            r.h = w;
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, alpha);
            SDL_RenderFillRect(renderer, &r);
        }
    }
}

array<array<int, SPRITE_SIZE>, FRAMES>
Player::flip(array<array<int, SPRITE_SIZE>, FRAMES> frames){
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
}

void Player::update(){
    if(state!="dead" && state!="spin"){
        if(this->falling && !game.controls.key_down){
            this->state = "drop";
            this->collider->passthru = true;
        }
        else if(this->bouncing && !game.controls.key_down){
            if(this->y<300){
                this->state = "turn";
            }else{
                this->state = "bound";
            }
            this->collider->passthru = true;
        }
        else{
            if(this->falling || this->bouncing){
                // if(state!="turn")
                this->state = this->direction=="left" ? "hop-left" : "hop-right";
                this->collider->passthru = true;
            }
            else{
                this->state = this->direction;
                this->collider->passthru = false;
            }
        }
    }
    else if(state=="spin"){

    }
    else if(state=="dead"){
        game.RESET = true;
        if(game.delay(3000, timestamp)){
            return;
        }
        game.restart();
    }
    // if(game.scrolling && (game.offset.x>=OFFSET || game.offset.x<=-OFFSET*2)){
    if(game.scrolling && (game.offset.x>OFFSET || game.offset.x<OFFSET*-2)){
        game.scrolling = false;
    }
    else if(x>game.center.x-OFFSET && x+width<game.center.x+OFFSET){
        game.scrolling = true;
    }

    ledge();
    collect();
    collision();
}

void Player::collect(){
    int pos = index(x, y);
    int tile = data.interactive[game.level-1][pos];

    if(tile>=RADIO && tile<=SAFE){
        for(int i=0; i<game.items.size(); i++){
            if(contains(game.items[i].group, pos) && !game.items[i].collected){
                game.items[i].collect();
            }
        }
    }
    else if(tile==BELL){
        for(int i=0; i<game.bells.size(); i++){
            if(contains(game.bells[i].group, pos) && !game.bells[i].dropped){
                game.bells[i].drop();
            }
        }
    }
    else if(tile==BALLOON){
        for(int i=0; i<game.balloons.size(); i++){
            if(contains(game.balloons[i].group, pos) && !game.balloons[i].collected){
                game.balloons[i].collect();
            }
        }
    }
    else if(tile==BONUS_BALLOON){
        if(contains(game.bonus_balloon.group, pos) && !game.bonus_balloon.collected){
            game.bonus_balloon.collect();
        }
    }
}

void Player::render(){
    SDL_Rect dest, src;
    dest.x = x;
    dest.y = y;

    dest.w = width*SCALE;
    dest.h = height*SCALE;

    src.x = 0;
    src.y = 0;
    src.w = width*SCALE;
    src.h = height*SCALE;

    if(game.controls.key_down ||
       state=="spin" || state=="dead" || state=="drop" || state=="bound"){
        if( frame >= FRAME_COUNT ) {
            if(state=="spin"){
                if(cycle<repeat){
                    cycle++;
                }
                else{
                    state = "dead";
                    cycle = 0;
                }
            }
            frame = 0;
        }
        else{
            frame++;
        }
    }

    SDL_SetTextureBlendMode((*cache)[state][frame], SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, (*cache)[state][frame], &src, &dest);
}

void Player::deaded(){
    if(!dead){
        dead = true;

        game.stopBGM();
        game.sound.effects.stop();

        state = "spin";
        bouncing = false;
        falling  = false;
        game.controls.lock = true;
        timestamp = SDL_GetTicks();

        if(game.state!="BONUS_ROUND"){
            game.sound.effects.play("dead");
            game.lives -= 1;
        }
    }
}

void Player::collision(){
    if(!dead){
        collider->update(x, y);

        for(int i=0; i<game.enemies.size(); i++){
            if(collider->check(game.enemies[i].collider)){
                deaded();
                break;
            }
        }

        if(collider->check(goro.collider)){
            deaded();
        }

        if(gosenzo.active && collider->check(gosenzo.collider)){
            deaded();
        }

        // for(int i=0; i<game.bells.size(); i++){
        //     if(collider->check(game.bells[i].collider) && !game.bells[i].dropped){
        //         game.bells[i].drop();
        //     }
        // }
    }
}

void Player::bounce(){
    if(!bouncing && bounces<=BOUNCES){
        bounces++;
    }

    falling = false;
    bouncing = true;
}

void Player::door(){
    for(int i=0; i<game.doors.size(); i++){
        if(game.doors[i].range()){
            game.doors[i].operate();

            if((abs(game.doors[i].x+game.offset.x-x)<width*SCALE*1.5&&game.doors[i].direction==RIGHT)||
               (abs(game.doors[i].x+game.offset.x-x)<width*SCALE*1.5&&game.doors[i].direction==LEFT)){
                if(game.doors[i].direction==LEFT && x<=game.doors[i].x+game.offset.x && game.doors[i].open){
                    x-=width*SCALE;
                }
                else if(game.doors[i].direction==RIGHT && x>=game.doors[i].x+game.offset.x && game.doors[i].open){
                    x+=width*SCALE;
                }
            }
            break;
        }
    }
}