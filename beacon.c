#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define MAX_HIST 200

int main(void) {

    // init the screen
    initscr();
    noecho();   
    raw();
    char ch;

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

    char** commands;
    int ncoms = 0;
    size_t len = 0;

    // while (ncoms <= MAX_HIST) {
    //     getline(&commands[ncoms], &len, file);
    //
    //     printw("%s\n", commands[ncoms]);
    //     ncoms += 1;
    // }

    // ui stuff
    printw("Seach:");
    ch = getch();

    refresh();

    attron(A_BOLD);
    printw("%c", ch);
    attroff(A_BOLD);

    refresh();

    // cleanup
    for(int i = 0; i < ncoms; i++) {
        free(commands[i]);
    }
    fclose(file);
    endwin();
}
