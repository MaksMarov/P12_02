#include "game.h"

void input(GameState* game) {
    int ch = getch();

    if (ch == ' ')
        game->running = 0;

    else if (ch == 'p' || ch == 'P')
        game->paused = !game->paused;

    else if (ch == 'a' || ch == 'A') {
        if (game->speed > 20) game->speed -= 10;
    }

    else if (ch == 'z' || ch == 'Z') {
        if (game->speed < 500) game->speed += 10;
    }
}