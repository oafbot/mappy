#include "game.hpp"
using namespace std;

GameObject::GameObject(){
    state    = "";
    type     = "";
    x        = 0;
    y        = 0;
    width    = DIM;
    height   = DIM;
    frame    = 0;
    cycle    = 0;
    repeat   = 0;
    animated = false;
    assigned = false;
    loop     = false;
    active   = false;
}

void GameObject::init(){
    int col, row;
    col = group[0] % LEVEL_WIDTH;
    row = floor(group[0] / LEVEL_WIDTH);
    x = col*BYTE*SCALE;
    y = row*BYTE*SCALE;
}

void GameObject::assign(int index){
    group.push_back(index);
    assigned = true;
    init();
}

void GameObject::reset(){}

void GameObject::draw(const array<int, SPRITE_SIZE> &bits){
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

void GameObject::draw(const array<int, SPRITE_SIZE> &bits, Coordinates offset){
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

            r.x = offset.x*SCALE + w*col;
            r.y = offset.y*SCALE + w*row;
            r.w = w;
            r.h = w;
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, alpha);
            SDL_RenderFillRect(renderer, &r);
        }
    }
}

void GameObject::render(){
    SDL_Rect dest, src;
    int width, height;

    SDL_QueryTexture(cache, NULL, NULL, &width, &height);

    dest.x = game.offset.x + x;
    dest.y = game.offset.y + y;
    dest.w = width;
    dest.h = height;

    src.x = 0;
    src.y = 0;
    src.w = width;
    src.h = height;
    SDL_SetTextureBlendMode(cache, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, cache, &src, &dest);
}


// -----------
// Trampoline
// -----------

Trampoline::Trampoline(){
    this->state = "green";
    this->type = "trampoline";
    this->frame = 5;
    this->width = 3*BYTE;
    this->height = BYTE;
    this->assigned = false;
    this->animated = false;
    this->loop = false;
    this->repeat = 0;
    this->cycle = 0;
    this->bounces = 0;
    this->active = false;
    this->jumper = "";
    this->cache = &game.cache.trampoline;

    array<array<int, TILE_SIZE>, BITMAP_SIZE> d = data.trampoline;
    vector< array<array<int, TILE_SIZE>, FRAMES> > g;

    array<array<int, TILE_SIZE>, FRAMES>
    left   = {d[3], d[4], d[5], d[4], d[3], d[2], d[1], d[0]},
    center = {d[9], d[10], d[11], d[10], d[9], d[8], d[7], d[6]},
    right  = {flip(d[3]), flip(d[4]), flip(d[5]), flip(d[4]), flip(d[3]), flip(d[2]), flip(d[1]), flip(d[0])};

    Coordinates a = {0, 0}, b = {BYTE, 0}, c = {BYTE*2, 0};

    g.push_back(left);
    g.push_back(center);
    g.push_back(right);

    layout.push_back(a);
    layout.push_back(b);
    layout.push_back(c);

    define("green", g);
    define("yellow", changeColor(g, 37));
    define("orange", changeColor(g, 29));
    define("red",    changeColor(g, 30));
    define("broken", changeColor(g, 0));

    this->compile();
}

void Trampoline::assign(int index){
    group.push_back(index);
    group.push_back(index+1);
    group.push_back(index+2);
    assigned = true;
    init();
}

void Trampoline::init(){
    int col, row;
    col = group[0] % LEVEL_WIDTH;
    row = floor(group[0] / LEVEL_WIDTH);
    this->x = col*BYTE*SCALE;
    this->y = (row-1)*BYTE*SCALE;
}

void Trampoline::define(string name, vector< array<array<int, TILE_SIZE>, FRAMES> > frames){
    this->states.insert(make_pair(name.c_str(), frames));
}

void Trampoline::render(){
    SDL_Rect dest, src;
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
            bounce();
            if(loop==true || cycle<repeat){
                frame = 0;
                cycle++;
            }
            else{
                reset();
            }
        }
        else{
            frame++;
        }
    }

    SDL_RenderCopy(renderer, (*cache)[state][frame], &src, &dest);
}

