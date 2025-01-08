#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace chrono;

//fuctions
void moveCursor(int x, int y);
void startScreen();
void displayMenu();
void instruction();
void history();
void playmenu();
void yesnoMenu();
void yesnoMenuend();
void moveshistory();
//board
void initializeBoardSize();
void displayBoard(char array[21][21], int x, int y);
void populateBoard(char array[21][21]);
bool isBoardfull();
void playingGame();
int liberties(int x, int y, char playerPiece, int group[400][2], int& groupSize);
void switchTurn();
bool invalidMove(int x, int y);
void capturePiece(int x, int y);
void removeCapturePiece();
void calculateTerritory();
void floodFill(int x, int y, char& enclosingColor, int& territory, bool visited[20][20]);
void displayResult();
//ai
void aiMakeMove();

//variables
int boardSize;
char board[21][21] = { '+' };
char pieces[2] = { 'B','W' };
int score[2] = { 0,0 };
char currentPlayer = pieces[0];
char opponentpiece;
bool ai = false;
int movesHistoryb[300][2] = { -1 };
int hisb = 0;
int movesHistoryw[300][2] = { -1 };
int hisw = 0;
bool show = true;

int main()
{
	startScreen();
	//displayMenu();
	return 0;
}

//DEFINITIONS

void startScreen()
{
	moveCursor(16, 5);
	cout << "\033[36m";
	cout << " *****   *****       *****   *****  *     *  ***** " << endl;
	Sleep(100);
	moveCursor(16, 6);
	cout << " *       *   *       *       *   *  **   **  *     " << endl;
	Sleep(100);
	moveCursor(16, 7);
	cout << " *  **   *   *   -   *  **   *****  * * * *  ****  " << endl;
	Sleep(100);
	moveCursor(16, 8);
	cout << " *   *   *   *       *   *   *   *  *  *  *  *     " << endl;
	Sleep(100);
	moveCursor(16, 9);
	cout << " *****   *****       *****   *   *  *     *  ***** " << endl;
	Sleep(100);
	cout << "\033[0m";

	moveCursor(27, 12);
	cout << "\033[33m";
	cout << "PRESS ENTER KEY TO CONTINUE\n";
	cout << "\033[0m";
enter:
	char validation = _getch();
	if (validation == '\r')
		displayMenu();
	else
		goto enter;
}

void moveCursor(int x, int y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<short>(x) , static_cast<short>(y) };
	SetConsoleCursorPosition(hConsole, pos);
}

void displayMenu()
{
	//reinitializing
	score[0] = 0;
	score[1] = 0;
	hisw = 0;
	hisb = 0;
	ai = false;
	show = true;

	int selected = 0;
	bool exits = false;
	string menuItems[4] = { "Play Game", "Instructions", "History", "Exit" };
	while (!exits)
	{
		system("cls");
		cout << "\033[91m" << "Control with arrow keys and press enter to continue\n" << "\033[91m\n";
		for (int i = 0; i < 4; ++i)
		{
			if (i == selected)
			{
				cout << "\033[33m" << ">> " << "\033[91m" << menuItems[i] << "\033[33m" << " <<\n" << "\033[93m";
			}
			else
			{
				cout << "\033[93m" << menuItems[i] << "\033[93m\n";
			}
		}

		char key = _getch();
		if (key == -32 || key == 224)
		{
			char arrow = _getch();
			if (arrow == 72)
			{
				selected = (selected - 1 + 4) % 4;
			}
			else if (arrow == 80)
			{
				selected = (selected + 1) % 4;
			}
		}
		else if (key == '\r')
		{
			switch (selected)
			{
			case 0:
				playmenu();
				initializeBoardSize();
				playingGame();
				calculateTerritory();
				displayResult();
				break;
			case 1:
				instruction();
				break;
			case 2:
				history();
				break;
			case 3:
				cout << "EXITING THE GAME.....\n";
				Sleep(2000);
				exit(0);
			}
		}
	}
}

