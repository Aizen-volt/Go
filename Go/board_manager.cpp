#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include "board_manager.h"
#include "config.h"
#include "conio2.h"
#include "game.h"


void ClearScreen() {
	struct text_info windowInfo;
	gettextinfo(&windowInfo);

	textbackground(BACKGROUND_COLOR);
	textcolor(TEXT_COLOR);
	clrscr();

	gotoxy(1, windowInfo.screenheight + 1);
	clrscr();

}



void DrawScreen(game_t* currentGameState) {

	ClearScreen();
	DrawLegend();
	DrawBoard(currentGameState->cellOwner, &currentGameState->boardSize, &currentGameState->xBoardOffset, currentGameState->cellsEndangered);
	DrawBoardSize(&currentGameState->boardSize);
	DrawPlayersTurn(currentGameState->currentPlayer);
	DrawPlayersPoints(currentGameState->playerPoints);
	ResetCursorPosition(currentGameState->cellOwner, currentGameState->cellsEndangered, currentGameState->cursorPosition, &currentGameState->boardSize, &currentGameState->boardEvenOffset, &currentGameState->xBoardOffset);
}



void DrawBoard(player** cellOwner, int* boardSize, int* xBoardOffset, bool** cellsEndangered) {

	DrawBorder(boardSize, xBoardOffset);

	//drawing vertical lines, we put vertical line or black/white stone depending on cell saved value
	for (int i = 0; i < BOARD_SIZE_X; i += 2) {
		for (int j = 0; j < *boardSize; j++) {
			gotoxy(BOARD_OFFSET_X + i - 2 - *xBoardOffset, BOARD_OFFSET_Y + j + 1);
			putch(HORIZONTAL_LINE);
			gotoxy(BOARD_OFFSET_X + i - 1 - *xBoardOffset, BOARD_OFFSET_Y + j + 1);

			switch (cellsEndangered[i / 2][j])
			{
			case true:
				textbackground(RED);
				break;
			case false:
				textbackground(BOARD_COLOR);
				break;
			default:
				break;
			}

			switch (cellOwner[i / 2][j]) {
			case cellEmpty:
				textcolor(TEXT_COLOR);
				putch(VERTICAL_LINE);
				break;
			case playerBlack:
				textcolor(BLACK);
				putch(STONE);
				break;
			case playerWhite:
				textcolor(WHITE);
				putch(STONE);
				textcolor(TEXT_COLOR);
				break;
			default:
				break;
			}
			textbackground(BOARD_COLOR);
		}
	}

	//drawing horizontal lines
	for (int i = 0; i < *boardSize; i++) {
		gotoxy(BOARD_OFFSET_X + BOARD_SIZE_X - 2 - *xBoardOffset, BOARD_OFFSET_Y + i + 1);
		putch(HORIZONTAL_LINE);
	}
}



void DrawBorder(int* boardSize, int* xBoardOffset) {

	textbackground(BOARD_COLOR);
	textcolor(TEXT_COLOR);

	//corners first
	gotoxy(BOARD_OFFSET_X - 3 - *xBoardOffset, BOARD_OFFSET_Y);
	putch(TOP_LEFT_CORNER);

	gotoxy(BOARD_OFFSET_X - 3 - *xBoardOffset, BOARD_OFFSET_Y + *boardSize + 1);
	putch(BOTTOM_LEFT_CORNER);

	gotoxy(BOARD_OFFSET_X + BOARD_SIZE_X - 1 - *xBoardOffset, BOARD_OFFSET_Y);
	putch(TOP_RIGHT_CORNER);

	gotoxy(BOARD_OFFSET_X + BOARD_SIZE_X - 1 - *xBoardOffset, BOARD_OFFSET_Y + *boardSize + 1);
	putch(BOTTOM_RIGHT_CORNER);

	// top/bottom border
	for (int i = 0; i < BOARD_SIZE_X + 1; i++) {
		gotoxy(BOARD_OFFSET_X - 2 + i - *xBoardOffset, BOARD_OFFSET_Y);
		putch(HORIZONTAL_DOUBLE_LINE);

		gotoxy(BOARD_OFFSET_X - 2 + i - *xBoardOffset, BOARD_OFFSET_Y + *boardSize + 1);
		putch(HORIZONTAL_DOUBLE_LINE);
	}

	// left/right border
	for (int i = 0; i < *boardSize; i++) {
		gotoxy(BOARD_OFFSET_X - 3 - *xBoardOffset, BOARD_OFFSET_Y + 1 + i);
		putch(VERTICAL_DOUBLE_LINE);

		gotoxy(BOARD_OFFSET_X + BOARD_SIZE_X - 1 - *xBoardOffset, BOARD_OFFSET_Y + 1 + i);
		putch(VERTICAL_DOUBLE_LINE);
	}
}



