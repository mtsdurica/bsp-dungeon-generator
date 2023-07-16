/**
 * @file demo.c
 * @author Matúš Ďurica (mtsdurica@gmail.com)
 * @brief Contains implementation of a short demo to showcase the BSP dungeon generator
 * @date 2023-07-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../include/dungeon_generator_bsp.h"
#include <ncurses.h>

int main()
{
    initscr();
    curs_set(false);
    noecho();
    cbreak();
    int y = 1, x = 1;
    // Generating the dungeon
    map playable = generate_dungeon();
    // Dumping generated dungeon to screen
    dump_dungeon(&playable);
    // Printing character
    mvprintw(y, x, "%c", '@');
    refresh();
    // Generic player movement loop
    while (true)
    {
        switch (getch())
        {
        case 'w':
            if (y > 0 && mvinch(y - 1, x) == WALKABLE_TILE)
            {
                mvprintw(y, x, "%c", WALKABLE_TILE);
                refresh();
                y--;
            }
            break;
        case 's':
            if (y < MAX_Y && mvinch(y + 1, x) == WALKABLE_TILE)
            {
                mvprintw(y, x, "%c", WALKABLE_TILE);
                refresh();
                y++;
            }
            break;
        case 'a':
            if (x > 0 && mvinch(y, x - 1) == WALKABLE_TILE)
            {
                mvprintw(y, x, "%c", WALKABLE_TILE);
                refresh();
                x--;
            }
            break;
        case 'd':
            if (x < MAX_X && mvinch(y, x + 1) == WALKABLE_TILE)
            {
                mvprintw(y, x, "%c", WALKABLE_TILE);
                refresh();
                x++;
            }
            break;
        case 'x':
            endwin();
            return 0;
            break;
        default:
            break;
        }
        mvprintw(y, x, "%c", '@');
        refresh();
    }
}