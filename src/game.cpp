#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "board_manager.h"
#include "conio2.h"

void InitGame_t(game_t* currentGameState) {
	//when memory allocation fails, we quit the program, otherwise we set all values to 0

	currentGameState->cellOwner = (player**)malloc(currentGameState->boardSize * sizeof(int*));
	currentGameState->cellsEndangered = (bool**)malloc(currentGameState->boardSize * sizeof(bool*));

	if (currentGameState->cellOwner != NULL && currentGameState->cellsEndangered != NULL) {
		for (int i = 0; i < currentGameState->boardSize; i++) {
			currentGameState->cellOwner[i] = (player*)malloc(currentGameState->boardSize * sizeof(int));
			currentGameState->cellsEndangered[i] = (bool*)malloc(currentGameState->boardSize * sizeof(bool));
			if (currentGameState->cellOwner[i] == NULL || currentGameState->cellsEndangered[i] == NULL)
				exit(1);
		}

		for (int i = 0; i < currentGameState->boardSize; i++) {
			for (int j = 0; j < currentGameState->boardSize; j++) {
				currentGameState->cellOwner[i][j] = cellEmpty;
				currentGameState->cellsEndangered[i][j] = false;
			}
		}
		currentGameState->playerPoints[playerBlack] = 0;
		currentGameState->playerPoints[playerWhite] = 0;
		currentGameState->currentPlayer = playerBlack;

		//if boardSize is even, we need to display some things 1 cell to the left, otherwise display is unclear
		currentGameState->boardEvenOffset = (currentGameState->boardSize % 2 == 0 ? 1 : 0);
	}
	else
		exit(1);
}



void DeleteGame_t(int* boardSize, player** cellOwner, bool** cellsEndangered) {

	for (int i = 0; i < *boardSize; i++) {
		free(cellOwner[i]);

	}
	free(cellOwner);
	free(cellsEndangered);
}



void StartGame(int boardSize) {
	//initializing game state struct
	//board has default size assigned when application is opened
	//changing board size triggers this function once again with different board size argument

	game_t game;
	game_t* currentGameState = &game;
	game.boardSize = boardSize;
	InitGame_t(currentGameState);

	//stores code of key input from players
	char characterInput;

	ClearScreen();
	DrawScreen(currentGameState);

	//when board is ready, get input from players as long as they want to
	while (true) {
		characterInput = getch();
		CheckCharacterInput(characterInput, currentGameState);
	}
}



void NewGame(int* boardSize) {

	textbackground(BLACK);
	textcolor(WHITE);
	StartGame(*boardSize);
}



void SaveGame(game_t* currentGameState) {

	//get filename from user and open it
	char fileName[50];
	GetFileName(currentGameState->cursorPosition, fileName);

	FILE* file = fopen(fileName, "w");
	if (!FileOpened(file, currentGameState->cursorPosition))
		return;

	//save all the information about game in the file
	fprintf(file, "%d\n", currentGameState->handicapOccured);

	fprintf(file, "%d\n", currentGameState->boardSize);

	for (int i = 0; i < currentGameState->boardSize; i++) {
		for (int j = 0; j < currentGameState->boardSize; j++) {
			fprintf(file, "%d ", currentGameState->cellOwner[i][j]);
			fprintf(file, "%d ", currentGameState->cellsEndangered[i][j]);
		}
	}

	fprintf(file, "%d ", currentGameState->previousKill[0]);
	fprintf(file, "%d ", currentGameState->previousKill[1]);

	fprintf(file, "%f\n", currentGameState->playerPoints[playerBlack]);
	fprintf(file, "%f\n", currentGameState->playerPoints[playerWhite]);

	fprintf(file, "%d\n", currentGameState->currentPlayer);

	//when all the info has been saved, close the file and draw confirmation
	fclose(file);

	DrawAlert("Game saved correctly!", ALERT_TIME, currentGameState->cursorPosition);
	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FILE_NAME);
	clreol();
}



