#include "game.hpp"
#include "sprite.c"
#include "player.c"
#include "physics.c"
#include "control.c"
#include "game.c"
#include "tile.c"
#include "data.c"
#include <type_traits>
#include <cstddef>

using namespace std;

Uint32 last_time;
Uint32 last_frame_time;
Uint32 current_time;
Uint32 ms_per_frame = 100; // animate at 10 fps
int current_frame = 0;
double delta_time = 0.0;

SDL_Window   *window;
SDL_Renderer *renderer;
SDL_Event     event;

Game    game;
Player  player;
Physics physics;

void init(){
    array<array<int, SPRITE_SIZE>, FRAMES>
    f0 = {b[0], b[1], b[0], b[1], b[0], b[1], b[0], b[1]}, // left, right
    f1 = {b[2], b[2], b[3], b[3], b[2], b[2], b[3], b[3]}, // drop
    f2 = {b[4], b[4], b[4], b[4], b[4], b[4], b[4], b[4]}, // bound
    f3 = {b[5], b[5], b[5], b[5], b[5], b[5], b[5], b[5]}, // hop
    f5 = {b[7], b[8], b[9], b[10], b[11], b[12], b[13], b[14]}, // spin
    f6 = {b[15], b[15], b[16], b[16], b[15], b[15], b[16], b[16]}; // dead
    player.define("left", f0);
    player.define("right", player.flip(f0));
    player.define("drop", f1);
    player.define("bound", f2);
    player.define("hop-left", f3);
    player.define("hop-right", player.flip(f3));
    player.define("spin", f5);
    player.define("dead", f6);
}

void gameloop(){
    game.controls.input();
    game.update();
    game.render();
}

int main(){
    game = * new Game();
    game.init(800, 600);

    player = * new Player();
    player.init(game.stage.width/2, game.stage.height/2);
    init();

    // last_frame_time = last_time = SDL_GetTicks();

    emscripten_set_main_loop(gameloop, 0, 0);
    return 1;
}