#ifndef VICTIM_H
#define VICTIM_H

#include <string>
using namespace std;

class Victim {
private:
    int id;
    int row, col;
    int distressLevel;   // 1-10
    string timeOfDay;    // morning, afternoon, evening, night
    string status;       // "waiting", "helped", "critical"

public:
    Victim(int id, int r, int c, int distress, string time);

    // Getters
    int getId()           const { return id; }
    int getRow()          const { return row; }
    int getCol()          const { return col; }
    int getDistress()     const { return distressLevel; }
    string getTime()      const { return timeOfDay; }
    string getStatus()    const { return status; }

    // Setters
    void setStatus(string s)    { status = s; }

    // Display
    void displayInfo() const;
    float getTimeFactor() const;
    string getTimeLabel() const;
};

#endif