void LoadGame(game_t* currentGameState) {

	//get filename from user and open it
	char fileName[50];
	GetFileName(currentGameState->cursorPosition, fileName);

	FILE* file = fopen(fileName, "r");
	if (!FileOpened(file, currentGameState->cursorPosition))
		return;

	//read all the information about game from the file
	fscanf(file, "%hd", &currentGameState->handicapOccured);

	fscanf(file, "%d", &currentGameState->boardSize);

	for (int i = 0; i < currentGameState->boardSize; i++) {
		for (int j = 0; j < currentGameState->boardSize; j++) {
			fscanf(file, "%hd", &currentGameState->cellOwner[i][j]);
			fscanf(file, "%hd", &currentGameState->cellsEndangered[i][j]);
		}
	}

	fscanf(file, "%hd", &currentGameState->previousKill[0]);
	fscanf(file, "%hd", &currentGameState->previousKill[1]);

	fscanf(file, "%f", &currentGameState->playerPoints[playerBlack]);
	fscanf(file, "%f", &currentGameState->playerPoints[playerWhite]);

	fscanf(file, "%hd", &currentGameState->currentPlayer);

	//when all the info was read, close the file, draw confirmation and draw screen with new content
	fclose(file);

	DrawAlert("Game loaded correctly!", ALERT_TIME, currentGameState->cursorPosition);

	ClearScreen();
	DrawScreen(currentGameState);
}



void GetFileName(int* cursorPosition, char fileName[]) {

	char input;
	int i = 0;

	textbackground(BLACK);
	textcolor(WHITE);
	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FILE_NAME);
	cputs("Enter file name: ");

	//get file name char after char until player presses enter or reaches fileName size
	//if backspace is pressed, remove last char from both array and screen
	do {
		input = getche();

		if (input == KEY_BACKSPACE && i > 0) {
			fileName[--i] = ' ';

			gotoxy(wherex(), wherey());
			putch(' ');
			gotoxy(wherex() - 1, wherey());
		}
		else {
			fileName[i] = input;
			i++;
		}
	} while (input != KEY_ENTER && i < 49);

	//end array with \0
	fileName[i - 1] = '\0';

	gotoxy(cursorPosition[cursorX], cursorPosition[cursorY]);
}



bool FileOpened(FILE* file, int* cursorPosition) {
	//in case file cannot be found/opened

	if (file == NULL) {
		DrawAlert("Could not open the file!", ALERT_TIME, cursorPosition);
		gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FILE_NAME);
		clreol();
		return false;
	}

	return true;
}



void Handicap(bool* gameHasStarted, int* cursorPosition, bool* handicapActive, bool* handicapOccured) {

	//handicap mode is unavailable when the games has started
	if (*gameHasStarted) {
		DrawAlert("Handicap not available, game has started!", ALERT_TIME, cursorPosition);
		return;
	}

	textbackground(BLACK);
	textcolor(WHITE);

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_HANDICAP_POSITION);
	cputs("h      - end handicap mode");

	textbackground(BOARD_COLOR);
	textcolor(TEXT_COLOR);

	gotoxy(cursorPosition[cursorX], cursorPosition[cursorY]);

	*handicapActive = true;
	*handicapOccured = true;
}



void EndHandicap(bool* handicapActive, bool* currentPlayer, int* cursorPosition) {

	textbackground(BLACK);
	textcolor(WHITE);

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_HANDICAP_POSITION);
	cputs("h      - handicap mode");

	textbackground(BOARD_COLOR);
	textcolor(TEXT_COLOR);

	*handicapActive = false;
	//now it's white's turn
	*currentPlayer = !(*currentPlayer);

	DrawPlayersTurn(*currentPlayer);
	gotoxy(cursorPosition[cursorX], cursorPosition[cursorY]);
}