void playmenu()
{
	int selected = 0;
	bool exits = false;
	string menuItems[4] = { "Play with AI", "Multiplayer", "Back to Menu" };
	while (!exits)
	{
		system("cls");
		cout << "\033[91m" << "Control with arrow keys and press enter to continue\n" << "\033[91m\n";
		for (int i = 0; i < 3; ++i)
		{
			if (i == selected)
			{
				cout << "\033[33m" << ">> " << "\033[91m" << menuItems[i] << "\033[33m" << " <<\n" << "\033[93m";
			}
			else
			{
				cout << "\033[93m" << menuItems[i] << "\033[0m\n";
			}
		}

		char key = _getch();
		if (key == -32 || key == 224)
		{
			char arrow = _getch();
			if (arrow == 72)
			{
				selected = (selected - 1 + 3) % 3;
			}
			else if (arrow == 80)
			{
				selected = (selected + 1) % 3;
			}
		}
		else if (key == '\r')
		{
			switch (selected)
			{
			case 0:
				ai = true;
				exits = true;
				return;
			case 1:
				ai = false;
				exits = true;
				return;
			case 2:
				displayMenu();
			}
		}
	}
}

void instruction()
{
	system("cls");
	cout << "\033[91m" << "The Go game is a classic strategy board game originating from China more than 4,000 years ago.\n";
	cout << "Here are the basic rules of Go : \n\n";
	cout << "\033[0m";

	cout << "\033[91m" << "Objective: \n" << "\033[93m" << "The goal of the game is to control more territory on the board than your opponent by the end of the game.\n\n";

	cout << "\033[91m" << "Setup: \n" << "\033[93m" << "The game is played on a grid, typically 19x19, but smaller boards (13x13 or 9x9) can be used for beginners.";
	cout << "\nTwo players take turns placing stones on the intersections of the grid:\n";
	cout << "\tBlack moves first.\n";
	cout << "\tWhite follows.\n";
	cout << "Once placed, stones do not move but can be captured and removed from the board.\n\n";

	cout << "\033[91m" << "Liberties: \n" << "\033[93m" << "Stones must have at least one empty adjacent intersection to stay on the board." <<
		"Stones with no liberties are captured and removed.\n\n";

	cout << "\033[91m" << "Territory: \n" << "\033[93m" << "Enclose empty intersections to claim them as territory.\n\n";
	cout << "\033[91m" << "KO Rule: \n" << "\033[93m" << "Recreating the same board position is forbidden.\n\n";
	cout << "\033[91m" << "End of Game: \n" << "\033[93m" << "Ends when both players agree. Scores are calculated based on controlled territory and captured stones.\n\n";
	cout << "\033[91m" << "Scoring: \n" << "\033[93m" << "Points = empty intersections in territory + captured stones. White gets extra points (komi) for going second.\n\n";
	cout << "\033[91m" << "Suicide Rule: \n" << "\033[93m" << "You cannot place a stone that immediately has no liberties unless it captures opponent stones.\n\n";
	cout << "\033[91m" << "SIMPLE TO LEARN, HARD TO MASTER\n\n";

	cout << "\033[93m" << "click any key to return to menu....\n" << "\033[0m";
	char c = _getch();
	displayMenu();
}

void history()
{
	system("cls");

	cout << "\033[91m" << "The game of Go has a long and rich history, dating back over 4,000 years. \nHere is a concise history of the game:\n\n";
	cout << "Origin: \n" << "\033[93m" << "Originated in China over 4,000 years ago, making it one of the oldest board games." <<
		"\nLegend attributes its creation to Emperor Yao as a teaching tool for his son.\n\n";

	cout << "\033[91m" << "Spread to other Countries: \n" << "\033[93m" << "Reached Korea (as Baduk) and Japan by the 7th century CE." <<
		"\nJapan formalized rules and professionalized the game during the Edo period.\n\n";

	cout << "\033[91m" << "Modernization: \n" << "\033[93m" << "Spread globally in the 20th century through books, films, and associations." <<
		"\nIn 2016, AI program AlphaGo defeated world champion Lee Sedol, highlighting the game’s complexity.\n\n";

	cout << "\033[91m" << "Significance Today: \n" << "\033[93m" << "Widely played worldwide, Go represents patience, strategy, and cultural heritage.\n\n";

	cout << "\033[93m" << "click any key to return to menu....\n" << "\033[0m";
	char c = _getch();
	displayMenu();
}

