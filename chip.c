#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "chip.h"
#include "instr.h"

static const uint8_t fontset[] = {
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

chip_t *
chip_init(void)
{
  chip_t *chip = calloc(1, sizeof(chip_t));

  memcpy(chip->memory, fontset, sizeof(fontset));
  memset(chip->v, 0, sizeof(chip->v));
  memset(chip->stack, 0, sizeof(chip->stack));

  chip->i = 0;
  chip->pc = 0x200;
  chip->sp = 0;
  chip->draw = false;
  chip->delay_timer = 0;
  chip->sound_timer = 0;

  memset(chip->display, COLOR_OFF, sizeof(chip->display));

  srand(time(NULL));

  return chip;
}

void
chip_skip(chip_t *chip)
{
  chip->pc += 2;
}

void
chip_cycle(chip_t *chip)
{
  uint16_t instr = chip->memory[chip->pc] << 8 | chip->memory[chip->pc + 1];

  run_instr(chip, instr);

  if (chip->delay_timer > 0) {
    chip->delay_timer--;
  }

  if (chip->sound_timer > 0) {
    chip->sound_timer--;
  }
}

void
chip_error(const char *fmt, ...)
{
  va_list arg;
  va_start(arg, fmt);

  vfprintf(stderr, fmt, arg);

  va_end(arg);

  fprintf(stderr, "\n");

  exit(EXIT_FAILURE);
}

void
load_game(chip_t *chip, const char *filename)
{
  FILE *game = fopen(filename, "rb");
  if (!game) {
    chip_error("unable to load game file %s", filename);
  }

  fread(&chip->memory[0x200], 1, MEMORY_SIZE - 0x200, game);
  fclose(game);
}
