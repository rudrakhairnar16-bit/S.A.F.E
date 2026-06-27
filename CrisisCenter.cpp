#include "CrisisCenter.h"
#include <iostream>
using namespace std;

CrisisCenter::CrisisCenter(int id, string name, int r, int c, int staff, int vehicles)
    : id(id), name(name), row(r), col(c),
      staffAvailable(staff), vehiclesAvailable(vehicles), totalResponses(0) {}

bool CrisisCenter::dispatchTeam() {
    if (staffAvailable > 0 && vehiclesAvailable > 0) {
        staffAvailable--;
        vehiclesAvailable--;
        totalResponses++;
        return true;
    }
    return false; // no resources available
}

void CrisisCenter::displayInfo() const {
    cout << "  Center #" << id << " : " << name << "\n";
    cout << "  Location     : Zone (" << row << ", " << col << ")\n";
    cout << "  Staff        : " << staffAvailable << " available\n";
    cout << "  Vehicles     : " << vehiclesAvailable << " available\n";
    cout << "  Responses    : " << totalResponses << " total\n";
}
