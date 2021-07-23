
#ifndef SRC_PADDLE_H_
#define SRC_PADDLE_H_

#include <SDL2/SDL.h>  
class Ball;

class Paddle {
 private:
    SDL_Rect bat;
    int yspeed;

 public:
    Paddle(int x, int y);

    static const int HEIGHT;
    static const int WIDTH;

    int predicted_y;

    int get_x() const;
    int get_y() const;
    SDL_Rect get_bat() const;
    int get_yspeed() const;
    void set_y(int new_y);
    void set_yspeed(int new_yspeed);
    void add_to_y(int new_y);
    int predict(Ball *ball); 
    void AI(Ball *ball);

};

#endif  
