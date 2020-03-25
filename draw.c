template <class T>
void Draw::plot(const T &bits, int units, Coordinates offset){
    string color;
    SDL_Rect r;
    int row, col, bit,
    w = SCALE,
    alpha = 255;

    for(int i=0; i<bits.size(); i++) {
        bit = bits[i];
        if(bit){
            color = data.palette[bit];
            SDL_Color c = hex2sdl(color);

            col = i % units;
            row = floor(i / units);

            r.x = w*col + offset.x;
            r.y = w*row + offset.y;
            r.w = w;
            r.h = w;
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, alpha);
            SDL_RenderFillRect(renderer, &r);
        }
    }
}

void Draw::render(SDL_Texture* cache, double x, double y){
    SDL_Rect dest, src;
    int width, height;

    SDL_QueryTexture(cache, NULL, NULL, &width, &height);

    dest.x = x;
    dest.y = y;
    dest.w = width;
    dest.h = height;

    src.x = 0;
    src.y = 0;
    src.w = width;
    src.h = height;
    SDL_SetTextureBlendMode(cache, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, cache, &src, &dest);
}

template <class T>
SDL_Texture* Draw::compile(T data, int cols, int units){
    int col, row;
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, cols*units*SCALE, ((data.size()/cols)+1)*units*SCALE);
    SDL_SetRenderTarget(renderer, texture);

    for(int i=0; i<data.size(); i++){
        col = i % cols;
        row = floor(i / cols);
        plot(data[i], units, {(double)col*SCALE*units, (double)row*SCALE*units});
    }

    return texture;
}

template <class T>
SDL_Texture* Draw::compile(T data, int units){
    int col, row;
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, units*SCALE, units*SCALE);
    SDL_SetRenderTarget(renderer, texture);
    plot(data, units, {0, 0});
    return texture;
}

void Draw::clear(){
     SDL_SetRenderTarget(renderer, NULL);
}