void FinishGame(game_t* currentGameState) {

	//we create an array that stores information about whether each cell has already been checked
	bool** cellChecked = (bool**)malloc(currentGameState->boardSize * sizeof(bool*));

	if (cellChecked != NULL) {
		for (int i = 0; i < currentGameState->boardSize; i++) {
			cellChecked[i] = (bool*)malloc(currentGameState->boardSize * sizeof(bool));

			if (cellChecked[i] == NULL)
				exit(1);
		}

		for (int i = 0; i < currentGameState->boardSize; i++) {
			for (int j = 0; j < currentGameState->boardSize; j++)
				cellChecked[i][j] = false;
		}
	}
	else
		exit(1);

	for (int i = 0; i < currentGameState->boardSize; i++) {
		for (int j = 0; j < currentGameState->boardSize; j++) {
			if (!cellChecked[i][j]) {
				int emptyCells = 0, areaOwner = cellEmpty;
				bool twoPlayersOccured = false;

				//we set cursor position to 1x1 and start checking
				SetCursorAbsolute(currentGameState->cursorPosition, BOARD_OFFSET_X - 1, BOARD_OFFSET_Y + 1);

				CheckEmptyCellsChain(currentGameState->cellOwner, currentGameState->cursorPosition, &currentGameState->boardSize, cellChecked, &emptyCells, i, j, &areaOwner, &twoPlayersOccured, &currentGameState->xBoardOffset);

				//we won't add checked area if it's surrounded by both players
				if (!twoPlayersOccured)
					currentGameState->playerPoints[areaOwner] += emptyCells;
			}
		}
	}

	//we add bonus points to white player since he starts as second
	//moreover if handicap occured and game ends with draw, we break it with advantage to the white player
	currentGameState->playerPoints[playerWhite] += PLAYER_WHITE_POINTS_BONUS;

	if (currentGameState->playerPoints[playerWhite] == currentGameState->playerPoints[playerBlack] &&
		currentGameState->handicapOccured)
		currentGameState->playerPoints[playerWhite] += 0.5;

	//when counting is done, display points, winner, free memory and exit program
	DrawPlayersPoints(currentGameState->playerPoints);
	DrawWinner(currentGameState->playerPoints);

	DeleteGame_t(&currentGameState->boardSize, currentGameState->cellOwner, currentGameState->cellsEndangered);

	textcolor(WHITE);
	exit(0);
}



void ExitGame(int* boardSize, player** cellOwner, bool** cellsEndangered) {
	//free all allocated memory before exiting game

	ClearScreen();
	DeleteGame_t(boardSize, cellOwner, cellsEndangered);
	exit(0);
}



void ChangeBoardSize(int* boardSize, int* cursorPosition) {

	char boardSizeChar[7];
	int temp; //stores board size converted from char array to int

	//we get board size from the user
	GetBoardSize(cursorPosition, boardSizeChar);

	//and convert it to int
	sscanf(boardSizeChar, "%d", &temp);

	//if size is invalid we cancel
	if (temp <= 2) {
		DrawAlert("Wrong board size!", ALERT_TIME, cursorPosition);

		gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FILE_NAME);
		cputs("                                 ");

		gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FILE_NAME + 1);
		cputs("                                 ");

		gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FILE_NAME + 2);
		cputs("                                 ");

		gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FILE_NAME + 3);
		cputs("                                 ");

		gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FILE_NAME + 4);
		cputs("                                 ");

		gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FILE_NAME + 5);
		cputs("                                 ");

		return;
	}

	//if not, we start new game with given size
	StartGame(temp);
}



void GetBoardSize(int* cursorPosition, char boardSize[]) {

	//draw possible sizes
	textbackground(BLACK);
	textcolor(WHITE);

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FILE_NAME);
	cputs("Select board size:");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FILE_NAME + 1);
	cputs("1) 9x9");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FILE_NAME + 2);
	cputs("2) 13x13");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FILE_NAME + 3);
	cputs("3) 19x19");

	gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FILE_NAME + 4);
	cputs("4) custom");

	char input;
	int i = 0;

	input = getch();

	switch (input)
	{
	case '1':
		boardSize[0] = '9';
		break;
	case '2':
		boardSize[0] = '1';
		boardSize[1] = '3';
		break;
	case '3':
		boardSize[0] = '1';
		boardSize[1] = '9';
		break;
	case '4':
		//custom size input looks like filename input
		gotoxy(LEGEND_OFFSET_X, LEGEND_OFFSET_Y + LEGEND_FILE_NAME + 5);
		cputs("Enter board size (2 digits max): ");

		do {
			input = getche();

			if (input == KEY_BACKSPACE && i > 0) {
				boardSize[--i] = ' ';

				gotoxy(wherex(), wherey());
				putch(' ');

				gotoxy(wherex() - 1, wherey());
			}
			else {
				boardSize[i] = input;
				i++;
			}
			if (i > 6)
				break;
		} while (input != KEY_ENTER);

		//adding \0 at the end of array
		boardSize[i - 1] = '\0';
		break;
	default:
		break;
	}

	gotoxy(cursorPosition[cursorX], cursorPosition[cursorY]);
}



