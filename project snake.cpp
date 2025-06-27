#include <bits/stdc++.h>
#include <conio.h>
#include <fstream>
#include <thread>
using namespace std;

bool gameover;
const int height = 20;
const int width = 20;
int x, y, AxisX, AxisY, score, tailLength, highscore = 0;
int speed;
vector<pair<int, int>> tail;
enum edirection { STOP = 0, UP, DOWN, LEFT, RIGHT };
edirection dir;

// Load high score from file
void loadHighScore() {
    ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highscore;
        file.close();
    }
}

// Save high score to file
void saveHighScore() {
    if (score > highscore) {
        ofstream file("highscore.txt");
        file << score;
        file.close();
    }
}

// Initialize game state
void setup() {
    gameover = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    AxisX = rand() % width;
    AxisY = rand() % height;
    score = 0;
    tailLength = 0;
    tail.clear();
}

// Draw the game board
void draw() {
    system("CLS"); // Use "clear" on Linux/Mac
    for (int i = 0; i < width + 2; i++) cout << "*";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) cout << "*";

            if (i == y && j == x)
                cout << "O"; // Snake head
            else if (i == AxisY && j == AxisX)
                cout << "F"; // Food
            else {
                bool printed = false;
                for (auto &segment : tail) {
                    if (segment.first == j && segment.second == i) {
                        cout << "o"; // Snake tail
                        printed = true;
                        break;
                    }
                }
                if (!printed) cout << " ";
            }

            if (j == width - 1) cout << "*";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++) cout << "*";
    cout << endl;

    cout << "Score: " << score << "\tHigh Score: " << highscore << endl;
}

// Handle keyboard input
void input() {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
            case 'w': if (dir != DOWN) dir = UP; break;
            case 'a': if (dir != RIGHT) dir = LEFT; break;
            case 's': if (dir != UP) dir = DOWN; break;
            case 'd': if (dir != LEFT) dir = RIGHT; break;
            case 'x': gameover = true; break;
        }
    }
}

// Game logic: movement, collisions, food, scoring
void logic() {
    if (dir == STOP) return;  // Snake will not move until direction is chosen

    int prevX = x, prevY = y;
    int prev2X, prev2Y;

    for (int i = 0; i < tailLength; i++) {
        prev2X = tail[i].first;
        prev2Y = tail[i].second;
        tail[i] = { prevX, prevY };
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case UP: y--; break;
        case DOWN: y++; break;
        case LEFT: x--; break;
        case RIGHT: x++; break;
        default: break;
    }

    // Wall collision
    if (x < 0 || x >= width || y < 0 || y >= height) {
        gameover = true;
        return;
    }

    // Self collision
    for (auto &segment : tail) {
        if (segment.first == x && segment.second == y) {
            gameover = true;
            return;
        }
    }

    // Eat food
    if (x == AxisX && y == AxisY) {
        score++;
        tailLength++;
        if (tail.empty())
            tail.push_back({ x, y }); // Add current head position
        else
            tail.push_back(tail.back()); // Extend tail from last segment

        AxisX = rand() % width;
        AxisY = rand() % height;
    }
}

// Main function
int main() {
    srand(time(0));
    loadHighScore();

    cout << "Select Difficulty:\n";
    cout << "1. Easy\n";
    cout << "2. Medium\n";
    cout << "3. Hard\n";
    cout << "Choice: ";

    int choice;
    cin >> choice;
    if (choice == 1) speed = 150;
    else if (choice == 2) speed = 100;
    else speed = 50;

    setup();

    while (!gameover) {
        draw();
        input();
        logic();
        this_thread::sleep_for(chrono::milliseconds(speed));
    }

    saveHighScore();

    cout << "\nGame Over!\n";
    cout << "Final Score: " << score << endl;
    cout << "High Score: " << highscore << endl;

    return 0;
}
9
