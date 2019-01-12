#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "chip.h"
#include "display.h"

int
main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "usage: %s game\n", argv[0]);
    return EXIT_FAILURE;
  }

  char *filename = argv[1];
  chip_t *chip = chip_init();
  display_t display = display_init(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  SDL_Event event;
  bool run = true;

  load_game(chip, filename);

  while (run) {
    chip_cycle(chip);

    if (chip->draw) {
      display_draw(display, chip->display);
      chip->draw = false;
    }

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        run = false;
        break;
      case SDL_KEYUP:
      case SDL_KEYDOWN:
        for (int i = 0; i < NUM_KEYS; i++) {
          if (event.key.keysym.scancode == SDL_KEYS[i]) {
            chip->keys[KEY_MAP[i]] = (event.type == SDL_KEYDOWN) ? 1 : 0;
            break;
          }
        }
        break;
      }
    }

    SDL_Delay(1);
  }

  free(chip);
  display_free(display);

  return EXIT_SUCCESS;
}
