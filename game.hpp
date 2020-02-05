#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>
// #include <cstdlib>
#include <SDL2/SDL.h>
// #include <SDL.h>
#include <emscripten.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <iterator>
using namespace std;

#define FRAME_COUNT 7
#define SCALE 2
#define DIM 16
#define SPEED 2
#define BYTE 8
#define WORD 16
#define LONG 32
#define FRAMES 8
#define SPRITE_SIZE 256
#define BITMAP_SIZE 32
#define TILE_SIZE 64
#define LEVEL_WIDTH 56
#define LEVEL_HEIGHT 36
#define LEVEL_SIZE 512 //2016 //56*36
// #define SQUAREx16 256

extern Uint32 last_time;
extern Uint32 last_frame_time;
extern Uint32 current_time;
extern Uint32 ms_per_frame;
extern int current_frame;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Rect background;
extern SDL_Event event;

extern double delta_time;
extern int    diff_time;

extern array <string, 56> palette;
extern array <array<int, SPRITE_SIZE>, BITMAP_SIZE> sprites;
extern array <array<int, SPRITE_SIZE>, BITMAP_SIZE> b;
extern array <array<int, TILE_SIZE>,   BITMAP_SIZE> tiles;
extern array <array<int, LEVEL_SIZE>, 16> levels;

struct Center{
    int x;
    int y;
};

class Stage{
    public:
        int left;
        int right;
        int top;
        int bottom;
        int width;
        int height;

        Stage(int w, int h);
};

class Control{
    public:
        bool left_key_down;
        bool right_key_down;
        bool up_key_down;
        bool down_key_down;
        bool key_down;

        void input();
        void keysup();
};

class Sprite{
    // private:
    public:
        int width;
        int height;
        int frame;
        double x;
        double y;
        bool gravitation;
        bool falling;
        bool bouncing;
        string direction;
        string state;

        array< array<int, 256>, 2> data;
        array< array<int, 256>, 2> frames;

    public:
        Sprite();
        void draw();
        void flip();
        void animate();
        void update();
        void move();
        void render();
};

class Player: public Sprite{
    public:
        int width;
        int height;
        int frame;
        double x;
        double y;
        bool gravitation;
        bool falling;
        bool bouncing;
        string direction;
        string state;
        map< string, array<array<int, SPRITE_SIZE>, FRAMES> > states;

        array< array<int, SPRITE_SIZE>, BITMAP_SIZE> data;
        array< array<int, 2>, 2> frames;

        Player();
        void init(double x, double y);
        void move();
        void update();
        void render();
        void draw(const array<array<int, SPRITE_SIZE>, FRAMES> &data);
        void define(string name, array<array<int, SPRITE_SIZE>, FRAMES> frames);
        array<array<int, SPRITE_SIZE>, FRAMES> flip(array<array<int, SPRITE_SIZE>, FRAMES> frames);
};

class Enemy: public Sprite{

};

class Gravity{
    // private:
    public:
        float gravity;
        float buoyancy;
        float lift;
        float speed;
        float delay;
        Player* player;
        Enemy* enemy;

        Gravity(float factor,  int delay);
        void bind(Player* player);
        void bind(Enemy enemy);
        void update();
        void reset();
};

class Physics{
    public:
        class Collision{
            public:
                void check();
        } collision;

        vector<Gravity> dropable;

        Physics();
        Gravity gravity(float factor,  int delay);
        void bounce();
        void update();
};

class Mapper{
    public:
        double x;
        double y;
        array<SDL_Texture*, 16> background;

        Mapper();
        void init();
        void compile(int level);
        void render();
        void draw(const array<int, TILE_SIZE> &data);
};

class Game{
    public:
        bool PAUSED;
        int  level;
        Control controls;
        Stage stage;
        Mapper mapper;

        Game();
        void init(int w, int h);
        void update();
        void render();
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

extern Game    game;
extern Physics physics;
extern Player  player;
extern Enemy   enemy;
// extern Tile    tile;

#endif