#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <chrono> // For timing

using namespace std;
using namespace chrono;

// Maze dimensions
const int rows = 10;
const int cols = 10;

// Represent different cell states in the maze
enum CellState {
    WALL,
    PATH,
    PLAYER,
    EXIT,
};

// Maze class to handle maze generation and player movement
class Maze {
public:
    Maze();
    void generateMaze();
    void printMaze();
    bool movePlayer(char direction);
    bool checkWin();
    void startTimer();
    void stopTimer();
    void displayTimer();

private:
    vector<vector<CellState>> maze;
    pair<int, int> playerPosition;
    pair<int, int> exitPosition;
    steady_clock::time_point startTime;

    void generateMazeRecursive(int row, int col);
    bool isCellValid(int row, int col);
};

// Maze constructor
Maze::Maze() {
    maze = vector<vector<CellState>>(rows, vector<CellState>(cols, WALL));
    srand(time(nullptr));

    // Set initial player position
    playerPosition = make_pair(1, 1);

    // Set initial exit position
    exitPosition = make_pair(rows - 2, cols - 2);
}

// Generate the maze using recursive backtracking
void Maze::generateMaze() {
    generateMazeRecursive(1, 1);
    maze[playerPosition.first][playerPosition.second] = PLAYER;
    maze[exitPosition.first][exitPosition.second] = EXIT;
}

void Maze::generateMazeRecursive(int row, int col) {
    maze[row][col] = PATH;

    // Define the order in which to visit neighboring cells
    int dirs[4][2] = {{0, 2}, {2, 0}, {0, -2}, {-2, 0}};

    // Shuffle the directions
    for (int i = 0; i < 4; ++i) {
        int randomIndex = rand() % 4;
        swap(dirs[i], dirs[randomIndex]);
    }

    // Visit the neighboring cells
    for (int i = 0; i < 4; ++i) {
        int newRow = row + dirs[i][0];
        int newCol = col + dirs[i][1];

        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols && maze[newRow][newCol] == WALL) {
            maze[row + dirs[i][0] / 2][col + dirs[i][1] / 2] = PATH;
            generateMazeRecursive(newRow, newCol);
        }
    }
}

// Check if the cell is within the maze boundaries and is a path
bool Maze::isCellValid(int row, int col) {
    return row >= 0 && row < rows && col >= 0 && col < cols && maze[row][col] != WALL;
}

// Print the current state of the maze
void Maze::printMaze() {
    system("CLS");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            switch (maze[i][j]) {
                case WALL:
                    cout << "# ";
                    break;
                case PATH:
                    cout << ". ";
                    break;
                case PLAYER:
                    cout << "P ";
                    break;
                case EXIT:
                    cout << "E ";
                    break;
            }
        }
        cout << endl;
    }
    displayTimer(); // Display the timer on the screen
}

// Move the player in the specified direction
bool Maze::movePlayer(char direction) {
    int newPlayerRow = playerPosition.first;
    int newPlayerCol = playerPosition.second;

    switch (direction) {
        case 'w':
            newPlayerRow--;
            break;
        case 's':
            newPlayerRow++;
            break;
        case 'a':
            newPlayerCol--;
            break;
        case 'd':
            newPlayerCol++;
            break;
    }

    if (isCellValid(newPlayerRow, newPlayerCol)) {
        // Move the player
        maze[playerPosition.first][playerPosition.second] = PATH;
        playerPosition = make_pair(newPlayerRow, newPlayerCol);
        maze[newPlayerRow][newPlayerCol] = PLAYER;
        return true;
    }

    return false;
}

// Check if the player has reached the exit
bool Maze::checkWin() {
    return playerPosition == exitPosition;
}

// Start the timer
void Maze::startTimer() {
    startTime = steady_clock::now();
}

// Stop the timer and print the elapsed time
void Maze::stopTimer() {
    auto endTime = steady_clock::now();
    auto elapsedTime = duration_cast<seconds>(endTime - startTime).count();
    cout << "Time taken: " << elapsedTime << " seconds" << endl;
}

// Display the timer on the screen
void Maze::displayTimer() {
    auto currentTime = steady_clock::now();
    auto elapsedTime = duration_cast<seconds>(currentTime - startTime).count();
    cout << "Time: " << elapsedTime << " seconds" << endl;
}

int main() {
    Maze mazeGame;
    mazeGame.generateMaze();
    mazeGame.startTimer(); // Start the timer

    char move;
    do {
        mazeGame.printMaze();
        cout << "Enter your move (w/a/s/d): ";
        move = _getch();

        if (mazeGame.movePlayer(move)) {
            if (mazeGame.checkWin()) {
                mazeGame.stopTimer(); // Stop the timer when the player wins
                cout << "Congratulations! You found the exit!" << endl;
                break;
            }
        }
        else {
            cout << "Invalid move. Try again." << endl;
        }
    } while (true);

    return 0;
}
