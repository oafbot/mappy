#ifndef __GAME_H__
#define __GAME_H__

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
#include <iostream>
using namespace std;

#define FRAME_COUNT 7
#define SCALE 2
#define DIM 16
#define SPEED 1.5
#define ENEMY_SPEED 1.25
#define BYTE 8
#define WORD 16
#define LONG 32
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define UP_RIGHT 4
#define DOWN_RIGHT 5
#define DOWN_LEFT 6
#define UP_LEFT 7
#define OFFSET 64
#define FRAMES 8
#define BOUNCES 3
#define SPRITE_SIZE 256
#define BITMAP_SIZE 64
#define TILE_SIZE 64
#define LEVEL_WIDTH 56
#define LEVEL_HEIGHT 36
#define LEVEL_SIZE LEVEL_WIDTH*LEVEL_HEIGHT //2016
#define TRAMPOLINE 2
#define DOOR_LEFT 3
#define DOOR_RIGHT 4
#define MAGIC_DOOR_L 5
#define MAGIC_DOOR_R 6
#define RADIO 7
#define TV 8
#define COMPUTER 9
#define PAINTING 10
#define SAFE 11
#define BELL 12

extern Uint32 last_time;
extern Uint32 last_frame_time;
extern Uint32 current_time;
extern Uint32 ms_per_frame;
extern int current_frame;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
// extern SDL_Rect background;
extern SDL_Event event;

extern double delta_time;
extern int    diff_time;

struct Data{
    array <string, 56> palette;
    array <array<int, SPRITE_SIZE>, BITMAP_SIZE> sprites;
    array <array<int, TILE_SIZE>,   BITMAP_SIZE> objects;
    array <array<int, TILE_SIZE>,   BITMAP_SIZE> tiles;
    array <array<int, LEVEL_SIZE>, 16> levels;
    array <array<int, LEVEL_SIZE>, 16> interactive;
    array <array<int, SPRITE_SIZE>, 6> items;
    array <int, 12> enemies;
    array <array<int, 2>, 10> spawn;
};

struct Coordinates{
    double x;
    double y;
};

class Stage{
    public:
        int left;
        int right;
        int top;
        int bottom;
        int width;
        int height;
        int full_width;
        int full_height;

        Stage(int w, int h);
};

// class FieldInterface{
//     int m_Size;
//     ~FieldInterface() = default;
// };

// template <typename T>
// class Field : public FieldInterface {
//     T m_Value;
// };

class Control{
    public:
        bool left_key_down;
        bool right_key_down;
        bool up_key_down;
        bool down_key_down;
        bool key_down;
        bool lock;

        void input();
        void keysup();
};

class Sprite{
    public:
        double x;
        double y;

        int width;
        int height;
        int frame;
        int cycle;
        int repeat;

        bool gravitation;
        bool falling;
        bool bouncing;
        bool dead;
        bool passthru;

        string direction;
        string state;
        string type;

        map< string, array<array<int, SPRITE_SIZE>, FRAMES> > states;
        //Collider *collider;

        // Sprite();
        // Sprite(const Sprite &S);
        // virtual ~Sprite();
        // virtual void animate() = 0;
        virtual void init(double x, double y) = 0;
        virtual void update() = 0;
        virtual void move() = 0;
        virtual void render() = 0;
        virtual void draw(const array<array<int, SPRITE_SIZE>, FRAMES> &data) = 0;
        virtual void define(string name, array<array<int, SPRITE_SIZE>, FRAMES> frames) = 0;
        virtual int  index(double x, double y) = 0;
        virtual int  adjacent(int direction) = 0;
        virtual int  adjacent(int direction, double x, double y) = 0;
        virtual bool traverse(int direction) = 0;
        virtual bool traverse(int direction, double x, double y) = 0;
        virtual void align()  = 0;
        virtual void deaded() = 0;
        virtual void bounce() = 0;
        virtual void ledge()  = 0;
        virtual array<array<int, SPRITE_SIZE>, FRAMES>
            flip(array<array<int, SPRITE_SIZE>, FRAMES> frames) = 0;
        // array<double, 2> position();
        // bool collision(Collider complement);
};

class GameObjects{

};

class GameObject{
    public:
        string state;
        string type;
        int width;
        int height;
        int frame;
        int cycle;
        int repeat;
        double x;
        double y;
        bool animated;
        bool loop;
        bool assigned;
        bool active;
        vector<int> group;
        vector< Coordinates > layout;
        map< string, vector< array<array<int, TILE_SIZE>, FRAMES> > >states;
        map<string, array<SDL_Texture*, FRAMES> > cache;

        GameObject();
        void init();
        void update();
        void render();
        void reset();
        void compile();
        void assign(int index);
        void define(string name, vector< array<array<int, TILE_SIZE>, FRAMES> > frames);
        void draw(const array<int, TILE_SIZE> &bits, Coordinates offset);
        array<int, TILE_SIZE> flip(const array<int, TILE_SIZE> frames);
};

class Trampoline: public GameObject{
    public:
        int bounces;
        string jumper;

        Trampoline();
        void init();
        void assign(int index);
        void reset();
        void render();
        void bounce();

        vector< array<array<int, TILE_SIZE>, FRAMES> >
        changeColor(vector< array<array<int, TILE_SIZE>, FRAMES> > grouped, int color);
};

class Door: public GameObject{
    public:
        string state;
        string type;
        bool   open;

        Door();
        void init();
        void assign(int index);
        void reset();
        void render();
};