void DrawLegend() {

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_AUTHOR_POSITION);
	textbackground(BLACK);
	textcolor(RED);
	cputs("Mateusz Kaszubowski 193050");

	textcolor(WHITE);

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_TASKS_POSITION);
	cputs("Functionalities: a,b,c,d,e,f,g,h,i,j,k,l,n");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_ARROWS_POSITION);
	cputs("arrows - move around the board ");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_EXIT_POSITION);
	cputs("q      - exit game");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_START_POSITION);
	cputs("n      - start a new game");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_CONFIRM_POSITION);
	cputs("enter  - confirm action");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_CANCEL_POSITION);
	cputs("escape - cancel action");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_PLACE_POSITION);
	cputs("i      - place stone on the board");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_SAVE_POSITION);
	cputs("s      - save game");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_LOAD_POSITION);
	cputs("l      - load game");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FINISH_POSITION);
	cputs("f      - finish game");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_BOARDCHANGE_POSITION);
	cputs("b      - change board size");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_HANDICAP_POSITION);
	cputs("h      - handicap mode");

}



void DrawPlayersTurn(bool currentPlayer) {

	textbackground(BLACK);
	textcolor(GREEN);

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_TURN_POSITION);
	cputs(currentPlayer == playerBlack ? "Black" : "White");
	cputs(" player's turn.");

	textcolor(WHITE);
	gotoxy(BOARD_OFFSET_X, BOARD_OFFSET_Y);
}



void DrawPlayersPoints(float* playerPoints) {
	//uses snprintf to convert float player points into char array - managable by cputs

	char points[20];

	textbackground(BLACK);
	textcolor(WHITE);

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_POINTS_B_POSITION);
	cputs("Player black: ");

	snprintf(points, sizeof(points), "%.1f", playerPoints[playerBlack]);

	textcolor(GREEN);
	cputs(points);
	textcolor(WHITE);

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_POINTS_W_POSITION);
	cputs("Player white: ");

	snprintf(points, sizeof(points), "%.1f", playerPoints[playerWhite]);

	textcolor(GREEN);
	cputs(points);
	textcolor(WHITE);

	//gotoxy(BOARD_OFFSET_X - 2 + ((BOARD_SIZE_X) / 2), BOARD_OFFSET_Y + 1 + (currentGameState->boardSize / 2));
}



void WriteCursorPosition(int* cursorPosition, int* xBoardOffset, int* boardSize) {
	//uses sprintf to convert int cursorPosition into char array - managable by cputs

	char temp[3];

	//we clean line first to ensure no additional digits are left on the screen
	textbackground(BLACK);
	textcolor(WHITE);
	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_CURSOR_POSITION);
	cputs("                               ");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_CURSOR_POSITION);
	cputs("Current cursor position: ");
	textcolor(GREEN);

	sprintf(temp, "%ld", BOARD_INDEX_FROM_CURSOR_X + 1 + *xBoardOffset);
	cputs(temp);

	putch('x');

	sprintf(temp, "%ld", BOARD_INDEX_FROM_CURSOR_Y + 1);
	cputs(temp);
	textcolor(WHITE);

	gotoxy(cursorPosition[cursorX], cursorPosition[cursorY]);
}



void DrawBoardSize(int* boardSize) {
	//uses sprintf to convert int boardSize into char array - managable by cputs

	char temp[3];
	sprintf(temp, "%d", *boardSize);

	textbackground(BLACK);
	textcolor(WHITE);

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_BOARDSIZE_POSITION);
	cputs("Board size: ");

	cputs(temp);
	putch('x');
	cputs(temp);
}



void DrawWinner(float* playerPoints) {
	//we draw winner depending on who's got more points

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_ALERT_POSITION);
	textcolor(RED);
	textbackground(BLACK);

	cputs("Player ");

	playerPoints[playerWhite] > playerPoints[playerBlack] ? cputs("white ") : cputs("black ");

	cputs("wins!!!");
}



