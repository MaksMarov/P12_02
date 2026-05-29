#include <stdio.h>
#include <unistd.h>

#include "game.h"

int main(void) {
    int state[HEIGHT][WIDTH];
    GameState game = {100, 1, 0, 0};

    init_state(state);

    initscr();
    start_color();
    use_default_colors();
    init_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_BLACK, -1);
    init_pair(3, COLOR_CYAN, -1);
    init_pair(4, COLOR_YELLOW, -1);
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

        game.tick++;

        usleep(game.speed * 1000);
    }

    endwin();
    return 0;
}