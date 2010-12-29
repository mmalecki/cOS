#include "screen.h"
#include <string.h>
#include "types.h"
#include "hardware/graphics/monitor.h"

static unsigned char* video = 0;

unsigned int x = 0;
unsigned int y = 0;
char attribute = 7;

void screen_putca(char c, char attr) {
  if (c == '\n') {
    x = 0;
    y++;
    if (y >= LINES - 1)
      screen_scroll();
    return;
  }
  int pos = (x + y * COLUMNS) * 2;
  *(video + pos) = c; 
  *(video + pos + 1) = attr;
  x++;
  if (x >= COLUMNS)
    screen_putc('\n');
}

void screen_putc(char c) {
  screen_putca(c, attribute);
}

void screen_putsa(char* str, char attr) {
  for (; *str != 0; str++)
    screen_putca(*str, attr);
}

void screen_puts(char* str) {
  screen_putsa(str, attribute);
}

void screen_init() {
  if (video == 0) {
    video = monitor_get_mem(); 
    screen_clear();
  }
}

void screen_clear() {  
  memclr(video, COLUMNS * LINES * 2);
  x = 0;
  y = 0;
}

void screen_scroll() {
  /*y--;
  memmove(video + (COLUMNS * 2), video, COLUMNS * (LINES - 1) * 2);
  */
  memmove(video, video + COLUMNS * 4, COLUMNS * (LINES - 2) * 2);
  memclr(video + COLUMNS * (LINES - 2) * 2, COLUMNS * 4);
  // which cleans last 2 lines
  y -= 2;
}