void PlaceStone(game_t* currentGameState) {
	//stone can't be placed if any of these occur

	if (CheckStonePresent(currentGameState->cellOwner, currentGameState->cursorPosition, &currentGameState->xBoardOffset))
		return;
	if (CheckSuicide(currentGameState))
		return;
	if (CheckKo(currentGameState->previousKill, currentGameState->cursorPosition, &currentGameState->xBoardOffset))
		return;

	//if stone can be placed, we put it temporarily on the board, so the player can confirm their choice
	textbackground(BOARD_COLOR);
	currentGameState->currentPlayer == playerBlack ? textcolor(BLACK) : textcolor(WHITE);
	putch(STONE);

	if (WaitForConfirmation()) {
		//some mechanics require the game to not have started so when the stone is placed we disable them
		currentGameState->gameHasStarted = true;

		//we assign current cell to the player who placed the stone in it
		currentGameState->cellOwner[BOARD_INDEX_FROM_CURSOR_X2][BOARD_INDEX_FROM_CURSOR_Y2] = (player)currentGameState->currentPlayer;

		gotoxy(currentGameState->cursorPosition[cursorX], currentGameState->cursorPosition[cursorY]);

		//stone has been placed, so we reset previousKill coordinates to enable placing stone in the Ko-prohibited cell
		currentGameState->previousKill[cursorX] = -1;
		currentGameState->previousKill[cursorY] = -1;

		//we check if stone placed has closed any enemy's stones chain
		CheckForKills(currentGameState);

		//if game is in handicap mode we want the black player to continue placing stones, otherwise players switch their turns
		if (!currentGameState->handicapActive)
			currentGameState->currentPlayer = !currentGameState->currentPlayer;

		DrawPlayersTurn(currentGameState->currentPlayer);
		ResetCursorPosition(currentGameState->cellOwner, currentGameState->cellsEndangered, currentGameState->cursorPosition, &currentGameState->boardSize, &currentGameState->boardEvenOffset, &currentGameState->xBoardOffset);
	}
	else {
		//player canceled their choice so we remove temporar stone from the board
		gotoxy(currentGameState->cursorPosition[cursorX], currentGameState->cursorPosition[cursorY]);
		putch(VERTICAL_LINE);
		gotoxy(currentGameState->cursorPosition[cursorX], currentGameState->cursorPosition[cursorY]);
	}
}



bool CheckStonePresent(player** cellOwner, int* cursorPosition, int* xBoardOffset) {
	//player can't put the stone in the cell that already contains stone

	if (cellOwner[BOARD_INDEX_FROM_CURSOR_X][BOARD_INDEX_FROM_CURSOR_Y] != cellEmpty) {
		DrawAlert("Cannot put a stone! Stone already there", ALERT_TIME, cursorPosition);
		return true;
	}

	return false;
}



bool CheckSuicide(game_t* currentGameState) {
	//player can't put the stone in the cell where it'd be immediately killed
	//unless they can kill having put the stone there

	//we temporarily assign the cell as current player's to simulate kill
	currentGameState->cellOwner[BOARD_INDEX_FROM_CURSOR_X2][BOARD_INDEX_FROM_CURSOR_Y2] = (player)currentGameState->currentPlayer;
	currentGameState->currentPlayer = !currentGameState->currentPlayer;

	if (NumberOfLiberties(currentGameState, true) == 0) {
		currentGameState->currentPlayer = !currentGameState->currentPlayer;

		//stone would be killed so we check if it can kill
		if (NumberOfKills(currentGameState) == 0) {
			currentGameState->cellOwner[BOARD_INDEX_FROM_CURSOR_X2][BOARD_INDEX_FROM_CURSOR_Y2] = cellEmpty;

			DrawAlert("That'd be an obvious suicide!", ALERT_TIME, currentGameState->cursorPosition);
			return true;
		}

		//stone can kill, it can be placed
		currentGameState->cellOwner[BOARD_INDEX_FROM_CURSOR_X2][BOARD_INDEX_FROM_CURSOR_Y2] = cellEmpty;
		return false;
	}

	//if player has liberties we cancel temporar assignments and proceed
	currentGameState->cellOwner[BOARD_INDEX_FROM_CURSOR_X2][BOARD_INDEX_FROM_CURSOR_Y2] = cellEmpty;
	currentGameState->currentPlayer = !currentGameState->currentPlayer;

	return false;
}



