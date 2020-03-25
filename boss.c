#include "game.hpp"
using namespace std;
Boss::Boss() : Enemy(10){
    array <array<int, SPRITE_SIZE>, BITMAP_SIZE> b = data.sprites;
    array<array<int, SPRITE_SIZE>, FRAMES>
    f0 = {b[17], b[18], b[19], b[18], b[17], b[18], b[19], b[17]}, // left, right
    f1 = {b[23], b[23], b[23], b[23], b[23], b[23], b[23], b[23]}, // hop
    f2 = {b[20], b[20], b[21], b[21], b[20], b[20], b[21], b[21]}, // bounce
    f3 = {b[24]}, // hide
    f4 = {b[22], b[22], b[22], b[22], b[22], b[22], b[22], b[22]}, // turned
    f5 = {b[25]}; // bonus

    define("left", f0);
    define("right", flip(f0));
    define("hop-left", f1);
    define("hop-right", flip(f1));
    define("drop", f2);
    define("bound", f2);
    define("turn", f4);
    define("hide", f3);
    define("reward", f5);

    this->width  = DIM;
    this->height = DIM;

    this->id          = 10;
    this->gravitation = true;
    this->falling     = false;
    this->bouncing    = false;
    this->animated    = true;
    this->direction   = "left";
    this->state       = "left";
    this->type        = "boss";
    this->mode        = "ROAM";
    this->frame       = 0;
    this->bounces     = -1;
    this->captured    = false;
    this->collider    = new Collider<Boss>(this);
    this->cache       = &game.cache.boss;

    this->compile();
    game.cache.reward = Draw::compile(data.items[16], DIM);


    collider->init(x + game.offset.x, y + game.offset.y, width, height);

    // collider->offset = 0;
};

void Boss::update(){
    if(active){
        if(!captured){
            if(falling && state!="hop-right" && state!="hop-left"){
                state = "drop";
                collider->passthru = true;
            }
            else if(bouncing && state!="hop-right" && state!="hop-left"){
                state = y<300 ? "turn" : "bound";
                collider->passthru = true;
            }
            else if(state=="hop-left" || state=="hop-right" || state=="hide" || state=="reward"){
                frame = 0;
                collider->passthru = true;
            }
            else if(falling || bouncing){
                collider->passthru = true;
            }
            else if(state!="hop-left" && state!="hop-right"){
                state = direction;
                collider->passthru = false;
            }

            animated = (state=="hop-left" || state=="hop-right" || state=="hide" || state=="reward") ? false : true;

            walk();
        }
        else if(captured){
            if(state == "right"){
                x -= SPEED;
                collider->passthru = true;
            }
            else if(state == "left"){
                x += SPEED;
                collider->passthru = true;
            }
        }
        else{
            collider->passthru = true;
            if(game.delay(5000, timestamp)){
                return;
            }
            respawn();
        }

        collider->update(x+game.offset.x, y+game.offset.y);
    }
    else if(released){
        if(game.timer.done(regenerate)){
            respawn();
        }
        else{
            // x = game.center.x + game.offset.x + OFFSET - width*SCALE/2;
            y = 100;
        }
    }
}

void Boss::define(string name, array<array<int, SPRITE_SIZE>, FRAMES> frames){
    this->states.insert(make_pair(name.c_str(), frames));
}