void initializeBoardSize()
{
	system("cls");
	do
	{

		cout << "\033[33m" << "Select the board size (Between 4 and 19) : \n" << "\033[93m";
		cin >> boardSize;

		if (cin.fail() || boardSize < 4 || boardSize > 19) {
			cout << "Invalid input! Please enter a number between 4 and 19.\n";
			Sleep(1000);

			cin.clear();
			while (cin.get() != '\n'); // discard all characters in the line
		}

	} while (boardSize > 19 || boardSize < 4);

	boardSize++;    //for accurAcy
	populateBoard(board);
}

void populateBoard(char board[21][21])
{
	system("cls");
	for (int i = 0; i < boardSize; ++i)
	{
		for (int j = 0; j < boardSize; ++j)
		{
			board[i][j] = '+';
		}
	}
	displayBoard(board, 0, 0);
}

void displayBoard(char board[21][21], int x, int y)
{
	moveCursor(0, 0);
	//system("cls");
	char up = 25;
	char left = 26;
	if (show)
	{
		cout << "\033[36m" << "Use arrow keys to Control\n\n";
		cout << "\033[93m";
	}

	for (int i = 0; i < boardSize; ++i)
	{
		if (i == x)
		{
			if (i == 0)
			{
				cout << "\033[91m" << "    " << up << "\033[93m" << "\n";
			}
			else
			{
				cout << "\033[91m" << "    " << up << "\033[93m" << "\n";
			}

		}
		else
		{
			cout << "    ";
		}
	}
	cout << endl;
	for (int i = 0; i < boardSize; ++i)
	{
		if (i == y)
		{
			cout << "\033[91m" << left << "\033[93m" << "   ";
		}
		else
		{
			cout << "    ";
		}
		for (int j = 0; j < boardSize; ++j)
		{
			if (j == boardSize - 1)
			{
				if (i == y && j == x)
				{
					cout << "\033[96m" << board[i][j] << "\033[93m";
				}
				else
				{
					if (board[i][j] != '+')
					{
						if (board[i][j] == 'B')
						{
							cout << "\033[91m" << board[i][j] << "\033[93m";
						}
						else if (board[i][j] == 'W')
						{
							cout << "\033[92m" << board[i][j] << "\033[93m";
						}

					}
					else
					{
						cout << "\033[33m" << board[i][j] << "\033[93m";
					}

				}
			}
			else
			{
				if (i == y && j == x)
				{
					cout << "\033[96m" << board[i][j] << "\033[93m" << "---";
				}
				else
				{
					if (board[i][j] != '+')
					{
						if (board[i][j] == 'B')
						{
							cout << "\033[91m" << board[i][j] << "\033[93m" << "---";
						}
						else if (board[i][j] == 'W')
						{
							cout << "\033[92m" << board[i][j] << "\033[93m" << "---";
						}

					}
					else
					{
						cout << "\033[33m" << board[i][j] << "\033[93m" << "---";
					}
				}
			}
		}
		cout << endl;
		for (int j = 0; j < boardSize; ++j)
		{
			if (i != boardSize - 1)
			{
				if (j == 0)
				{
					cout << "    |";
				}
				else if (j == boardSize - 1)
				{
					cout << "   |";
				}
				else
				{
					cout << "   |";
				}
			}
		}
		cout << endl;
	}
}

bool isBoardfull()
{
	for (int i = 0; i < boardSize; ++i)
	{
		for (int j = 0; j < boardSize; ++j)
		{
			if (board[i][j] == '+')
			{
				return false;
			}
		}
	}
	return true;
}

void switchTurn()
{
	currentPlayer = ((currentPlayer == pieces[0]) ? pieces[1] : pieces[0]);
	opponentpiece = ((currentPlayer == pieces[0]) ? pieces[1] : pieces[0]);
}

