#include "game.hpp"

Audio::Audio( char* file_name,  bool priority_value ) {
    strcpy( FileName, file_name );
    priority = priority_value;

    if( SDL_LoadWAV(FileName, &spec, &buf, &len) == NULL ) {
        printf("Failed to load wave file: %s\n", SDL_GetError());
    }
}

void Audio::play() {
    if( priority || SDL_GetQueuedAudioSize(device_id) > 1 ) {
        SDL_ClearQueuedAudio(device_id);
    }

    int success = SDL_QueueAudio(device_id, buf, len);
    if( success < 0 ) {
        printf("SDL_QueueAudio %s failed: %s\n", FileName, SDL_GetError());
    }
}


Sound::Sound(){

    // music = Music();
}

void Sound::init(){
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024 ) < 0 ){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(2);
    }
}

// Sound::load(string type, string name, string file){
//     if(type=="effects"){
//         effects[name] = Mix_LoadWAV(file);
//     }
//     else if(type=="music"){
//         music[name] = Mix_LoadMUS(file);
//     }
// }

void Sound::Effects::load(string name, char* file){
    library[name] = Mix_LoadWAV(file);
}

void Sound::Effects::play(string name){
    Mix_PlayChannel(-1, library[name], 0);
}

// void Sound::Music::pause(){
//     Mix_PauseMusic();
// }

// void Sound::Music::resume(){
//     Mix_ResumeMusic();
// }

// void Sound::Music::stop(){
//     Mix_HaltMusic();
// }


void Sound::Music::load(string name, char* file){
    library[name] = Mix_LoadMUS(file);
}

void Sound::Music::play(string name){
    Mix_PlayMusic(library[name], 0);
    bgm = library[name];
}

void Sound::Music::loop(string name){
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