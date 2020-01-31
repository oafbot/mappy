#include "game.hpp"
using namespace std;

Game::Game(){
    game->PAUSED = false;
}

// void Game::init(){
//     // physics->
//     Gravity* g = physics->gravity(1, 0);
//     g->gravity.bind(player);
//     physics->dropable.push_back(g);
// }

void Game::init(){
    // physics->
    Gravity* g = physics->gravity(0.125, 0);
    g->bind(*player);
    physics->dropable.push_back(*g);
}