#include "src/pong.h"

#include <iostream>

#include "src/ball.h"
#include "src/paddle.h"
#include "src/utilities.h"

const int Pong::SCREEN_WIDTH = 1366;
const int Pong::SCREEN_HEIGHT = 768;
const int HEIGHT = 60;

Pong::Pong(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_ShowCursor(0);
    window = SDL_CreateWindow("Pong",
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN || SDL_WINDOW_FULLSCREEN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
            SDL_RENDERER_PRESENTVSYNC);

    ball = new Ball(SCREEN_WIDTH/2-ball->LENGTH/2,
            SCREEN_HEIGHT/2-ball->LENGTH/2);
    left_paddle = new Paddle(40, SCREEN_HEIGHT/2-Paddle::HEIGHT/2);
    right_paddle = new Paddle(SCREEN_WIDTH-(40+Paddle::WIDTH),
            SCREEN_HEIGHT/2-Paddle::HEIGHT/2);

    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
    paddle_sound = Mix_LoadWAV("resources/sounds/paddle_hit.wav");
    wall_sound = Mix_LoadWAV("resources/sounds/wall_hit.wav");
    score_sound = Mix_LoadWAV("resources/sounds/score_update.wav");

    SDL_WarpMouseInWindow(window, SCREEN_WIDTH - Paddle::HEIGHT, SCREEN_HEIGHT/2);
    SDL_GetMouseState(&mouse_x, &mouse_y);
    
    TTF_Init();  // Initialize font.
    font_color = {255, 255, 255, 255};
    font_name = "resources/fonts/NES-Chimera/NES-Chimera.ttf";
    font_image_launch = renderText("Press SPACE to start",
            font_name, font_color, 16, renderer);

    exit = false;

}

Pong::~Pong() {
    Mix_FreeChunk(paddle_sound);
    Mix_FreeChunk(wall_sound);
    Mix_FreeChunk(score_sound);

    Mix_CloseAudio();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

void Pong::execute() {
    while (!exit) {
        input();
        update();
        render();
        SDL_Delay(10);
    }
}

void Pong::input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEMOTION) {
            SDL_GetMouseState(&mouse_x, &mouse_y);
            SDL_GetRelativeMouseState(&rel_x, &rel_y);
        }

        if (event.type == SDL_QUIT) {
            exit = true;
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    exit = true;
                    break;

                case SDLK_SPACE:
                    if (ball->status == ball->READY) {
                        ball->status = ball->LAUNCHED;
                        ball->launch_ball(left_paddle);
                    }
                    break;
                
                case SDLK_r:
                    ball->reset();
                    break;

                case SDLK_F11:
                    int flags = SDL_GetWindowFlags(window);
                    if (flags & SDL_WINDOW_FULLSCREEN) {
                        SDL_SetWindowFullscreen(window, 0);
                    } else {
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                    }
                    break;
            }
        }
    }
}

void Pong::update() {
    right_paddle->set_y(mouse_y);
    right_paddle->set_yspeed(rel_y);

    left_paddle->AI(ball);

    if (ball->status == ball->READY) 
        return;

    ball->update_speed();

    if (ball->collides_with(left_paddle)) {
        ball->bounces_off(left_paddle);
        Mix_PlayChannel(-1, paddle_sound, 0); 
    } else if (ball->collides_with(right_paddle)) {
        ball->bounces_off(right_paddle);
        Mix_PlayChannel(-1, paddle_sound, 0);
    }

    if (int loc = ball->wall_collision()) {
        ball->dx += (loc*ball->spin)/4; 
        ball->spin = (ball->spin)/((ball->dx>0?1:-1)*loc*2);
        ball->dy *= -1; 
        Mix_PlayChannel(-1, wall_sound, 0);  
    }

    ball->puck.x += ball->dx;

    if (ball->dx > 0)
        swing = (ball->puck.x * (ball->dx/4) * (ball->spin/2))/SCREEN_WIDTH;
    else  
        swing = ((SCREEN_WIDTH - ball->puck.x) * (ball->dx/4) * ball->spin/2)/SCREEN_WIDTH;

    ball->puck.y += ball->dy + swing;
    ball->prev_swing = ball->swing;
    ball->swing = swing;

    if (ball->puck.x > SCREEN_WIDTH || ball->puck.x < 0) {
        Mix_PlayChannel(-1, score_sound, 0);
        ball->reset();
    }

}

void Pong::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect paddle1 = left_paddle->get_bat();
    SDL_RenderFillRect(renderer, &paddle1);

    SDL_Rect paddle2 = right_paddle->get_bat();
    SDL_RenderFillRect(renderer, &paddle2);

    SDL_Rect pong_ball = ball->puck;
    SDL_RenderFillRect(renderer, &pong_ball);
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect pr = {60, left_paddle->predict(ball),10,10};
    SDL_RenderFillRect(renderer, &pr);

//    SDL_RenderDrawLine(renderer, pong_ball.x + 5, pong_ball.y + 5, (pong_ball.x + 5 + 6*ball->dx), (pong_ball.y + 5 + 6*(ball->dy + ball->swing)));
    
    if(ball->status == Ball::READY){
    renderTexture(font_image_launch,
                renderer, SCREEN_WIDTH / 2 - 160, SCREEN_HEIGHT - 30);
    }
    SDL_RenderPresent(renderer);
}

