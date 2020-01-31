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

class Player{
    public:
        float x;
        float y;
        int frame;
        // float player_rotation = PI;
        // float player_dx = 0.0;
        // float player_dy = 1.0;
        // float player_vx = 0.0;
        // float player_vy = 0.0;
};

// SDL_Color hex2sdl(std::string input) {
//     if (input[0] == '#')
//         input.erase(0, 1);

//     unsigned long value = stoul(input, nullptr, 16);

//     SDL_Color color;

//     color.a = (value >> 24) & 0xff;
//     color.r = (value >> 16) & 0xff;
//     color.g = (value >> 8) & 0xff;
//     color.b = (value >> 0) & 0xff;
//     return color;
// }