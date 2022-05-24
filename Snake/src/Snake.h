/*
 *    Snake.h - A library implementing the game of Snake
*/


#ifndef Snake_h
#define Snake_h

#include <LEDMatrixMagic.h>

typedef struct point {
    int x, y;
} point_t;

typedef struct node node_t;
typedef struct snake snake_t;

class List {
    node_t *start = nullptr;
    node_t *end = nullptr;
    size_t len;
public:
    void add_node_p(point_t p);
    void add_node_xy(int x, int y);
    void print_list();
    void pop_front();
    bool exists(point_t p);
    node_t *get_back();
    node_t *get_front();
    size_t get_len();
    ~List();
};


#define RIGHT 0
#define UP    1
#define DOWN  2
#define LEFT  3 


// list funcs
typedef struct node {
    point_t p;
    struct node* next;
} node_t;

typedef struct snake {
    List *body;
    int direction;
} snake_t;


class Snake {
    List *body = nullptr;
    int direction = UP;
public:
    Snake();
    ~Snake();
    // returns: 
    // -1 if it would hit itself
    // -1 if it would hit a wall
    // 1 if it would hit an apple
    // 0 otherwise
    int go_forward(point_t *apple);
    void set_direction(int dir);

    friend class Game;
};


class Game {
    byte **screen;
    int max_x, max_y;
    Snake snake;
    point_t apple{-1, -1};
    LEDMatrixMagic *m;
private:
    Game(LEDMatrixMagic *m);
public:
    static Game* get_game(LEDMatrixMagic *m);
    ~Game();
    void do_step();
    void generate_apple();
    void render_dot(int x, int y, int value);
    void render();
    void renderASCII();
    void set_renderer(LEDMatrixMagic *m);
    void get_input(char c);
    void resetGame();
    void game_over_print();
};

#endif



