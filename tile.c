Mapper::Mapper(){
    this->x = 0;
    this->y = 0;
}

void Mapper::init(){
    setStatus("Compiling levels...\n");
    for(int i=0; i<levels.size(); i++){
        this->compile(i);
    };
}

void Mapper::compile(int level){
    int col;
    int row;
    SDL_Texture *texture;

    // printf("%s\n", this->state.c_str());

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                LEVEL_WIDTH*BYTE*SCALE, LEVEL_HEIGHT*BYTE*SCALE);
    SDL_SetRenderTarget(renderer, texture);

    for(int i=0; i<LEVEL_SIZE; i++){
        if(i){
            col = i % 56;
            row = floor(i / 56);

            this->x = col*8*SCALE;
            this->y = row*8*SCALE;
            this->draw(tiles[levels[level-1][i]]);
        }
    }
    this->background[level-1] = texture;

    SDL_SetRenderTarget(renderer, NULL);
}

void Mapper::render(){
    SDL_RenderCopy(renderer, this->background[game.level-1], NULL, NULL);
}

void Mapper::draw(const array<int, TILE_SIZE> &data){
    string color;
    SDL_Rect r;

    int w = SCALE;
    int row;
    int col;
    int bit;
    int alpha = 255;

    for(int i=0; i < TILE_SIZE; i++) {
        bit = data[i];
        if(bit){
            color = palette[bit];
            SDL_Color c = hex2sdl(color);

            col = i % 8;
            row = floor(i / 8);

            r.x = this->x + w*col;
            r.y = this->y + w*row;
            r.w = w;
            r.h = w;
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, alpha);
            SDL_RenderFillRect(renderer, &r );
        }
    }
}