bool invalidMove(int x, int y)
{
	if (board[y][x] != '+')
	{
		return true;
	}

	board[y][x] = currentPlayer;
	opponentpiece = (currentPlayer == 'B') ? 'W' : 'B';

	//liberties of the placed piece
	int group[400][2];
	int groupSize = 0;
	int libertiesCount = liberties(x, y, currentPlayer, group, groupSize);

	// Check if move captures opponent's pieces
	bool capturesOpponent = false;
	for (int dy = -1; dy <= 1; ++dy)
	{
		for (int dx = -1; dx <= 1; ++dx)
		{
			if ((dx == 0 && dy == 0) || (dx != 0 && dy != 0)) continue;

			int nx = x + dx;
			int ny = y + dy;

			if (nx >= 0 && nx < boardSize && ny >= 0 && ny < boardSize)
			{
				if (board[ny][nx] == opponentpiece)
				{
					int oppGroup[400][2];
					int oppGroupSize = 0;
					int oppLiberties = liberties(nx, ny, opponentpiece, oppGroup, oppGroupSize);

					if (oppLiberties == 0)
					{
						capturesOpponent = true;
						break;
					}
				}
			}
		}
	}
	board[y][x] = '+';

	//Move is valid if:
	//It has liberties OR
	//It captures opponent's pieces
	return !(libertiesCount > 0 || capturesOpponent);

}

int liberties(int x, int y, char playerPiece, int group[400][2], int& groupSize)
{
	bool visited[20][20] = { false };
	int stack[400][2];
	int top = -1;

	stack[++top][0] = x;
	stack[top][1] = y;
	visited[y][x] = true;
	groupSize = 0;

	int liberties = 0;

	while (top >= 0)
	{
		int cx = stack[top][0];
		int cy = stack[top--][1];

		group[groupSize][0] = cx;
		group[groupSize][1] = cy;
		groupSize++;
		for (int dx = -1; dx <= 1; ++dx)
		{
			for (int dy = -1; dy <= 1; ++dy)
			{
				if ((dx == 0 && dy == 0) || (dx != 0 && dy != 0))
				{
					continue;
				}

				int nx = cx + dx;
				int ny = cy + dy;

				if (nx < 0 || nx >= boardSize || ny < 0 || ny >= boardSize)
				{
					continue;
				}

				if (!visited[ny][nx])
				{
					visited[ny][nx] = true;

					if (board[ny][nx] == '+')
					{
						liberties++;
					}
					else if (board[ny][nx] == playerPiece)
					{
						stack[++top][0] = nx;
						stack[top][1] = ny;
					}
				}
			}
		}
	}

	return liberties;
}

void capturePiece(int x, int y)
{
	char playerPiece = board[y][x];
	int group[400][2];
	int groupSize = 0;

	int liberty = liberties(x, y, playerPiece, group, groupSize);

	if (liberty == 0)
	{
		char capturedColor = board[y][x];
		for (int i = 0; i < groupSize; ++i)
		{
			board[group[i][1]][group[i][0]] = '+';
		}

		if (capturedColor == 'B')
		{
			score[1] = score[1] + groupSize;
		}
		else if (capturedColor == 'W')
		{
			score[0] = score[0] + groupSize;
		}
	}
}

void removeCapturePiece()
{
	for (int i = 0; i < boardSize; ++i)
	{
		for (int j = 0; j < boardSize; ++j)
		{
			if (board[i][j] == 'B' || board[i][j] == 'W')
			{
				capturePiece(j, i);
			}
		}
	}
}

