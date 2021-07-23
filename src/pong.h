#ifndef SRC_PONG_H_
#define SRC_PONG_H_

#include <SDL2/SDL.h>             
#include <SDL2/SDL_ttf.h>        
#include <SDL2/SDL_mixer.h>     

#include <string>

class Ball;
class Paddle;

class Pong {
 private:
    SDL_Window* window;  
    SDL_Renderer* renderer; 

    Ball* ball;
    Paddle* left_paddle;
    Paddle* right_paddle;

    int swing;

    Mix_Chunk* paddle_sound;

    Mix_Chunk* wall_sound;

    Mix_Chunk* score_sound;

    int mouse_x, mouse_y;  
    int rel_x, rel_y;

    std::string font_name;

    SDL_Color font_color;
    SDL_Texture* font_image_left_score;
    SDL_Texture* font_image_right_score;
    SDL_Texture* font_image_winner;
    SDL_Texture* font_image_restart;
    SDL_Texture* font_image_launch;

    int left_score;
    int right_score;

    bool left_score_changed;
    bool right_score_changed;

    bool exit;  

 public:
    static const int SCREEN_WIDTH;
    static const int SCREEN_HEIGHT;

    Pong(int argc, char *argv[]);
    ~Pong();
    void execute();
    void input();
    void update();
    void render();
};

#endif 
