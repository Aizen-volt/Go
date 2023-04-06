# Go
# 
![Exemplary gameplay](exemplary-gameplay.png)
# Basics of the game of Go
Go is an abstract strategy board game for exactly two players. The goal of the game is to surround (“conquer”)
more territory than the opponent.
The rules of (a simplified version of) the game are as follows:
- 1. The game is played on a square boards (ja: goban), with 19 lines crossing the board horizontally and
19 lines crossing the board vertically (board 19x19 – 361 intersections). Sometimes, the board size is
13x13 or even 9x9.
- 2. Players put black and white stones on the intersections, one stone at the time. The player that plays
with the black ones is the one that starts first.
- 3. The goal is to surround the largest surface on the board using your own stones.
- 4. A stone that was put on the board stays there unless it is captured. The stone is captured when it is
surrounded from all four sides by either the stone stones of a opposing colour or the edge of the board.
The free sides are called liberties, e.g. a stone that is surrounded by two stones of a opposing colour
has two liberties.
- 5. The player is not allowed to put a suicide stone on the board (that is – a stone that after the placement
immediately does not have any liberties). The player can put a stone on a intersection with no liberties
if and only if after the placement one of the stones of a opposing colour are captured.
- 6. The Ko rule has to be kept – the stones on the board must never repeat a previous position of stones.
Moves which would do so are forbidden, and thus only moves elsewhere on the board are permitted
that turn.
These rules can be however elaborated further.
# General guidelines of the project
The program should be written with the usage of the provided template. The template provides the ability
to use advanced capabilities of a console in a Windows system. Usage of instructions cin/cout/printf/scanf
is forbidden. In order to write on a screen and read from a keyboard only methods provided in the template
should be used.
# Program controls
The program should be driven by a keyboard input with the following meaning assigned to keys:
• arrows: moving the cursor over the board;
• q: quit the program;
• n: start a new game;
• enter: confirm choice and end player’s turn;
• esc: cancel current action;
• i: place a stone on the board;
• s: save the game state;
• l: load the game state;
• f: finish the game.
# Requirements
- (a) Display a board (in one of the sizes: 9x9, 13x13, 19x19) and a legend of the program. The board should
have a border. The legend should contain name, surname and album number, list of the implemented
functionalities (in the form of a list of points from this instruction and a list of working keys) and a score
for both black and white players. By default the legend should be displayed on the left side of a screen,
while the board should be displayed on the right. There should be an ability to easily move these
elements with a change of respective constants in the code of the program.
- (b) Move through the board with arrows. The legend should contain the current position of the cursor. It
should be impossible for the cursor to leave the board and the current position should be visible on the
board. The q key should end the program.
- (c) New game. Pressing n key resets the player tiles and returns the board to the initial state.
- (d) Simple stone placement – pressing key i places a stone on the board at cursor position, as long as the
target cell is empty and and it is not an obvious suicide (the stone after placing has at least one liberty).
The game is always played by two players – after the black stone is placed, the white turn begins and
so on.
- (e) Simple capturing – the program should correctly detect the capture of a single stone, remove it from
the board and increment the score of the right player.
- (f) Saving and restoring game state. Pressing s key allows the user to enter the name of the file
in which the state of the game is stored. Pressing l key asks for a file name restores the state from
the given file (if such file exists). The order of stones drawn should also be preserved: when a game
is saved and then 3 stones are placed, when the game is reloaded and again 3 stones are drawn, the
have to be located at the same exact intersections.
- (g) Forcing the following of the rule of Ko. When the player try to make another move in a Ko fight,
after the first player did it during their round, the game should not allow to do so. If the game is saved
after the Ko fight was started, it also should keep the current state and don’t allow the second player
to make a Ko move after loading the game.
- (h) Changing the size of the board. The players should be able to choose every possible size of the
board, as long as it fits within the borders of the window. The option should first show three standard,
predefined options (9x9, 13x13 and 19x19) and then show the fourth option for choosing the custom
size.
- (i) Scrolling. The players should be able to choose every possible size of the board, even if it does
not fit within the borders of the window. This point should be implemented after the previous one.
- (j) Capturing. The program should correctly detect the capture of every possible chain of stones,
remove it from the board and increment the score of the right player.
- (k) Game state editor. The player should be allowed to enter every possiible initial configuration of
black stones before the start of the gameplay (to introduce a handicap).
- (l) Scoring. Pressing f key should finish the game and show the score, calculated according to
the rules described in Section 1.3. The komi must also be taken into consideration – when some initial
handicap is entered on the board, the value of the additional points that whites get must be different.
- (m) Detection of the finished game. The program should find out if any other move can still be made
without reducing the score of one or both players and stop the game, showing the score.
- (n) Atari. The program should mark the stones that have just one liberty (reminder: chains share
their liberties) and might be captured in the next move of the opponent
# Final remarks
Configuration of the program should make it possible to easily change all parameters, not only those
explicitly pointed in the above description. Easy change is understood as modification of a constant in
the program.
- The project can be written in an object oriented manner, however it is forbidden to use the standard C++
library (including the string type, cin, cout, vector etc.)(Attention: the string type from the C++ library
should not be confused with the string.h library in C – functions from string.h can be used).
- Handling of files should be implemented with the usage of the standard C library (family of functions
f???? - e.g. fopen, fread, fclose etc.). C++ mechanisms (e.g. fstream) can not be used to this end.
- Each fragment of code subjected to grading should be written by the student independently. Using a
code found on the Internet, received from other people or written with help from other people (excluding
help received during project consultations) is forbidden.
- Pay attention to a proper division of code into functions in order to avoid duplication of code. For
example it might occur, that in few given points a code which checks whether a given digit meets the
requirements of a given rule in a given field. In this case placing such a code inside a function, which
is next called in appropriate places, is natural and encouraged.
- Constant units in the program should be described by appropriate comments.
