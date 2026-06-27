#include "City.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

City::City(int r, int c) : rows(r), cols(c) {
    grid.assign(r, vector<int>(c, NORMAL));
    labels.assign(r, vector<string>(c, ""));
}

bool City::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "  [ERROR] Could not open " << filename << endl;
        return false;
    }
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            file >> grid[i][j];
    file.close();
    return true;
}

void City::display() const {
    cout << "\n  CITY MAP (10x10)\n";
    cout << "  ";
    for (int c = 0; c < cols; c++)
        cout << "  " << c << " ";
    cout << "\n";

    for (int r = 0; r < rows; r++) {
        cout << "  " << r << " ";
        for (int c = 0; c < cols; c++) {
            switch (grid[r][c]) {
                case NORMAL: cout << "[ . ]"; break;
                case DANGER: cout << "[ ! ]"; break;
                case CRISIS: cout << "[ C ]"; break;
                case POLICE: cout << "[ P ]"; break;
                case VICTIM: cout << "[ V ]"; break;
                case PATH:   cout << "[ > ]"; break;
                default:     cout << "[ . ]"; break;
            }
        }
        cout << "\n";
    }

    cout << "\n  LEGEND: [.]=Normal  [!]=Danger  [C]=Crisis Center";
    cout << "\n          [P]=Police  [V]=Victim  [>]=Route Path\n\n";
}

void City::displayWithPath(vector<pair<int,int>>& path, vector<pair<int,int>>& victims) const {
    // overlay path and victims on a temp copy
    vector<vector<int>> temp = grid;

    for (auto& p : path)
        if (temp[p.first][p.second] == NORMAL || temp[p.first][p.second] == DANGER)
            temp[p.first][p.second] = PATH;

    for (auto& v : victims)
        temp[v.first][v.second] = VICTIM;

    cout << "\n  CITY MAP WITH ROUTE\n";
    cout << "  ";
    for (int c = 0; c < cols; c++)
        cout << "  " << c << " ";
    cout << "\n";

    for (int r = 0; r < rows; r++) {
        cout << "  " << r << " ";
        for (int c = 0; c < cols; c++) {
            switch (temp[r][c]) {
                case NORMAL: cout << "[ . ]"; break;
                case DANGER: cout << "[ ! ]"; break;
                case CRISIS: cout << "[ C ]"; break;
                case POLICE: cout << "[ P ]"; break;
                case VICTIM: cout << "[ V ]"; break;
                case PATH:   cout << "[ > ]"; break;
                default:     cout << "[ . ]"; break;
            }
        }
        cout << "\n";
    }
    cout << "\n  LEGEND: [.]=Normal  [!]=Danger  [C]=Crisis Center";
    cout << "\n          [P]=Police  [V]=Victim  [>]=Route Path\n\n";
}

int City::getCell(int r, int c) const {
    if (!isValid(r, c)) return -1;
    return grid[r][c];
}

void City::setCell(int r, int c, int val) {
    if (isValid(r, c)) grid[r][c] = val;
}

bool City::isValid(int r, int c) const {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}
