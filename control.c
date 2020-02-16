#include "game.hpp"
using namespace std;

void Control::input(){
    if( SDL_PollEvent( &event ) ){
        switch( event.type ){
            case SDL_KEYDOWN:
                switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        if(!player.dead && !lock){
                            this->left_key_down = true;
                            this->key_down = true;
                            // cout << player.x+game.offset.x << ", " << player.y+game.offset.y << endl;
                            if(!player.falling && !player.bouncing){
                                player.state = "left";
                            }
                            // else if(player.direction!="left"){
                            //     player.state = "turn";
                            // }
                            else{
                                player.state = "hop-left";
                            }
                            player.direction = "left";
                        }
                        break;
                    case SDLK_RIGHT:
                        if(!player.dead && !lock){
                            this->right_key_down = true;
                            this->key_down = true;

                            if(!player.falling && !player.bouncing){
                                player.state = "right";
                            }
                            // else if(player.direction!="right"){
                            //     player.state = "turn";
                            // }
                            else{
                                player.state = "hop-right";
                            }
                            player.direction = "right";
                        }
                        break;
                    case SDLK_UP:
                        this->up_key_down = true;
                        this->key_down = true;
                        // player.direction = "up";
                        // if(!player.falling && !player.bouncing){
                        //     player.state = "up";
                        // }
                        break;
                    case SDLK_DOWN:
                        this->down_key_down = true;
                        this->key_down = true;
                        // player.direction = "down";
                        // if(!player.falling && !player.bouncing){
                        //     player.state = "down";
                        // }
                        break;
                    default:
                        // key_down = false;
                        break;
                }
                break;

            case SDL_KEYUP:
                switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        this->left_key_down = false;
                        this->keysup();
                        break;
                    case SDLK_RIGHT:
                        this->right_key_down = false;
                        this->keysup();
                        break;
                    case SDLK_UP:
                        this->up_key_down = false;
                        this->keysup();
                        break;
                    case SDLK_DOWN:
                        this->down_key_down = false;
                        this->keysup();
                        break;
                    default:
                        this->keysup();
                        break;
                }
                break;

            default:
                break;
        }
    }
}

void Control::keysup(){
    if(this->left_key_down || this->up_key_down || this->right_key_down || this->down_key_down){
        this->key_down = true;
    }
    else{
        this->key_down = false;
    }
}