#include "game.hpp"
using namespace std;

Game::Game(){
    game->PAUSED = false;
}

void Game::init(){
    physics->gravity.bind(player);
}