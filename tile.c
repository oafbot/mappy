Mapper::Mapper(){
    this->x = 0;
    this->y = 0;
}

void Mapper::init(){
    // setStatus("Compiling levels...\n");
    this->compile();
}

void Mapper::compile(){
    int col, row;
    SDL_Rect dest, src;
    SDL_Texture *texture;

    for(int i=0; i<data.tiles.size(); i++){
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, BYTE*SCALE, BYTE*SCALE);
        SDL_SetRenderTarget(renderer, texture);

        this->draw(data.tiles[i]);

        game.cache.tiles.push_back(texture);
        SDL_SetRenderTarget(renderer, NULL);
    }

    for(int level=0; level<data.levels.size(); level++){
        // cout << "level " << level+1 << endl;

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                    LEVEL_WIDTH*BYTE*SCALE, LEVEL_HEIGHT*BYTE*SCALE);

        SDL_SetRenderTarget(renderer, texture);

        for(int i=0; i<LEVEL_SIZE; i++){
            if(i){
                col = i % LEVEL_WIDTH;
                row = floor(i / LEVEL_WIDTH);

                this->x = col*BYTE*SCALE;
                this->y = row*BYTE*SCALE;

                dest.x = x;
                dest.y = y;
                dest.w = BYTE*SCALE;
                dest.h = BYTE*SCALE;

                src.x = 0;
                src.y = 0;
                src.w = BYTE*SCALE;
                src.h = BYTE*SCALE;



                SDL_RenderCopy(renderer, game.cache.tiles[data.levels[level][i]], &src, &dest);
            }
        }
        this->background[level] = texture;

        SDL_SetRenderTarget(renderer, NULL);
    }
}

void Mapper::render(){
    SDL_Rect dest, src;
    dest.x = game.offset.x;
    dest.y = 0;
    dest.w = LEVEL_WIDTH*BYTE*SCALE;
    dest.h = LEVEL_HEIGHT*BYTE*SCALE;

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

            r.x = w*col;
            r.y = w*row;
            r.w = w;
            r.h = w;
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, alpha);
            SDL_RenderFillRect(renderer, &r );
        }
    }
}