#ifndef UTILS_H
#define UTILS_H

#define INF 100

int taxicab_geometry(int, int, int, int);

int max(int, int);

int distance_for_character(int, int, int, int);

void tab_neighbor(int, int, int, int, int**);

void tab_advance(int **, int **);

void get_next_pos(int, int, int, int, int*);

#endif