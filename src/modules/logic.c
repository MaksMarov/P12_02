#include "game.h"

int count_neighbors(int field[HEIGHT][WIDTH], int y, int x) {
    int count = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dy == 0 && dx == 0) continue;

            int ny = (y + dy + HEIGHT) % HEIGHT;
            int nx = (x + dx + WIDTH) % WIDTH;

            count += field[ny][nx];
        }
    }

    return count;
}

void update_field(int field[HEIGHT][WIDTH]) {
    int next[HEIGHT][WIDTH];

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int n = count_neighbors(field, y, x);

            if (field[y][x])
                next[y][x] = (n == 2 || n == 3);
            else
                next[y][x] = (n == 3);
        }
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            field[y][x] = next[y][x];
        }
    }
}