# Connect 4 Game

This Connect 4 implementation was developed on the macOS platform.  
The project is built upon the starter code and framework provided by the UCSC CMPM 123 course.  

A new `Connect4.cpp` class was created inheriting from `game.cpp`, following the structure used in `TicTacToe.cpp`. The game is integrated into the main menu as a fourth selectable option alongside:

---

## Game Modes and Features

### Game Mode Selection

Connect 4 now supports two play modes:

- **vs Player** – Two players (Red and Yellow) take turns dropping pieces.
- **vs AI** – A human player competes against an AI opponent.

Players are displayed as **Red** and **Yellow** instead of Player 0 and Player 1.

---

## Animation (Extra Credit Feature)

When a piece is placed:

- The piece animates downward into position.
- Instead of instantly appearing in its final slot, it visually “falls” into place.
- This improves visual feedback and user experience.

---

## Additional Controls

- **Restart Button** – Can be clicked at any time to reset the current match.
- **Exit Button** – Returns the user to the main game selection menu.
- After a win, input is disabled until Restart or Exit is selected.

---

## Building

Build using CMake:

```bash
cd build
cmake ..
make
