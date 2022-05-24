#include <stdio.h>
#include <stdlib.h>



// void get_input(snake_t *s) {
//     int ok = 0;
//     while (!ok) {
//         int ch = getchar();
//         ok = 1;
//         switch (ch)
//         {
//         case 'a':
//             if (s->direction != RIGHT)
//                 s->direction = LEFT;
//             break;
//         case 'd':
//             if (s->direction != LEFT)
//                 s->direction = RIGHT;
//             break;
//         case 's':
//             if (s->direction != UP)
//                 s->direction = DOWN;
//             break;
//         case 'w':
//             if (s->direction != DOWN)
//                 s->direction = UP;
//             break;
//         default:
//             ok = 0;
//             break;
//         }
//     }
// }

// int main() {
//     snake_t *snake = calloc(1, sizeof(snake_t));
//     snake->body = calloc(1, sizeof(list_t));

//     add_node_xy(snake->body, 1, 1);
//     add_node_xy(snake->body, 1, 2);
//     add_node_xy(snake->body, 2, 2);

//     print_list(snake->body);

//     point_t apple = {3, 3};

//     srand(69);

//     while (1) {
//         renderASCII(snake, &apple);
//         get_input(snake);
//         int hit = go_forward(snake, &apple);
//         if (hit == 1) {
//             int ok = 0;
//             while (!ok) {
//                 apple.x = rand() % MAX_X;
//                 apple.y = rand() % MAX_Y;
//                 ok = 1;

//                 node_t *tmp = snake->body->start;
//                 while (tmp) {
//                     if (tmp->p.x == apple.x && tmp->p.y == apple.y) {
//                         ok = 0;
//                         break;
//                     }
//                     tmp = tmp->next;
//                 }
//             }
//         }
//     }

    

//     while (snake->body->start)
//         pop_front(snake->body);
//     free(snake->body);
//     free(snake);
// }