void Trampoline::draw(const array<int, TILE_SIZE> &bits, Coordinates offset){
    string color;
    SDL_Rect r;

    int w = SCALE;
    int row;
    int col;
    int bit;
    int alpha = 255;

    for(int i=0; i<TILE_SIZE; i++) {
        bit = bits[i];
        if(bit){
            color = data.palette[bit];
            SDL_Color c = hex2sdl(color);

            col = i % BYTE;
            row = floor(i / BYTE);

            r.x = offset.x*SCALE + w*col;
            r.y = offset.y*SCALE + w*row;
            r.w = w;
            r.h = w;
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, alpha);
            SDL_RenderFillRect(renderer, &r);
        }
    }
}

void Trampoline::compile(){
    if(!contains(game.cached, type)){
        for(map< string, vector< array< array<int, TILE_SIZE>, FRAMES> > >::iterator it=states.begin(); it!=states.end(); ++it){
            string s = it->first;
            array<SDL_Texture*, FRAMES> images;

            for(int f=0; f<FRAMES; f++){
                SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width*SCALE, height*SCALE);
                SDL_SetRenderTarget(renderer, texture);

                for(int i=0; i<layout.size(); i++){
                    draw(it->second[i][f], layout[i]);
                }

                images[f] = texture;
            }
            this->cache->insert(make_pair(s, images));
        }
        game.cached.push_back("trampoline");
        SDL_SetRenderTarget(renderer, NULL);
    }
}

array<int, TILE_SIZE>
Trampoline::flip(const array<int, TILE_SIZE> bits){
    array<int, TILE_SIZE> flipped;
    array<array<int, BYTE>, TILE_SIZE/BYTE> rows;
    array<int, TILE_SIZE> flat;
    int row;
    int col;

    for(int i=0; i < bits.size(); i++){
        col = i % BYTE;
        row = floor(i / BYTE);

        if(col==0){
            array<int, BYTE> r;
            rows[row] = r;
        }
        rows[row][col] = bits[i];
    }

    for(int k=0; k<rows.size(); k++){
        reverse(rows[k].begin(), rows[k].end());
        for(int l=0; l<rows[k].size(); l++){
            flat[k*BYTE+l] = rows[k][l];
        }
    }
    return flat;
}

vector< array<array<int, TILE_SIZE>, FRAMES> >
Trampoline::changeColor(vector< array<array<int, TILE_SIZE>, FRAMES> > grouped, int color){
    vector< array<array<int, TILE_SIZE>, FRAMES> > replace;
    for(int i=0; i<grouped.size(); i++){
        array<array<int, TILE_SIZE>, FRAMES> array;
        for(int f=0; f<FRAMES; f++){
            for(int b=0; b<TILE_SIZE; b++){
                if(grouped[i][f][b]!=0){
                    array[f][b] = color;
                }else{
                    array[f][b] = 0;
                }
            }
        }
        replace.push_back(array);
    }
    return replace;
}

int Trampoline::tier(){
    int row = (LEVEL_HEIGHT-group[0]/LEVEL_WIDTH);
    return row/FLOOR_HEIGHT;
}


void Trampoline::reset(){
    this->cycle = 0;
    this->frame = 5;
    this->animated = false;
    this->active = false;

    if(game.RESET || game.COMPLETE){
        state = "green";
    }
}

void Trampoline::bounce(){
    if(jumper=="player" && bounces<=BOUNCES){
        bounces++;
    }
    switch(bounces){
        case 0:
            state = "green";
            break;
        case 1:
            state = "yellow";
            break;
        case 2:
            state = "orange";
            break;
        case 3:
            state = "red";
            break;
        case 4:
            state = "broken";
            break;
    }

    active = false;
}

void Trampoline::clear(){
    if(state!="broken"){
        reset();
        bounces = 0;
        state   = "green";
        active  = false;
        jumper  = "";
    }
}


// -----------
// Item
// -----------

Item::Item(int id){
    this->id = id;
    this->state = "default";
    this->width  = DIM;
    this->height = DIM;
    this->collected = false;

    switch(id+7){
        case RADIO:
            type = "radio";
            points = 100;
            break;
        case TV:
            type = "tv";
            points = 200;
            break;
        case COMPUTER:
            type = "computer";
            points = 300;
            break;
        case PAINTING:
            type = "painting";
            points = 400;
            break;
        case SAFE:
            type = "safe";
            points = 500;
            break;
    }

    this->cache = game.cache.item[type];
    this->compile();
}

