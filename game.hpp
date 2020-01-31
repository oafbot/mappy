#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
// #include <iostream>
#include <emscripten.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string>
#include <vector>
// #include <cstdlib>
#include <array>
using namespace std;

#define FRAME_COUNT 7
#define SCALE 2
#define DIM 16
#define SPEED 1

extern Uint32 last_time;
extern Uint32 last_frame_time;
extern Uint32 current_time;
extern Uint32 ms_per_frame;
extern int current_frame;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Rect background;
extern SDL_Event event;

extern bool left_key_down;
extern bool right_key_down;
extern bool up_key_down;
extern bool down_key_down;
extern bool space_key_down;
extern bool key_down;

extern double delta_time;
extern int diff_time;

extern array <string, 56> palette;
extern array <array<int, 256>, 2> bitmap;


struct Stage{
    int left;
    int right;
    int top;
    int bottom;
    int width;
    int height;
};

struct Center{
    int x;
    int y;
};

class Control{

};

class Sprite{
    // private:
    public:
        int width;
        int height;
        double x;
        double y;
        int frame;
        bool gravitate;
        // int row;
        // int col;
        // int bit;
        // int alpha;
        // string color;
        // SDL_Rect r;
        array< array<int, 256>, 2> data;
        array< array<int, 256>, 2> frames;

    public:
        Sprite(int x, int y);
        void draw();
        void flip();
        void animate();
        void update();
        void move();
        virtual void render();
};

class Player: public Sprite{
    public:
        int width;
        int height;
        double x;
        double y;
        int frame;
        bool grav;
        bool fall;
        bool bounce;

        Player(int x, int y);
        void move();
        void render();
};

class Enemy: public Sprite{

};

class Physics{
    public:
        class Gravity{
            // private:
            public:
                float gravity;
                float buoyancy;
                float lift;
                float speed;
                float delay;
                // vector<Sprite*> sprites;
                // template<typename T>
                vector<Player*> sprites;

                Gravity(float factor,  int delay);
                // void bind(Sprite* sprite);
                void bind(Player* sprite);
                void update();
                void reset();
        } gravity;

        class Collision{
            public:
                void check();
        } collision;

        Physics();
        void bounce();
};


class Game{
    public:
        bool PAUSED;
        Game();
        void init();
};

SDL_Color hex2sdl(std::string input) {
    if (input[0] == '#')
        input.erase(0, 1);

    unsigned long value = stoul(input, nullptr, 16);

    SDL_Color color;

    color.a = (value >> 24) & 0xff;
    color.r = (value >> 16) & 0xff;
    color.g = (value >> 8) & 0xff;
    color.b = (value >> 0) & 0xff;
    return color;
}

extern Game* game;
extern Physics* physics;
extern Player* player;
extern Enemy* enemy;
extern Stage stage;

#endif