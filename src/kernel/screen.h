#ifndef _SCREEN_H
#define _SCREEN_H

#define LINES   24
#define COLUMNS 80

#define kputs(s)     screen_puts(s)
#define kputc(c)     screen_putc(c)
#define kputsa(s, a) screen_putsa(s, a)
#define kputca(c, a) screen_putca(c, a)
#define kcls         screen_clear
// Sorry, I was just tired of this "screen" shit.

/*
 * About all attr parameters:
 *
 * They represent a char attribute, which is putten to video memory right after
 * char. This byte contains background and foreground color, as presented here:
 *
 *  _____ ______ ______ ______ ______ ______ ______ ______
 * | __  |  __  |  __  |      | ___   |  __  |      |  __  |
 * |   / | |__  | |__  | |__| |  __|  |  __| |  /|  | |  | |
 * |  /  | |__| |  __| |    | | ___|  | |__  |   |  | |__| |
 * |     | red  | green| blue |intense| red  | green| blue |
 * |_____|______|______|______|_______|______|______|______|
 *       \_____background____/ \________foreground_________/
 *
 * 7th bit is BIOS-dependent (do not use)
 */

void screen_putca(char c, char attr);
void screen_putc(char c);
void screen_putsa(char* str, char addr);
void screen_puts(char* str);
void screen_init();
void screen_clear();
void screen_scroll();

#endif