void DrawConfirmation() {
	//draw confirmation query for putting a stone

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_ALERT_POSITION);
	textbackground(BLACK);
	textcolor(RED);

	cputs("Confirm your choice: ENTER / ESCAPE");

	textcolor(TEXT_COLOR);
	textbackground(BOARD_COLOR);
	//gotoxy(GetCursorX(currentGameState) - BOARD_EVEN_OFFSET, GetCursorY(currentGameState));
}



void RemoveConfirmation() {
	//remove confirmation query for putting a stone

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_ALERT_POSITION);
	textbackground(BLACK);

	cputs("                                   ");

	textcolor(TEXT_COLOR);
	textbackground(CELL_HIGHLIGHTED_COLOR);
	//gotoxy(GetCursorX(currentGameState) - BOARD_EVEN_OFFSET, GetCursorY(currentGameState));
}



void DrawAlert(const char* alert, int sleepTime, int* cursorPosition) {
	//draw alert given as an argument, wait and then delete

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_ALERT_POSITION);
	textbackground(BLACK);
	textcolor(RED);

	cputs(alert);

	Sleep(sleepTime);

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_ALERT_POSITION);
	textcolor(TEXT_COLOR);
	cputs("                                          ");

	gotoxy(cursorPosition[cursorX], cursorPosition[cursorY]);
}



void CheckCharacterInput(int characterInput, game_t* currentGameState) {

	switch (characterInput)
	{
	case 'q':
		ExitGame(&currentGameState->boardSize, currentGameState->cellOwner, currentGameState->cellsEndangered);
		break;
	case 'n':
		NewGame(&currentGameState->boardSize);
		break;
	case 'i':
		PlaceStone(currentGameState);
		break;
	case 's':
		SaveGame(currentGameState);
		break;
	case 'l':
		LoadGame(currentGameState);
		break;
	case 'f':
		FinishGame(currentGameState);
		break;
	case 'b':
		ChangeBoardSize(&currentGameState->boardSize, currentGameState->cursorPosition);
		break;
	case 'h':
		//turn on/off handicap mode
		currentGameState->handicapActive ?
			EndHandicap(&currentGameState->handicapActive, &currentGameState->currentPlayer, currentGameState->cursorPosition) :
			Handicap(&currentGameState->gameHasStarted, currentGameState->cursorPosition, &currentGameState->handicapActive, &currentGameState->handicapOccured);
		break;
	case ARROW_UP:
	case ARROW_DOWN:
	case ARROW_LEFT:
	case ARROW_RIGHT:
		MoveCursor(characterInput, currentGameState->cellOwner, currentGameState->cursorPosition, &currentGameState->boardSize, &currentGameState->xBoardOffset, currentGameState->cellsEndangered);
		break;
	default:
		break;
	}
}



void MoveCursor(int characterInput, player** cellOwner, int* cursorPosition, int* boardSize, int* xBoardOffset, bool** cellsEndangered) {
	//depending on direction, we check if the move can be made (no border in the way), if board needs to be moved (directory cell not visible at the moment)
	//we turn off previous position highlighting and highlight the new one

	switch (characterInput)
	{
	case ARROW_UP:
		if (BOARD_INDEX_FROM_CURSOR_Y - 1 >= 0) {
			ChangeCellColor(BOARD_COLOR, cellOwner, cellsEndangered, cursorPosition, boardSize, xBoardOffset);
			SetCursor(cursorPosition, 0, -1);
		}
		break;
	case ARROW_DOWN:
		if (BOARD_INDEX_FROM_CURSOR_Y + 1 <= *boardSize - 1) {
			ChangeCellColor(BOARD_COLOR, cellOwner, cellsEndangered, cursorPosition, boardSize, xBoardOffset);
			SetCursor(cursorPosition, 0, 1);
		}
		break;
	case ARROW_LEFT:
		if (BOARD_INDEX_FROM_CURSOR_X - 1 + *xBoardOffset >= 0) {
			ChangeCellColor(BOARD_COLOR, cellOwner, cellsEndangered, cursorPosition, boardSize, xBoardOffset);
			SetCursor(cursorPosition, -2, 0);
			CheckOffsetX(cellOwner, cursorPosition, xBoardOffset, boardSize, cellsEndangered);
		}
		break;
	case ARROW_RIGHT:
		if (BOARD_INDEX_FROM_CURSOR_X + 1 + *xBoardOffset <= *boardSize - 1) {
			ChangeCellColor(BOARD_COLOR, cellOwner, cellsEndangered, cursorPosition, boardSize, xBoardOffset);
			SetCursor(cursorPosition, 2, 0);
			CheckOffsetX(cellOwner, cursorPosition, xBoardOffset,  boardSize, cellsEndangered);
		}
		break;
	default:
		break;
	}
	
	gotoxy(cursorPosition[cursorX], cursorPosition[cursorY]);
	ChangeCellColor(CELL_HIGHLIGHTED_COLOR, cellOwner, cellsEndangered, cursorPosition, boardSize, xBoardOffset);
	WriteCursorPosition(cursorPosition, xBoardOffset, boardSize);
}



