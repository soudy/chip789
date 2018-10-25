#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>
#include <stdbool.h>

#define MEMORY_SIZE 4096
#define REGISTER_SIZE 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define STACK_SIZE 16
#define NUM_KEYS 16
#define FONT_SIZE 80

#define COLOR_ON 0xFFFFFFFF
#define COLOR_OFF 0xFF000000

enum {
  HALT = 1 << 0,
  DRAW = 1 << 1
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
void chip_skip(chip_t *chip);
void chip_cycle(chip_t *chip);
void chip_error(const char *fmt, ...);
void load_game(chip_t *chip, const char *filename);

#endif /* !CHIP_H */
