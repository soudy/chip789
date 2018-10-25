#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

#define PIXEL_SIZE 10

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  int width, height;
} display_t;

display_t display_init(int width, int height);
void display_draw(display_t display, uint32_t *screen);
void display_free(display_t display);

#endif /* !DISPLAY_H */
