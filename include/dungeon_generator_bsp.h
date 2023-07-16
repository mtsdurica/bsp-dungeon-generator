/**
 * @file dungeon_generator_bsp.h
 * @author Matúš Ďurica (mtsdurica@gmail.com)
 * @brief Contains the data structs and public functions of the library
 * @date 2023-07-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <bits/types.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_X 50                // Max base room width
#define MAX_Y 25                // Max base room height
#define UINT_CHANGEABLE uint8_t // Changeable data type if needed to generate larger maps than 0-255
#define PSEUDO_ROOM_SIZE 4      // Pseudo max room size
#define CORNER_TILE '#'         // Corner tile character
#define WALL_TILE '#'           // Wall tile character
#define WALKABLE_TILE ' '       // Walkable tile character

typedef enum tiles
{
    Walkable,
    Corner,
    Wall
} tiles;

typedef struct point
{
    UINT_CHANGEABLE x;
    UINT_CHANGEABLE y;
} point;

typedef struct wall
{
    point start; // Start point of the wall
    point end;   // End point of the wall
} wall;

typedef struct room
{
    wall top;            // Top wall
    wall bot;            // Bottom wall
    wall left;           // Left wall
    wall right;          // Right wall
    struct room *child1; // Left/top child
    struct room *child2; // Right/Bottom child
    point center;        // Center point of the room
} room;

typedef struct map
{
    tiles tiles[MAX_X][MAX_Y];
} map;

void dump_dungeon(map *);

map generate_dungeon();
