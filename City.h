#ifndef CITY_H
#define CITY_H

#include <vector>
#include <string>
using namespace std;

// Cell types in the city grid
#define NORMAL  0
#define DANGER  1
#define CRISIS  2
#define POLICE  3
#define VICTIM  4
#define PATH    5

class City {
private:
    int rows, cols;
    vector<vector<int>> grid;
    vector<vector<string>> labels; // names of special cells

public:
    City(int r = 10, int c = 10);
    bool loadFromFile(const string& filename);
    void display() const;
    void displayWithPath(vector<pair<int,int>>& path, vector<pair<int,int>>& victims) const;
    int getCell(int r, int c) const;
    void setCell(int r, int c, int val);
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    bool isValid(int r, int c) const;
};

#endif
