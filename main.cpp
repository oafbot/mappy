#include "game.hpp"
#include "audio.c"
#include "data.c"
#include "draw.c"
#include "title.c"
#include "text.c"
#include "demo.c"
#include "sprite.c"
#include "player.c"
#include "enemy.c"
#include "boss.c"
#include "gosenzo.c"
#include "physics.c"
#include "control.c"
#include "objects.c"
#include "timer.c"
#include "game.c"
#include "tile.c"
#include <type_traits>
#include <cstddef>

using namespace std;

SDL_Window   *window;
SDL_Renderer *renderer;
SDL_Event     event;

Game    game;
Player  player;
Physics physics;
Boss    goro;
Gosenzo gosenzo;

void gameloop(){
    game.loop();
    // EM_ASM( {GetState($0);}, game.state.c_str() );
}

int main(){
    EM_ASM( InitWrappers() );

    game = * new Game();
    game.init(800, 600);
    game.setup();

    emscripten_set_main_loop(gameloop, 0, 0);
    return 1;
}

extern "C"
bool startButton(){
    if(game.state=="LOADING"){
        return false;
    }
    else if(game.state=="START_SCREEN"){
        game.start();
        return true;
    }
    else if(game.state=="RUNNING" || game.state=="BONUS_ROUND"){
        game.pause();
        return true;
    }
    else if(game.state=="GAME_OVER" || game.state=="RESTART"){
        game.timeout = 0;
        game.clear();
        game.sound.effects.stop();
        game.stopBGM();

        game.reset();
        return true;
    }
    // cout << game.state << endl;
    return false;
}

// extern "C"
// string getState(){ return game.state; }