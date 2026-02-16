# Tic-Tac-Toe Game

This logger implementation was developed on the macOS platform. The entire project is built upon the starter code and materials provided by UCSC CMPM 123 course.

## Game Modes and AI Design

In this assignment, the original AI mode from the previous version was simplified into a **Simple Mode**, and two more advanced AI difficulty levels were added: **Medium Mode** and **Hard Mode**.

The goal of this redesign was to clearly separate different levels of decision-making complexity and to demonstrate the progression of game AI techniques taught in class.

### Simple Mode (Random)
Simple Mode does not use any game-tree search or evaluation strategy.  
The AI selects moves Random, serving as a baseline behavior and an easy opponent for players.

### Medium Mode (Negamax Algorithm)
Medium Mode uses the Negamax algorithm to evaluate possible game states.
Capable of draw (or win, if human player is not optimizing); this should be true if algorithm is implemented correctly.
This mode demonstrates full game-tree traversal and optimal play for a small search space like Tic-Tac-Toe.

### Hard Mode (Alpha-Beta Pruning)
Branches that cannot influence the final decision are pruned early. This significantly reduces the number of nodes evaluated without changing the final decision.
Although Tic-Tac-Toe has a small state space, this mode demonstrates how pruning improves efficiency and scalability for larger games.


## Building

Build using CMake:

```bash
cd build
cmake ..
make
```

Run the compiled executable:

```bash
./demo
```

No external dependencies are required beyond the base environment.

## Project Structure

```
.
├── classes/
│   ├── GameManager
│   ├── Board / TicTacToe
│   ├── Cell / Square
│   └── UIManager
├── imgui/
│   └── ImGui library files
├── resources/
│   └── assets and resources
├── build/
│   └── compiled output
├── Application.cpp
├── Application.h
├── CMakeLists.txt
└── README.md
```

- `classes/` contains all gameplay and logic code
- `imgui/` includes the ImGui UI framework
- `resources/` stores visual and UI resources
- `build/` contains compiled output
