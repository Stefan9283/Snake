#include <stdio.h>
#include <stdlib.h>

#define MAX_X 16
#define MAX_Y 16

typedef struct point {
    int x, y;
} point_t;

// list funcs 
typedef struct node {
    point_t p;
    struct node* next;
} node_t;

typedef struct list {
    node_t *start;
    node_t *end;
} list_t;

void add_node_p(list_t *l, point_t p) {
    node_t *n = calloc(1, sizeof(node_t));
    n->p = p;

    if (l->start == NULL) {
        l->start = n;
        l->end = n;
        return;
    }

    l->end->next = n;
    l->end = n;
}

void add_node_xy(list_t *l, int x, int y) {
    point_t p;
    p.x = x;
    p.y = y;
    add_node_p(l, p);
}

void print_list(list_t *l) {
    node_t *tmp = l->start;
    printf("[ ");
    while (tmp) {
        printf("(%d %d) ", tmp->p.x, tmp->p.y);
        tmp = tmp->next;
    }
    printf("]\n");
}

void pop_front(list_t *l) {
    if (l->start == NULL) return;
    if (l->start == l->end) {
        free(l->start);
        l->start = NULL;
        l->end = NULL;
        return;
    }

    node_t *free_me = l->start;
    l->start = l->start->next;
    free(free_me);
}

// list funcs end

#define RIGHT 0
#define UP    1
#define DOWN  2
#define LEFT  3 

typedef struct snake {
    list_t *body;
    int direction;
} snake_t;

// returns: 
// -1 if it would hit itself
// -1 if it would hit a wall
// 1 if it would hit an apple
// 0 otherwise

int go_forward(snake_t *s, point_t *apple) {
    point_t new_head = s->body->end->p;
    switch (s->direction)
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
    add_node_p(s->body, new_head);

    if (new_head.x == apple->x && new_head.y == apple->y)
        return 1;
    pop_front(s->body);

    return 69; // TODO
}


void render(snake_t *s, point_t *apple) {
    char screen[16][16];
    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 16; j++)
            screen[i][j] = ' ';

    node_t *tmp = s->body->start;
    while (tmp) {
        screen[tmp->p.x][tmp->p.y] = '@';
        if (!tmp->next)
            screen[tmp->p.x][tmp->p.y] = '%';
        tmp = tmp->next;
    }

    screen[apple->x][apple->y] = '$';

    system("clear");

    printf("##################\n");
    for (int i = 0; i < 16; i++) {
        printf("#");
        for (int j = 0; j < 16; j++)
            printf("%c", screen[i][j]);
        printf("#\n");
    }
    printf("##################\n");
}

void get_input(snake_t *s) {
    int ok = 0;
    while (!ok) {
        int ch = getchar();
        ok = 1;
        switch (ch)
        {
        case 'a':
            if (s->direction != RIGHT)
                s->direction = LEFT;
            break;
        case 'd':
            if (s->direction != LEFT)
                s->direction = RIGHT;
            break;
        case 's':
            if (s->direction != UP)
                s->direction = DOWN;
            break;
        case 'w':
            if (s->direction != DOWN)
                s->direction = UP;
            break;
        default:
            ok = 0;
            break;
        }
    }
}

int main() {
    snake_t *snake = calloc(1, sizeof(snake_t));
    snake->body = calloc(1, sizeof(list_t));

    add_node_xy(snake->body, 1, 1);
    add_node_xy(snake->body, 1, 2);
    add_node_xy(snake->body, 2, 2);

    print_list(snake->body);

    point_t apple_position = {3, 3};

    srand(69);

    while (1) {
        render(snake, &apple_position);
        get_input(snake);
        int hit = go_forward(snake, &apple_position);
        if (hit == 1) {
            int ok = 0;
            while (!ok) {
                apple_position.x = rand() % MAX_X;
                apple_position.y = rand() % MAX_Y;
                ok = 1;

                node_t *tmp = snake->body->start;
                while (tmp) {
                    if (tmp->p.x == apple_position.x && tmp->p.y == apple_position.y) {
                        ok = 0;
                        break;
                    }
                    tmp = tmp->next;
                }
            }
        }
    }

    

    while (snake->body->start)
        pop_front(snake->body);
    free(snake->body);
    free(snake);
}
