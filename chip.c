#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "chip.h"
#include "instr.h"

chip_t *
chip_init(void)
{
  chip_t *chip = calloc(1, sizeof(chip_t));

  memcpy(chip->memory, FONT_SET, sizeof(FONT_SET));
  memset(chip->v, 0, sizeof(chip->v));
  memset(chip->stack, 0, sizeof(chip->stack));
  memset(chip->display, COLOR_OFF, sizeof(chip->display));

  chip->i = 0;
  chip->pc = 0x200;
  chip->sp = 0;
  chip->draw = false;
  chip->delay_timer = 0;
  chip->sound_timer = 0;

  srand(time(NULL));

  return chip;
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
