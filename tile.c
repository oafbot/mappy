Mapper::Mapper(){
    this->x = 0;
    this->y = 0;
}

void Mapper::init(){
    // setStatus("Compiling levels...\n");
    for(int i=0; i<data.levels.size(); i++){
        this->compile(i);
    };
}

void Mapper::compile(int level){
    int col;
    int row;
    SDL_Texture *texture;

    // printf("Loading stage %d\n", level+1);
    // printf("%s\n", this->state.c_str());

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                LEVEL_WIDTH*BYTE*SCALE, LEVEL_HEIGHT*BYTE*SCALE);
    SDL_SetRenderTarget(renderer, texture);

    for(int i=0; i<LEVEL_SIZE; i++){
        if(i){
            col = i % LEVEL_WIDTH;
            row = floor(i / LEVEL_WIDTH);

            this->x = col*BYTE*SCALE;
            this->y = row*BYTE*SCALE;
            this->draw(data.tiles[data.levels[level-1][i]]);
        }
    }
    this->background[level-1] = texture;

    SDL_SetRenderTarget(renderer, NULL);
}

void Mapper::render(){
    SDL_Rect dest, src;
    dest.x = game.offset.x;
    dest.y = 0;
    dest.w = LEVEL_WIDTH*BYTE*SCALE;  //game.stage.width;
    dest.h = LEVEL_HEIGHT*BYTE*SCALE; //game.stage.height;

    src.x = 0;
    src.y = 0;
    src.w = LEVEL_WIDTH*BYTE*SCALE;
    src.h = LEVEL_HEIGHT*BYTE*SCALE;

    SDL_RenderCopy(renderer, background[game.level-1], &src, &dest);
}

void Mapper::draw(const array<int, TILE_SIZE> &bits){
    string color;
    SDL_Rect r;

    int w = SCALE;
    int row;
    int col;
    int bit;
    int alpha = 255;

    for(int i=0; i < TILE_SIZE; i++) {
        bit = bits[i];
        if(bit){
            color = data.palette[bit];
            SDL_Color c = hex2sdl(color);

            col = i % BYTE;
            row = floor(i / BYTE);

            r.x = this->x + w*col;
            r.y = this->y + w*row;
            r.w = w;
            r.h = w;
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, alpha);
            SDL_RenderFillRect(renderer, &r );
        }
    }
}