
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "lib2048.h"

void init(game_t *g)
{
    memset(g, 0, sizeof(game_t));
}

size_t sizet_rand()
{
    union {
        int nums[RAND_SIZE];
        size_t num;
    } data;
    
    for (size_t i = 0; i < RAND_SIZE; i++)
    {
        data.nums[i] = rand();
    }

    return data.num;
}

size_t spawn(game_t *g)
{
    size_t numSpotsLeft = FIELD_NUM_ROWS*FIELD_NUM_COLS - g->numSpotsTaken;

    if (numSpotsLeft > 0)
    {
        srand((unsigned int) time(NULL));

        // 10% chance for 4, else 2
        field_t value = (rand() % 10 == 0) ? 2 : 1;

        size_t spot = sizet_rand() % numSpotsLeft;
        size_t s = 0;

        for (size_t i = 0; i < FIELD_NUM_ROWS; i++)
        {
            for (size_t j = 0; j < FIELD_NUM_COLS; j++)
            {
                if (g->field[i][j] == 0)
                {
                    if (s == spot)
                    {
                        g->field[i][j] = value;
                        g->numSpotsTaken += 1;
                        return (((i * FIELD_NUM_ROWS) + j) * value) + 1;
                    }

                    s += 1;
                }
            }
        }
    }

    return 0;
}

void print(game_t *g)
{
    for (size_t i = 0; i < FIELD_NUM_ROWS; i++)
    {
        for (size_t j = 0; j < FIELD_NUM_COLS; j++)
        {
            if (g->field[i][j] > 0)
            {
                printf("%6d ", (1 << g->field[i][j]) >> 1);
            }
            else
            {
                printf("     . ");
            }
        }
        printf("\n");
    }
}

bool move(game_t *g, move_direction_t dir)
{
    game_t t;
    init(&t);
    t.score = g->score;
    size_t numSpotsTaken = 0;
    bool moved = 0;

    for (size_t i = 0; i < FIELD_NUM_ROWS; i++)
    {
        size_t x_curr = 0;
        size_t x_last = 0;

        for (size_t j = 0; j < FIELD_NUM_COLS; j++)
        {
            size_t r, c, r_curr, r_last, c_curr, c_last;

            r = i;
            c = j;
            r_curr = i;
            r_last = i;
            c_curr = x_curr;
            c_last = x_last;

            switch (dir)
            {
            case MV_LEFT:   MVSET(i,                      j,                      i,                           i,                           x_curr,                      x_last                     ); break;
            case MV_RIGHT:	MVSET(i,                      FIELD_NUM_COLS - j - 1, i,                           i,                           FIELD_NUM_COLS - x_curr - 1, FIELD_NUM_COLS - x_last - 1); break;
            case MV_UP:     MVSET(j,                      i,                      x_curr,                      x_last,                      i,                           i                          ); break;
            case MV_DOWN:   MVSET(FIELD_NUM_ROWS - j - 1, i,                      FIELD_NUM_ROWS - x_curr - 1, FIELD_NUM_ROWS - x_last - 1, i,                           i                          ); break;
            default:
                abort();
            }

            if (g->field[r][c] > 0)
            {
                if(t.field[r_last][c_last] == g->field[r][c])
                {
                    field_t value = t.field[r_last][c_last] += 1;
                    t.score += ((size_t) 1 << value);
                    x_last = x_curr;
                    moved = 1;
                }
                else
                {
                    if (r_curr != r || c_curr != c) moved = 1;

                    t.field[r_curr][c_curr] = g->field[r][c];
                    numSpotsTaken += 1;
                    x_last  = x_curr;
                    x_curr += 1;
                }

            }
        }
    }

    t.numSpotsTaken = numSpotsTaken;

    memcpy(g, &t, sizeof(game_t));

    return moved;
}

bool canmove(game_t* g)
{
    for (size_t i = 0; i < FIELD_NUM_ROWS; i++)
    {
        for (size_t j = 0; j < FIELD_NUM_COLS; j++)
        {
            if (g->field[i][j] == 0) return true;
            if (j > 0 && (g->field[i][j] == g->field[i][j-1])) return true;
            if (i > 0 && (g->field[i][j] == g->field[i-1][j])) return true;
        }
    }

    return false;
}

gameId_t encode(game_t* g)
{
    size_t pos = 0;
    gameId_t id = { 0 };

    for (size_t i = 0; i < FIELD_NUM_ROWS; i++)
    {
        for (size_t j = 0; j < FIELD_NUM_COLS; j++)
        {
            field_t value = g->field[i][j];
            
            id.str[pos++] = value < 10 ? '0' + value : 'a' + value;
        }
    }

    return id;
}

void decode(game_t* g, gameId_t id)
{
    for (size_t m = 0; m < GAME_ID_STR_LEN; m++)
    {
        size_t i = m / FIELD_NUM_ROWS;
        size_t j = m % FIELD_NUM_COLS;

        unsigned char c = id.str[m];

        g->field[i][j] = c >= '0' && c <= '9' ? c - '0' : c - 'a';
    }
}
