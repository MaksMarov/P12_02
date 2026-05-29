#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define ALIVE 'o'
#define DEAD ' '

typedef struct {
    int speed;
    int running;
    int paused;
} GameState;

void init_field(int field[HEIGHT][WIDTH]);
void draw_field(int field[HEIGHT][WIDTH], GameState* game);
void update_field(int field[HEIGHT][WIDTH]);
int count_neighbors(int field[HEIGHT][WIDTH], int y, int x);
void handle_input(GameState* game);

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

        if (!game.paused) {
            update_field(field);
        }

        usleep(game.speed * 1000);
    }

    endwin();
    return 0;
}

void init_field(int field[HEIGHT][WIDTH]) {
    char ch;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            ch = getchar();

            while (ch != '.' && ch != 'O' && ch != EOF) {
                ch = getchar();
            }

            field[y][x] = (ch == 'O');
        }
    }
}

void draw_field(int field[HEIGHT][WIDTH], GameState *game) {
    clear();

    mvaddch(0, 0, '+');
    for (int x = 1; x <= WIDTH; x++)
        mvaddch(0, x, '-');
    mvaddch(0, WIDTH + 1, '+');

    for (int y = 0; y < HEIGHT; y++) {
        mvaddch(y + 1, 0, '|');

        for (int x = 0; x < WIDTH; x++) {
            mvaddch(y + 1, x + 1, field[y][x] ? 'o' : ' ');
        }

        mvaddch(y + 1, WIDTH + 1, '|');
    }

    mvaddch(HEIGHT + 1, 0, '+');
    for (int x = 1; x <= WIDTH; x++)
        mvaddch(HEIGHT + 1, x, '-');
    mvaddch(HEIGHT + 1, WIDTH + 1, '+');

    mvprintw(HEIGHT + 2, 0,
        "A/Z speed | P pause | SPACE quit | Delay: %d | %s",
        game->speed,
        game->paused ? "PAUSED" : "RUNNING"
    );

    refresh();
}

void update_field(int field[HEIGHT][WIDTH]) {
    int next[HEIGHT][WIDTH];

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int neighbors = count_neighbors(field, y, x);

            if (field[y][x]) {
                next[y][x] = (neighbors == 2 || neighbors == 3);
            } else {
                next[y][x] = (neighbors == 3);
            }
        }
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            field[y][x] = next[y][x];
        }
    }
}

int count_neighbors(int field[HEIGHT][WIDTH], int y, int x) {
    int count = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int ny = (y + dy + HEIGHT) % HEIGHT;
            int nx = (x + dx + WIDTH) % WIDTH;

            if (!(dy == 0 && dx == 0)) {
                count += field[ny][nx];
            }
        }
    }

    return count;
}

void handle_input(GameState* game) {
    int ch = getch();

    if (ch == ' ') {
        game->running = 0;
    } else if (ch == 'p' || ch == 'P') {
        game->paused = !game->paused;
    } else if (ch == 'a' || ch == 'A') {
        if (game->speed > 20) {
            game->speed -= 10;
        }
    } else if (ch == 'z' || ch == 'Z') {
        if (game->speed < 500) {
            game->speed += 10;
        }
    }
}