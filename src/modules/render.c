#include <ncurses.h>

#include "game.h"

#define CRT_X 4
#define CRT_Y 3

#define SCREEN_X 8
#define SCREEN_Y 7

static void draw_frame_top(void);
static void draw_field(int state[HEIGHT][WIDTH]);
static void draw_status(GameState* game);
static void draw_controls(GameState* game);

void draw(int state[HEIGHT][WIDTH], GameState* game) {
    clear();

    draw_frame_top();
    draw_field(state);
    draw_status(game);
    draw_controls(game);

    refresh();
}

static void draw_frame_top(void) {
    mvprintw(CRT_Y + 0, CRT_X,
             "+--------------------------------------------------------------------------------------+");

    mvprintw(CRT_Y + 1, CRT_X,
             "| SYSTEM STATUS: ONLINE                                               SIGNAL: ACTIVE * |");

    mvprintw(CRT_Y + 2, CRT_X,
             "+--------------------------------------------------------------------------------------+");

    mvprintw(CRT_Y + 3, CRT_X,
             "| :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: |");
}

static void draw_field(int state[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        mvprintw(SCREEN_Y + y, CRT_X, "| ::");

        for (int x = 0; x < WIDTH; x++) {
            if (state[y][x]) {
                attron(COLOR_PAIR(1));
                mvaddch(SCREEN_Y + y, SCREEN_X + x, 'O');
                attroff(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2));
                mvaddch(SCREEN_Y + y, SCREEN_X + x, '.');
                attroff(COLOR_PAIR(2));
            }
        }

        mvprintw(SCREEN_Y + y, SCREEN_X + WIDTH, ":: |");
    }

    mvprintw(SCREEN_Y + HEIGHT, CRT_X,
             "| :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: |");

    mvprintw(SCREEN_Y + HEIGHT + 2, CRT_X,
             "|--------------------------------------------------------------------------------------|");
}

static void draw_status(GameState* game) {
    attron(COLOR_PAIR(3));

    mvprintw(SCREEN_Y + HEIGHT + 3, CRT_X + 2, "STATUS");
    mvprintw(SCREEN_Y + HEIGHT + 3, CRT_X + 22, "DELAY");
    mvprintw(SCREEN_Y + HEIGHT + 3, CRT_X + 39, "POWER");
    mvprintw(SCREEN_Y + HEIGHT + 3, CRT_X + 54, "HDD");
    mvprintw(SCREEN_Y + HEIGHT + 3, CRT_X + 66, "SIGNAL");

    attroff(COLOR_PAIR(3));

    if (game->paused) {
        attron(COLOR_PAIR(4));
        mvprintw(SCREEN_Y + HEIGHT + 3, CRT_X + 10, "[ PAUSED ]");
        attroff(COLOR_PAIR(4));
    } else {
        attron(COLOR_PAIR(1));
        mvprintw(SCREEN_Y + HEIGHT + 3, CRT_X + 10, "[ RUNNING ]");
        attroff(COLOR_PAIR(1));
    }

    attron(COLOR_PAIR(1));
    mvprintw(SCREEN_Y + HEIGHT + 3, CRT_X + 30, "%3d ms", game->speed);
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(1));
    mvprintw(SCREEN_Y + HEIGHT + 3, CRT_X + 47, (game->tick / 15) % 2 ? "[o]" : "[ ]");
    attroff(COLOR_PAIR(1));

    if (!game->paused && (game->tick % 6 < 3)) {
        attron(COLOR_PAIR(1));
        mvprintw(SCREEN_Y + HEIGHT + 3, CRT_X + 59, "--");
        attroff(COLOR_PAIR(1));
    } else {
        mvprintw(SCREEN_Y + HEIGHT + 3, CRT_X + 59, "---");
    }

    if ((game->tick / 55) % 2) {
        attron(COLOR_PAIR(1));
        mvprintw(SCREEN_Y + HEIGHT + 3, CRT_X + 75, "ONLINE");
        attroff(COLOR_PAIR(1));
    } else {
        attron(COLOR_PAIR(3));
        mvprintw(SCREEN_Y + HEIGHT + 3, CRT_X + 75, "STABLE");
        attroff(COLOR_PAIR(3));
    }
}

static void draw_controls(GameState* game) {
    (void)game;

    mvprintw(SCREEN_Y + HEIGHT + 4, CRT_X,
             "|--------------------------------------------------------------------------------------|");

    attron(COLOR_PAIR(3));

    mvprintw(SCREEN_Y + HEIGHT + 5, CRT_X + 3, "[A] SPEED+");
    mvprintw(SCREEN_Y + HEIGHT + 5, CRT_X + 20, " [Z] SPEED-");
    mvprintw(SCREEN_Y + HEIGHT + 5, CRT_X + 37, " [P] PAUSE");
    mvprintw(SCREEN_Y + HEIGHT + 5, CRT_X + 53, " [SPACE] EXIT");

    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(1));

    const char* anim[] = {"<    >", "<<  >>", "<<<>>>", "<<  >>"};

    mvprintw(SCREEN_Y + HEIGHT + 5, CRT_X + 76, "%s", anim[(game->tick / 4) % 4]);

    attroff(COLOR_PAIR(1));

    mvprintw(SCREEN_Y + HEIGHT + 6, CRT_X,
             "+--------------------------------------------------------------------------------------+");
}