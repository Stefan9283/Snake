#include "Snake.h"
#include <Arduino.h>
#include <SPI.h>
#include <string.h>

#define MAX_X 16
#define MAX_Y 16

using namespace std;

Game *game_ptr = nullptr;

void List::add_node_p(point_t p) {
    node_t *n = calloc(1, sizeof(node_t));
    n->p = p;

    if (start == NULL) {
        start = n;
        end = n;
        return;
    }

    end->next = n;
    end = n;

    len++;
}
void List::add_node_xy(int x, int y) {
    point_t p;
    p.x = x;
    p.y = y;
    add_node_p(p);
    len++;
}
void List::print_list() {
    node_t *tmp = start;
    Serial.print("[ ");
    while (tmp) {
        Serial.print("(");
        Serial.print(tmp->p.x);
        Serial.print(", ");
        Serial.print(tmp->p.y);
        Serial.print(") ");
        tmp = tmp->next;
    }
    Serial.print("]\n");
}
void List::pop_front() {
    if (start == NULL)
        return;
    len--;
    if (start == end) {
        free(start);
        start = NULL;
        end = NULL;
        return;
    }

    node_t *free_me = start;
    start = start->next;
    free(free_me);
}
node_t *List::get_back() {
    return end;
}
node_t *List::get_front() {
    return start;
}
size_t List::get_len() {
    return len;
}
bool List::exists(point_t p) {
    node_t *tmp = this->start;
    while (tmp) {
        if (tmp->p.x == p.x && tmp->p.y == p.y)
            return -1;
        tmp = tmp->next;
    }

    return false;
}
List::~List() {
    while (start)
        pop_front();
}


Snake::Snake() {
    body = new List();
}
Snake::~Snake() {
    delete body;
}
int Snake::go_forward(point_t *apple) {
    point_t new_head = this->body->get_back()->p;

    switch (this->direction)
    {
    case RIGHT:
        new_head.y++;
        break;
    case UP:
        new_head.x--;
        break;
    case DOWN:
        new_head.x++;
        break;
    case LEFT:
        new_head.y--;
        break;
    }

    // collision with self
    if (body->exists(new_head))
        return -1;

    // collision with walls
    if (new_head.x < 0 || new_head.x >= MAX_X || 
        new_head.y < 0 || new_head.y >= MAX_Y)
        return -1;

    body->add_node_p(new_head);
    game_ptr->render_dot(new_head.x, new_head.y, 1);

    // collision with apple
    if (new_head.x == apple->x && new_head.y == apple->y)
        return 1;

    point_t p = body->get_front()->p;
    game_ptr->render_dot(p.x, p.y, 0);
    body->pop_front();

    return 0;
}
void Snake::set_direction(int dir) {
    if ((dir == UP && direction != DOWN) ||
        (dir == DOWN && direction != UP) ||
        (dir == RIGHT && direction != LEFT) ||
        (dir == LEFT && direction != RIGHT))
            direction = dir;
}


Game* Game::get_game(LEDMatrixMagic *m) {
    if (!game_ptr)
        game_ptr = new Game(m);
    return game_ptr;
}
Game::Game(LEDMatrixMagic *m) {
    srand(random(300));
    set_renderer(m);

    screen = malloc(sizeof(char*) * MAX_X);
    for (int i = 0; i < MAX_X; i++)
        screen[i] = malloc(sizeof(char) * MAX_Y);

    point_t snake_head = {MAX_X / 2, MAX_Y / 2};
    snake.body->add_node_p(snake_head);

    generate_apple();
}
Game::~Game() {
    for (int i = 0; i < max_x; i++)
        free(screen[i]);
    free(screen);
}
void Game::do_step() {
    int hit = snake.go_forward(&apple);
    if (hit == 1)
        generate_apple();
    if (hit == -1)
        resetGame();
}
void Game::generate_apple() {
    int ok = 0;
    while (!ok) {
        apple.x = rand() % MAX_X;
        apple.y = rand() % MAX_Y;
        ok = !snake.body->exists(apple);
    }
    render_dot(apple.x, apple.y, 1);
}
void Game::resetGame() {
    for (int d = 0; d < 4; d++)
        m->clear(d);

    game_over_print();

    Serial.print("Final score: ");
    Serial.println(snake.body->get_len());
    
    delete snake.body;
    snake.body = new List();
    point_t snake_head = {MAX_X / 2, MAX_Y / 2};
    snake.body->add_node_p(snake_head);

    generate_apple();

}
void Game::render_dot(int x, int y, int value) {
    int addr = 2;
    if (x >= 8 && y >= 8) {
        addr = 0;
        m->set_led(addr, x % 8, y % 8, value);
    } else if (x >= 8) {
        addr = 3;
        m->set_led(addr, 7 - x % 8, 7 - y % 8, value);
    } else if (y >= 8) {
        addr = 1;
        m->set_led(addr, x % 8, y % 8, value);
    } else {
        m->set_led(addr, 7 - x % 8, 7 - y % 8, value);
    }

}
void Game::render() {
    for (int d = 0; d < 4; d++)
        m->clear(d);

    int x, y, addr;

    // render apple
    x = apple.x;
    y = apple.y;
    render_dot(x, y, 1);

    // render snake
    node_t *tmp = snake.body->get_front();
    while (tmp) {
        // render dot
        x = tmp->p.x;
        y = tmp->p.y;

        render_dot(x, y, 1);
        tmp = tmp->next;
    }
}
void Game::set_renderer(LEDMatrixMagic *m) {
    this->m = m;
}
void Game::get_input(char c) {
    switch (c) {
        case 'w':
            snake.set_direction(UP);
            break;
        case 's':
            snake.set_direction(DOWN);
            break;
        case 'a':
            snake.set_direction(LEFT);
            break;
        case 'd':
            snake.set_direction(RIGHT);
            break;
    }
}
//  for debugging purposes
void Game::renderASCII() {
    snake.body->print_list();

    // clean the matrix
    for (int i = 0; i < MAX_X; i++)
        memset(screen[i], ' ', MAX_Y * sizeof(char));

    // add the body of the snake
    node_t *tmp = snake.body->get_front();
    while (tmp->next) {
        screen[tmp->p.x][tmp->p.y] = '@';
        tmp = tmp->next;
    }

    // add the head of the snake
    if (tmp)
        screen[tmp->p.x][tmp->p.y] = '%';

    // add the apple
    screen[apple.x][apple.y] = '$';

    // print
    Serial.print("##################\n");
    for (int i = 0; i < MAX_X; i++) {
        Serial.print("#");
        for (int j = 0; j < MAX_Y; j++)
            Serial.print(screen[i][j]);
        Serial.print("#\n");
    }
    Serial.print("##################\n");
}
