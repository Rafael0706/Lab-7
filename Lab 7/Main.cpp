/*
Rafael Gallegos Cespedes
Computer Science Fall 2024
Due: Dec. 17, 2024
Lab 7 Battleship
*/
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constants for grid size and ship sizes
const int GRID_SIZE = 10;
const char WATER = '~';
const char SHIP = '#';
const char HIT = 'H';
const char MISS = 'M';

// Ship sizes
const vector<pair<string, int>> SHIPS = {
    {"Carrier", 5},
    {"Battleship", 4},
    {"Cruiser", 3},
    {"Submarine", 3},
    {"Destroyer", 2}
};

// Grid type definition
typedef vector<vector<char>> Grid;

// Function prototypes
void initializeGrid(Grid& grid);
void displayGrid(const Grid& grid, bool showShips);
bool placeShip(Grid& grid, int size, int x, int y, char direction);
void placeComputerShips(Grid& grid);
bool fire(Grid& grid, int x, int y);
bool checkGameOver(const Grid& grid);
void userPlaceShips(Grid& grid);

int main() {
    srand(time(0));  // Seed random number generator

    // User and computer grids
    Grid userGrid(GRID_SIZE, vector<char>(GRID_SIZE));
    Grid computerGrid(GRID_SIZE, vector<char>(GRID_SIZE));
    Grid computerGuessGrid(GRID_SIZE, vector<char>(GRID_SIZE));

    // Initialize grids
    initializeGrid(userGrid);
    initializeGrid(computerGrid);
    initializeGrid(computerGuessGrid);

    cout << "Welcome to Battleship!\n";
    cout << "Place your ships on the grid:\n";
    userPlaceShips(userGrid);

    cout << "\nThe computer is placing its ships...\n";
    placeComputerShips(computerGrid);

    bool gameOver = false;
    int x, y;

    // Game loop
    while (!gameOver) {
        // Display user's grid and computer's guess grid
        cout << "\nYour Grid:\n";
        displayGrid(userGrid, true);

        cout << "\nComputer's Guess Grid:\n";
        displayGrid(computerGuessGrid, false);

        // User's turn to fire
        cout << "\nEnter coordinates to fire (x y): ";
        cin >> x >> y;

        if (fire(computerGrid, x - 1, y - 1)) {
            cout << "You hit a ship!\n";
            computerGuessGrid[y - 1][x - 1] = HIT;
        }
        else {
            cout << "You missed!\n";
            computerGuessGrid[y - 1][x - 1] = MISS;
        }

        // Check if the game is over
        if (checkGameOver(computerGrid)) {
            cout << "\nCongratulations! You destroyed all the computer's ships!\n";
            break;
        }

        // Computer's turn to fire
        x = rand() % GRID_SIZE;
        y = rand() % GRID_SIZE;

        cout << "\nComputer fires at (" << x + 1 << ", " << y + 1 << "): ";
        if (fire(userGrid, x, y)) {
            cout << "It's a hit!\n";
        }
        else {
            cout << "It's a miss!\n";
        }

        // Check if the game is over
        if (checkGameOver(userGrid)) {
            cout << "\nThe computer destroyed all your ships. You lose!\n";
            break;
        }
    }

    cout << "Game Over. Thanks for playing!\n";
    return 0;
}

// Initialize the grid with water
void initializeGrid(Grid& grid) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = WATER;
        }
    }
}

// Display the grid
void displayGrid(const Grid& grid, bool showShips) {
    cout << "   ";
    for (int i = 0; i < GRID_SIZE; i++) cout << i + 1 << " ";
    cout << "\n";

    for (int i = 0; i < GRID_SIZE; i++) {
        cout << i + 1 << "  ";
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == SHIP && !showShips)
                cout << WATER << " ";
            else
                cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
}

// Place ships for the user
void userPlaceShips(Grid& grid) {
    for (const auto& ship : SHIPS) {
        int x, y;
        char direction;

        while (true) {
            cout << "Place your " << ship.first << " (size " << ship.second << ")\n";
            cout << "Enter coordinates (x y) and direction (H for horizontal, V for vertical): ";
            cin >> x >> y >> direction;

            if (placeShip(grid, ship.second, x - 1, y - 1, direction)) {
                displayGrid(grid, true);
                break;
            }
            else {
                cout << "Invalid position. Try again.\n";
            }
        }
    }
}

// Randomly place computer's ships
void placeComputerShips(Grid& grid) {
    for (const auto& ship : SHIPS) {
        int x, y;
        char direction;

        while (true) {
            x = rand() % GRID_SIZE;
            y = rand() % GRID_SIZE;
            direction = (rand() % 2 == 0) ? 'H' : 'V';

            if (placeShip(grid, ship.second, x, y, direction)) break;
        }
    }
}

// Place a ship on the grid
bool placeShip(Grid& grid, int size, int x, int y, char direction) {
    if (direction == 'H') {
        if (x + size > GRID_SIZE) return false;

        for (int i = 0; i < size; i++) {
            if (grid[y][x + i] != WATER) return false;
        }

        for (int i = 0; i < size; i++) {
            grid[y][x + i] = SHIP;
        }
    }
    else if (direction == 'V') {
        if (y + size > GRID_SIZE) return false;

        for (int i = 0; i < size; i++) {
            if (grid[y + i][x] != WATER) return false;
        }

        for (int i = 0; i < size; i++) {
            grid[y + i][x] = SHIP;
        }
    }
    else {
        return false;
    }

    return true;
}

// Fire at a grid position
bool fire(Grid& grid, int x, int y) {
    if (grid[y][x] == SHIP) {
        grid[y][x] = HIT;
        return true;
    }
    else if (grid[y][x] == WATER) {
        grid[y][x] = MISS;
        return false;
    }
    return false;  // Already fired there
}

// Check if all ships are destroyed
bool checkGameOver(const Grid& grid) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == SHIP) return false;
        }
    }
    return true;
}
