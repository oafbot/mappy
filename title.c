void Title::compile(){
    cache = Draw::compile(data.title, 6, 32);
    SDL_SetRenderTarget(renderer, NULL);
}

void Title::render(double x, double y){
    int w, h;

    position.x = x;
    position.y = y;

    SDL_QueryTexture(cache, NULL, NULL, &w, &h);
    width = w;
    height = h;
    Draw::render(cache, x, y);
}

void Title::display(){
    int w, h;
    double x,y;

    SDL_QueryTexture(cache, NULL, NULL, &w, &h);
    width = w;
    height = h;
    x = game.center.x - width/2 + 16*SCALE;
    y = game.center.y - height*1.8;
    Title::render(x, y);
}