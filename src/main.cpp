#include "conio2.h"
#include "game.h"

int main() {

	//initializing conio2 library in case of C compiler
	#ifndef __cplusplus
		Conio2_Init();
	#endif

	settitle("Mateusz Kaszubowski 193050");

	//get rid of blinking cursor
	_setcursortype(_NOCURSOR);

	StartGame(DEFAULT_BOARD_SIZE);

	_setcursortype(_NORMALCURSOR);
	return 0;
}
