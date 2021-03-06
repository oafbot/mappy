#ifndef __GAME_H__
#define __GAME_H__

// #include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

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
#include <set>

using namespace std;
#define START_LEVEL 1
#define PLAY 1
#define DISPLAY 0
#define ON  1
#define OFF 0
#define FRAME_COUNT 7
#define SCALE 2
#define DIM 16
#define SPEED 1.5
#define ENEMY_SPEED 1
#define BOSS_SPEED 1.25
#define WAVE_SPEED 2
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
#define DOOR_SIZE 576
#define BITMAP_SIZE 64
#define TILE_SIZE 64
#define ITEMS_SIZE 30
#define LEVEL_WIDTH 56
#define LEVEL_HEIGHT 36
#define LEVEL_SIZE LEVEL_WIDTH*LEVEL_HEIGHT //2016
#define DOOR_RANGE 6
#define GROUND_FLOOR 480
#define FLOORS 6
#define FLOOR_HEIGHT ((LEVEL_HEIGHT-10)/FLOORS)
#define OVERTIME 80000
#define TIME_LIMIT 100000

#define EMPTY 0
#define SOLID 1
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
#define BALLOON 13
#define BONUS_BALLOON 14

#define WHITE 2
#define RED 30
#define GREEN 42
#define BLUE 13
#define YELLOW 36
#define ORANGE 37
#define PINK 21

#define AUDIO_THEME (char*)"/audio/theme.ogg"
#define AUDIO_BONUS (char*)"/audio/bonus.ogg"
#define AUDIO_FAST (char*)"/audio/fast.ogg"
#define AUDIO_START (char*)"/audio/start.ogg"
#define AUDIO_FANFARE (char*)"/audio/fanfare.ogg"
#define AUDIO_RESULTS (char*)"/audio/results.ogg"
#define AUDIO_DEAD (char*)"/audio/dead.ogg"
#define AUDIO_CLEAR (char*)"/audio/clear.ogg"
#define AUDIO_ITEM (char*)"/audio/item.ogg"
#define AUDIO_WAVE (char*)"/audio/wave.ogg"
#define AUDIO_BELL (char*)"/audio/bell.ogg"
#define AUDIO_JUMP (char*)"/audio/trampoline.ogg"
#define AUDIO_GAME_OVER (char*)"/audio/gameover.ogg"
#define AUDIO_HURRY1 (char*)"/audio/hurryx1.ogg"
#define AUDIO_HURRY2 (char*)"/audio/hurryx2.ogg"


class Stage;
class Sound;
class Text;
class Control;
class Timer;
template <class T> class Collider;
class Gravity;
class Sprite;
class GameObject;
class Trampoline;
class Item;
class Wave;
class Door;
class Balloon;
class Bell;
class Player;
class Enemy;
class Physics;
class Mapper;
class Game;
class Draw;
class Title;
class Demo;

// class FieldInterface{
//     int m_Size;
//     ~FieldInterface() = default;
// };

// template <typename T>
// class Field : public FieldInterface {
//     T m_Value;
// };

struct Data{
    array <string, 56> palette;
    array <array<int, SPRITE_SIZE>, BITMAP_SIZE> sprites;
    array <array<int, TILE_SIZE>,   BITMAP_SIZE> trampoline;
    array <array<int, TILE_SIZE>, 160> tiles;
    array <array<int, DOOR_SIZE>,  12> doors;
    array <array<int, LEVEL_SIZE>, 16> levels;
    array <array<int, LEVEL_SIZE>, 16> interactive;
    array <array<int, LEVEL_WIDTH*8>, 16> foreground;
    array <array<int, SPRITE_SIZE>, ITEMS_SIZE> items;
    array <array<int, TILE_SIZE>, 48> alpha;
    array <array<int, SPRITE_SIZE>, 32> points;
    array <array<int, 1024>, 11> title;
    array <int, 16> enemies;
    array <array<int, 2>, 10> spawn;
};

struct Coordinates{
    double x;
    double y;
};