bool CheckKo(int* previousKill, int* cursorPosition, int* xBoardOffset) {
	//player can't put the stone in the cell where they've just been killed

	if (previousKill[cursorX] == BOARD_INDEX_FROM_CURSOR_X &&
		previousKill[cursorY] == BOARD_INDEX_FROM_CURSOR_Y) {
		DrawAlert("Cannot place the stone due to the Ko Rule!", ALERT_TIME, cursorPosition);
		return true;
	}

	return false;
}



bool WaitForConfirmation() {
	//Draw confirmation query, then wait until players confirms or cancels their choice

	char characterInput;

	DrawConfirmation();

	while (true) {
		characterInput = getch();

		switch (characterInput) {
		case KEY_ENTER:
			RemoveConfirmation();
			return true;
			break;
		case KEY_ESC:
			RemoveConfirmation();
			return false;
			break;
		default:
			break;
		}
	}
}



void CheckForKills(game_t* currentGameState) {

	//check liberties of potential stone chains surrounding placed stone
	CheckLiberties(currentGameState, -2, 0);
	CheckLiberties(currentGameState, 2, 0);
	CheckLiberties(currentGameState, 0, -1);
	CheckLiberties(currentGameState, 0, 1);
}



void CheckLiberties(game_t* currentGameState, int x, int y) {

	SetCursor(currentGameState->cursorPosition, x, y);

	//if current cursor position exceeds board size we go back and return
	if (BOARD_INDEX_FROM_CURSOR_X2 < 0 ||
		BOARD_INDEX_FROM_CURSOR_X2 >= currentGameState->boardSize ||
		BOARD_INDEX_FROM_CURSOR_Y2 < 0 ||
		BOARD_INDEX_FROM_CURSOR_Y2 >= currentGameState->boardSize) {
		SetCursor(currentGameState->cursorPosition, -x, -y);
		return;
	}

	//if checked cell isn't empty we start checking liberties
	if (currentGameState->cellOwner[BOARD_INDEX_FROM_CURSOR_X2][BOARD_INDEX_FROM_CURSOR_Y2] != cellEmpty)
		NumberOfLiberties(currentGameState, false); //argument "false" indicates that we will potentially kill the chain

	//having finished, go back to the previous cursor position
	SetCursor(currentGameState->cursorPosition, -x, -y);
}



