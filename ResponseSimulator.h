#ifndef RESPONSESIMULATOR_H
#define RESPONSESIMULATOR_H

#include "City.h"
#include "Victim.h"
#include "CrisisCenter.h"
#include <vector>
#include <string>
using namespace std;

// Stores result for each victim
struct ResponseResult {
    int victimId;
    int victimRow, victimCol;
    string centerName;
    int centerRow, centerCol;
    int routeDistance;
    float responseTimeMin;
    int score;
    string outcome;
    string teamType;
    vector<pair<int,int>> path;
};

class ResponseSimulator {
private:
    City& city;
    vector<CrisisCenter>& centers;
    vector<ResponseResult> results;

    // Core algorithms
    float cellCost(int r, int c, float timeFactor) const;
    vector<pair<int,int>> dijkstraPath(int sr, int sc, int er, int ec, float timeFactor) const;
    float pathCost(const vector<pair<int,int>>& path, float timeFactor) const;
    CrisisCenter* findNearestCenter(Victim& v);
    int calculateScore(float responseTime, int distressLevel, bool isDangerZone) const;
    string getOutcome(int score) const;
    string getTeamType(int distressLevel) const;

public:
    ResponseSimulator(City& city, vector<CrisisCenter>& centers);

    void simulate(vector<Victim>& victims);
    void displayResult(const ResponseResult& r) const;
    void displayAllResults() const;
    void saveReport(const string& filename, const vector<Victim>& victims) const;
    void displaySafetyTips(const vector<Victim>& victims) const;
};

#endif
