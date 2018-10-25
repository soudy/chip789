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

## License
Copyright © 2018 Steven Oud

Distributed under the MIT license.
