#include "display.h"

display_t
display_init(int width, int height)
{
  SDL_Init(SDL_INIT_VIDEO);

  display_t display = {
    .width = width,
    .height = height
  };

  display.window = SDL_CreateWindow(
    "chip789",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    display.width * PIXEL_SIZE,
    display.height * PIXEL_SIZE,
    SDL_WINDOW_OPENGL);

  display.renderer = SDL_CreateRenderer(display.window, -1, SDL_RENDERER_ACCELERATED);

  display.texture = SDL_CreateTexture(
    display.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
    display.width, display.height);

  return display;
}

void
display_draw(display_t display, uint32_t *screen)
{
  SDL_UpdateTexture(display.texture, NULL, screen,
                    display.width * sizeof(uint32_t));
  SDL_RenderCopy(display.renderer, display.texture, NULL, NULL);
  SDL_RenderPresent(display.renderer);
}

void
display_free(display_t display)
{
  SDL_DestroyTexture(display.texture);
  SDL_DestroyRenderer(display.renderer);
  SDL_DestroyWindow(display.window);
  SDL_Quit();
}
