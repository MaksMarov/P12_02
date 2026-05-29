#include <stdio.h>

#include "game.h"

void init_state(int state[HEIGHT][WIDTH]) {
    char ch;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            ch = getchar();

            while (ch != '.' && ch != 'O' && ch != EOF) {
                ch = getchar();
            }

            state[y][x] = (ch == 'O');
        }
    }
}