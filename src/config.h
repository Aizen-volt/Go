#ifndef CONFIG_H_

#define CONFIG_H_
//includes all constants used in game

//NOT TO BE CHANGED

//codes for input/output management
#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77
#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_BACKSPACE 8
#define VERTICAL_LINE '\xC5'
#define HORIZONTAL_LINE '\xC4'
#define VERTICAL_DOUBLE_LINE '\xBA'
#define HORIZONTAL_DOUBLE_LINE '\xCD'
#define TOP_LEFT_CORNER '\xC9'
#define TOP_RIGHT_CORNER '\xBB'
#define BOTTOM_LEFT_CORNER '\xC8'
#define BOTTOM_RIGHT_CORNER '\xBC'
#define STONE 254

//some commands shortcuts to make it easier to manage board
#define BOARD_SIZE_X 2 * *boardSize
#define BOARD_INDEX_FROM_CURSOR_X (cursorPosition[cursorX] - BOARD_OFFSET_X + 3 - *xBoardOffset) / 2 - 1 
#define BOARD_INDEX_FROM_CURSOR_X2 (currentGameState->cursorPosition[cursorX] - BOARD_OFFSET_X + 3) / 2 - 1 - currentGameState->xBoardOffset
#define BOARD_INDEX_FROM_CURSOR_Y cursorPosition[cursorY] - BOARD_OFFSET_Y - 1
#define BOARD_INDEX_FROM_CURSOR_Y2 currentGameState->cursorPosition[cursorY] - BOARD_OFFSET_Y - 1

//END OF NOT TO BE CHANGED

//CONFIGURABLE

//color of console window background
#define BACKGROUND_COLOR BLACK

#define BOARD_COLOR LIGHTGRAY

//color of vertical/horizontal lines on the board
#define TEXT_COLOR BLACK

//color of the cell that cursor points at at the moment
#define CELL_HIGHLIGHTED_COLOR DARKGRAY

#define BORDER_COLOR DARKGRAY

//time in ms after which an alert disappears
#define ALERT_TIME 2000

//size of board that is being drawn when the app is opened - can be overwritten by the user at anytime using ChangeBoardSize mechanism
#define DEFAULT_BOARD_SIZE 19

//amount of points player white gets at the end of the game since they're one move behind
#define PLAYER_WHITE_POINTS_BONUS 6.5

//values below CANNOT repeat - it'll cause the legend to display incorrectly
#define LEGEND_AUTHOR_POSITION 0
#define LEGEND_TASKS_POSITION 1
#define LEGEND_ARROWS_POSITION 2
#define LEGEND_EXIT_POSITION 3
#define LEGEND_START_POSITION 4
#define LEGEND_CONFIRM_POSITION 5
#define LEGEND_CANCEL_POSITION 6
#define LEGEND_PLACE_POSITION 7
#define LEGEND_SAVE_POSITION 8
#define LEGEND_LOAD_POSITION 9
#define LEGEND_FINISH_POSITION 10
#define LEGEND_BOARDCHANGE_POSITION 11
#define LEGEND_HANDICAP_POSITION 12
#define LEGEND_BOARDSIZE_POSITION 13
#define LEGEND_TURN_POSITION 14
#define LEGEND_POINTS_B_POSITION 15
#define LEGEND_POINTS_W_POSITION 16
#define LEGEND_ALERT_POSITION 18
#define LEGEND_CURSOR_POSITION 21
#define LEGEND_FILE_NAME 22

//choosing values that are too close to each other may cause overlapping of elements
#define LEGEND_OFFSET_X 4
#define LEGEND_OFFSET_Y 3
#define BOARD_OFFSET_X 55
#define BOARD_OFFSET_Y 2

//END OF CONFIGURABLE
#endif
