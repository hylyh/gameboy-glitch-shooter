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

#define PLAYER_SIZE 5
#define BULLET_SIZE 3

#define LINE_LENGTH 3

#define NUM_BULLETS 5
#define MAX_BULLET_CIRCLE_RAD 50

UBYTE input;
UBYTE lastInput;

typedef struct Player {
  UBYTE x;
  UBYTE y;
  UBYTE dir;
  UBYTE dirty;
} Player;

Player player;

typedef struct Bullet {
  UBYTE x;
  UBYTE y;
  BYTE dx;
  BYTE dy;

  // If true, then special kind of cone bullets
  UBYTE circle;
  UBYTE radius;
} Bullet;

Bullet bullets[NUM_BULLETS];

UBYTE bulletIndex;

void reset_bullet(UBYTE i)
{
  bullets[i].x = 1;
  bullets[i].y = 1;
  bullets[i].dx = 0;
  bullets[i].dy = 0;
  bullets[i].circle = FALSE;
  bullets[i].radius = 0;
}

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
  UBYTE i;

  player.x = GRAPHICS_WIDTH / 2;
  player.y = GRAPHICS_HEIGHT / 2;
  player.dir = DIR_UPLEFT;

  bulletIndex = 0;

  for(i = 0; i < NUM_BULLETS; i++) {
    reset_bullet(i);
  }
}

// Get the x/y offset for the given direction
void get_dir_offsets(UBYTE dir, BYTE *x, BYTE *y)
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

void shoot_bullet()
{
  bullets[bulletIndex].x = player.x;
  bullets[bulletIndex].y = player.y;
  get_dir_offsets(player.dir, &bullets[bulletIndex].dx, &bullets[bulletIndex].dy);

  bulletIndex += 1;
  if(bulletIndex >= NUM_BULLETS) {
    bulletIndex = 0;
  }
}

void shoot_circle()
{
  bullets[bulletIndex].x = player.x;
  bullets[bulletIndex].y = player.y;
  bullets[bulletIndex].circle = TRUE;
  bullets[bulletIndex].radius = 0;

  get_dir_offsets(player.dir, &bullets[bulletIndex].dx, &bullets[bulletIndex].dy);

  bulletIndex += 1;
  if(bulletIndex >= NUM_BULLETS) {
    bulletIndex = 0;
  }
}

void update_player()
{
  BYTE offset_x;
  BYTE offset_y;

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
  if(input & (J_DOWN | J_UP | J_LEFT | J_RIGHT)) {
    get_dir_offsets(player.dir, &offset_x, &offset_y);
    if (offset_x != 0 || offset_y != 0) {
      player.dirty = TRUE;
    }

    player.x += offset_x;
    player.y += offset_y;
  }

  if(input & J_A && !(lastInput & J_A)) {
    shoot_bullet();
  }

  if(input & J_B && !(lastInput & J_B)) {
    shoot_circle();
  }
}

void draw_player()
{
  BYTE line_offset_x;
  BYTE line_offset_y;

  if(player.dirty) {
    // Player moved, clear the old one
    color(WHITE, WHITE, SOLID);
    circle(player.x, player.y, PLAYER_SIZE, M_FILL);

    player.dirty = FALSE;
  }

  color(BLACK, WHITE, OR);
  box(player.x - PLAYER_SIZE / 2, player.y - PLAYER_SIZE / 2,
      player.x + PLAYER_SIZE / 2, player.y + PLAYER_SIZE / 2,
      M_FILL);

  get_dir_offsets(player.dir, &line_offset_x, &line_offset_y);
  line_offset_x *= LINE_LENGTH;
  line_offset_y *= LINE_LENGTH;

  line(player.x, player.y, player.x + line_offset_x, player.y + line_offset_y);
}

void update_bullets()
{
  UBYTE i;
  for(i = 0; i < NUM_BULLETS; i++) {
    bullets[i].x += bullets[i].dx;
    bullets[i].y += bullets[i].dy;

    if(bullets[i].circle) {
      bullets[i].radius += 2;

      if(bullets[i].radius > MAX_BULLET_CIRCLE_RAD) {
        reset_bullet(i);
      }
    }
  }
}

void draw_bullets()
{
  UBYTE i;
  for(i = 0; i < NUM_BULLETS; i++) {
    if (bullets[i].circle) {
      color(LTGREY, LTGREY, XOR);
      circle(bullets[i].x, bullets[i].y, bullets[i].radius, M_NOFILL);
    }
    else {
      color(DKGREY, DKGREY, XOR);
      box(bullets[i].x - BULLET_SIZE / 2, bullets[i].y - BULLET_SIZE / 2,
          bullets[i].x + BULLET_SIZE / 2, bullets[i].y + BULLET_SIZE / 2,
          M_FILL);
    }
  }
}

void main()
{
  init_draw();
  init_game();

  while(TRUE) {
    lastInput = input;
    input = joypad();

    update_player();
    draw_player();

    update_bullets();
    draw_bullets();
  }
}
