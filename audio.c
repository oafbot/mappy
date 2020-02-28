#include "game.hpp"

Sound::Sound(){

    // music = Music();
}

void Sound::init(){
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024 ) < 0 ){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(2);
    }
}

void Sound::Effects::load(string name, char* file){
    library[name] = Mix_LoadWAV(file);
}

void Sound::Effects::play(string name){
    Mix_PlayChannel(-1, library[name], 0);
}

void Sound::Effects::loop(string name){
    Mix_PlayChannel(-1, library[name], -1);
}

void Sound::Effects::pause(){
    Mix_Pause(-1);
}

void Sound::Effects::resume(){
    Mix_Resume(-1);
}

void Sound::Effects::stop(){
    Mix_HaltChannel(-1);
}


void Sound::Music::load(string name, char* file){
    library[name] = Mix_LoadMUS(file);
}

void Sound::Music::play(string name){
    Mix_PlayMusic(library[name], 0);
    bgm = library[name];
}

void Sound::Music::loop(string name){
    Mix_RewindMusic();
    Mix_PlayMusic(library[name], -1);
    bgm = library[name];
}

void Sound::Music::pause(){
    Mix_PauseMusic();
}

void Sound::Music::resume(){
    Mix_ResumeMusic();
}

void Sound::Music::stop(){
    Mix_HaltMusic();
}