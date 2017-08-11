#include <gb/gb.h>
#include <gb/drawing.h>
#include <stdio.h>

#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3
#define DIR_UPLEFT 4
#define DIR_UPRIGHT 5
#define DIR_DOWNLEFT 6
#define DIR_DOWNRIGHT 7

#define PLAYER_WIDTH 3
#define PLAYER_HEIGHT 3

#define LINE_LENGTH 3

UBYTE input;

typedef struct Player {
  UBYTE x;
  UBYTE y;
  UBYTE dir;
  UBYTE dirty;
} Player;

Player player;

void init_draw()
{
  // For some reason, color() won't work unless you draw something first, so do that, then undo it
  box(0, 0, 1, 1, M_FILL);
  color(WHITE, WHITE, SOLID);
  box(0, 0, 1, 1, M_FILL);
  color(BLACK, WHITE, SOLID);
}

void init_game()
{
  player.x = GRAPHICS_WIDTH / 2;
  player.y = GRAPHICS_HEIGHT / 2;
  player.dir = DIR_UPLEFT;
}

// Get the x/y offset for the given direction
void get_dir_offsets(UBYTE dir, UBYTE *x, UBYTE *y)
{
  // Set x offset
  switch(dir) {
  case DIR_UP:
  case DIR_DOWN:
    (*x) = 0;
    break;
  case DIR_LEFT:
  case DIR_UPLEFT:
  case DIR_DOWNLEFT:
    (*x) = -1;
    break;
  case DIR_RIGHT:
  case DIR_UPRIGHT:
  case DIR_DOWNRIGHT:
    (*x) = 1;
    break;
  }


  // Set y offset
  switch(dir) {
  case DIR_LEFT:
  case DIR_RIGHT:
    (*y) = 0;
    break;
  case DIR_UP:
  case DIR_UPLEFT:
  case DIR_UPRIGHT:
    (*y) = -1;
    break;
  case DIR_DOWN:
  case DIR_DOWNLEFT:
  case DIR_DOWNRIGHT:
    (*y) = 1;
    break;
  }
}

void update_player()
{
  UBYTE offset_x;
  UBYTE offset_y;

  // Check for pure directional inputs (i.e. nothing else is pressed)
  if(input == J_DOWN) {
    player.dir = DIR_DOWN;
  }
  else if(input == J_UP) {
    player.dir = DIR_UP;
  }
  else if(input == J_LEFT) {
    player.dir = DIR_LEFT;
  }
  else if(input == J_RIGHT) {
    player.dir = DIR_RIGHT;
  }
  // Check for combined inputs
  else if(input & J_DOWN && input & J_LEFT) {
    player.dir = DIR_DOWNLEFT;
  }
  else if(input & J_DOWN && input & J_RIGHT) {
    player.dir = DIR_DOWNRIGHT;
  }
  else if(input & J_UP && input & J_LEFT) {
    player.dir = DIR_UPLEFT;
  }
  else if(input & J_UP && input & J_RIGHT) {
    player.dir = DIR_UPRIGHT;
  }

  // Move the player based on direction (if a movement button is pressed)
  if (input & (J_DOWN | J_UP | J_LEFT | J_RIGHT)) {
    get_dir_offsets(player.dir, &offset_x, &offset_y);
    if (offset_x != 0 || offset_y != 0) {
      player.dirty = TRUE;
    }

    player.x += offset_x;
    player.y += offset_y;
  }
}

void draw_player()
{
  UBYTE line_offset_x;
  UBYTE line_offset_y;

  if(player.dirty) {
    // Player moved, clear the old one
    color(WHITE, WHITE, SOLID);
    circle(player.x, player.y, PLAYER_WIDTH + LINE_LENGTH, M_FILL);

    player.dirty = FALSE;
  }

  color(BLACK, WHITE, OR);
  box(player.x - 1, player.y - 1, player.x + 1, player.y + 1, M_FILL);

  get_dir_offsets(player.dir, &line_offset_x, &line_offset_y);
  line_offset_x *= LINE_LENGTH;
  line_offset_y *= LINE_LENGTH;

  line(player.x, player.y, player.x + line_offset_x, player.y + line_offset_y);
}

void main()
{
  init_draw();
  init_game();

  while(TRUE) {
    input = joypad();

    update_player();
    draw_player();
  }

  draw_player();
}
