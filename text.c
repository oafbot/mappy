Text::Text(){
    width   = BYTE;
    height  = BYTE;
    visible = true;
    unit    = BYTE*SCALE;
    kern    = 0;
}

void Text::init(vector<int> c, int w, int h){
    colors  = c;
    box     = {w, h};
    cols    = w/unit;
    rows    = h/unit;

    colors.insert(colors.begin(), 2);
    compile();
}

void Text::compile(){
    // array<char, 42> keys = {"0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","!","?",".","/","(c)","@"};
    array<char, 42> keys = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','!','?','.','/','%','@'};

    for(int c=0; c<colors.size(); c++){
        map<char, SDL_Texture*> characters;

        for(int i=0; i<data.alpha.size(); i++){
            SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, unit, unit);
            SDL_SetRenderTarget(renderer, texture);

            draw(data.alpha[i], colors[c]);

            // cache[c][i] = texture;
            characters.insert(make_pair(keys[i], texture));
            // SDL_SetRenderTarget(renderer, NULL);
        }
        alpha.insert(make_pair(colors[c], characters));
        // alpha.insert({colors[c], characters});
    }

    SDL_SetRenderTarget(renderer, NULL);
}

void Text::draw(const array<int, TILE_SIZE> &bits, int color){
    SDL_Rect r;

    int w = SCALE;
    int row;
    int col;
    int bit;
    int alpha = 255;

    for(int i=0; i<TILE_SIZE; i++){
        bit = bits[i];
        if(bit){
            SDL_Color c = hex2sdl(data.palette[color]);
            col = i % BYTE;
            row = floor(i / BYTE);

            r.x = w*col;
            r.y = w*row;
            r.w = w;
            r.h = w;
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, alpha);
            SDL_RenderFillRect(renderer, &r);
        }
    }
}

void Text::center(string text, int color, int row){
    int size = text.size();
    double offset = game.center.x - ((size*unit + ((size-1)*kern)) / 2);
    Coordinates c = { offset, (double)row*unit };
    render(text, color, c);
}

void Text::render(string text, int color, Coordinates offset){
    SDL_Rect dest, src;
    dest.x = offset.x;
    dest.y = offset.y;
    dest.w = unit; //text.size()*unit + (text.size()-1)*kern;
    dest.h = unit;

    src.x = 0;
    src.y = 0;
    src.w = unit;
    src.h = unit;

    if(visible){
        for(int i=0; i<text.size(); i++){
            dest.x = offset.x + (unit + kern)*i;
            // cout << text[i];
            // map<char, SDL_Texture*> chars = alpha[color];
            if(text[i]!=' '){
                SDL_RenderCopy(renderer, alpha[color][text[i]], &src, &dest);
            }
            else{
                dest.x += unit;
            }
        }
        // for(char& c: text){ dest.x += BYTE*SCALE + kern; SDL_RenderCopy(renderer, alpha[color][c], &src, &dest); }
    }
    // cout << endl;
}

void Text::render(string text, int color, int col, int row){
    Coordinates c = { (double)col*unit, (double)row*unit };
    render(text, color, c);
}

int Text::getWidth(string text){
    int size = text.size();
    return size*unit + ((size-1)*kern);
}

void Text::score(int sc, int digits){
    string s = to_string(sc);
    int offset = digits - s.size();
    for(int i=0; i<offset; i++){
        s = " " + s;
    }
    render(s, WHITE, 1, 1);
}

void Text::score(int sc){
    int digits = 10;
    score(sc, digits);
}

// void Text::animate(){

// }