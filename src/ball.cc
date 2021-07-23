#include "src/ball.h"
#include "src/pong.h"
#include "src/paddle.h"
#include <stdlib.h>

const int Ball::LENGTH = 10;

Ball::Ball(int x, int y) {
    status = READY;

    puck.x = x;
    puck.y = y;
    puck.w = LENGTH;
    puck.h = LENGTH;

    dx = 0;
    dy = 0;
    
    lhit = false;
    rhit = false;

    spin = 0;
    speed = 0;
    prev_swing = 0;
    swing = 0;

    hits = 0;

}

void Ball::launch_ball(Paddle *ai_paddle) {
    dx = (6 + rand()%4)*(rand()%2?1:-1);
    dy = (rand()%4)*(rand()%2?1:-1);
    spin = 0;

//    dx = 6;
//    dy = 0;
//    spin = 10;

    prev_swing = 0;
    swing = 0;
    
    status = LAUNCHED;

}

void Ball::bounces_off(Paddle *paddle) {
    if (paddle == nullptr) return;

    int yspeed = paddle->get_yspeed();

    dy = dy + (yspeed + (dx>0?1:-1)*spin)/4;
    spin = (dx>0?1:-1)*(yspeed + (yspeed>0?1:-1)*abs(spin))/2;

    dx = -dx;

}

void Ball::update_speed() {
    if (hits == 7){
        speed++;
        hits = 0;
    }

}

int Ball::wall_collision() {
    if(puck.y + dy + swing < 0)
        return -1;
    else if (puck.y + LENGTH + dy + swing >= Pong::SCREEN_HEIGHT)
        return 1;
    else 
        return 0;

}

bool Ball::collides_with(Paddle *paddle) {
    SDL_Rect temp = paddle->get_bat();
    if(SDL_HasIntersection(&puck, &temp)) {
        if (puck.x > Pong::SCREEN_WIDTH/2 && !rhit) {
            rhit = true;
            lhit = false;
            return true;
        } else if (puck.x < Pong::SCREEN_WIDTH/2 && !lhit){
            rhit = false;
            lhit = true;
            return true;
        }
    } 
    return false;
    
}

void Ball::reset() {
    puck.x = Pong::SCREEN_WIDTH/2 - LENGTH/2;
    puck.y = Pong::SCREEN_HEIGHT/2 - LENGTH/2;

    dx = 0;
    dy = 0;
    lhit = false;
    rhit = false;
    spin = 0;
    prev_swing = 0;
    swing = 0;
    status = READY;

    speed = 8;
    hits = 0;
}