int NumberOfLiberties(game_t* currentGameState, bool suicideCheck) {

	int liberties = 0;

	//we create an array that stores information about whether each cell has already been checked 
	bool** cellChecked = (bool**)malloc(currentGameState->boardSize * sizeof(bool*));

	if (cellChecked != NULL) {
		for (int i = 0; i < currentGameState->boardSize; i++) {
			cellChecked[i] = (bool*)malloc(currentGameState->boardSize * sizeof(bool));

			if (cellChecked[i] == NULL)
				exit(1);
		}

		for (int i = 0; i < currentGameState->boardSize; i++) {
			for (int j = 0; j < currentGameState->boardSize; j++)
				cellChecked[i][j] = false;
		}
	}
	else
		exit(1);

	CheckSide(currentGameState->cellOwner, currentGameState->cursorPosition, &currentGameState->boardSize, &currentGameState->currentPlayer, cellChecked, &liberties, 0, 0, &currentGameState->xBoardOffset);

	//if chain is surrounded, we kill each stone in it
	if (!suicideCheck && liberties == 0) {
		for (int i = 0; i < currentGameState->boardSize; i++) {
			for (int j = 0; j < currentGameState->boardSize; j++) {
				if (cellChecked[i][j] == true) {
					SetCursorAbsolute(currentGameState->cursorPosition, BOARD_OFFSET_X - 1 + 2 * i, BOARD_OFFSET_Y + 1 + j);

					Kill(currentGameState);
				}
			}
		}
	}

	//if there's only 1 liberty left, we highlight the chain as endangered
	else if (!suicideCheck && liberties == 1) {
		for (int i = 0; i < currentGameState->boardSize; i++) {
			for (int j = 0; j < currentGameState->boardSize; j++) {
				if (cellChecked[i][j] == true)
					MarkEndangered(currentGameState->cellOwner, currentGameState->cellsEndangered, currentGameState->cursorPosition, &currentGameState->currentPlayer, i, j, &currentGameState->xBoardOffset);
			}
		}
	}

	//if chain gets new liberty, we turn of highlighting
	else if (!suicideCheck && liberties > 1) {
		for (int i = 0; i < currentGameState->boardSize; i++) {
			for (int j = 0; j < currentGameState->boardSize; j++) {
				if (cellChecked[i][j] == true && currentGameState->cellsEndangered[i][j])
					MarkEndangered(currentGameState->cellOwner, currentGameState->cellsEndangered, currentGameState->cursorPosition, &currentGameState->currentPlayer, i, j, &currentGameState->xBoardOffset);
			}
		}
	}

	//we free the allocated memory
	for (int i = 0; i < currentGameState->boardSize; i++)
		free(cellChecked[i]);
	free(cellChecked);

	return liberties;
}



void CheckSide(player** cellOwner, int* cursorPosition, int* boardSize, bool* currentPlayer, bool** cellChecked, int* pLiberties, int xOffset, int yOffset, int* xBoardOffset) {

	//we don't want to exceed board size
	if (BOARD_INDEX_FROM_CURSOR_X + xOffset < 0 ||
		BOARD_INDEX_FROM_CURSOR_X + xOffset >= *boardSize ||
		BOARD_INDEX_FROM_CURSOR_Y + yOffset < 0 ||
		BOARD_INDEX_FROM_CURSOR_Y + yOffset >= *boardSize)
		return;

	//if checked cell is empty, we add liberty for the chain
	if (cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset] == cellEmpty) {
		(*pLiberties)++;
		return;
	}

	//we don't want to check cell that has already been checked
	if (cellChecked[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset] == true)
		return;
	cellChecked[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset] = true;

	//we check surrounding cells that don't exceed board size
	//if they contain stone belonging to the chain, we repeat the whole procedure from their perspective

	//right
	if (!(BOARD_INDEX_FROM_CURSOR_X + xOffset + 1 >= *boardSize)) {
		if (cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset + 1][BOARD_INDEX_FROM_CURSOR_Y + yOffset] != *currentPlayer)
			CheckSide(cellOwner, cursorPosition, boardSize, currentPlayer, cellChecked, pLiberties, xOffset + 1, yOffset, xBoardOffset);
	}

	//left
	if (!((BOARD_INDEX_FROM_CURSOR_X + xOffset - 1 < 0))) {
		if (cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset - 1][BOARD_INDEX_FROM_CURSOR_Y + yOffset] != *currentPlayer)
			CheckSide(cellOwner, cursorPosition, boardSize, currentPlayer, cellChecked, pLiberties, xOffset - 1, yOffset, xBoardOffset);
	}

	//down
	if (!(BOARD_INDEX_FROM_CURSOR_Y + yOffset + 1 >= *boardSize)) {
		if (cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset + 1] != *currentPlayer)
			CheckSide(cellOwner, cursorPosition, boardSize, currentPlayer, cellChecked, pLiberties, xOffset, yOffset + 1, xBoardOffset);
	}

	//up
	if (!(BOARD_INDEX_FROM_CURSOR_Y + yOffset - 1 < 0)) {
		if (cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset - 1] != *currentPlayer)
			CheckSide(cellOwner, cursorPosition, boardSize, currentPlayer, cellChecked, pLiberties, xOffset, yOffset - 1, xBoardOffset);
	}
}



