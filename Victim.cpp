#include "Victim.h"
#include <iostream>
#include <iomanip>
using namespace std;

Victim::Victim(int id, int r, int c, int distress, string time)
    : id(id), row(r), col(c), distressLevel(distress),
      timeOfDay(time), status("waiting") {}

float Victim::getTimeFactor() const {
    if (timeOfDay == "morning")   return 1.0f;
    if (timeOfDay == "afternoon") return 1.2f;
    if (timeOfDay == "evening")   return 1.6f;
    if (timeOfDay == "night")     return 2.2f;
    return 1.0f;
}

string Victim::getTimeLabel() const {
    if (timeOfDay == "morning")   return "06:30 AM";
    if (timeOfDay == "afternoon") return "02:15 PM";
    if (timeOfDay == "evening")   return "08:45 PM";
    if (timeOfDay == "night")     return "11:30 PM";
    return "Unknown";
}

void Victim::displayInfo() const {
    cout << "\n  +-----------------------------------------+\n";
    cout << "  | VICTIM #" << id << " DETAILS                        |\n";
    cout << "  +-----------------------------------------+\n";
    cout << "  | Location    : Zone (" << row << ", " << col << ")\n";
    cout << "  | Time        : " << getTimeLabel() << " (" << timeOfDay << ")\n";
    cout << "  | Distress    : " << distressLevel << "/10\n";
    cout << "  | Status      : " << status << "\n";
    cout << "  +-----------------------------------------+\n";
}
