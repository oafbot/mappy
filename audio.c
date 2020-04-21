#include "game.hpp"

Sound::Sound(){
    rate = 44100;
    buffers = 1024;
    format = MIX_DEFAULT_FORMAT;

    // rate = 22050;
    // buffers = 1024;
    // format = AUDIO_S16SYS;
    // flags = MIX_INIT_MP3;
}

void Sound::init(){
    int result = 0;

    if( Mix_OpenAudio( rate, format, MIX_DEFAULT_CHANNELS, buffers ) < 0 ){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(2);
    }

    // if (flags != (result = Mix_Init(flags))) {
    //     printf("Could not initialize mixer (result: %d).\n", result);
    //     printf("Mix_Init: %s\n", Mix_GetError());
    //     exit(1);
    // }

    Mix_AllocateChannels(16);
}

void Sound::Effects::load(string name, char* file){
    library[name] = Mix_LoadWAV(file);
}

int Sound::Effects::play(string name){
    int channel = Mix_PlayChannel(-1, library[name], 0);
    channels.insert(make_pair(name.c_str(), channel));
    return channel;
}

void Sound::Effects::play(string name, int channel){
    // cout << channel << endl;
    Mix_PlayChannel(channel, library[name], 0);
    channels.insert(make_pair(name.c_str(), channel));
}

int Sound::Effects::loop(string name){
    int channel = Mix_PlayChannel(-1, library[name], -1);
    channels.insert(make_pair(name.c_str(), channel));
    return channel;
}

void Sound::Effects::loop(string name, int channel){
    // cout << channel << endl;
    Mix_PlayChannel(channel, library[name], -1);
    channels.insert(make_pair(name.c_str(), channel));
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

void Sound::Effects::stop(int channel){
    Mix_HaltChannel(channel);
}

bool Sound::Effects::playing(int channel){
    return Mix_Playing(channel);
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

bool Sound::Music::playing(){
    return Mix_PlayingMusic();
}