struct Info{
    string type;
    string name;
    double x;
    double y;
    int    timestamp;
    bool   expired;
    int    direction;
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

class Sound{
    public:
        int flags;
        int rate;
        int buffers;
        Uint16 format;
        Sound();
        void init();

        class Effects{
            public:
                map< string, Mix_Chunk* > library;
                map< string, int > channels;
                void load(string name, char* file);
                int  play(string name);
                void play(string name, int channel);
                int  loop(string name);
                void loop(string name, int channel);
                void pause();
                void stop();
                void stop(int channel);
                void resume();
                bool playing(int channel);
        } effects;

        class Music{
            public:
                Mix_Music* bgm;
                map< string, Mix_Music* > library;
                void load(string name, char* file);
                void play(string name);
                void loop(string name);
                void pause();
                void stop();
                void resume();
                bool playing();
        } music;
};

class Text{
    public:
        int  height;
        int  width;
        int  kern;
        int  unit;
        int  cols;
        int  rows;
        bool visible;
        bool blink;

        struct TextBox{
            int width;
            int height;
        } box;

        vector<int> colors;
        map< int, map<char, SDL_Texture*> > alpha;

        Text();
        void init(vector<int> c, int w, int h);
        void compile();
        void draw(const array<int, TILE_SIZE> &bits, int color);
        void render(string text, int color, Coordinates offset);
        void render(string text, int color, int col, int row);
        void center(string text, int color, int row);
        int  getWidth(string text);
        void indent(string text, int color, int full, Coordinates c);
        void score(int score, int digits);
        void score(int score);
        void hiscore();
        // void color(int color);
};

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

class Timer{
    vector<Uint32> timestamp;
    vector<Uint32> duration;

    public:
        int start(Uint32 timeout);
        bool done(int id);
        void reset(int id);
};

template <class T>
class Collider{
    public:
        int width;
        int height;
        int offset;
        double x;
        double y;
        bool passthru;
        T* object;

    Collider(T* obj);
    void init(double x, double y, int w, int h);
    void update(double x, double y);
    template <class T2>
    bool check(Collider<T2>* collider);
    void debug();
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
        void bound(T* sprite, Trampoline trampoline);
        template <class T>
        bool fallthru(T* sprite);
};

class GameObject{
    public:
        string state;
        string type;
        double x;
        double y;
        int    width;
        int    height;
        int    frame;
        int    cycle;
        int    repeat;
        bool   animated;
        bool   assigned;
        bool   loop;
        bool   active;

        vector<int>  group;
        SDL_Texture* cache;

        GameObject();
        void init();
        void draw(const array<int, SPRITE_SIZE> &bits);
        void draw(const array<int, SPRITE_SIZE> &bits, Coordinates offset);
        void render();
        void reset();
        void compile();
        void assign(int index);
};

class Trampoline: public GameObject{
    public:
        int bounces;
        bool targeted;
        string jumper;

        vector< Coordinates > layout;
        map< string, vector< array<array<int, TILE_SIZE>, FRAMES> > >states;
        map<string, array<SDL_Texture*, FRAMES> >* cache;

        Trampoline();
        void init();
        void assign(int index);
        void reset();
        void update();
        void render();
        void bounce();
        void clear();

        void define(string name, vector< array<array<int, TILE_SIZE>, FRAMES> > frames);
        void compile();
        void draw(const array<int, TILE_SIZE> &bits, Coordinates offset);
        int  tier();

        array<int, TILE_SIZE>
        flip(const array<int, TILE_SIZE> frames);

        vector< array<array<int, TILE_SIZE>, FRAMES> >
        changeColor(vector< array<array<int, TILE_SIZE>, FRAMES> > grouped, int color);
};

class Item: public GameObject{
    public:
        int  id;
        int  points;
        bool collected;

        string state;
        string type;

        Item(int id);
        void assign(int index);
        void reset();
        void compile();
        void render();
        void collect();
};

class Wave: public GameObject{
    public:
        string state;
        int direction;
        int id;
        int channel;

