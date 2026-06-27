#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "City.h"
#include "Victim.h"
#include "CrisisCenter.h"
#include "ResponseSimulator.h"
using namespace std;

void printHeader() {
    cout << "\n";
    cout << "  ================================================================\n";
    cout << "  ||                                                            ||\n";
    cout << "  ||       RAPE CRISIS CENTER RESPONSE TIME SIMULATOR          ||\n";
    cout << "  ||              Women Safety Project — C++                   ||\n";
    cout << "  ||                                                            ||\n";
    cout << "  ================================================================\n\n";
}

void printSeparator() {
    cout << "\n  ----------------------------------------------------------------\n";
}

string getTimeOfDay() {
    int choice;
    cout << "\n  Select time of day:\n";
    cout << "    1. Morning   (06:00 AM - 12:00 PM) — Normal traffic\n";
    cout << "    2. Afternoon (12:00 PM - 06:00 PM) — Moderate traffic\n";
    cout << "    3. Evening   (06:00 PM - 10:00 PM) — High risk\n";
    cout << "    4. Night     (10:00 PM - 06:00 AM) — Very high risk\n";
    cout << "  Enter choice (1-4): ";

    while (true) {
        cin >> choice;
        if (cin.fail() || choice < 1 || choice > 4) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid! Enter 1-4: ";
        } else break;
    }

    if (choice == 1) return "morning";
    if (choice == 2) return "afternoon";
    if (choice == 3) return "evening";
    return "night";
}

int getDistressLevel() {
    int level;
    cout << "  Enter distress level (1=low to 10=critical): ";
    while (true) {
        cin >> level;
        if (cin.fail() || level < 1 || level > 10) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid! Enter 1-10: ";
        } else break;
    }
    return level;
}

int main() {
    printHeader();

    // -------------------------
    // Step 1: Load City Map
    // -------------------------
    City city(10, 10);
    cout << "  [LOADING]  Loading city map from file...\n";
    if (!city.loadFromFile("data/city_map.txt")) {
        cout << "  [ERROR]    Could not load city map. Exiting.\n";
        return 1;
    }
    cout << "  [SUCCESS]  City map loaded!\n";

    // -------------------------
    // Step 2: Setup Crisis Centers
    // -------------------------
    vector<CrisisCenter> centers = {
        CrisisCenter(1, "Shakti Crisis Center",   1, 8, 5, 3),
        CrisisCenter(2, "Nirbhaya Help Center",   4, 2, 4, 2),
        CrisisCenter(3, "Aasha Foundation",       7, 6, 6, 4),
        CrisisCenter(4, "Suraksha Care Center",   0, 4, 3, 2),
    };

    // -------------------------
    // Step 3: Display City Map
    // -------------------------
    city.display();

    cout << "  CRISIS CENTERS IN CITY:\n";
    for (auto& c : centers) c.displayInfo();

    // -------------------------
    // Step 4: Enter Victims
    // -------------------------
    printSeparator();
    int numVictims;
    cout << "\n  How many victims to simulate? (1-5): ";
    while (true) {
        cin >> numVictims;
        if (cin.fail() || numVictims < 1 || numVictims > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid! Enter 1-5: ";
        } else break;
    }

    vector<Victim> victims;

    for (int i = 1; i <= numVictims; i++) {
        printSeparator();
        cout << "\n  ENTER DETAILS FOR VICTIM #" << i << "\n\n";

        int row, col;
        cout << "  Enter victim row (0-9): ";
        while (true) {
            cin >> row;
            if (cin.fail() || row < 0 || row > 9) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  Invalid! Enter 0-9: ";
            } else break;
        }

        cout << "  Enter victim col (0-9): ";
        while (true) {
            cin >> col;
            if (cin.fail() || col < 0 || col > 9) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  Invalid! Enter 0-9: ";
            } else break;
        }

        // Check if cell is valid for victim placement
        int cell = city.getCell(row, col);
        if (cell == CRISIS || cell == POLICE) {
            cout << "  [WARNING]  That zone is a Crisis Center or Police Post.\n";
            cout << "             Please choose a different location.\n";
            i--; continue;
        }

        string time    = getTimeOfDay();
        int distress   = getDistressLevel();

        victims.push_back(Victim(i, row, col, distress, time));
        cout << "  [ADDED]    Victim #" << i << " registered at Zone ("
             << row << ", " << col << ")\n";
    }

    // -------------------------
    // Step 5: Run Simulation
    // -------------------------
    printSeparator();
    ResponseSimulator simulator(city, centers);
    simulator.simulate(victims);

    // -------------------------
    // Step 6: Display Results
    // -------------------------
    simulator.displayAllResults();

    // -------------------------
    // Step 7: Show Map With Routes
    // -------------------------
    vector<pair<int,int>> allPaths;
    vector<pair<int,int>> victimPositions;
    for (auto& v : victims)
        victimPositions.push_back({v.getRow(), v.getCol()});

    cout << "\n  Displaying city map with victim positions...\n";
    city.displayWithPath(allPaths, victimPositions);

    // -------------------------
    // Step 8: Safety Tips
    // -------------------------
    simulator.displaySafetyTips(victims);

    // -------------------------
    // Step 9: Save Report
    // -------------------------
    printSeparator();
    simulator.saveReport("data/report.txt", victims);

    cout << "\n  ================================================================\n";
    cout << "  ||          SIMULATION COMPLETE — THANK YOU                  ||\n";
    cout << "  ||      Every second saved is a life protected.              ||\n";
    cout << "  ================================================================\n\n";

    return 0;
}
