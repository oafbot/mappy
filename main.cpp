#include "game.hpp"
#include "sprite.c"
#include "player.c"
#include "physics.c"
#include "game.c"

using namespace std;

Uint32 last_time;
Uint32 last_frame_time;
Uint32 current_time;
Uint32 ms_per_frame = 100; // animate at 10 fps
int current_frame = 0;

bool left_key_down  = false;
bool right_key_down = false;
bool up_key_down    = false;
bool down_key_down  = false;
bool key_down       = false;

double delta_time = 0.0;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

Player* player;
Game* game;
Physics* physics;
Stage stage;

// char palette[10][8] = {"NULL", "#000000", "#BCBCBC", "#FCFCFC", "#0078F8", "#F8A4C0", "#AC7C00", "#F8B800", "#FCA044", "#0058F8"};
// array<string, 10> palette = {"NULL", "#000000", "#BCBCBC", "#FCFCFC", "#0078F8", "#F8A4C0", "#AC7C00", "#F8B800", "#FCA044", "#0058F8"};
array<string, 56> palette = {"NULL", "#000000", "#fcfcfc", "#f8f8f8", "#bcbcbc", "#7c7c7c", "#787878", "#a4e4fc", "#3cbcfc", "#0078f8", "#0000fc", "#b8b8f8", "#6888fc", "#0058f8", "#0000bc", "#d8b8f8", "#9878f8", "#6844fc", "#4428bc", "#f8d8f8", "#f8b8f8", "#f878f8", "#d800cc", "#940084", "#f8a4c0", "#f85898", "#e40058", "#a80020", "#f0d0b0", "#f87858", "#f83800", "#a81000", "#fce0a8", "#fca044", "#e45c10", "#881400", "#f8d878", "#f8b800", "#ac7c00", "#503000", "#d8f878", "#b8f818", "#00b800", "#007800", "#b8f8b8", "#58d854", "#00a800", "#006800", "#b8f8d8", "#58f898", "#00a844", "#005800", "#00fcfc", "#00e8d8", "#008888", "#004058"};
array <array<int, 256>, 2> bitmap = {{{0,0,0,0,4,4,0,0,0,0,0,0,4,4,0,0,0,0,0,4,24,24,4,0,9,9,0,4,24,24,4,0,0,0,4,24,24,24,4,9,9,9,9,24,24,24,24,4,0,0,4,24,24,24,13,36,36,13,9,24,24,24,24,4,0,0,4,24,9,9,9,9,9,9,9,24,24,24,24,4,0,0,0,4,24,2,2,4,2,2,4,4,24,24,24,4,0,0,0,0,4,1,2,4,1,2,4,4,24,24,4,0,37,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,37,0,0,33,4,4,4,4,4,4,4,4,9,4,4,0,0,37,4,9,9,2,2,9,9,9,9,9,9,4,4,0,0,0,4,9,9,9,9,9,9,9,9,0,0,0,0,0,0,38,38,9,36,9,9,36,9,9,36,9,38,0,0,0,0,38,38,38,9,9,9,9,9,9,9,9,38,38,0,0,0,38,38,38,9,9,9,9,9,9,9,38,38,38,0,0,0,0,38,38,0,0,0,0,0,0,9,38,38,38,0,0,0,0,0,0,0,0,0,0,0,0,38,38,38,0,0},{0,0,0,0,4,4,0,0,0,0,0,0,4,4,0,0,0,0,0,4,24,24,4,0,9,9,0,4,24,24,4,0,0,0,4,24,24,24,4,9,9,9,9,24,24,24,24,4,0,0,4,24,24,24,13,36,36,13,9,24,24,24,24,4,0,0,4,24,9,9,9,9,9,9,9,24,24,24,24,4,0,0,0,4,24,2,2,4,2,2,4,4,24,24,24,4,0,37,0,0,4,1,2,4,1,2,4,4,24,24,4,0,0,37,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,37,4,0,33,4,4,4,4,4,4,4,4,0,0,0,0,4,4,9,9,9,2,2,9,9,9,9,9,0,0,0,0,4,4,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,36,9,9,36,9,9,36,0,4,4,0,0,0,0,0,0,9,9,9,9,9,9,9,0,4,4,0,0,0,0,0,0,0,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,38,38,38,38,0,0,0,0,0,0,0,0,0,0,0,38,38,38,38,38,0,0,0,0,0}}};


void input(){
    if( SDL_PollEvent( &event ) ){
        switch( event.type ){
            case SDL_KEYDOWN:
                switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        left_key_down = true;
                        key_down = true;
                        break;
                    case SDLK_RIGHT:
                        right_key_down = true;
                        key_down = true;
                        break;
                    case SDLK_UP:
                        up_key_down = true;
                        key_down = true;
                        break;
                    case SDLK_DOWN:
                        down_key_down = true;
                        key_down = true;
                        break;
                    default:
                        // key_down = false;
                        break;
                }
                break;

            case SDL_KEYUP:
                switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        left_key_down = false;
                        key_down = false;
                        break;
                    case SDLK_RIGHT:
                        right_key_down = false;
                        key_down = false;
                        break;
                    case SDLK_UP:
                        up_key_down = false;
                        key_down = false;
                        break;
                    case SDLK_DOWN:
                        down_key_down = false;
                        key_down = false;
                        break;
                    default:
                        key_down = false;
                        break;
                }
                break;

            default:
                break;
        }
    }
}

void update(){
    player->move();
    physics->gravity.update();
}

void render(){
    player->render();
}

void gameloop(){
    input();
    update();
    render();
}

int main() {
    stage.left   = 0;
    stage.right  = 800;
    stage.top    = 0;
    stage.bottom = 600;
    stage.width  = 800;
    stage.height = 600;

    // stage  = new Stage(800, 600);
    player = new Player(stage.width/2, stage.height/2);
    game   = new Game();
    game->init();

    SDL_Init( SDL_INIT_VIDEO );
    SDL_CreateWindowAndRenderer(stage.width, stage.height, 0, &window, &renderer);

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );

    // last_frame_time = last_time = SDL_GetTicks();

    emscripten_set_main_loop(gameloop, 0, 0);
    return 1;
}