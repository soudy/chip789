#ifndef INSTR_H
#define INSTR_H

#include <stdint.h>

#include "chip.h"

void run_instr(chip_t *chip, uint16_t instr);

#endif /* !INSTR_H */
