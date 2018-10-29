#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "chip.h"

void
chip_skip(chip_t *chip)
{
  chip->pc += 2;
}

static void
chip_cls(chip_t *chip) {
  memset(chip->display, COLOR_OFF, sizeof(chip->display));
}

static void
chip_ret(chip_t *chip)
{
  if (chip->sp == 0) {
    chip_error("stack underflow");
  }

  chip->pc = chip->stack[--chip->sp];
}

static void
chip_call(chip_t *chip, uint16_t addr)
{
  if (chip->sp > STACK_SIZE) {
    chip_error("stack overflow");
  }

  chip->stack[chip->sp] = chip->pc;
  chip->sp++;
  chip->pc = addr;
}

static void
chip_add(chip_t *chip, uint8_t addr, uint8_t a, uint8_t b)
{
  // Set VF to 1 if the result overflows, otherwise 0
  chip->v[0xF] = (a > 0 && b > UINT8_MAX - a) ? 1 : 0;
  chip->v[addr] = a + b;
}

static void
chip_sub(chip_t *chip, uint8_t addr, uint8_t a, uint8_t b)
{
  chip->v[0xF] = (a > b) ? 1 : 0;
  chip->v[addr] = a - b;
}

static void
chip_shr(chip_t *chip, uint8_t x)
{
  chip->v[0xF] = (chip->v[x] & 0x01) ? 1 : 0;
  chip->v[x] >>= 2;
}

static void
chip_shl(chip_t *chip, uint8_t x)
{
  chip->v[0xF] = (chip->v[x] & 0x80) ? 1 : 0;
  chip->v[x] <<= 2;
}

static void
chip_wait_for_key(chip_t *chip, uint8_t x)
{
  chip->pc -= 2;

  for (int i = 0; i < NUM_KEYS; i++) {
    if (chip->keys[i] == 1) {
      chip->v[x] = i;
      chip_skip(chip);
      break;
    }
  }
}

static void
chip_bcd(chip_t *chip, uint8_t x)
{
  uint8_t vx = chip->v[x];
  chip->memory[chip->i] = vx / 100;
  chip->memory[chip->i + 1] = (vx / 10) % 10;
  chip->memory[chip->i + 2] = vx % 10;
}

static void
chip_draw(chip_t *chip, uint8_t x, uint8_t y, uint8_t n)
{
  uint8_t pixel;
  chip->v[0xF] = 0;

  for (int yline = 0; yline < n; yline++) {
    pixel = chip->memory[chip->i + yline];

    for (int xline = 0; xline < 8; xline++) {
      if (pixel & (0x80 >> xline)) {
        int index = (chip->v[x] + xline) % DISPLAY_WIDTH +
                    ((chip->v[y] + yline) % DISPLAY_HEIGHT) * DISPLAY_WIDTH;

        if (chip->display[index] == COLOR_ON) {
          chip->v[0xF] = 1;
          chip->display[index] = COLOR_OFF;
        } else {
          chip->display[index] = COLOR_ON;
        }

        chip->draw = true;
      }
    }
  }
}

