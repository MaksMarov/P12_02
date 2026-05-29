#include <stdio.h>
#include <unistd.h>

#include "game.h"

int main(void) {
    int field[HEIGHT][WIDTH];
    GameState game = {100, 1, 0};

    init_field(field);

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    if (freopen("/dev/tty", "r", stdin) == NULL) {
        endwin();
        return 1;
    }

    while (game.running) {
        draw_field(field, &game);
        handle_input(&game);

        if (!game.paused) update_field(field);

        usleep(game.speed * 1000);
    }

    endwin();
    return 0;
}