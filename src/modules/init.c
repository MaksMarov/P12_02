#include "game.h"
#include <stdio.h>

void init_field(int field[HEIGHT][WIDTH]) {
    char ch;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            ch = getchar();

            while (ch != '.' && ch != 'O' && ch != EOF) {
                ch = getchar();
            }

            field[y][x] = (ch == 'O');
        }
    }
}