void playingGame() {
	system("cls");

	int x = 0, y = 0;
	bool playing = true;
	const int timeLimit = 15;

	while (playing)
	{
		system("cls");
		removeCapturePiece();
		displayBoard(board, x, y);
		cout << "\033[36m" << "Current player: " << "\033[96m" << currentPlayer << "\033[36m" << "\tPress ESC to exit\n\n";
		cout << "Player 1 score: " << "\033[96m" << score[0] << "\033[36m" << "\tPlayer 2 score: " << "\033[96m" << score[1] << "\n\n" << "\033[93m";

		auto turnStartTime = steady_clock::now();
		bool turnOver = false;

		while (!turnOver)
		{
			auto currentTime = steady_clock::now();
			auto elapsed = duration_cast<seconds>(currentTime - turnStartTime).count();
			if (elapsed > timeLimit)
			{
				cout << "\nTime's up! Turn skipped.\n";
				Sleep(1000);
				switchTurn();
				x = 0;
				y = 0;
				turnOver = true;
				break;
			}

			if (_kbhit())
			{
				char key = _getch();
				if (key == -32 || key == 224) // Arrow keys
				{
					char arrow = _getch();
					if (arrow == 72) // Up arrow
					{
						y = (y - 1 + boardSize) % boardSize; // Move up
					}
					else if (arrow == 80) // Down arrow
					{
						y = (y + 1) % boardSize; // Move down
					}
					else if (arrow == 75) // Left arrow
					{
						x = (x - 1 + boardSize) % boardSize; // Move left
					}
					else if (arrow == 77) // Right arrow
					{
						x = (x + 1) % boardSize; // Move right
					}
					system("cls");
					displayBoard(board, x, y);
					cout << "\033[36m" << "Current player: " << "\033[96m" << currentPlayer << "\033[36m" << "\tPress ESC to exit\n\n";
					cout << "Player 1 score: " << "\033[96m" << score[0] << "\033[36m" << "\tPlayer 2 score: " << "\033[96m" << score[1] << "\n\n" << "\033[93m";
					cout << "\033[36m" << "Time left: " << "\033[96m" << (timeLimit - elapsed) << " seconds\n\n" << "\033[93m";
				}
				else if (key == '\r')
				{ // Enter key
					if (!invalidMove(x, y))
					{
						removeCapturePiece();
						board[y][x] = currentPlayer;
						if (currentPlayer == 'B')
						{
							movesHistoryb[hisb][0] = x;
							movesHistoryb[hisb][1] = y;
							hisb++;
						}
						else if (currentPlayer == 'W')
						{
							movesHistoryw[hisw][0] = x;
							movesHistoryw[hisw][1] = y;
							hisw++;
						}
						switchTurn();
						x = 0;
						y = 0;
						turnOver = true;
					}
					else {
						cout << "\033[36m" << "Invalid Move. Place your piece somewhere else\n" << "\033[93m";
						Sleep(1000);
					}
				}
				else if (key == 27) // ESC key
				{
					playing = false;
					turnOver = true;
				}
			}
		}

		if (ai)
		{
			if (currentPlayer == 'W')
			{
				aiMakeMove();
				removeCapturePiece();
				switchTurn();
				system("cls");
				displayBoard(board, x, y);
				cout << "\033[36m" << "Current player: " << "\033[96m" << currentPlayer << "\033[36m" << "\tPress ESC to exit\n\n";
				cout << "Player 1 score: " << "\033[96m" << score[0] << "\033[36m" << "\tPlayer 2 score: " << "\033[96m" << score[1] << "\n\n" << "\033[93m";
			}
		}

		if (isBoardfull()) {
			system("cls");
			cout << "\033[91m" << "GAME OVER!! BOARD FULL\n" << "\033[93m";
			Sleep(2000);
			playing = false;
		}
	}
}

void aiMakeMove()
{
	int bestI = -1, bestJ = -1, maxliberties = -1;
	bool capture = false;

	for (int i = 0; i < boardSize; ++i)
	{
		for (int j = 0; j < boardSize; ++j)
		{
			if (board[i][j] == '+')
			{
				board[i][j] = currentPlayer;
				for (int di = -1; di <= 1; ++di)
				{
					for (int dj = -1; dj <= 1; ++dj)
					{
						if (dj == 0 && di == 0)
						{
							continue;
						}
						int ni = i + di;
						int nj = j + dj;
						if (nj<0 && nj>boardSize && ni<0 && ni>boardSize)
						{
							continue;
						}
						int oppgroup[400][2];
						int oppgroupSize = 0;
						int oppliberties = liberties(j, i, currentPlayer, oppgroup, oppgroupSize);
						if (oppliberties == 0)
						{
							bestI = ni;
							bestJ = nj;
							board[i][j] = '+';
							capture = true;
							goto makeMove;
						}
					}
				}
				int group[400][2];
				int groupSize = 0;
				int currentLiberties = liberties(j, i, currentPlayer, group, groupSize);
				if (currentLiberties > maxliberties)
				{
					bestI = i;
					bestJ = j;
					maxliberties = currentLiberties;
				}
				board[i][j] = '+';
			}
		}
	}
	if (bestI == -1 && bestJ == -1)
	{
		srand(time(0));
		while (true)
		{
			int i = rand() % boardSize;
			int j = rand() % boardSize;
			if (!invalidMove(i, j) && i >= 0 && j >= 0)
			{
				board[i][j] = currentPlayer;
				removeCapturePiece();
				switchTurn();
				movesHistoryw[hisw][0] = i;
				movesHistoryw[hisw][1] = j;
				hisw++;
				break;
			}
		}
	}
makeMove:
	if (bestI != -1 && bestJ != -1)
	{
		board[bestI][bestJ] = currentPlayer;
		if (currentPlayer == 'B')
		{
			movesHistoryb[hisb][0] = bestI;
			movesHistoryb[hisb][1] = bestJ;
			hisb++;
		}
		else if (currentPlayer == 'W')
		{
			movesHistoryw[hisw][0] = bestI;
			movesHistoryw[hisw][1] = bestJ;
			hisw++;
		}
	}
}

