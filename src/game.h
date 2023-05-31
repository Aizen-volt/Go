#pragma warning(disable : 5208)

#ifndef GAME_H_
#define GAME_H_

#include "config.h"
#include "stdio.h"

enum player { playerBlack = 0, playerWhite = 1, cellEmpty = 2 };

typedef struct {
	bool gameHasStarted = false;
	bool handicapActive = false;
	bool handicapOccured = false;
	bool currentPlayer;

	int xBoardOffset = 0; //changes board position when cursor goes beyond window
	int boardEvenOffset; //used for even number size boards management
	int previousKill[2]; //stores coordinates of last kill - for Ko rule
	float playerPoints[2];
	int cursorPosition[2];
	int boardSize = 19;
	bool** cellsEndangered; //stores information whether stone can be killed in the next move 
	player** cellOwner; //stores information about cell content
} game_t;

void InitGame_t(game_t* currentGameState);
void DeleteGame_t(int* boardSize, player** cellOwner, bool** cellsEndangered);

void StartGame(int boardSize);
void NewGame(int* boardSize);

void SaveGame(game_t* currentGameState);
void LoadGame(game_t* currentGameState);
void GetFileName(int* cursorPosition, char fileName[]);
bool FileOpened(FILE* file, int* cursorPosition);

void Handicap(bool* gameHasStarted, int* cursorPosition, bool* handicapActive, bool* handicapOccured);
void EndHandicap(bool* handicapActive, bool* currentPlayer, int* cursorPosition);

void FinishGame(game_t* currentGameState);
void ExitGame(int* boardSize, player** cellOwner, bool** cellsEndangered);

void ChangeBoardSize(int* boardSize, int* cursorPosition);
void GetBoardSize(int* cursorPosition, char boardSize[]);

void PlaceStone(game_t* currentGameState);
bool CheckStonePresent(player** cellOwner, int* cursorPosition, int* xBoardOffset);
bool CheckSuicide(game_t* currentGameState);
bool CheckKo(int* previousKill, int* cursorPosition, int* xBoardOffset);
bool WaitForConfirmation();

void CheckForKills(game_t* currentGameState);
void CheckLiberties(game_t* currentGameState, int x, int y);
int NumberOfLiberties(game_t* currentGameState, bool suicideCheck);
void CheckSide(player** cellOwner, int* cursorPosition, int* boardSize, bool* currentPlayer, bool** cellChecked, int* pLiberties, int xOffset, int yOffset, int* xBoardOffset);
void CheckEmptyCellsChain(player** cellOwner, int* cursorPosition, int* boardSize, bool** cellChecked, int* emptyCells, int xOffset, int yOffset, int* areaOwner, bool* twoPlayersOccured, int* xBoardOffset);

int NumberOfKills(game_t* currentGameState);
void CheckPossibleKill(game_t* currentGameState, int x, int y, int* pKillsPossible);
void Kill(game_t* currentGameState);

void MarkEndangered(player** cellOwner, bool** cellsEndangered, int* cursorPosition, bool* currentPlayer, int i, int j, int* xBoardOffset);

#endif