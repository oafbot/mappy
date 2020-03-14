Demo::Demo(){
    title = * new Title();
    title.compile();
    lines = { "namcopy presents", "% 2020 namcopy", "starring" };
}

void Demo::scrollLeft(string text, int pos, int inc, int color){
    position[pos].x -= inc;
    game.text.render(text, color, position[pos]);
}

void Demo::credits(double goal){
    if(game.timer.done(game.timeout)){
        title.position.y += SCALE;

        if(title.position.y>goal){
            scene += 1;
            game.timer.reset(game.timeout);
            game.timeout = game.timer.start(800);
            // cout << scene << endl;
        }
    }
};

void Demo::hide(double x, double y, int width, int height){
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = width;
    r.h = height;
    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &r);
}

void Demo::start(){
    int size;
    game.text.center(lines[0], WHITE, 2);
    game.text.center(lines[1], WHITE, 30);

    title.display();

    size = lines[0].size();
    Coordinates c0 = {(game.center.x - ((size*game.text.unit + ((size-1)*game.text.kern)) / 2)), (double)2*game.text.unit};
    position.push_back(c0);

    size = lines[1].size();
    Coordinates c1 = {(game.center.x - ((size*game.text.unit + ((size-1)*game.text.kern)) / 2)), (double)30*game.text.unit};
    position.push_back(c1);

    size = lines[2].size();
    Coordinates c2 = { (double)game.stage.right + game.text.getWidth(lines[0]), (double)2*game.text.unit };
    position.push_back(c2);

    scene = 0;
}

void Demo::cast(){
    double y = game.center.y-title.height-player.height*SCALE*3;
    switch(scene){
        default:
        case 6:
        case 5:
            game.enemies[0].reset(game.center.x-game.offset.x-goro.width*SCALE/2, y+256);
            game.enemies[0].render();

            game.text.center("meowky", ORANGE, 24);
            game.text.center("as naughty folks", PINK, 26);
        case 4:
        case 3:
        case 2:
            goro.reset(game.center.x-game.offset.x-goro.width*SCALE/2, y + 128/*game.center.y-goro.height*SCALE*3*/);
            goro.animated = false;
            goro.state = "hop-left";
            goro.render();

            game.text.center("goro", YELLOW, 16);
            game.text.center("as boss the big bit", RED, 18);
        case 1:
            player.reset(game.center.x-player.width*SCALE/2, y);
            player.render();

            game.text.center("mappy", WHITE, 8);
            game.text.center("as micro police", BLUE, 10);
            break;
        case 0:
            break;
    }
}


void Demo::animate(){
    if(!game.interval()){ return; }

    if(position[2].x > game.center.x-game.text.getWidth(lines[2])/2){
        scrollLeft(lines[0], 0, SCALE, 2);
        scrollLeft(lines[1], 1, SCALE, 2);
        scrollLeft(lines[2], 2, SCALE, 42);
        title.display();
    }
    else{
        game.text.center(lines[2], 42, 2);

        if(scene<1){
            title.display();
            game.timer.reset(game.timeout);
            game.timeout = game.timer.start(800);
            scene = 1;
        }
        else if(scene==1){
            credits(155);
        }
        else if(scene==2){
            credits(178);
        }
        else if(scene==3){
            credits(game.center.y-DIM);
        }
        else if(scene==4){
            credits(game.center.y+8);
        }
        else if(scene==5){
            credits(game.center.y+110);
        }
        else if(scene==6){
            credits(game.center.y+136);
        }
        else if(scene==7){
            credits(game.stage.bottom+128);
            // game.timer.reset(game.timeout);
            // game.timeout = game.timer.start(2000);
        }
        else if(scene>=8){
            if(game.timer.done(game.timeout)){
                reset();
            }
        }

        cast();
        hide(0, title.position.y, game.stage.width, game.stage.height);
        title.render(title.position.x, title.position.y);
    }
    // cout << scene <<endl;
}

void Demo::reset(){
    position.clear();
    game.clear();
    start();
}