void floodFill(int x, int y, char& enclosingColor, int& territory, bool visited[20][20])
{
	int stack[400][2];
	int top = -1;
	stack[++top][0] = x;
	stack[top][1] = y;
	visited[y][x] = true;
	territory = 0;
	bool isenclosed = true;
	char boundaryColor = '+';

	while (top >= 0)
	{
		int cx = stack[top][0];
		int cy = stack[top][1];
		top--;
		territory++;

		for (int dy = -1; dy <= 1; ++dy)
		{
			for (int dx = -1; dx <= 1; ++dx)
			{
				if ((dx == 0 && dy == 0) || (dx != 0 && dy != 0))
				{
					continue;
				}
				int nx = cx + dx;
				int ny = cy + dy;
				if (nx < 0 || nx > boardSize - 1 || ny < 0 || ny > boardSize - 1)
				{
					isenclosed = false;
				}
				else
				{
					if (board[ny][nx] == '+' && !visited[ny][nx])
					{
						stack[++top][0] = nx;
						stack[top][1] = ny;
						visited[ny][nx] = true;
					}
					else if (board[ny][nx] != '+')
					{
						if (boundaryColor == '+')
							boundaryColor = board[ny][nx];
						else if (board[ny][nx] != boundaryColor)
							isenclosed = false;
					}
				}

			}
		}

	}

	if (!isenclosed || boundaryColor == '+')
	{
		territory = 0;
		enclosingColor = '+';
	}
	else
	{
		enclosingColor = boundaryColor;
	}
}

void calculateTerritory()
{
	bool visited[20][20] = { false };

	for (int i = 0; i < boardSize; ++i)
	{
		for (int j = 0; j < boardSize; ++j)
		{
			if (board[i][j] == '+' && !visited[i][j])
			{
				char enclosingColor = '+';
				int territory = 0;

				floodFill(j, i, enclosingColor, territory, visited);

				if (enclosingColor == 'B')
				{
					score[0] += territory;
				}
				else if (enclosingColor == 'W')
				{
					score[1] += territory;
				}
			}
		}
	}
}

void displayResult()
{
	show = false;
	system("cls");
	displayBoard(board, 0, 0);
	cout << "\033[36m" << "Player 1 score: " << "\033[96m" << score[0] << "\033[36m" << "\tPlayer 2 Score: " << "\033[96m" << score[1] << "\n" << "\033[93m";
	cout << "\033[36m" << "+1 for Player 2 for going second\n\n" << "\033[93m";
	score[1]++;
	if (score[0] > score[1])
	{
		cout << "\033[91m" << "PLAYER 1 WON!!!!\n\n" << "\033[36m";
		cout << "\033[36m" << "Player 1 score: " << "\033[96m" << score[0] << "\033[36m" << "\tPlayer 2 Score: " << "\033[96m" << score[1] << "\n" << "\033[93m";

	}
	else if (score[0] < score[1])
	{
		cout << "\033[91m" << "PLAYER 2 WON!!!!\n\n" << "\033[36m";
		cout << "\033[36m" << "Player 2 score: " << "\033[96m" << score[1] << "\033[36m" << "\tPlayer 1 Score: " << "\033[96m" << score[0] << "\n" << "\033[93m";

	}
	else
	{
		cout << "\033[91m" << "ITS A TIE!!!!\n\n" << "\033[36m";
		cout << "\033[36m" << "Player 1 score: " << "\033[96m" << score[0] << "\033[36m" << "\tPlayer 2 Score: " << "\033[96m" << score[1] << "\n" << "\033[93m";

	}

	cout << "\n\nPRESS ANY KEY TO CONTINUE\n";
	char validation = _getch();

	yesnoMenu();

}

