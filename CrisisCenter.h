#ifndef CRISISCENTER_H
#define CRISISCENTER_H

#include <string>
using namespace std;

class CrisisCenter {
private:
    int id;
    string name;
    int row, col;
    int staffAvailable;
    int vehiclesAvailable;
    int totalResponses;

public:
    CrisisCenter(int id, string name, int r, int c, int staff, int vehicles);

    // Getters
    int getId()               const { return id; }
    string getName()          const { return name; }
    int getRow()              const { return row; }
    int getCol()              const { return col; }
    int getStaff()            const { return staffAvailable; }
    int getVehicles()         const { return vehiclesAvailable; }
    int getTotalResponses()   const { return totalResponses; }

    // Actions
    bool dispatchTeam();           // reduces available staff/vehicles
    void addResponse()             { totalResponses++; }

    // Display
    void displayInfo() const;
};

#endif
