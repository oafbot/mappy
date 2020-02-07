#include "game.hpp"
using namespace std;

// void Timer::setInterval(auto function, int interval) {
//     this->clear = false;
//     std::thread t([=]() {
//         while(true) {
//             if(this->clear) return;
//             std::this_thread::sleep_for(std::chrono::milliseconds(interval));
//             if(this->clear) return;
//             function();
//         }
//     });
//     t.detach();
// }

// void Timer::setTimeout(auto function, int delay) {
//     this->clear = false;
//     std::thread t([=]() {
//         if(this->clear) return;
//         std::this_thread::sleep_for(std::chrono::milliseconds(delay));
//         if(this->clear) return;
//         function();
//     });
//     t.detach();
// }

// void Timer::stop() {
//     this->clear = true;
// }

Stage::Stage(int w, int h){
    this->left   = 0;
    this->right  = w;
    this->top    = 0;
    this->bottom = h;
    this->width  = w;
    this->height = h;

}

Game::Game() : stage(0,0){
    this->level = 1;
    this->PAUSED = false;
    this->controls = * new Control();
    this->mapper   = * new Mapper();
    // this->timer    =   Timer();
    this->offset.x = 0;
    this->offset.y = 0;
    this->scrolling = true;
}

void Game::update(){
    player.update();
    player.move();
    physics.update();
}

bool Game::delay(){
    int ms;

    current_time = SDL_GetTicks();
    ms = current_time - last_time;

    if( ms < ms_per_frame ){
        return true;
    }
    return false;
}

void Game::render(){
    if(delay()){
        return;
    };

    mapper.render();
    player.render();

    last_time = current_time;
}

void Game::init(int w, int h){
    this->stage = * new Stage(w, h);
    this->center.x = this->stage.width/2;
    this->center.y = this->stage.height/2;

    physics = * new Physics();
    Gravity g = physics.gravity(0.125, 0);/*0.125*/
    g.bind(&player);
    physics.dropable.push_back(g);

    SDL_Init( SDL_INIT_VIDEO );
    SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );
    // SDL_RenderPresent(renderer);
    this->mapper.init();
}