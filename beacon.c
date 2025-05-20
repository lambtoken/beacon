#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define MAX_HIST 3000

void draw_commands(char** commands, size_t n, int y) {
    for(int i = n; i > 0; i--) {
        if (i == y) {
            attron(COLOR_PAIR(1));
             attron(A_BOLD);
        } else {
            attron(COLOR_PAIR(0));
            attroff(A_BOLD);
        }
        mvprintw(n-i + 2, 0, "%s\n", commands[i]);
        attroff(COLOR_PAIR(0));
        attroff(COLOR_PAIR(1));
        refresh();
    }
}

void cleanup(char** commands, size_t n, FILE* file) {

    fclose(file);

    for(int i = 0; i < n; i++) {
        free(commands[i]);
    }
    free(commands);
}

int main(void) {

    // // init the screen
    initscr();
    noecho();   
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    curs_set(0);

    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    // load bash history
    // ~/.bash_history

    char* buffer;
    char* lines;

    char fullpath[1024];

    char* home = getenv("HOME");

    if (home) {
        snprintf(fullpath, sizeof(fullpath), "%s%s", getenv("HOME"), "/.bash_history");
    }

    FILE *file = fopen(fullpath, "r");

    if (!file) {
        printw("%s\n", home);
        endwin();
        perror("Failed to open .bash_history!");
        exit(1);
    }

    system("bash -c 'history -a'");

    char** commands;
    size_t ncoms = 0;
    size_t len = 0;

    fseek(file, 0, SEEK_END);
    size_t file_len = ftell(file);
    rewind(file);

    commands = (char**)malloc(sizeof(char*) * MAX_HIST);

    while (getline(&commands[ncoms], &len, file) != -1) {
        ncoms += 1;
    }

    // // ui stuff
    // printw("Seach:");

    refresh();

    // attron(A_BOLD);
    // printw("%c", ch);
    // attroff(A_BOLD);

    int y = ncoms - 1;
    int ch;

    draw_commands(commands, ncoms, y);

    while((ch = getch()) != 'q') {
        if (ch == KEY_UP || ch == 'k') y++;
        if (ch == KEY_DOWN || ch == 'j') y--;

        if (y < 0) y = 0;
        if (y > ncoms - 1) y = ncoms - 1;

        // 10 is enter
        if (ch == 10 || ch == KEY_ENTER) { 
            endwin();
            system(commands[y]);
            cleanup(commands, ncoms, file);
            return 0;
        }

        mvprintw(0, 0, "%ld commands loaded\n", ncoms);
        draw_commands(commands, ncoms, y);
    }

    cleanup(commands, ncoms, file);
    endwin();
}
