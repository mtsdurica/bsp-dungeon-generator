/**
 * @file dungeon_generator_bsp.c
 * @author Matúš Ďurica (mtsdurica@gmail.com)
 * @brief Contains implementation of the BSP dungeon generator
 * @version 0.1
 * @date 2023-07-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../include/dungeon_generator_bsp.h"

/**
 * @brief Check equality of 2 points
 *
 * @param p1 Point 1
 * @param p2 Point 2
 * @return true If they are equal
 * @return false If they are not equal
 */
static bool points_equal(point *p1, point *p2)
{
    if (p1->x == p2->x && p1->y == p2->y)
        return true;
    else
        return false;
}

/**
 * @brief Draw horizontal wall
 *
 * @param w Wall to be drawn
 */
static void draw_horizontal_wall(wall *w, map *map)
{
    for (int x = w->start.x; x <= w->end.x; x++)
    {
        point tmp = {x, w->start.y};
        if (points_equal(&tmp, &w->start) || points_equal(&tmp, &w->end))
            map->tiles[x][w->start.y] = Corner;
        else
            map->tiles[x][w->start.y] = Wall;
    }
}

/**
 * @brief Draw vertical wall
 *
 * @param w Wall to be drawn
 */
static void draw_vertical_wall(wall *w, map *map)
{
    for (int y = w->start.y; y <= w->end.y; y++)
    {
        point tmp = {w->start.x, y};
        if (points_equal(&tmp, &w->start) || points_equal(&tmp, &w->end))
            map->tiles[w->start.x][y] = Corner;
        else
            map->tiles[w->start.x][y] = Wall;
    }
}

/**
 * @brief Get the center point of the child
 *
 * @param child Child
 * @return point Point of the center
 */
static point get_child_center(room *child)
{
    point center;
    center.x = child->top.start.x + (child->top.end.x - child->top.start.x) / 2;
    center.y = child->right.start.y + (child->right.end.y - child->right.start.y) / 2;
    return center;
}

/**
 * @brief Generate random number in range of 2 numbers
 *
 * @param min Min in range
 * @param max Max in range
 * @return UINT_CHANGEABLE Random number
 */
static UINT_CHANGEABLE random_in_range(UINT_CHANGEABLE min, UINT_CHANGEABLE max)
{
    // Calculate the middle of the range
    UINT_CHANGEABLE middle = (min + max) / 2;
    // Calculate the allowable deviation from the middle
    UINT_CHANGEABLE deviation = (max - min) / 3;
    return (rand() % (deviation * 2 + 1)) + (middle - deviation);
}

static bool generate_vertical_split(room *, map *map);

/**
 * @brief Generate horizontal split, splitting the parent room into 2 children
 *
 * @param r Parent room
 * @return true Ignored
 * @return false When ending recursion
 */
static bool generate_horizontal_split(room *r, map *map)
{
    if (r == NULL)
        return false;

    if (r->left.start.y + (PSEUDO_ROOM_SIZE / 2) >= r->left.end.y - (PSEUDO_ROOM_SIZE / 2))
        return false;

    UINT_CHANGEABLE rand_y =
        random_in_range(r->left.start.y + (PSEUDO_ROOM_SIZE / 2), r->left.end.y - (PSEUDO_ROOM_SIZE / 2));

    wall new_split = {{r->top.start.x, rand_y}, {r->top.end.x, rand_y}};

    room child1 = {r->top, new_split, {r->top.start, new_split.start}, {r->top.end, new_split.end}, NULL, NULL, {0, 0}};
    r->child1 = &child1;

    room child2 = {new_split, r->bot, {new_split.start, r->bot.start}, {new_split.end, r->bot.end}, NULL, NULL, {0, 0}};
    r->child2 = &child2;

    draw_horizontal_wall(&new_split, map);

    generate_vertical_split(&child1, map);
    child1.center = get_child_center(&child1);

    generate_vertical_split(&child2, map);
    child2.center = get_child_center(&child2);

    for (int i = child2.center.y; i > child1.center.y; i--)
        map->tiles[child2.center.x][i] = Walkable;

    refresh();
    return true;
}

/**
 * @brief Generate horizontal split, splitting the parent room into 2 children
 *
 * @param r Parent room
 * @return true Ignored
 * @return false When ending recursion
 */
static bool generate_vertical_split(room *r, map *map)
{
    if (r == NULL)
        return false;

    if (r->top.start.x + PSEUDO_ROOM_SIZE >= r->top.end.x - PSEUDO_ROOM_SIZE)
        return false;

    UINT_CHANGEABLE rand_x = random_in_range(r->top.start.x + PSEUDO_ROOM_SIZE, r->top.end.x - PSEUDO_ROOM_SIZE);

    wall new_split = {{rand_x, r->top.start.y}, {rand_x, r->bot.start.y}}; // third thingy maybe wrong

    room child1 = {
        {r->top.start, new_split.start}, {r->bot.start, new_split.end}, r->left, new_split, NULL, NULL, {0, 0}};
    r->child1 = &child1;

    room child2 = {{new_split.start, r->top.end}, {new_split.end, r->bot.end}, new_split, r->right, NULL, NULL, {0, 0}};
    r->child2 = &child2;

    draw_vertical_wall(&new_split, map);

    generate_horizontal_split(&child1, map);
    child1.center = get_child_center(&child1);

    generate_horizontal_split(&child2, map);
    child2.center = get_child_center(&child2);

    for (int i = child2.center.x; i > child1.center.x; i--)
        map->tiles[i][child2.center.y] = Walkable;

    return true;
}

/**
 * @brief Initialize base room
 *
 * @return room Base room
 */
static room initialize_base(map *map)
{
    room new_room = {{{0, 0}, {MAX_X - 1, 0}},
                     {{0, MAX_Y - 1}, {MAX_X - 1, MAX_Y - 1}},
                     {{0, 0}, {0, MAX_Y - 1}},
                     {{MAX_X - 1, 0}, {MAX_X - 1, MAX_Y - 1}},
                     NULL,
                     NULL,
                     {0, 0}};
    draw_horizontal_wall(&new_room.top, map);
    draw_horizontal_wall(&new_room.bot, map);
    draw_vertical_wall(&new_room.left, map);
    draw_vertical_wall(&new_room.right, map);
    return new_room;
}

/**
 * @brief Initialize map array stored in map struct
 *
 * @return map Initialized map struct
 */
static map initialize_map()
{
    map map;
    for (int x = 0; x < MAX_X; x++)
        for (int y = 0; y < MAX_Y; y++)
            map.tiles[x][y] = Walkable;
    return map;
}

/**
 * @brief Draw generated dungeon from map
 *
 * @param map Map struct containing array with drawn dungeon
 */
void dump_dungeon(map *map)
{
    char drawn_tile;
    for (int x = 0; x < MAX_X; x++)
        for (int y = 0; y < MAX_Y; y++)
        {
            switch (map->tiles[x][y])
            {
            case Corner:
                drawn_tile = CORNER_TILE;
                break;
            case Wall:
                drawn_tile = WALL_TILE;
                break;
            case Walkable:
                drawn_tile = WALKABLE_TILE;
                break;
            default:
                break;
            }
            mvprintw(y, x, "%c", drawn_tile);
        }
}

/**
 * @brief Generate dungeon using BSP
 *
 * @return map Struct containing the array with generated dungeon
 */
map generate_dungeon()
{
    srand(time(NULL));

    map map = initialize_map();

    room base = initialize_base(&map);

    if (rand() % 2)
        (void)generate_vertical_split(&base, &map);
    else
        (void)generate_horizontal_split(&base, &map);

    return map;
}