void CheckEmptyCellsChain(player** cellOwner, int* cursorPosition, int* boardSize, bool** cellChecked, int* emptyCells, int xOffset, int yOffset, int* areaOwner, bool* twoPlayersOccured, int* xBoardOffset) {

	//we don't want to check cells that have already been checked
	if (cellChecked[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset])
		return;
	cellChecked[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset] = true;


	//if owner of current area hasn't been settled yet and we find non-empty cell, we make that player owner of the whole area
	if (*areaOwner == cellEmpty) {
		if (cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset] != cellEmpty)
			*areaOwner = cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset];

		//we might find stone in one of surrounding cells
		else {
			if (BOARD_INDEX_FROM_CURSOR_X + xOffset + 1 < *boardSize && cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset + 1][BOARD_INDEX_FROM_CURSOR_Y + yOffset] != cellEmpty)
				*areaOwner = cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset + 1][BOARD_INDEX_FROM_CURSOR_Y + yOffset];

			else if (BOARD_INDEX_FROM_CURSOR_X + xOffset - 1 >= 0 && cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset - 1][BOARD_INDEX_FROM_CURSOR_Y + yOffset] != cellEmpty)
				*areaOwner = cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset - 1][BOARD_INDEX_FROM_CURSOR_Y + yOffset];

			else if (BOARD_INDEX_FROM_CURSOR_Y + yOffset + 1 < *boardSize && cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset + 1] != cellEmpty)
				*areaOwner = cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset + 1];

			else if (BOARD_INDEX_FROM_CURSOR_Y + yOffset - 1 >= 0 && cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset - 1] != cellEmpty)
				*areaOwner = cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset - 1];
		}
	}

	//if area has an owner we check if surrounding cells contain stone of the opposite player
	else if (
		(BOARD_INDEX_FROM_CURSOR_X + xOffset + 1 < *boardSize && cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset + 1][BOARD_INDEX_FROM_CURSOR_Y + yOffset] != *areaOwner && cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset + 1][BOARD_INDEX_FROM_CURSOR_Y + yOffset] != cellEmpty) ||
		(BOARD_INDEX_FROM_CURSOR_X + xOffset - 1 >= 0 && cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset - 1][BOARD_INDEX_FROM_CURSOR_Y + yOffset] != *areaOwner && cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset - 1][BOARD_INDEX_FROM_CURSOR_Y + yOffset] != cellEmpty) ||
		(BOARD_INDEX_FROM_CURSOR_Y + yOffset + 1 < *boardSize && cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset + 1] != *areaOwner && cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset + 1] != cellEmpty) ||
		(BOARD_INDEX_FROM_CURSOR_Y + yOffset - 1 >= 0 && cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset - 1] != *areaOwner && cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset - 1] != cellEmpty)
		) {
		*twoPlayersOccured = true;
	}

	//if current cell is empty we add it to the potential points gain
	if (cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset] == cellEmpty)
		(*emptyCells)++;

	//then, if we don't exceed board size and meet another empty cell, we repeat the whole procedure from its perspective

	//right
	if (BOARD_INDEX_FROM_CURSOR_X + xOffset + 1 < *boardSize) {
		if (cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset + 1][BOARD_INDEX_FROM_CURSOR_Y + yOffset] == cellEmpty)
			CheckEmptyCellsChain(cellOwner, cursorPosition, boardSize, cellChecked, emptyCells, xOffset + 1, yOffset, areaOwner, twoPlayersOccured, xBoardOffset);
	}

	//left
	if (BOARD_INDEX_FROM_CURSOR_X + xOffset - 1 >= 0) {
		if (cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset - 1][BOARD_INDEX_FROM_CURSOR_Y + yOffset] == cellEmpty)
			CheckEmptyCellsChain(cellOwner, cursorPosition, boardSize, cellChecked, emptyCells, xOffset - 1, yOffset, areaOwner, twoPlayersOccured, xBoardOffset);
	}

	//down
	if (BOARD_INDEX_FROM_CURSOR_Y + yOffset + 1 < *boardSize) {
		if (cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset + 1] == cellEmpty)
			CheckEmptyCellsChain(cellOwner, cursorPosition, boardSize, cellChecked, emptyCells, xOffset, yOffset + 1, areaOwner, twoPlayersOccured, xBoardOffset);
	}

	//up
	if (BOARD_INDEX_FROM_CURSOR_Y + yOffset - 1 >= 0) {
		if (cellOwner[BOARD_INDEX_FROM_CURSOR_X + xOffset][BOARD_INDEX_FROM_CURSOR_Y + yOffset - 1] == cellEmpty)
			CheckEmptyCellsChain(cellOwner, cursorPosition, boardSize, cellChecked, emptyCells, xOffset, yOffset - 1, areaOwner, twoPlayersOccured, xBoardOffset);
	}

}



