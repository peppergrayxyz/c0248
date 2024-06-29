#pragma once

#include <stddef.h>

#define FIELD_NUM_ROWS 4
#define FIELD_NUM_COLS 4
#define RAND_SIZE ((sizeof(size_t) / sizeof (int)) + 1)
#define MVSET(R, C, R_CURR, R_LAST, C_CURR, C_LAST) \
 r = (R); c = (C); r_curr = (R_CURR); r_last = (R_LAST); c_curr = (C_CURR); c_last = (C_LAST);

typedef unsigned char field_t;

#define GAME_ID_STR_LEN (FIELD_NUM_ROWS*FIELD_NUM_COLS*sizeof(field_t))

typedef struct gameId_st
{
    unsigned char str[GAME_ID_STR_LEN + 1];
} gameId_t;

typedef struct game_st {
    size_t score;
    field_t field[FIELD_NUM_ROWS][FIELD_NUM_COLS];
    size_t numSpotsTaken;
} game_t;


typedef enum move_direction_en {
    MV_LEFT,
    MV_RIGHT,
    MV_UP,
    MV_DOWN
} move_direction_t;

void init(game_t* g);
size_t spawn(game_t* g);
void print(game_t* g);
bool move(game_t* g, move_direction_t dir);
bool canmove(game_t* g);
gameId_t encode(game_t* g);
void decode(game_t* g, gameId_t id);
