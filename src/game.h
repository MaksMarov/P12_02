#ifndef GAME_H
#define GAME_H

#include <ncurses.h>

#define WIDTH 80
#define HEIGHT 25

typedef struct {
    int speed;
    int running;
    int paused;
} GameState;

void init_field(int field[HEIGHT][WIDTH]);

void draw_field(int field[HEIGHT][WIDTH], GameState *game);

void update_field(int field[HEIGHT][WIDTH]);

int count_neighbors(int field[HEIGHT][WIDTH], int y, int x);

void handle_input(GameState *game);

#endif