        Collider<Wave> *collider;
        vector<Enemy*> captured;
        map<string, array<SDL_Texture*, FRAMES> >* cache;
        vector< Coordinates > layout;
        map< string, vector< array<array<int, SPRITE_SIZE>, FRAMES> > >states;

        Wave(int direction, int id);
        void init();
        void assign(int index);
        void define(string name, vector< array<array<int, SPRITE_SIZE>, FRAMES> > frames);
        void reset();
        void render();
        void compile();
        void activate();
        bool range();
        void cleanup();
        void update();
        void release();
        void score();

        array<int, SPRITE_SIZE>
        flip(const array<int, SPRITE_SIZE> bits);
};

class Door: public GameObject{
    public:
        string state;
        string type;
        string key;
        bool   open;
        int    direction;
        int    id;
        int    uid;

        struct Offset{
            double x;
            double y;
        } offset;

        map< string, vector< array<int, DOOR_SIZE> > > states;
        map<string,  vector<SDL_Texture*> >* cache;

        Wave wave;

        Door(int id, int uid);
        void assign(int index);
        void define(string name, vector< array<int, DOOR_SIZE> > frames);
        void draw(const array<int, DOOR_SIZE> &bits);
        void reset();
        void render();
        void compile();
        void operate();
        bool range();
        void knockout();
        void shockwave();
        void cleanup();

        vector< array<int, DOOR_SIZE> > flip(const vector< array<int, DOOR_SIZE> > frames);
};

class Balloon: public GameObject{
    public:
        string state;
        string type;
        int    points;
        bool   collected;
        array< array<int, SPRITE_SIZE>, 4> frames;
        array<array< array<int, SPRITE_SIZE>, 4>, 4> bonus_frames;
        array<SDL_Texture*, 4>* cache;

        Balloon();
        Balloon(bool bonus);
        void assign(int index);
        void reset();
        void render();
        void compile();
        void cleanup();
        void collect();
};

class Bell: public GameObject{
    public:
        // int  id;
        int  points;
        bool dropped;
        bool tallied;
        // string state;
        Collider<Bell> *collider;
        vector<Enemy*> squashed;

        Bell();
        // void init(int index);
        void assign(int index);
        void reset();
        void render();
        void compile();
        void cleanup();
        void drop();
        void update();
        void tally();
};

class Points: public GameObject{
    public:
        map<string, array<int, SPRITE_SIZE> > states;
        map<string, SDL_Texture*>* cache;
        vector<Info> display;

        Points();
        void define(string name, array<int, SPRITE_SIZE> bits);
        void compile();
        void render();
        void item(int points, int factor, Coordinates pos);
        void wave(int points, bool dub, Coordinates pos);
        void reset();
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
        bool animated;

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
        virtual void compile() = 0;
        // virtual void draw(const array<array<int, SPRITE_SIZE>, FRAMES> &data) = 0;
        virtual void draw(const array<int, SPRITE_SIZE> &bits) = 0;
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
        bool animated;
        bool gravitation;
        bool falling;
        bool bouncing;
        bool dead;
        Uint32 timestamp;
        string direction;
        string state;
        string type;
        map< string, array<array<int, SPRITE_SIZE>, FRAMES> > states;
        map<string, array<SDL_Texture*, FRAMES> >* cache;

        Collider<Player> *collider;
        Gravity *gravity;
        Trampoline trampoline;

        Player();
        virtual void init(double x, double y);
        virtual void move();
        virtual void update();
        virtual void render();
        virtual void compile();
        virtual void define(string name, array<array<int, SPRITE_SIZE>, FRAMES> frames);
        virtual int  index();
        virtual int  index(double x, double y);
        virtual int  adjacent(int direction);
        virtual int  adjacent(int direction, double x, double y);
        virtual bool traverse(int direction);
        virtual bool traverse(int direction, double x, double y);
        virtual array<array<int, SPRITE_SIZE>, FRAMES>
            flip(array<array<int, SPRITE_SIZE>, FRAMES> frames);
        virtual void align();
        virtual void align(bool horiz);
        virtual void deaded();
        virtual void bounce();
        virtual void ledge();

