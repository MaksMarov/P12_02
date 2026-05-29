#include "game.h"

void draw_field(int field[HEIGHT][WIDTH], GameState* game) {
    clear();

    mvaddch(0, 0, '+');
    for (int x = 1; x <= WIDTH; x++) mvaddch(0, x, '-');
    mvaddch(0, WIDTH + 1, '+');

    for (int y = 0; y < HEIGHT; y++) {
        mvaddch(y + 1, 0, '|');

        for (int x = 0; x < WIDTH; x++) {
            mvaddch(y + 1, x + 1, field[y][x] ? 'o' : ' ');
        }

        mvaddch(y + 1, WIDTH + 1, '|');
    }

    mvaddch(HEIGHT + 1, 0, '+');
    for (int x = 1; x <= WIDTH; x++) mvaddch(HEIGHT + 1, x, '-');
    mvaddch(HEIGHT + 1, WIDTH + 1, '+');

    mvprintw(HEIGHT + 2, 0, "A/Z speed | P pause | SPACE quit | Delay: %d | %s", game->speed,
             game->paused ? "PAUSED" : "RUNNING");

    refresh();
}