#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define MEMORY_SIZE 4096
#define REGISTER_SIZE 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define STACK_SIZE 16
#define NUM_KEYS 16

#define COLOR_ON 0xFFFFFFFF
#define COLOR_OFF 0xFF000000

static const uint8_t KEY_MAP[NUM_KEYS] = {
  0x01, 0x02, 0x03, 0x0c, // 1 2 3 c
  0x04, 0x05, 0x06, 0x0d, // 4 5 6 d
  0x07, 0x08, 0x09, 0x0e, // 7 8 9 e
  0x0a, 0x00, 0x0b, 0x0f  // a 0 b f
};

static const SDL_Scancode SDL_KEYS[NUM_KEYS] = {
  SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4, // 1 2 3 4
  SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R, // q w e r
  SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F, // a s d f
  SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V  // z x c v
};

static const uint8_t FONT_SET[] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

typedef struct {
  uint8_t memory[MEMORY_SIZE];
  uint8_t v[REGISTER_SIZE];
  uint16_t i;
  uint16_t pc;
  uint8_t delay_timer, sound_timer;

  uint16_t stack[STACK_SIZE];
  uint8_t sp;

  bool draw;
  uint32_t display[DISPLAY_WIDTH * DISPLAY_HEIGHT];
  uint8_t keys[NUM_KEYS];
} chip_t;

chip_t *chip_init(void);
void chip_cycle(chip_t *chip);
void chip_error(const char *fmt, ...);
void load_game(chip_t *chip, const char *filename);

#endif /* !CHIP_H */
