void GameObject::draw(const array<int, TILE_SIZE> &bits, Coordinates offset){
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


array<int, TILE_SIZE> GameObject::flip(const array<int, TILE_SIZE> bits){
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



void Item::draw(const array<int, SPRITE_SIZE> &bits){
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
void Wave::draw(const array<int, SPRITE_SIZE> &bits){
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

void Item::init(){
    int col, row;
    col = group[0] % LEVEL_WIDTH;
    row = floor(group[0] / LEVEL_WIDTH);
    this->x = col*BYTE*SCALE;
    this->y = row*BYTE*SCALE;
}

void Door::init(){
    int col, row;
    col = group[0] % LEVEL_WIDTH;
    row = floor(group[0] / LEVEL_WIDTH);
    this->x = col*BYTE*SCALE;
    this->y = row*BYTE*SCALE;
}