int NumberOfKills(game_t* currentGameState) {
	int killsPossible = 0;

	CheckPossibleKill(currentGameState, -2, 0, &killsPossible);
	CheckPossibleKill(currentGameState, 2, 0, &killsPossible);
	CheckPossibleKill(currentGameState, 0, -1, &killsPossible);
	CheckPossibleKill(currentGameState, 0, 1, &killsPossible);

	return killsPossible;
}



void CheckPossibleKill(game_t* currentGameState, int x, int y, int* pKillsPossible) {
	SetCursor(currentGameState->cursorPosition, x, y);

	//if current cursor position exceeds board size we go back and return
	if (BOARD_INDEX_FROM_CURSOR_X2 < 0 ||
		BOARD_INDEX_FROM_CURSOR_X2 >= currentGameState->boardSize ||
		BOARD_INDEX_FROM_CURSOR_Y2 < 0 ||
		BOARD_INDEX_FROM_CURSOR_Y2 >= currentGameState->boardSize) {
		SetCursor(currentGameState->cursorPosition, -x, -y);
		return;
	}

	//if checked cell isn't empty we start checking liberties
	if (currentGameState->cellOwner[BOARD_INDEX_FROM_CURSOR_X2][BOARD_INDEX_FROM_CURSOR_Y2] != cellEmpty &&
		NumberOfLiberties(currentGameState, true) == 0) {
		(*pKillsPossible)++;
	}

	//having finished, go back to the previous cursor position
	SetCursor(currentGameState->cursorPosition, -x, -y);
}



void Kill(game_t* currentGameState) {

	//we don't want player to kill their own stones
	if (currentGameState->cellOwner[BOARD_INDEX_FROM_CURSOR_X2][BOARD_INDEX_FROM_CURSOR_Y2] == currentGameState->currentPlayer)
		return;

	//we mark the cell so that Ko rule isn't broken
	currentGameState->previousKill[0] = BOARD_INDEX_FROM_CURSOR_X2;
	currentGameState->previousKill[1] = BOARD_INDEX_FROM_CURSOR_Y2;

	//we empty the cell
	currentGameState->cellOwner[BOARD_INDEX_FROM_CURSOR_X2][BOARD_INDEX_FROM_CURSOR_Y2] = cellEmpty;
	currentGameState->cellsEndangered[BOARD_INDEX_FROM_CURSOR_X2][BOARD_INDEX_FROM_CURSOR_Y2] = false;

	//and increase killer's points
	currentGameState->playerPoints[currentGameState->currentPlayer]++;

	textbackground(BOARD_COLOR);
	textcolor(TEXT_COLOR);
	gotoxy(currentGameState->cursorPosition[cursorX], currentGameState->cursorPosition[cursorY]);
	putch(VERTICAL_LINE);

	textbackground(CELL_HIGHLIGHTED_COLOR);

	DrawPlayersPoints(currentGameState->playerPoints);
}



void MarkEndangered(player** cellOwner, bool** cellsEndangered, int* cursorPosition, bool* currentPlayer, int i, int j, int* xBoardOffset) {

	//we need to match highlighted stone color with color of stone before highlighting
	bool stoneColor = (cellOwner[i][j] == *currentPlayer ? *currentPlayer : !*currentPlayer);

	cellsEndangered[i][j] = !cellsEndangered[i][j];

	gotoxy(BOARD_OFFSET_X - 1 + 2 * i, BOARD_OFFSET_Y + 1 + j);
	textbackground(cellsEndangered[i][j] ? RED : BOARD_COLOR);
	textcolor(stoneColor == playerWhite ? WHITE : BLACK);

	putch(STONE);

	textbackground(BOARD_COLOR);
	textcolor(TEXT_COLOR);
}