#ifndef BOARD_MANAGER_H_
#define BOARD_MANAGER_H_

#include "config.h"
#include "game.h"

enum cursor {cursorX = 0, cursorY = 1};

void ClearScreen();
void DrawScreen(game_t* currentGameState);
void DrawBoard(player** cellOwner, int* boardSize, int* xOffset, bool** cellsEndangered);
void DrawBorder(int* boardSize, int* xOffset);

void DrawLegend();
void DrawPlayersTurn(bool currentPlayer);
void DrawPlayersPoints(float* playersPoints);
void WriteCursorPosition(int* cursorPosition, int* xOffset, int* boardSize);

void DrawBoardSize(int* boardSize);
void DrawWinner(float* playerPoints);
void DrawConfirmation();
void RemoveConfirmation();
void DrawAlert(const char* alert, int sleepTime, int* cursorPosition);

void CheckCharacterInput(int characterInput, game_t* currentGameState);

void MoveCursor(int characterInput, player** cellOwner, int* cursorPosition, int* boardSize, int* xOffset, bool** cellsEndangered);
void SetCursor(int* cursorPosition, int x, int y);
void SetCursorAbsolute(int* cursorPosition, int x, int y);
void ResetCursorPosition(player** cellOwner, bool** cellsEndangered, int* cursorPosition, int* boardSize, int* boardEvenOffset, int* xBoardOffset);

void ChangeCellColor(int color, player** cellOwner, bool** cellsEndangered, int* cursorPosition, int* boardSize, int* xBoardOffset);

void CheckOffsetX(player** cellOwner, int* cursorPosition, int* xOffset, int* boardSize, bool** cellsEndangered);

#endif
