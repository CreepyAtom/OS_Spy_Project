#include <stdlib.h>
#include "utils.h"

int taxicab_geometry(int x0, int y0, int x1, int y1) {
    return abs(x0-x1) + abs(y0-y1);
}

int max(int x, int y) {
    if (x > y) {
        return x;
    }
    return y;
}

int distance_for_character(int x0, int y0, int x1, int y1) {
    return max(abs(x0-x1), abs(y0-y1));
}

void tab_neighbor(int x0, int y0, int x1, int y1, int **tab) {
    // tab[10][3];
    int i;
    if (((x0 == 0) || (x0 == 6)) && ((y0 = 0) || (y0 == 6))) {
        tab[0][2] = 4; // CORNER
    } else if (((x0 == 0) || (x0 == 6)) || ((y0 = 0) || (y0 == 6))) {
        tab[0][2] = 6; // EDGE
    } else {
        tab[0][2] = 9; // MIDDLE
    }
    tab[1][0] = tab[2][0] = tab[3][0] = x0-1;
    tab[4][0] = tab[5][0] = tab[6][0] = x0;
    tab[7][0] = tab[8][0] = tab[9][0] = x0+1;
    tab[1][1] = tab[4][1] = tab[7][1] = y0-1;
    tab[2][1] = tab[5][1] = tab[8][1] = y0;
    tab[3][1] = tab[6][1] = tab[9][1] = y0+1;
    for (i=1; i<10; i++) {
        tab[i][2] = distance_for_character(tab[i][0], tab[i][1], x1, y1);
    }
    if (x0 == 0) {
        tab[1][2] = tab[4][2] = tab[7][2] = INF;
    } else if (x0 == 6) {
        tab[3][2] = tab[6][2] = tab[9][2] = INF;
    }
    if (y0 == 0) {
        tab[1][2] = tab[2][2] = tab[3][2] = INF;
    } else if (y0 == 6) {
        tab[7][2] = tab[8][2] = tab[9][2] = INF;
    }
}

void tab_advance(int ** tab, int ** res) {
    // res[10][2];
    int index = 0;
    int i;
    int value = tab[5][2]; // Distance to the goal from the initial position
    for (i=1; i<10; i++) {
        if (tab[i][2] < value) {
            index++;
            res[index+1][0] = tab[i][0];
            res[index+1][1] = tab[i][1];
        }
    }
    res[0][0] = index; // Number of cases that get closer the character to its destination
}

void get_next_pos(int x0, int y0, int x1, int y1, int *next_pos) {
    // next_pos[2];
    int tab[10][3];
    tab_neighbor(x0, y0, x1, y1, (int **) tab);
    int res[10][2];
    tab_advance((int **) tab, (int **) res);
    int rnd = 1 + (int) rand()%tab[0][0];
    next_pos[0] = tab[rnd][0];
    next_pos[1] = tab[rnd][1];
}