void Boss::draw(const array<int, SPRITE_SIZE> &bits){
    string color;
    SDL_Rect r;

    int w = SCALE;
    int row;
    int col;
    int bit;
    int alpha = 255;

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

void Boss::compile(){
    if(!contains(game.cached, type)){
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
    }
}

void Boss::render(){
    if(active){
        SDL_Rect dest, src;

        // dest.x = game.center.x + game.stage.width - (1024 - x) + game.offset.x;
        dest.x = game.offset.x + x;
        dest.y = game.offset.y + y;

        dest.w = width*SCALE;
        dest.h = height*SCALE;

        src.x = 0;
        src.y = 0;
        src.w = width*SCALE;
        src.h = height*SCALE;

        if(animated){
            if( frame >= FRAME_COUNT ){
                frame = 0;
            }
            else{
                frame++;
            }
        }

        SDL_SetTextureBlendMode((*cache)[state][frame], SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, (*cache)[state][frame], &src, &dest);

        if(state=="reward"){
            Draw::render(game.cache.reward, x-width*SCALE/2+game.offset.x, y-width+game.offset.y);
        }
    }
}

int Boss::behind(){
    int pos = index(x, y);
    int tile = data.interactive[game.level-1][pos];

    if(tile>=RADIO && tile<=SAFE){
        for(int i=0; game.items.size(); i++){
            if(contains(game.items[i].group, pos)){
                if(!game.items[i].collected)
                    return game.items[i].group[0];
                return false;
            }
        }
    }
    return false;
}

void Boss::hide(int index){
    // cout<< "hide" << endl;
    int delay = (rand()%4+2)*1000;
    hiding = game.timer.start(delay);
    mode = "HIDE";
    state = "hide";
    collider->passthru = true;
    x = index%LEVEL_WIDTH*BYTE*SCALE;
}

void Boss::reward(){
    game.timer.reset(hiding);
    hiding = game.timer.start(2000);

    state = "reward";
    mode  = "HIDE";
    collider->passthru = true;
}

void Boss::pounce(){
    // cout << "pounce" << endl;
    mode = "POUNCE";
    state = rand()%2==0 ? "left" : "right";
    direction = state;
    int i = index(x, y);
    x = state=="left" ? (i-2)%LEVEL_WIDTH*BYTE*SCALE : (i+1)%LEVEL_WIDTH*BYTE*SCALE;
    game.timer.reset(hiding);
    collider->passthru = false;
}

void Boss::decision(){
    if(mode!="HIDE" && mode!="POUNCE"){
        int i = behind();
        int r = rand()%4;
        if(r && i){
            hide(i);
        }
    }
    else if(mode=="HIDE"){
        if(game.timer.done(hiding)){
            pounce();
        }
    }
    else{
        mode = "ROAM";
        collider->passthru = false;
    }
}

void Boss::move(){
    int tile;
    // cout << direction << endl;
    if(direction=="left"){
        tile = adjacent(LEFT);

        if(traverse(LEFT)){
            state = "left";
            x -= BOSS_SPEED;
        }
        else if(rand()%2 && (tile==DOOR_LEFT || tile==DOOR_RIGHT)){
            opendoor(LEFT);
        }
        else{
            state = "right";
            direction = "right";
            x += BOSS_SPEED;
        }
    }
    else if(direction=="right"){
        tile = adjacent(RIGHT);

        if(traverse(RIGHT)){
            state = "right";
            x += BOSS_SPEED;
        }
        else if(rand()%2 && (tile==DOOR_LEFT || tile==DOOR_RIGHT)){
            opendoor(LEFT);
        }
        else{
            state = "left";
            direction = "left";
            x -= BOSS_SPEED;
        }
    }
}

void Boss::walk(){
    if(mode!="HIDE"){
        // cout << state << endl;
        if(state!="bound" && state!="drop" && state!="hop-left" && state!="hop-right" && state!="turn"){
            move();
        }
        else if(state=="bound" || state=="turn"){
            jump();
        }
        else if(state=="drop"){
            if(direction=="left" && !aligned()){
                x -= ENEMY_SPEED;
            }
            else if(direction=="right" && !aligned()){
                x += ENEMY_SPEED;
            }
        }
        else if(state=="hop-left" || state=="hop-right"){
            frame = 0;
            hopoff();
        }
    }

    if(game.delay(500, timestamp)){
        return;
    };

    if(state!="bound" && state!="drop" && state!="turn" && state!="hop-right" && state!="hop-left"){
        decision();
    }
    timestamp = SDL_GetTicks();
}

void Boss::reset(double x, double y){
    this->x = x;
    this->y = y;
    this->gravitation = true;
    this->falling     = false;
    this->bouncing    = false;
    this->direction   = "left";
    this->state       = "left";
    this->type        = "boss";
    this->frame       = 0;
    this->animated    = true;
    this->active      = true;
    this->bounces     = -1;
    this->collider->passthru = false;
    this->collider->update(x+game.offset.x, y+game.offset.y);
}

void Boss::balloons(){
    for(int i=0; i<game.balloons.size(); i++){
        if(contains(game.balloons[i].group, index()) && !game.balloons[i].collected){
            game.balloons[i].collect();
        }
    }
}