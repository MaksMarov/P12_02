#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25

typedef struct {
    int delay;
    int running;
    int paused;
    int tick;
} GameState;

void init_state(int field[HEIGHT][WIDTH]);
void draw(int field[HEIGHT][WIDTH], GameState* game);
void update(int field[HEIGHT][WIDTH]);
int count_neighbors(int field[HEIGHT][WIDTH], int y, int x);
void input(GameState* game);

void draw_frame_top(void);
void draw_field(int state[HEIGHT][WIDTH]);
void draw_status(GameState* game);
void draw_controls(GameState* game);

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
        usleep(game.delay * 1000);
    }

    endwin();
    return 0;
}

void init_state(int state[HEIGHT][WIDTH]) {
    char ch;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            ch = getchar();

            while (ch != '.' && ch != 'O' && ch != EOF) ch = getchar();

            state[y][x] = (ch == 'O');
        }
    }
}

void input(GameState* game) {
    int ch = getch();

    if (ch == ' ')
        game->running = 0;
    else if (ch == 'p' || ch == 'P')
        game->paused = !game->paused;
    else if (ch == 'a' || ch == 'A') {
        if (game->delay > 20) game->delay -= 10;
    } else if (ch == 'z' || ch == 'Z') {
        if (game->delay < 500) game->delay += 10;
    }
}

int count_neighbors(int state[HEIGHT][WIDTH], int y, int x) {
    int count = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dy == 0 && dx == 0) continue;

            int ny = (y + dy + HEIGHT) % HEIGHT;
            int nx = (x + dx + WIDTH) % WIDTH;

            count += state[ny][nx];
        }
    }

    return count;
}

void update(int state[HEIGHT][WIDTH]) {
    int next[HEIGHT][WIDTH];

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int n = count_neighbors(state, y, x);

            if (state[y][x])
                next[y][x] = (n == 2 || n == 3);
            else
                next[y][x] = (n == 3);
        }
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            state[y][x] = next[y][x];
        }
    }
}

void draw(int state[HEIGHT][WIDTH], GameState* game) {
    clear();

    draw_frame_top();
    draw_field(state);
    draw_status(game);
    draw_controls(game);

    refresh();
}

void draw_frame_top(void) {
    mvprintw(3, 4,
             "+--------------------------------------------------------------------------------------+");

    mvprintw(4, 4,
             "| SYSTEM STATUS: ONLINE                                               SIGNAL: ACTIVE * |");

    mvprintw(5, 4,
             "+--------------------------------------------------------------------------------------+");

    mvprintw(6, 4,
             "| :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: |");
}

void draw_field(int state[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        mvprintw(7 + y, 4, "| ::");

        for (int x = 0; x < WIDTH; x++) {
            if (state[y][x]) {
                attron(COLOR_PAIR(1));
                mvaddch(7 + y, 8 + x, 'O');
                attroff(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2));
                mvaddch(7 + y, 8 + x, '.');
                attroff(COLOR_PAIR(2));
            }
        }

        mvprintw(7 + y, 8 + WIDTH, ":: |");
    }

    mvprintw(7 + HEIGHT, 4,
             "| :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: |");

    mvprintw(7 + HEIGHT + 2, 4,
             "|--------------------------------------------------------------------------------------|");
}

void draw_status(GameState* game) {
    attron(COLOR_PAIR(3));

    mvprintw(7 + HEIGHT + 3, 6, "STATUS");
    mvprintw(7 + HEIGHT + 3, 26, "DELAY");
    mvprintw(7 + HEIGHT + 3, 43, "POWER");
    mvprintw(7 + HEIGHT + 3, 58, "HDD");
    mvprintw(7 + HEIGHT + 3, 70, "SIGNAL");

    attroff(COLOR_PAIR(3));

    if (game->paused) {
        attron(COLOR_PAIR(4));
        mvprintw(7 + HEIGHT + 3, 14, "[ PAUSED ]");
        attroff(COLOR_PAIR(4));
    } else {
        attron(COLOR_PAIR(1));
        mvprintw(7 + HEIGHT + 3, 14, "[ RUNNING ]");
        attroff(COLOR_PAIR(1));
    }

    attron(COLOR_PAIR(1));
    mvprintw(7 + HEIGHT + 3, 33, "%3d ms", game->delay);
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(1));
    mvprintw(7 + HEIGHT + 3, 50, (game->tick / 15) % 2 ? "[o]" : "[ ]");
    attroff(COLOR_PAIR(1));

    if (!game->paused && (game->tick % 6 < 3)) {
        attron(COLOR_PAIR(1));
        mvprintw(7 + HEIGHT + 3, 62, "--");
        attroff(COLOR_PAIR(1));
    } else {
        mvprintw(7 + HEIGHT + 3, 62, "---");
    }

    if ((game->tick / 55) % 2) {
        attron(COLOR_PAIR(1));
        mvprintw(7 + HEIGHT + 3, 77, "ONLINE");
        attroff(COLOR_PAIR(1));
    } else {
        attron(COLOR_PAIR(3));
        mvprintw(7 + HEIGHT + 3, 77, "STABLE");
        attroff(COLOR_PAIR(3));
    }
}

void draw_controls(GameState* game) {
    (void)game;

    mvprintw(7 + HEIGHT + 4, 4,
             "|--------------------------------------------------------------------------------------|");

    attron(COLOR_PAIR(3));

    mvprintw(7 + HEIGHT + 5, 7, "[A] SPEED+");
    mvprintw(7 + HEIGHT + 5, 24, "[Z] SPEED-");
    mvprintw(7 + HEIGHT + 5, 41, "[P] PAUSE");
    mvprintw(7 + HEIGHT + 5, 57, "[SPACE] EXIT");

    attroff(COLOR_PAIR(3));

    const char* anim[] = {"<    >", "<<  >>", "<<<>>>", "<<  >>"};

    attron(COLOR_PAIR(1));
    mvprintw(7 + HEIGHT + 5, 76, "%s", anim[(game->tick / 4) % 4]);
    attroff(COLOR_PAIR(1));

    mvprintw(7 + HEIGHT + 6, 4,
             "+--------------------------------------------------------------------------------------+");
}