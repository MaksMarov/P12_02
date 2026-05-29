#include <stdio.h>
#include <unistd.h>

#include "game.h"

int main(void) {
    int state[HEIGHT][WIDTH];
    GameState game = {100, 1, 0};

    init_state(state);

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
        draw(state, &game);
        input(&game);

        if (!game.paused) update(state);

        usleep(game.speed * 1000);
    }

    endwin();
    return 0;
}