void Item::assign(int index){
    group.push_back(index);
    group.push_back(index+1);
    group.push_back(index+LEVEL_WIDTH);
    group.push_back(index+LEVEL_WIDTH+1);
    assigned = true;
    init();
}

void Item::compile(){
    // if(!contains(game.cached, type)){
    // cout<<type<<endl;
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width*SCALE, height*SCALE);
    SDL_SetRenderTarget(renderer, texture);

    draw(data.items[id]);

    cache = texture;
    SDL_SetRenderTarget(renderer, NULL);
        // game.cached.push_back(type);
    // }
}

void Item::render(){
    if(!collected && game.state!="BONUS_ROUND"){
        GameObject::render();
    }
}

void Item::collect(){
    collected = true;
    game.sound.effects.play("item");

    if(game.pickup==id){
        // cout << "bonus " << points << "x" << game.factor << endl;
        game.points.item(points, game.factor, {x, y});
        game.score += points*game.factor;
        game.factor += 1;
    }
    else{
        game.points.item(points, 1, {x, y});
        game.score += points;
    }
    game.pickup = id;

    game.collected.push_back(id);

    if(game.collected.size()==10){
        game.timestamp = SDL_GetTicks();
    }

    // cout<< "score: " << game.score << endl;
}

void Item::reset(){
    collected = false;
    assigned  = false;
    group.clear();
}


// -----------
// Door
// -----------

Door::Door(int id, int uid) : wave(RIGHT, uid){
    this->id       = id;
    this->uid      = uid;
    this->state    = "closed";
    this->frame    = 0;
    this->width    = 3*BYTE;
    this->height   = 3*BYTE;
    this->assigned = false;
    this->animated = false;
    this->loop     = false;
    this->repeat   = 0;
    this->cycle    = 0;
    this->active   = false;
    this->open     = false;

    array<array<int, DOOR_SIZE>, 12> d = data.doors;

    vector< array<int, DOOR_SIZE> >
    closed  = {d[0]},
    closing = {d[4], d[3], d[2], d[1], d[0]},
    opening = {d[0], d[1], d[2], d[3], d[4]},
    opened  = {d[4]},

    magic_closed  = {d[5]},
    magic_closing = {d[9], d[8], d[7], d[6], d[5]},
    magic_opening = {d[5], d[6], d[7], d[8], d[9]},
    magic_opened  = {d[9]};

    switch(id+3){
        case DOOR_RIGHT:
            define("closed",  closed);
            define("closing", closing);
            define("opening", opening);
            define("opened",  opened);
            type = "standard";
            direction = RIGHT;
            offset.x = BYTE*SCALE;
            offset.y = -2*BYTE*SCALE;
            break;
        case DOOR_LEFT:
            define("closed",  flip(closed));
            define("closing", flip(closing));
            define("opening", flip(opening));
            define("opened",  flip(opened));
            type = "standard";
            direction = LEFT;
            offset.x = -BYTE*SCALE;
            offset.y = -2*BYTE*SCALE;
            break;
        case MAGIC_DOOR_R:
            define("closed",  magic_closed);
            define("closing", magic_closing);
            define("opening", magic_opening);
            define("opened",  magic_opened);
            type = "magic";
            direction = RIGHT;
            offset.x = BYTE*SCALE;
            offset.y = -2*BYTE*SCALE;
            wave = * new Wave(RIGHT, uid);
            break;
        case MAGIC_DOOR_L:
            define("closed",  flip(magic_closed));
            define("closing", flip(magic_closing));
            define("opening", flip(magic_opening));
            define("opened",  flip(magic_opened));
            type = "magic";
            direction = LEFT;
            offset.x = -BYTE*SCALE;
            offset.y = -2*BYTE*SCALE;
            wave = * new Wave(LEFT, uid);
            break;
    }

    // todo: pool resources instead of compiling for each instance.
    this->key = "door-"+to_string(id);
    this->cache = &game.cache.door[key];
    this->compile();
}

void Door::assign(int index){
    this->group.push_back(index);
    this->assigned = true;
    this->init();

    if(id+3==MAGIC_DOOR_L || id+3==MAGIC_DOOR_R){
        wave.assign(index);
    }
}

