# chip789
chip789 is a Chip-8 emulator written in C11 using SDL2 graphics. Chip-8 is a
simple, interpreted, programming language which was first used on some
do-it-yourself computer systems in the late 1970s and early 1980s. The COSMAC
VIP, DREAM 6800, and ETI 660 computers are a few examples. These computers
typically were designed to use a television as a display, had between 1 and 4K
of RAM, and used a 16-key hexadecimal keypad for input. The interpreter took
up only 512 bytes of memory, and programs, which were entered into the computer
in hexadecimal, were even smaller.


## Dependencies
* [SDL2](https://www.libsdl.org/)
* A C11 compiler

## Usage
The emulator can be built with `make`, which will create `build/chip789`.
To run a game, `$ chip789 game`, where game is a Chip-8 binary. There are some
examples in the `games/` directory.

The keypad is implemented as such:
```
1	2	3	C     1	2	3	4
4	5	6	D	->	q	w	e	r
7	8	9	E     a	s	d	f
A	0	B	F		  z	x	c	v
```

The keypad on the left is the layout of the original computers which used the
Chip-8 language, the keypad on the right is the layout implemented in this
implementation.

## License
Copyright © 2018 Steven Oud

Distributed under the MIT license.