class Item: public GameObject{
    public:
        int  id;
        int  points;
        bool collected;

        string state;
        string type;

        SDL_Texture* cache;

        Item(int id);
        void init();
        void assign(int index);
        void reset();
        void compile();
        void draw(const array<int, SPRITE_SIZE> &bits);
        void render();
        void collect();
};

template <class T>
class Collider{
    public:
        int width;
        int height;
        double x;
        double y;
        T* object;

    Collider(T* obj);
    void init(double x, double y, int w, int h);
    void update(double x, double y);
    template <class T2>
    bool check(Collider<T2> collider);
};

class Gravity{
    public:
        float gravity;
        float buoyancy;
        float lift;
        float speed;
        float delay;
        float min;
        float max;
        string type;

        Gravity(float factor,  int delay);

        template <class T>
        void update(T* sprite);
        void reset();
        template <class T>
        void bound(T* sprite);
        template <class T>
        bool fallthru(T* sprite);
};

class Player: public Sprite{
    public:
        double x;
        double y;
        int width;
        int height;
        int frame;
        int cycle;
        int repeat;
        int bounces;
        bool gravitation;
        bool falling;
        bool bouncing;
        bool dead;
        bool passthru;
        string direction;
        string state;
        string type;
        map< string, array<array<int, SPRITE_SIZE>, FRAMES> > states;
        struct Absolute{
            double x;
            double y;
        } absolute;

        Collider<Player> *collider;
        Gravity *gravity;

        Player();
        // ~Player();
        // Player(const Player &P);             // copy constructor
        // Player & operator=(const Player &P); // assignment
        virtual void init(double x, double y);
        virtual void move();
        virtual void update();
        virtual void render();
        virtual void define(string name, array<array<int, SPRITE_SIZE>, FRAMES> frames);
        virtual int  index(double x, double y);
        virtual int  adjacent(int direction);
        virtual int  adjacent(int direction, double x, double y);
        virtual bool traverse(int direction);
        virtual bool traverse(int direction, double x, double y);
        virtual array<array<int, SPRITE_SIZE>, FRAMES>
            flip(array<array<int, SPRITE_SIZE>, FRAMES> frames);
        // array<double, 2> position();
        virtual void align();
        virtual void align(bool horiz);
        virtual void deaded();
        virtual void bounce();
        virtual void ledge();

        void draw(const array<array<int, SPRITE_SIZE>, FRAMES> &data);
        template <class T>
        bool collision(Collider<T> complement);
        void collect();
};

class Enemy: public Sprite{
    public:
        double x;
        double y;
        int width;
        int height;
        int frame;
        int tier;
        int bounces;
        bool gravitation;
        bool falling;
        bool bouncing;
        bool ko;
        bool animated;
        bool passthru;
        // Uint32 current;
        Uint32 timestamp;
        string direction;
        string state;
        string type;
        string mode;
        map< string, array<array<int, SPRITE_SIZE>, FRAMES> > states;
        map<string, array<SDL_Texture*, FRAMES> > cache;
        struct Absolute{
            double x;
            double y;
        } absolute;
        Collider<Enemy> *collider;
        Gravity *gravity;

        Enemy();
        // ~Enemy();
        virtual void init(double x, double y);
        virtual void move();
        virtual void update();
        virtual void render();
        virtual array<array<int, SPRITE_SIZE>, FRAMES>
            flip(array<array<int, SPRITE_SIZE>, FRAMES> frames);
        virtual void define(string name, array<array<int, SPRITE_SIZE>, FRAMES> frames);
        virtual int index(double x, double y);
        virtual int adjacent(int direction);
        virtual int adjacent(int direction, double x, double y);
        virtual bool traverse(int direction);
        virtual bool traverse(int direction, double x, double y);
        // array<double, 2> position();
        virtual void align();
        virtual void align(bool horiz);
        virtual void deaded();
        virtual void draw(const array<array<int, SPRITE_SIZE>, FRAMES> &data){};
        virtual void bounce();
        virtual void ledge();

        void draw(const array<int, SPRITE_SIZE> &bits);
        void compile();
        void koed();
        bool collision(Collider<Player> complement);
        void wander();
        void pursue();
        void decision();
        void walk();
        void hopoff();
        void hop(int i);
        bool aligned();
        void trampoline();
};

class Physics{
    public:
        class Collision{
            public:
                void check();
        } collision;

        Physics();
        Gravity* gravity(float factor,  int delay);
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
        bool scrolling;
        int  level;
        int  tiers;
        int  score;
        int  hiscore;
        int  pickup;
        int  factor;

        struct Center{
            double x;
            double y;
        } center;

        struct Offset{
            double x;
            double y;
        } offset;

        Control controls;
        Stage stage;
        Mapper mapper;
        vector<int>   skip;
        vector<Enemy> enemies;
        vector<Item>  items;
        map< string, vector<Trampoline> > objects;
        // Timer timer;

        Game();
        void init(int w, int h);
        void update();
        void render();
        bool delay();
        bool delay(int delay, Uint32 start);
        void setup();
        void renderObjects();
        int trampoline();
};

// *************
//    helpers
// *************

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

template <typename T>
bool contains(vector<T> haystack, T needle){
    return find(begin(haystack), end(haystack), needle)!=end(haystack);
}


extern Game    game;
extern Physics physics;
extern Player  player;
extern Enemy   enemy;
extern struct Data data;
// extern struct Coordinates;

#endif