void Door::cleanup(){
    this->state    = "closed";
    this->assigned = false;
    this->animated = false;
    this->open     = false;
    this->frame    = 0;
    this->x        = 0;
    this->y        = 0;
    this->group.clear();

    if(id+3==MAGIC_DOOR_R || id+3==MAGIC_DOOR_L){
        wave.cleanup();
    }
}

void Door::define(string name, vector< array<int, DOOR_SIZE> > frames){
    this->states.insert(make_pair(name.c_str(), frames));
}

void Door::compile(){
    if(!contains(game.cached, key)){
        for(map< string, vector< array<int, DOOR_SIZE> > >::iterator it=states.begin(); it!=states.end(); ++it){
            string s = it->first;
            vector<SDL_Texture*> images;

            for(int f=0; f<it->second.size(); f++){
                SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width*SCALE, height*SCALE);
                SDL_SetRenderTarget(renderer, texture);
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

                draw(it->second[f]);

                images.push_back(texture);

            }
            cache->insert(make_pair(s, images));
        }
        SDL_SetRenderTarget(renderer, NULL);

        game.cached.push_back(key);
    }
}

void Door::render(){
    if(assigned){
        SDL_Rect dest, src;

        dest.x = x + offset.x + game.offset.x;
        dest.y = y + offset.y + game.offset.y;

        dest.w = width*SCALE;
        dest.h = height*SCALE;

        src.x = 0;
        src.y = 0;
        src.w = width*SCALE;
        src.h = height*SCALE;

        if(animated){
            if( frame >= 4 ){
                reset();
            }
            else{
                frame++;
            }
        }
        SDL_SetTextureBlendMode((*cache)[state][frame], SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, (*cache)[state][frame], &src, &dest);
    }
}