void SetCursor(int* cursorPosition, int x, int y) {
	//changes cursor position by value given

	cursorPosition[cursorX] += x;
	cursorPosition[cursorY] += y;
}



void SetCursorAbsolute(int* cursorPosition, int x, int y) {
	//sets cursor position to an absolute value given

	cursorPosition[cursorX] = x;
	cursorPosition[cursorY] = y;
}



void ResetCursorPosition(player** cellOwner, bool** cellsEndangered, int* cursorPosition, int* boardSize, int* boardEvenOffset, int* xBoardOffset) {
	//moves cursor to the center of the board

	cursorPosition[cursorX] = BOARD_OFFSET_X - 2 + ((BOARD_SIZE_X) / 2) - *boardEvenOffset;
	cursorPosition[cursorY] = BOARD_OFFSET_Y + 1 + (*boardSize / 2);

	gotoxy(cursorPosition[cursorX], cursorPosition[cursorY]);
	ChangeCellColor(CELL_HIGHLIGHTED_COLOR, cellOwner, cellsEndangered, cursorPosition, boardSize, xBoardOffset);
}



void ChangeCellColor(int color, player** cellOwner, bool** cellsEndangered, int* cursorPosition, int* boardSize, int* xBoardOffset) {
	//change background color while keeping its content - used for highlighting cursor position,
	//turning off highlighting of previous cursor position and highlighting endangered stone chains

	switch (cellsEndangered[BOARD_INDEX_FROM_CURSOR_X][BOARD_INDEX_FROM_CURSOR_Y])
	{
	case true:
		textbackground(RED);
		break;
	case false:
		textbackground(color);
		break;
	default:
		break;
	}

	switch (cellOwner[BOARD_INDEX_FROM_CURSOR_X][BOARD_INDEX_FROM_CURSOR_Y])
	{
	case cellEmpty:
		textcolor(TEXT_COLOR);
		putch(VERTICAL_LINE);
		break;
	case playerBlack:
		textcolor(BLACK);
		putch(STONE);
		break;
	case playerWhite:
		textcolor(WHITE);
		putch(STONE);
		break;
	}
	gotoxy(cursorPosition[cursorX], cursorPosition[cursorY]);
}



void CheckOffsetX(player** cellOwner, int* cursorPosition, int* xBoardOffset,  int* boardSize, bool** cellsEndangered) {
	//we use text_info struct to get window size, then check if cursor goes beyond that size - if so we change offset and redraw screen

	struct text_info windowInfo;
	gettextinfo(&windowInfo);

	int tempOffset = *xBoardOffset;
	int tempCursorPositionX, tempCursorPositionY, cursorOffset;
	tempCursorPositionX = cursorPosition[cursorX];
	tempCursorPositionY = cursorPosition[cursorY];

	if (cursorPosition[cursorX] > windowInfo.screenwidth - 2) {
		*xBoardOffset += 2;
		cursorOffset = 2;
	}
	else if (BOARD_INDEX_FROM_CURSOR_X < 0) {
		*xBoardOffset -= 2;
		cursorOffset = -2;
	}

	if (tempOffset != *xBoardOffset) {
		ClearScreen();
		DrawLegend();
		DrawBoard(cellOwner, boardSize, xBoardOffset, cellsEndangered);

		ChangeCellColor(BOARD_COLOR, cellOwner, cellsEndangered, cursorPosition, boardSize, xBoardOffset);
		SetCursorAbsolute(cursorPosition, tempCursorPositionX - cursorOffset, tempCursorPositionY);
	}
}