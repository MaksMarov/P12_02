#ifndef GAME_H
#define GAME_H

#include <ncurses.h>

#define WIDTH 80
#define HEIGHT 25

typedef struct {
    int speed;
    int running;
    int paused;
    int tick;
} GameState;

void init_state(int field[HEIGHT][WIDTH]);

void draw(int field[HEIGHT][WIDTH], GameState *game);

void update(int field[HEIGHT][WIDTH]);

int count_neighbors(int field[HEIGHT][WIDTH], int y, int x);

void input(GameState *game);

#endif