void Door::draw(const array<int, DOOR_SIZE> &bits){
    string color;
    SDL_Rect r;

    int w = SCALE;
    int row;
    int col;
    int bit;
    int alpha = 255;

    for(int i=0; i<DOOR_SIZE; i++) {
        bit = bits[i];
        if(bit){
            color = data.palette[bit];
            SDL_Color c = hex2sdl(color);

            col = i % 24;
            row = floor(i / 24);

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

vector< array<int, DOOR_SIZE> >
Door::flip(const vector< array<int, DOOR_SIZE> > frames){
    int row, col;
    array<int, DOOR_SIZE> bits;
    array<int, DOOR_SIZE> flat;
    array<array<int, 3*BYTE>, DOOR_SIZE/(3*BYTE)> rows;
    vector< array<int, DOOR_SIZE> > flipped;

    for(int f=0; f < frames.size(); f++){
        bits = frames[f];
        flat = {};

        for(int i=0; i < bits.size(); i++){
            col = i % (3*BYTE);
            row = floor(i / (3*BYTE));

            if(col==0){
                array<int, 3*BYTE> r;
                rows[row] = r;
            }
            rows[row][col] = bits[i];
        }

        for(int k=0; k<rows.size(); k++){
            reverse(rows[k].begin(), rows[k].end());
            for(int l=0; l<rows[k].size(); l++){
                flat[k*(3*BYTE)+l] = rows[k][l];
            }
        }
        flipped.push_back(flat);
    }
    return flipped;
}

bool Door::range(){
    int pos = player.index();
    if(!assigned)
        return false;
    return (pos < group[0] && pos > group[0]-DOOR_RANGE) || (pos > group[0] && pos < group[0]+DOOR_RANGE);
}

void Door::operate(int index){
    if(assigned){
        if(state=="closed"){
            state = "opening";
            open = true;
            animated = true;

            if(type=="standard"){
                knockout();
            }
            else if(type=="magic"){
                shockwave();
            }
        }
        else if(state=="opened"){
            state = "closing";
            open = false;
            animated = true;
        }
    }
}

void Door::reset(){
    if(state=="opening"){
        state = "opened";
        animated = false;
    }
    else if(state=="closing"){
        state = "closed";
        animated = false;
    }
    frame = 0;
}

void Door::knockout(){
    for(int i=0; i<game.enemies.size(); i++){
        int door = group[0],
            enemy = game.enemies[i].index();

        if(direction==LEFT && enemy>door-DOOR_RANGE && enemy<door-1){
            game.enemies[i].knockedout(RIGHT, x);
        }
        else if(direction==RIGHT && enemy<door+DOOR_RANGE && enemy>door+1){
            game.enemies[i].knockedout(LEFT, x);
        }
    }
}

void Door::shockwave(){
    wave.activate();
}


// -----------
// Wave
// -----------

Wave::Wave(int d, int id){
    type = "wave";
    id = id;
    channel = -1;
    direction = d;
    animated = true;
    collider = new Collider<Wave>(this);
    collider->init(x+game.offset.x, y+game.offset.y, DIM*SCALE, DIM*SCALE-BYTE);
    cache = &game.cache.wave;

    array<array<int, SPRITE_SIZE>, 16> m = data.items;
    array<array<int, SPRITE_SIZE>, FRAMES>
    left_top     = {m[8],  m[9],  m[10], m[9],  m[8],  m[9],  m[10], m[9]},
    left_bottom  = {m[11], m[12], m[13], m[12], m[11], m[12], m[13], m[12]},
    right_top    = {flip(m[8]),  flip(m[9]),  flip(m[10]), flip(m[9]),  flip(m[8]),  flip(m[9]),  flip(m[10]), flip(m[9])},
    right_bottom = {flip(m[11]), flip(m[12]), flip(m[13]), flip(m[12]), flip(m[11]), flip(m[12]), flip(m[13]), flip(m[12])};

    Coordinates a = {0, 0}, b = {0, DIM};

    vector< array<array<int, SPRITE_SIZE>, FRAMES> > g1, g2;

    g1.push_back(left_top);
    g1.push_back(left_bottom);
    g2.push_back(right_top);
    g2.push_back(right_bottom);

    layout.push_back(a);
    layout.push_back(b);

    define("left", g1);
    define("right", g2);
    compile();
}

void Wave::init(){
    GameObject::init();
    y -= 3*BYTE*SCALE;
}

void Wave::assign(int index){
    group.push_back(index);
    assigned = true;
    init();
}

void Wave::compile(){
    if(!contains(game.cached, type)){
        for(map< string, vector< array< array<int, SPRITE_SIZE>, FRAMES> > >::iterator it=states.begin(); it!=states.end(); ++it){
            string s = it->first;
            array<SDL_Texture*, FRAMES> images;

            for(int f=0; f<FRAMES; f++){
                SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width*SCALE, height*2*SCALE);
                SDL_SetRenderTarget(renderer, texture);

                for(int i=0; i<layout.size(); i++){
                    draw(it->second[i][f], layout[i]);
                }

                images[f] = texture;
            }
            this->cache->insert(make_pair(s, images));
        }
        game.cached.push_back("wave");
        SDL_SetRenderTarget(renderer, NULL);
    }
}

void Wave::define(string name, vector< array<array<int, SPRITE_SIZE>, FRAMES> > frames){
    states.insert(make_pair(name.c_str(), frames));
}

void Wave::render(){
    if(active){
        SDL_Rect dest, src;

        SDL_QueryTexture((*cache)[state][frame], NULL, NULL, &width, &height);

        dest.x = game.offset.x + x;
        dest.y = game.offset.y + y;
        dest.w = width;
        dest.h = height;

        src.x = 0;
        src.y = 0;
        src.w = width;
        src.h = height;

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
    }
}

array<int, SPRITE_SIZE>
Wave::flip(const array<int, SPRITE_SIZE> bits){
    array<int, SPRITE_SIZE> flipped;
    array<array<int, DIM>, SPRITE_SIZE/DIM> rows;
    array<int, SPRITE_SIZE> flat;
    int row;
    int col;

    for(int i=0; i < bits.size(); i++){
        col = i % DIM;
        row = floor(i / DIM);

        if(col==0){
            array<int, DIM> r;
            rows[row] = r;
        }
        rows[row][col] = bits[i];
    }

    for(int k=0; k<rows.size(); k++){
        reverse(rows[k].begin(), rows[k].end());
        for(int l=0; l<rows[k].size(); l++){
            flat[k*DIM+l] = rows[k][l];
        }
    }
    return flat;
}

void Wave::update(){
    bool found;

    if(active){
        if((direction==LEFT && x>0) || (direction==RIGHT && x<game.stage.right+64)){
            x = direction==LEFT ? x - SPEED : x + SPEED;

            if(collider->check(goro.collider)){
                found = false;
                goro.capture(direction);

                for(int j=0; j<captured.size(); j++){
                    if(captured[j]->type=="boss"){ found = true; break; }
                }
                if(!found) captured.push_back(&goro);
            }

            for(int i=0; i<game.enemies.size(); i++){
                found = false;
                if(collider->check(game.enemies[i].collider)){
                    game.enemies[i].capture(direction);

                    for(int j=0; j<captured.size(); j++){
                        if(captured[j]->id==game.enemies[i].id){ found = true; break; }
                    }
                    if(!found) captured.push_back(&game.enemies[i]);
                }
            }
        }
        else{
            release();
        }
        collider->update(x+game.offset.x, y+game.offset.y+BYTE*SCALE);
    }
}

void Wave::release(){
    active = false;
    for(int i=0; i<captured.size(); i++){
        captured[i]->release();
    }
    score();
    captured.clear();
    game.sound.effects.stop(channel);
}

// void Wave::render(){
//     if(active){
//         GameObject::render();
//     }
// }

void Wave::activate(){
    active = true;
    channel = game.sound.effects.loop("wave");
    if(direction==LEFT){
        state = "left";
    }
    else{
        state = "right";
    }
}

void Wave::score(){
    bool boss = false;
    int score = 0;

    for(int i=0; i<captured.size(); i++){
        if(captured[i]->type=="boss") boss = true;
    }

    switch(captured.size()){
        case 1:
            score = 200;
            break;
        case 2:
            score = 400;
            break;
        case 3:
            score = 800;
            break;
        case 4:
            score = 1200;
            break;
        case 5:
            score = 1600;
            break;
        case 6:
            score = 2000;
            break;
        case 7:
            score = 3000;
            break;
        case 8:
            score = 4000;
            break;
        case 9:
            score = 5000;
            break;
        case 10:
            score = 6000;
            break;
    }

    if(captured.size())
        game.points.wave(score, boss, {x, y});

    if(boss){
        score = score * 2;
    }

    game.score += score;
}

bool Wave::range(){return true;}

void Wave::reset(){
    group.clear();
    assigned = false;
    active = false;
}

void Wave::cleanup(){
    bool boss = false;

    reset();

    for(int i=0; i<captured.size(); i++){
        captured[i]->captured = false;
        captured[i]->gravitation = true;
    }

    score();

    captured.clear();

    if(game.sound.effects.playing(channel)){
        game.sound.effects.stop(channel);
    }

    channel = -1;
}


// -----------
// Balloon
// -----------

Balloon::Balloon(){
    type     = "balloon";
    assigned  = false;
    collected = false;
    animated  = true;
    points    = 200;
    cache = &game.cache.balloon;
    frames = {data.items[6], data.items[6], data.items[7], data.items[7]};
    compile();
}

void Balloon::assign(int index){
    group.push_back(index);
    group.push_back(index+1);
    group.push_back(index+LEVEL_WIDTH);
    group.push_back(index+LEVEL_WIDTH+1);
    assigned = true;
    active = true;
    init();
}

void Balloon::compile(){
    if(!contains(game.cached, type)){
        for(int f=0; f<frames.size(); f++){
            SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width*SCALE, height*SCALE);
            SDL_SetRenderTarget(renderer, texture);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

            draw(frames[f]);

            (*cache)[f] = texture;
        }

        SDL_SetRenderTarget(renderer, NULL);
        game.cached.push_back(type);
    }
}

void Balloon::render(){
    if(active){
        SDL_Rect dest, src;

        dest.x = game.offset.x + x;
        dest.y = game.offset.y + y;

        dest.w = width*SCALE;
        dest.h = height*SCALE;

        src.x = 0;
        src.y = 0;
        src.w = width*SCALE;
        src.h = height*SCALE;

        frame = animated && frame<frames.size()-1 ? frame+1 : 0;

        SDL_SetTextureBlendMode((*cache)[frame], SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, (*cache)[frame], &src, &dest);
    }
}

void Balloon::collect(){
    collected = true;
    active    = false;
    game.sound.effects.play("item");

    game.score += points;

    // game.collected.push_back(id);

    // if(game.collected.size()==10){
    //     game.timestamp = SDL_GetTicks();
    // }

    // cout<< "score: " << game.score << endl;
}

void Balloon::cleanup(){
    assigned = false;
    collected = false;
    active = false;
}

void Balloon::reset(){
    collected = false;
    assigned = false;
    active = false;
}

// -----------
// Bell
// -----------



// -----------
// Points
// -----------

Points::Points(){
    type = "points";
    cache = &game.cache.points;

    define("100", data.points[0]);
    define("200", data.points[1]);
    define("300", data.points[2]);
    define("400", data.points[3]);
    define("500", data.points[4]);
    define("x2",  data.points[5]);
    define("x3",  data.points[6]);
    define("x4",  data.points[7]);
    define("x5",  data.points[8]);
    define("x6",  data.points[9]);
    define("00",  data.points[10]);
    define("2",   data.points[11]);
    define("4",   data.points[12]);
    define("8",   data.points[13]);
    define("12",  data.points[14]);
    define("16",  data.points[15]);
    define("20",  data.points[16]);
    define("30",  data.points[17]);
    define("40",  data.points[18]);
    define("50",  data.points[19]);
    define("60",  data.points[20]);
    define("x2b", data.points[21]);

    compile();
}

void Points::define(string name, array<int, SPRITE_SIZE> bits){
    states.insert(make_pair(name.c_str(), bits));
}

void Points::compile(){
    if(!contains(game.cached, type)){
        for(map< string, array<int, SPRITE_SIZE> >::iterator it=states.begin(); it!=states.end(); ++it){
            string s = it->first;

            SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                                     width*SCALE, height*SCALE);
            SDL_SetRenderTarget(renderer, texture);

            draw(it->second);

            cache->insert(make_pair(s, texture));
        }

        game.cached.push_back(type);
        SDL_SetRenderTarget(renderer, NULL);
    }
}