void yesnoMenu()
{
	show = false;
	int selected = 0;
	bool exits = false;
	string menuItems[4] = { "YES", "NO" };
	while (!exits)
	{
		system("cls");
		displayBoard(board, 0, 0);
		cout << "DO YOU WANT TO SEE THE MOVES HISTORY?\n\n";
		for (int i = 0; i < 2; ++i)
		{
			if (i == selected)
			{
				cout << "\033[33m" << ">> " << "\033[91m" << menuItems[i] << "\033[33m" << " <<\n" << "\033[93m";
			}
			else
			{
				cout << "\033[93m" << menuItems[i] << "\033[93m\n";
			}
		}

		char key = _getch();
		if (key == -32 || key == 224)
		{
			char arrow = _getch();
			if (arrow == 72)
			{
				selected = (selected - 1 + 2) % 2;
			}
			else if (arrow == 80)
			{
				selected = (selected + 1) % 2;
			}
		}
		else if (key == '\r')
		{
			switch (selected)
			{
			case 0:
				moveshistory();
				return;
			case 1:
				yesnoMenuend();
				return;
			}
		}
	}
}

void yesnoMenuend()
{
	show = false;
	int selected = 0;
	bool exits = false;
	string menuItems[4] = { "YES", "NO" };
	while (!exits)
	{
		system("cls");
		displayBoard(board, 0, 0);
		cout << "DO YOU WANT RETURN TO MAIN MENU?\n\n";
		for (int i = 0; i < 2; ++i)
		{
			if (i == selected)
			{
				cout << "\033[33m" << ">> " << "\033[91m" << menuItems[i] << "\033[33m" << " <<\n" << "\033[93m";
			}
			else
			{
				cout << "\033[93m" << menuItems[i] << "\033[93m\n";
			}
		}

		char key = _getch();
		if (key == -32 || key == 224)
		{
			char arrow = _getch();
			if (arrow == 72)
			{
				selected = (selected - 1 + 2) % 2;
			}
			else if (arrow == 80)
			{
				selected = (selected + 1) % 2;
			}
		}
		else if (key == '\r')
		{
			switch (selected)
			{
			case 0:
				displayMenu();
				return;
			case 1:
				cout << "EXITING THE GAME.....\n";
				Sleep(2000);
				exit(0);
			}
		}
	}
}

void moveshistory()
{
	show = false;
	char check = 'B';
	for (int i = 0; i < boardSize; ++i)
	{
		for (int j = 0; j < boardSize; ++j)
		{
			board[i][j] = '+';
		}
	}

	int movesHistory1[300][2] = { -1 };
	int his1 = hisb - 1;
	int movesHistory2[300][2] = { -1 };
	int his2 = hisw - 1;

	for (int i = hisb - 1, k = 0; i >= 0; --i, ++k)
	{
		movesHistory1[k][0] = movesHistoryb[i][0];
		movesHistory1[k][1] = movesHistoryb[i][1];
	}
	for (int i = hisw - 1, k = 0; i >= 0; --i, ++k)
	{
		movesHistory2[k][0] = movesHistoryw[i][0];
		movesHistory2[k][1] = movesHistoryw[i][1];
	}

	while (his1 >= 0 || his2 >= 0)
	{
		system("cls");
		displayBoard(board, 0, 0);

		if (check == 'B' && his1 >= 0)
		{
			int x = movesHistory1[his1][0];
			int y = movesHistory1[his1][1];
			//board[y][x] = 'B';

			if (x >= 0 && x < boardSize && y >= 0 && y < boardSize) {
				board[y][x] = 'B';
			}
			his1--;
			check = 'W';
		}
		else if (check == 'W' && his2 >= 0)
		{
			int x = movesHistory2[his2][0];
			int y = movesHistory2[his2][1];
			//board[y][x] = 'W';

			if (x >= 0 && x < boardSize && y >= 0 && y < boardSize) {
				board[y][x] = 'W';
			}
			his2--;
			check = 'B';
		}

		cout << "Click any key to see the next move\n";
		char validation = _getch();
	}
	cout << "All the moves have been shown. Do you want to return back to the menu?\n";
	yesnoMenuend();
}