        // void draw(const array<array<int, SPRITE_SIZE>, FRAMES> &data);
        void draw(const array<int, SPRITE_SIZE> &bits);
        // template <class T>
        // bool collision(Collider<T> complement);
        void collision();
        void collect();
        void door();
        void reset(double x, double y);
};

class Enemy: public Sprite{
    public:
        double x;
        double y;
        double slide;
        double speed;
        int id;
        int width;
        int height;
        int frame;
        int tier;
        int bounces;
        bool active;
        bool gravitation;
        bool falling;
        bool bouncing;
        // bool ko;
        bool animated;
        bool captured;
        bool released;
        bool blink;

        Uint32 timestamp;
        Uint32 regenerate;
        string direction;
        string state;
        string type;
        string mode;
        vector<int> blinkon;
        map< string, array<array<int, SPRITE_SIZE>, FRAMES> > states;
        map<string, array<SDL_Texture*, FRAMES> >* cache;

        Collider<Enemy> *collider;
        Gravity *gravity;
        Trampoline trampoline;

        Enemy(int id);
        // ~Enemy();
        bool operator!=(const Enemy &other){ return id != other.id; }
        virtual void init(double x, double y);
        virtual void move();
        virtual void update();
        virtual void render();
        virtual array<array<int, SPRITE_SIZE>, FRAMES>
            flip(array<array<int, SPRITE_SIZE>, FRAMES> frames);
        virtual void define(string name, array<array<int, SPRITE_SIZE>, FRAMES> frames);
        virtual int index();
        virtual int index(double x, double y);
        virtual int adjacent(int direction);
        virtual int adjacent(int direction, double x, double y);
        virtual bool traverse(int direction);
        virtual bool traverse(int direction, double x, double y);
        virtual void align();
        virtual void align(bool horiz);
        virtual void deaded();
        virtual void draw(const array<array<int, SPRITE_SIZE>, FRAMES> &data){};
        virtual void bounce();
        virtual void ledge();

        void draw(const array<int, SPRITE_SIZE> &bits);
        void compile();
        void koed();
        void wander();
        void pursue();
        void decision();
        void walk();
        void hopoff();
        void hop(int i);
        bool aligned();
        void jump();
        void knockedout(int direction, double door_x);
        void reset(double x, double y);
        void respawn();
        void capture(int direction);
        void release(int delay);
        void deactivate();
        void opendoor(int direction);
        void spaceout();
};

class Boss: public Enemy{
    public:
        int hiding;
        map< string, array<array<int, SPRITE_SIZE>, FRAMES> > states;
        map<string, array<SDL_Texture*, FRAMES> >* cache;

        Collider<Boss> *collider;
        Gravity *gravity;

        virtual void define(string name, array<array<int, SPRITE_SIZE>, FRAMES> frames);

        Boss();
        void compile();
        void render();
        void draw(const array<int, SPRITE_SIZE> &bits);
        void update();
        void walk();
        void decision();
        void move();
        void hide(int index);
        void pounce();
        void reset(double x, double y);
        void reward();
        int  behind();
        void balloons();
};

class Gosenzo: public Enemy{
    public:
        map< string, array<array<int, SPRITE_SIZE>, FRAMES> > states;
        map<string, array<SDL_Texture*, FRAMES> >* cache;

        Collider<Gosenzo> *collider;

        virtual void define(string name, array<array<int, SPRITE_SIZE>, FRAMES> frames);

        Gosenzo();
        void compile();
        void render();
        void draw(const array<int, SPRITE_SIZE> &bits);
        void update();
        void walk();
        void fork();
        void decision();
        void move();
        void reset();
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

class Title{
    public:
        int width;
        int height;
        SDL_Texture* cache;
        Coordinates position;

        void display();
        void compile();
        void render(double x, double y);
};

class Draw{
    public:
        static void clear();
        static void render(SDL_Texture* cache, double x, double y);

        template <class T>
        static void plot(const T &bits, int units, Coordinates offset);

        template <class T>
        static SDL_Texture* compile(T data, int cols, int units);