void
run_instr(chip_t *chip, uint16_t instr)
{
  chip_skip(chip);

  uint8_t a = (instr & 0xF000) >> 12;
  uint8_t x = (instr & 0x0F00) >> 8;
  uint8_t y = (instr & 0x00F0) >> 4;
  uint8_t n = (instr & 0x000F);
  uint8_t kk = instr & 0x00FF;
  uint16_t xyn = instr & 0x0FFF;

  switch (a) {
  case 0x0:
    switch (xyn) {
    case 0x0E0:
      // CLS
      chip_cls(chip);
      break;
    case 0x0EE:
      // RET
      chip_ret(chip);
      break;
    default:
      // SYS
      // The SYS instruction is only used on the old computers on which Chip-8
      // was originally implemented. It is ignored by modern interpreters.
      break;
    }
    break;
  case 0x1:
    // JP addr
    chip->pc = xyn;
    break;
  case 0x2:
    // CALL addr
    chip_call(chip, xyn);
    break;
  case 0x3:
    // SE Vx, byte
    if (chip->v[x] == kk) {
      chip_skip(chip);
    }
    break;
  case 0x4:
    // SNE Vx, byte
    if (chip->v[x] != kk) {
      chip_skip(chip);
    }
    break;
  case 0x5:
    // SE Vx, Vy
    if (chip->v[x] == chip->v[y]) {
      chip_skip(chip);
    }
    break;
  case 0x6:
    // LD Vx, byte
    chip->v[x] = kk;
    break;
  case 0x7: {
    // ADD Vx, byte
    chip_add(chip, x, chip->v[x], kk);
    break;
  }
  case 0x8:
    switch (n) {
    case 0x0:
      // LD Vx, Vy
      chip->v[x] = chip->v[y];
      break;
    case 0x1:
      // OR Vx, Vy
      chip->v[x] |= chip->v[y];
      break;
    case 0x2:
      // AND Vx, Vy
      chip->v[x] &= chip->v[y];
      break;
    case 0x3:
      // XOR Vx, Vy
      chip->v[x] ^= chip->v[y];
      break;
    case 0x4: {
      // ADD Vx, Vy
      chip_add(chip, x, chip->v[x], chip->v[y]);
      break;
    }
    case 0x5:
      // SUB Vx, Vy
      chip_sub(chip, x, chip->v[x], chip->v[y]);
      break;
    case 0x6:
      // SHR Vx {, Vy}
      chip_shr(chip, x);
      break;
    case 0x7:
      // SUBN Vx, Vy
      chip_sub(chip, x, chip->v[y], chip->v[x]);
      break;
    case 0xE:
      // SHL Vx {, Vy}
      chip_shl(chip, x);
      break;
    }
    break;
  case 0x9:
    switch (n) {
    case 0x0:
      // SNE Vx, Vy
      if (chip->v[x] != chip->v[y]) {
        chip_skip(chip);
      }
      break;
    }
    break;
  case 0xA:
    // LD I, addr
    chip->i = xyn;
    break;
  case 0xB:
    // JP V0, addr
    chip->pc = xyn + chip->v[0];
    break;
  case 0xC:
    // RND Vx, byte
    chip->v[x] = (rand() % 0xFF) & kk;
    break;
  case 0xD:
    // DRW Vx, Vy, nibble
    chip_draw(chip, x, y, n);
    break;
  case 0xE:
    switch (kk) {
    case 0x9E:
      // SKP Vx
      if (chip->keys[chip->v[x]] == 1) {
        chip_skip(chip);
      }
      break;
    case 0xA1:
      // SKNP Vx
      if (chip->keys[chip->v[x]] == 0) {
        chip_skip(chip);
      }
      break;
    }
  case 0xF:
    switch (kk) {
    case 0x07:
      // LD Vx, DT
      chip->v[x] = chip->delay_timer;
      break;
    case 0x0A:
      // LD Vx, K
      chip_wait_for_key(chip, x);
      break;
    case 0x15:
      // LD DT, Vx
      chip->delay_timer = chip->v[x];
      break;
    case 0x18:
      // LD ST, Vx
      chip->sound_timer = chip->v[x];
      break;
    case 0x1E:
      // ADD I, Vx
      chip->i += chip->v[x];
      break;
    case 0x29:
      // LD F, Vx
      chip->i = chip->v[x] * 5;
      break;
    case 0x33:
      // LD B, Vx
      chip_bcd(chip, x);
      break;
    case 0x55:
      // LD [I], Vx
      memcpy(chip->memory + chip->i, chip->v, x + 1);
      break;
    case 0x65:
      // LD Vx, [I]
      memcpy(chip->v, chip->memory + chip->i, x + 1);
      break;
    }
  }
}
