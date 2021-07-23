#ifndef SRC_BALL_H_
#define SRC_BALL_H_

#include <SDL2/SDL.h>

class Paddle;

class Ball {
 public:
        Ball(int x, int y);
        ~Ball() {}

        enum Status {READY, LAUNCHED};
        Status status;
        
        SDL_Rect puck;
        
        int dx;
        int dy;
        static const int LENGTH;
       
        bool lhit;
        bool rhit;

        int spin;
        int speed;
        int prev_swing;
        int swing;
                   
        int hits;

        void launch_ball(Paddle *ai_paddle);
        void update_speed();
        int wall_collision();
        bool collides_with(Paddle *paddle);
        void bounces_off(Paddle *paddle);
        void reset();

};

#endif  