        template <class T>
        static SDL_Texture* compile(T data, int units);
};

class Demo{
    public:
        Title title;
        vector<Coordinates> position;
        int scene;
        array<string, 16> lines;
        SDL_Texture* logo;
        double logox;

        Demo();
        void cast();
        void start();
        void reset();
        void animate();
        void credits(double goal);
        void scrollLeft(string text, int pos, int inc, int color);
        void hide(double x, double y, int width, int height);
};

class Mapper{
    public:
        double x;
        double y;
        array<SDL_Texture*, 16> background;
        array<SDL_Texture*, 16> foreground;

        Mapper();
        void init();
        void compile();
        void render(string layer);
        void draw(const array<int, TILE_SIZE> &data);
        void draw(const array<int, SPRITE_SIZE> &bits, Coordinates offset);
};

class Game{
    public:
        bool PAUSED;
        bool WAIT;
        bool RESET;
        bool COMPLETE;
        bool GAME_OVER;
        bool START;
        bool scrolling;
        bool music;
        bool perfect;
        bool bonus;
        int  mode;
        int  level;
        int  tiers;
        int  hiscore;
        int  pickup;
        int  factor;
        int  lives;
        int  timeout;
        int  overtime;
        int  round;
        double hurry;
        Uint32 limit;
        Uint32 score;
        Uint32 lifeup;
        Uint32 timestamp;
        Uint32 start_time;
        Uint32 current_time;
        Uint32 last_time;
        Uint32 pause_time;
        Uint32 ms_per_frame;
        Uint32 delta;
        Uint32 time;

        struct Center{
            double x;
            double y;
        } center;

        struct Offset{
            double x;
            double y;
        } offset;

        string state;
        // struct State{
        //     bool GAME_OVER;
        // } state;

        Control controls;
        Stage   stage;
        Mapper  mapper;
        Sound   sound;
        Timer   timer;
        Text    text;
        Demo    demo;
        Points  points;
        Balloon bonus_balloon;

        vector<int>   skip;
        vector<int>   collected;
        vector<Enemy> enemies;
        vector<Item>  items;
        vector<Door>  doors;
        vector<Trampoline> trampolines;
        vector<Balloon> balloons;
        vector<Bell> bells;
        vector<string> cached;

        struct Cache{
            map<string, array<SDL_Texture*, FRAMES> > player;
            map<string, array<SDL_Texture*, FRAMES> > enemy;
            map<string, array<SDL_Texture*, FRAMES> > boss;
            map<string, array<SDL_Texture*, FRAMES> > gosenzo;
            map<string, array<SDL_Texture*, FRAMES> > trampoline;
            map<string, array<SDL_Texture*, FRAMES> > wave;
            map<string, map<string, vector<SDL_Texture*> > > door;
            map<string, SDL_Texture*> item;
            map<string, SDL_Texture*> points;
            vector<SDL_Texture*> tiles;
            array<SDL_Texture*, 4> balloon;
            array<SDL_Texture*, 4> big_balloon;
            array<SDL_Texture*, 16> background;
            array<SDL_Texture*, 11> title;
            SDL_Texture* badge;
            SDL_Texture* reward;
            SDL_Texture* gameover;
            SDL_Texture* hurry;
        } cache;

        Game();
        void init(int w, int h);
        void update();
        void render();
        bool interval();
        bool delay(int delay, Uint32 start);
        void setup();
        void renderObjects();
        void renderLives();
        int  trampoline();
        void reset();
        void restart();
        void restage();
        void loop();
        void clear();
        void complete();
        void start();
        void pause();
        void playBGM();
        void stopBGM();
        void results();
        void interlude();
        void gameover();
        void speedup();
        void overlimit();
        void clock();
        void warning();
        bool isBonusRound(int lvl);
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

// em_arg_callback_func status(em_arg_callback_func status){
//     cout << status << endl;
//     return status;
// }

extern Game    game;
extern Physics physics;
extern Player  player;
extern Boss    goro;
extern Gosenzo gosenzo;
extern struct  Data data;

extern SDL_Window   *window;
extern SDL_Renderer *renderer;
extern SDL_Event     event;

#endif