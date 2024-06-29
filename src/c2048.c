
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lib2048.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#include <conio.h>
#define GETCH() _getch()
#define CLEAR() system("cls")

#else

#include <termios.h>
#include <unistd.h>

/* reads from keypress, doesn't echo */
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= (unsigned) ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

#define GETCH() getch()
#define CLEAR() system("clear");

#endif

int main()
{
    int ch;
    game_t g;
    bool moved = true;
    gameId_t id;
    size_t numMoves = 0;

    init(&g);

    do
    {
        if (moved)
        {
            numMoves++;
            spawn(&g);
        }

        id = encode(&g);
        CLEAR();
        printf("\nStep: %zu Score: %zu (%s)\n\n", numMoves, g.score, id.str);
        print(&g);

        if (!canmove(&g))
        {
            printf("\nGame Over!");
            break;
        }

        ch = GETCH();

        switch (ch)
        {
        case 224:
            ch = GETCH();
            switch (ch)
            {
            case 75: // left 
                moved = move(&g, MV_LEFT);
                break;
            case 72: // up
                moved = move(&g, MV_UP);
                break;
            case 80: // down
                moved = move(&g, MV_DOWN);
                break;
            case 77: // right
                moved = move(&g, MV_RIGHT);
                break;
            }
            break;
        }

    } while (ch != 'x');

    return EXIT_SUCCESS;
}