void Points::render(){
    for(int i=0; i<display.size(); i++){
        if(display[i].type=="item"){
            if(!game.timer.done(display[i].timestamp) && !display[i].expired){
                Draw::render((*cache)[display[i].name], display[i].x+game.offset.x, display[i].y+game.offset.y);
            }
            else{
                game.timer.reset(display[i].timestamp);
                display[i].expired = true;
            }
        }
        else{
            if(display[i].x>game.stage.left-OFFSET*2&& display[i].x<game.stage.right+OFFSET*2){
                display[i].x = display[i].direction==LEFT ? display[i].x - SCALE*BYTE : display[i].x + SCALE*BYTE;
                Draw::render((*cache)[display[i].name], display[i].x+game.offset.x, display[i].y+game.offset.y);
            }
            else{
                display[i].expired = true;
            }
        }
    }

    for(int i=0; i<display.size(); i++){
        if(display[i].expired) display.erase(display.begin()+i);
    }
}

void Points::item(int points, int factor, Coordinates pos){
    display.push_back({
        "item",
        to_string(points),
        pos.x,
        pos.y-height*SCALE*0.75,
        game.timer.start(2000),
        false
    });

    if(factor>1)
        display.push_back({
            "item",
            "x" + to_string(factor),
            pos.x+width*SCALE,
            pos.y-height*SCALE*0.75,
            game.timer.start(2000),
            false
        });
}

void Points::wave(int points, bool dub, Coordinates pos){
    display.push_back({
        "wave",
        to_string(points/100),
        pos.x,
        pos.y+height*SCALE,
        NULL,
        false,
        pos.x > game.center.x ? LEFT : RIGHT
    });

    display.push_back({
        "wave",
        "00",
        pos.x+width*SCALE,
        pos.y+height*SCALE,
        NULL,
        false,
        pos.x > game.center.x ? LEFT : RIGHT
    });

    if(dub)
        display.push_back({
            "wave",
            "x2b",
            pos.x+(width*SCALE*2),
            pos.y+height*SCALE,
            NULL,
            false,
            pos.x > game.center.x ? LEFT : RIGHT
        });
}

void Points::reset(){
    for(int i=0; i<display.size(); i++){
        game.timer.reset(display[i].timestamp);
    }
    display.clear();
}