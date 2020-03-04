#include "game.hpp"

int Timer::start(Uint32 timeout){
    for(int i=0; i<timestamp.size(); i++){
        if(timestamp[i]==0){
            timestamp[i] = SDL_GetTicks();
            duration[i] = timeout;
            return i;
        }
    }
    timestamp.push_back(SDL_GetTicks());
    duration.push_back(timeout);
    return timestamp.size()-1;
}

bool Timer::done(int id){
    Uint32 delta = SDL_GetTicks() - timestamp[id];
    // cout << id << " " << duration[id] << " " << delta << endl;
    if(delta < duration[id]){
        return false;
    }
    // reset(id);
    return true;
}

void Timer::reset(int id){
    timestamp[id] = 0;
    duration[id]  = 0;
}