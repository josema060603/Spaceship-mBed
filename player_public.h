//=================================================================
// The header file is for module "player"
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================
#ifndef PLAYER_PUBLIC_H
#define PLAYER_PUBLIC_H

#include "doubly_linked_list.h"
#include "globals.h"
#include "graphics.h"
#include "mbed.h"

//==== [private settings] ====
#define PLAYER_INIT_Y 90
#define PLAYER_SIZE 11
#define MISSILE_LENGTH 6
#define MISSILE_WIDTH 4
#define PLAYER_COLOR 0x0000FF // blue
#define PLAYER_MISSILE_SPEED 5
#define PLAYER_MISSILE_COLOR 0x0000FF // blue

typedef struct {
  boundingBox box;

  // moving unit in y dir
} MISSILE;
extern int sprite2_arr[];
extern int sprite3_arr[];
typedef struct {
  int x;
  int y; // The y-coordinate of missile current position
  int height;
  int score; // Current score
  int missile_length;
  boundingBox *missile_boundingBox;
  int lives;
  bool player_has_missile; // The missile status: 0->In the air 1->With Player
} PLAYER;

/** getter for user to acquire info without accessing structure
 */
PLAYER player_get_info(void);
MISSILE missile_get_info(void);

int *getSprite(void);

/** Initialize all values in the PLAYER struct, then draw the player
 */
void player_init(void);

/** Check if player's current location is valid, then move the player
 *  "player.height" pixels up
 */
void player_moveUp(void);

/** Check if player's current location is valid, then move the player
 *  "player.height" pixels down
 */
void player_moveDown(void);

void player_moveLeft(void);

void player_moveRight(void);

/** Throw missile by only by changing a value in the PLAYER struct
 */
void player_throw(void);

/** Check the status of the missile in the PLAYER struct and draw the missile
 * accordingly
 */
void player_missile_draw(void);

/** Draw the player
 */
void player_draw(int color);

/** Reset missile's coordinates so that it comes back to the player's position,
 *  then redraw the missile. The missile must not be visible when it's at the
 * player's position
 */
void player_missile_return(void);

/** Increment the score by 1
 */
void player_score_inc(void);
void player_score_reset(void);
void player_draw_sprite(void);
void player_missile_draw_blank(void);
void draw_main_screen(int lang);
void print_inst(int lang);
void print_select_difficulty(int lang);
#endif // PLAYER_PUBLIC_H