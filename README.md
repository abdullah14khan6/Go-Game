# Go Game Implementation in C++

A console-based implementation of the classic board game Go, featuring both single-player (vs AI) and multiplayer modes.
This project demonstrates fundamental game development concepts including board state management, turn-based gameplay,
and simple AI decision making.

## 🎮 Features

- Interactive console-based interface with colored pieces
- Flexible board sizes (4x19)
- Single-player mode with AI opponent (although AI is not very smart because i didnt use any algorithm)
- Two-player local multiplayer
- Move history replay system
- Territory scoring with capture tracking
- 15-second time limit per turn
- Basic AI with liberty-based decision making
- Comprehensive game instructions and history

## 📥 Installation & Setup

### Using Visual Studio (Recommended)
1. Clone this repository
2. Open the `.sln` file in Visual Studio
3. Build and run the project (F5 or ctrl+F5)

### Using Other Compilers (g++, clang, etc.)
1. Ensure you have a C++ compiler installed
2. Clone this repository
3. Navigate to the source directory
4. Compile the source file:
   ```bash
   g++ go.cpp -o go
   ```
   or
   ```bash
   clang++ go.cpp -o go
   ```
5. Run the executable:
   ```bash
   ./go
   ```

### Prerequisites
- Windows operating system (for console color support)
- C++11 compatible compiler
- For Visual Studio users: VS 2019 or later recommended

## 🎯 How to Play

1. Launch the game
2. Use arrow keys to navigate menus
3. Choose between:
   - Play with AI
   - Multiplayer
   - Instructions
   - History
4. Select board size (4-19)
5. During gameplay:
   - Use arrow keys to move cursor
   - Press Enter to place a piece
   - Press ESC to exit
   - Each turn has a 15-second time limit

## 🎲 Game Rules

- Black plays first, followed by White
- Players take turns placing stones on intersections
- Stones must have at least one liberty (empty adjacent intersection) to remain on the board
- Capturing occurs when a stone or group has no liberties
- Territory is counted at game end
- White receives a +1 point komi (compensation for going second)

## 🖥️ Sample Output
![image](https://github.com/user-attachments/assets/e6d11296-e2a3-4124-a0ad-ab4690cce3c2)
![image](https://github.com/user-attachments/assets/89dbfc0d-1cd6-43a6-af7a-6be1c4d2419c)


## 📁 Project Structure
```
Go-Game-main/
│
├── Go-Game/
|   ├── Go-Game.vcxproj
|   ├── Go-Game.vcxproj.filters
│   └── go.cpp
│
├── .gitignore
├── .gitattributes
├── README.md
└── LICENSE
```

## ⚙️ Technical Details

- Written in C++
- Uses Windows Console API for cursor movement and colors
- Implements flood fill algorithm for territory calculation
- Features a basic AI using liberty-based decision making
- Handles suicide moves validation

## 🤝 Contributing

Feel free to fork this project and submit pull requests. Areas for improvement include:
- Cross-platform compatibility
- Enhanced AI strategies
- GUI implementation
- Network multiplayer support
- Save/Load game functionality

## 📜 License

This project is licensed under the MIT License - see the LICENSE file for details.
