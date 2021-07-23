#include "src/paddle.h"

#include "src/ball.h"
#include "src/pong.h"

const int Paddle::HEIGHT = 60;
const int Paddle::WIDTH = 10;

Paddle::Paddle(int new_x, int new_y) {
    bat.x = new_x - WIDTH/2;
    bat.y = new_y - HEIGHT/2;
    bat.w = WIDTH;
    bat.h = HEIGHT;
    predicted_y = Pong::SCREEN_HEIGHT/2;
}

int Paddle::get_x() const {
    return bat.x;
}

int Paddle::get_y() const {
    return bat.y;
}

SDL_Rect Paddle::get_bat() const{
    return bat;
}

int Paddle::get_yspeed() const {
    return yspeed;
}

void Paddle::set_y(int new_y) {
    bat.y = new_y - HEIGHT/2;
    if (bat.y < 0)
        bat.y = 0;
    else if (bat.y + HEIGHT > Pong::SCREEN_HEIGHT)
        bat.y = Pong::SCREEN_HEIGHT - HEIGHT;

}

void Paddle::set_yspeed(int new_yspeed) {    
    yspeed = new_yspeed;
}

void Paddle::add_to_y(int new_y) {
    bat.y += new_y;

    if (bat.y < 0)
        bat.y = 0;
    else if (bat.y + HEIGHT > Pong::SCREEN_HEIGHT)
        bat.y = Pong::SCREEN_HEIGHT - HEIGHT;
}

// Imprecise prediction of ball position on the y-axis.
int Paddle::predict(Ball *ball) {
    int swing = ball -> swing;
    int spin = ball -> spin;

    float steps = static_cast<float>(ball->puck.x - bat.x)/ball->dx;
//    int dots = ball->puck.x - bat.x; 

//    int tot_swing = (spin*dots*(dots + 1))/(4*Pong::SCREEN_WIDTH);

//    int dest  = ball->puck.y + steps*ball->dy + tot_swing;
    
    int dest  = ball->puck.y - steps*(ball->dy + swing + ((swing - ball->prev_swing)*(steps+1))/2);

    int number_of_reflexions = (dest / Pong::SCREEN_HEIGHT);

    // Predictions taking into consideration upper and bottom
    // wall collisions.

    // Even number of reflexions.
    if (number_of_reflexions % 2 == 0)
        dest = dest % Pong::SCREEN_HEIGHT;

    // Odd number of reflexions.
    else
        dest = Pong::SCREEN_HEIGHT - (dest % Pong::SCREEN_HEIGHT);

    return dest;
}


// Basic AI movement.
void Paddle::AI(Ball *ball) {
//    // Ball on the left 3/5th side of the screen and going left.
//    if (ball->x < Pong::SCREEN_WIDTH*3/5 && ball->dx < 0) {
//        yspeed = ball->speed/8 * 5;
//        // Follow the ball.
//        if (y + (HEIGHT - ball->LENGTH)/2 < ball->predicted_y-2)
//            add_to_y(yspeed);
//        else if (y + (HEIGHT - ball->LENGTH)/2 > ball->predicted_y+2)
//            add_to_y(-yspeed);
//    // Ball is anywhere on the screen but going right.
//    } else if (ball->dx >= 0) {
//        yspeed = 2;
//        // Left paddle slowly moves to the center.
//        if (y + HEIGHT / 2 < Pong::SCREEN_HEIGHT/2)
//            add_to_y(2);
//        else if (y + HEIGHT / 2 > Pong::SCREEN_HEIGHT/2)
//            add_to_y(-2);
//    }
    
    set_y(ball->puck.y);

    set_yspeed(ball->dy + ball->swing); 
}
