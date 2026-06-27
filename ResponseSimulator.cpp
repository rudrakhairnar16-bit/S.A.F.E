#include "ResponseSimulator.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>
#include <vector>
#include <cmath>
#include <climits>
#include <algorithm>
#include <ctime>
using namespace std;

ResponseSimulator::ResponseSimulator(City& city, vector<CrisisCenter>& centers)
    : city(city), centers(centers) {}

// Cost to pass through a cell based on type and time of day
float ResponseSimulator::cellCost(int r, int c, float timeFactor) const {
    int cell = city.getCell(r, c);
    float base = 1.0f;
    if (cell == DANGER) base = 1.0f + (timeFactor * 1.5f); // danger zones slower at night
    if (cell == POLICE) base = 0.5f;                        // police zones faster
    if (cell == NORMAL) base = 1.0f * timeFactor;
    return base;
}

// Dijkstra's algorithm — finds fastest route weighted by time of day
vector<pair<int,int>> ResponseSimulator::dijkstraPath(int sr, int sc, int er, int ec, float timeFactor) const {
    int rows = city.getRows();
    int cols = city.getCols();

    vector<vector<float>> dist(rows, vector<float>(cols, 1e9));
    vector<vector<pair<int,int>>> prev(rows, vector<pair<int,int>>(cols, {-1,-1}));

    // priority queue: {cost, row, col}
    priority_queue<tuple<float,int,int>, vector<tuple<float,int,int>>, greater<>> pq;

    dist[sr][sc] = 0;
    pq.push({0, sr, sc});

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!pq.empty()) {
        auto [d, r, c] = pq.top(); pq.pop();

        if (r == er && c == ec) break;
        if (d > dist[r][c]) continue;

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (!city.isValid(nr, nc)) continue;
            if (city.getCell(nr, nc) == CRISIS && !(nr == er && nc == ec)) continue;

            float cost = d + cellCost(nr, nc, timeFactor);
            if (cost < dist[nr][nc]) {
                dist[nr][nc] = cost;
                prev[nr][nc] = {r, c};
                pq.push({cost, nr, nc});
            }
        }
    }

    // Reconstruct path
    vector<pair<int,int>> path;
    int cr = er, cc = ec;
    while (!(cr == sr && cc == sc)) {
        path.push_back({cr, cc});
        auto [pr, pc] = prev[cr][cc];
        if (pr == -1) break;
        cr = pr; cc = pc;
    }
    path.push_back({sr, sc});
    reverse(path.begin(), path.end());
    return path;
}

float ResponseSimulator::pathCost(const vector<pair<int,int>>& path, float timeFactor) const {
    float cost = 0;
    for (auto& [r, c] : path)
        cost += cellCost(r, c, timeFactor);
    return cost;
}

// Find nearest crisis center with available resources
CrisisCenter* ResponseSimulator::findNearestCenter(Victim& v) {
    CrisisCenter* best = nullptr;
    float bestCost = 1e9;

    for (auto& center : centers) {
        if (center.getStaff() <= 0) continue; // skip if no staff

        vector<pair<int,int>> path = dijkstraPath(
            v.getRow(), v.getCol(),
            center.getRow(), center.getCol(),
            v.getTimeFactor()
        );

        float cost = pathCost(path, v.getTimeFactor());
        if (cost < bestCost) {
            bestCost = cost;
            best = &center;
        }
    }
    return best;
}

int ResponseSimulator::calculateScore(float responseTime, int distressLevel, bool isDangerZone) const {
    int score = 100;
    score -= (int)(responseTime * 2.5f);       // deduct for slow response
    score -= (distressLevel - 1) * 3;          // deduct for high distress
    if (isDangerZone) score -= 10;             // deduct for danger zone
    return max(5, min(100, score));
}

string ResponseSimulator::getOutcome(int score) const {
    if (score >= 80) return "EXCELLENT  — Help arrived quickly, victim is safe";
    if (score >= 60) return "GOOD       — Help arrived, victim receiving care";
    if (score >= 40) return "MODERATE   — Delayed response, victim needs support";
    if (score >= 20) return "POOR       — Significant delay, critical situation";
    return "CRITICAL   — Severe delay, immediate escalation needed";
}

string ResponseSimulator::getTeamType(int distressLevel) const {
    if (distressLevel >= 8) return "Medical Team + Counselor + Police";
    if (distressLevel >= 5) return "Counselor + Medical Support";
    return "Counselor + Support Staff";
}

void ResponseSimulator::simulate(vector<Victim>& victims) {
    results.clear();
    cout << "\n  ============================================\n";
    cout << "  ||   SIMULATION RUNNING — PLEASE WAIT    ||\n";
    cout << "  ============================================\n\n";

    for (auto& victim : victims) {
        cout << "\n  [ALERT]    Distress signal from Victim #" << victim.getId()
             << " at Zone (" << victim.getRow() << "," << victim.getCol() << ")\n";
        cout << "  [TIME]     " << victim.getTimeLabel()
             << " — Time factor: " << victim.getTimeFactor() << "x\n";
        cout << "  [SEARCH]   Scanning city for fastest crisis center route...\n";

        CrisisCenter* center = findNearestCenter(victim);

        if (!center) {
            cout << "  [ERROR]    No crisis center available! All resources exhausted.\n";
            victim.setStatus("no resource");
            continue;
        }

        // Get path
        vector<pair<int,int>> path = dijkstraPath(
            victim.getRow(), victim.getCol(),
            center->getRow(), center->getCol(),
            victim.getTimeFactor()
        );

        float routeCost = pathCost(path, victim.getTimeFactor());
        float responseTime = routeCost * 3.5f; // convert cost to minutes
        int distance = (int)path.size() - 1;
        bool isDanger = city.getCell(victim.getRow(), victim.getCol()) == DANGER;
        int score = calculateScore(responseTime, victim.getDistress(), isDanger);
        string outcome = getOutcome(score);
        string team = getTeamType(victim.getDistress());

        // Dispatch team
        center->dispatchTeam();
        victim.setStatus("helped");

        // Store result
        ResponseResult res;
        res.victimId      = victim.getId();
        res.victimRow     = victim.getRow();
        res.victimCol     = victim.getCol();
        res.centerName    = center->getName();
        res.centerRow     = center->getRow();
        res.centerCol     = center->getCol();
        res.routeDistance = distance;
        res.responseTimeMin = responseTime;
        res.score         = score;
        res.outcome       = outcome;
        res.teamType      = team;
        res.path          = path;
        results.push_back(res);

        // Console output
        cout << "  [FOUND]    " << center->getName()
             << " at Zone (" << center->getRow() << "," << center->getCol() << ")\n";
        cout << "  [ROUTE]    Distance: " << distance << " zones | "
             << "Cost: " << fixed << setprecision(1) << routeCost << "\n";
        if (isDanger)
            cout << "  [WARNING]  Victim in DANGER ZONE — priority escalated!\n";
        cout << "  [DISPATCH] Team: " << team << "\n";
        cout << "  [ETA]      Estimated response time: "
             << fixed << setprecision(1) << responseTime << " minutes\n";
        cout << "  [SCORE]    " << score << "/100 — " << outcome << "\n";
        cout << "  ------------------------------------------\n";
    }
}

void ResponseSimulator::displayAllResults() const {
    cout << "\n  ============================================\n";
    cout << "  ||         SIMULATION SUMMARY             ||\n";
    cout << "  ============================================\n\n";

    cout << "  " << left << setw(8)  << "Victim"
         << setw(14) << "Location"
         << setw(26) << "Crisis Center"
         << setw(10) << "ETA(min)"
         << setw(8)  << "Score"
         << "Outcome\n";
    cout << "  " << string(85, '-') << "\n";

    float totalTime = 0;
    int totalScore = 0;

    for (auto& r : results) {
        string loc = "(" + to_string(r.victimRow) + "," + to_string(r.victimCol) + ")";
        cout << "  " << left
             << setw(8)  << ("#" + to_string(r.victimId))
             << setw(14) << loc
             << setw(26) << r.centerName
             << setw(10) << fixed << setprecision(1) << r.responseTimeMin
             << setw(8)  << (to_string(r.score) + "/100")
             << r.outcome.substr(0, 20) << "\n";
        totalTime  += r.responseTimeMin;
        totalScore += r.score;
    }

    cout << "  " << string(85, '-') << "\n";
    if (!results.empty()) {
        cout << "  Average Response Time : "
             << fixed << setprecision(1) << totalTime / results.size() << " minutes\n";
        cout << "  Average Score         : "
             << totalScore / results.size() << "/100\n";
    }
    cout << "\n";
}

void ResponseSimulator::displaySafetyTips(const vector<Victim>& victims) const {
    cout << "\n  ============================================\n";
    cout << "  ||       SAFETY RECOMMENDATIONS          ||\n";
    cout << "  ============================================\n\n";

    // Check if any night victims
    bool hasNight = false, hasDanger = false;
    for (auto& v : victims) {
        if (v.getTime() == "night") hasNight = true;
        if (city.getCell(v.getRow(), v.getCol()) == DANGER) hasDanger = true;
    }

    cout << "  GENERAL RECOMMENDATIONS:\n";
    cout << "  >> Install emergency call points in every danger zone\n";
    cout << "  >> Increase crisis center staff during peak hours\n";
    cout << "  >> Add police patrol routes near all danger zones\n";
    cout << "  >> Create fast-response corridor between centers\n\n";

    if (hasNight) {
        cout << "  NIGHT-SPECIFIC RECOMMENDATIONS:\n";
        cout << "  >> Double patrol units between 10PM - 6AM\n";
        cout << "  >> Install emergency SOS poles with lights in dark zones\n";
        cout << "  >> Pre-position response vehicles closer to danger areas\n\n";
    }

    if (hasDanger) {
        cout << "  DANGER ZONE RECOMMENDATIONS:\n";
        cout << "  >> Add CCTV surveillance in all danger zones immediately\n";
        cout << "  >> Place crisis center satellite posts near danger zones\n";
        cout << "  >> Train community volunteers as first-responders\n\n";
    }

    if (!results.empty()) {
        int avgScore = 0;
        for (auto& r : results) avgScore += r.score;
        avgScore /= results.size();

        if (avgScore < 60) {
            cout << "  URGENT SYSTEM IMPROVEMENTS NEEDED:\n";
            cout << "  >> Average response score is LOW (" << avgScore << "/100)\n";
            cout << "  >> Recommend adding 2 more crisis centers to city\n";
            cout << "  >> Increase vehicle fleet by at least 50%\n";
            cout << "  >> Consider dedicated emergency corridors on city map\n\n";
        }
    }
}

void ResponseSimulator::saveReport(const string& filename, const vector<Victim>& victims) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "  [ERROR] Could not save report to " << filename << endl;
        return;
    }

    time_t now = time(0);
    string timestamp = ctime(&now);

    file << "================================================================\n";
    file << "     RAPE CRISIS CENTER RESPONSE TIME SIMULATOR — FULL REPORT\n";
    file << "================================================================\n";
    file << "Generated  : " << timestamp;
    file << "Victims    : " << victims.size() << "\n";
    file << "Centers    : " << centers.size() << "\n";
    file << "================================================================\n\n";

    // Victim details
    file << "----------------------------------------------------------------\n";
    file << "SECTION 1 — VICTIM DETAILS\n";
    file << "----------------------------------------------------------------\n\n";

    for (auto& v : victims) {
        file << "Victim #" << v.getId() << "\n";
        file << "  Location      : Zone (" << v.getRow() << ", " << v.getCol() << ")\n";
        file << "  Time of Day   : " << v.getTimeLabel() << " (" << v.getTime() << ")\n";
        file << "  Distress Level: " << v.getDistress() << "/10\n";
        file << "  Status        : " << v.getStatus() << "\n\n";
    }

    // Crisis center details
    file << "----------------------------------------------------------------\n";
    file << "SECTION 2 — CRISIS CENTER STATUS\n";
    file << "----------------------------------------------------------------\n\n";

    for (auto& c : centers) {
        file << "Center #" << c.getId() << " — " << c.getName() << "\n";
        file << "  Location      : Zone (" << c.getRow() << ", " << c.getCol() << ")\n";
        file << "  Staff Left    : " << c.getStaff() << "\n";
        file << "  Vehicles Left : " << c.getVehicles() << "\n";
        file << "  Total Served  : " << c.getTotalResponses() << " victims\n\n";
    }

    // Response results
    file << "----------------------------------------------------------------\n";
    file << "SECTION 3 — RESPONSE RESULTS (PER VICTIM)\n";
    file << "----------------------------------------------------------------\n\n";

    float totalTime = 0;
    int totalScore = 0;

    for (auto& r : results) {
        file << "Victim #" << r.victimId << " Response\n";
        file << "  Victim Zone   : (" << r.victimRow << ", " << r.victimCol << ")\n";
        file << "  Center Reached: " << r.centerName << "\n";
        file << "  Center Zone   : (" << r.centerRow << ", " << r.centerCol << ")\n";
        file << "  Route Distance: " << r.routeDistance << " zones\n";
        file << "  Response Time : " << fixed << setprecision(1) << r.responseTimeMin << " minutes\n";
        file << "  Team Dispatch : " << r.teamType << "\n";
        file << "  Score         : " << r.score << "/100\n";
        file << "  Outcome       : " << r.outcome << "\n";

        // Print path
        file << "  Route Taken   : ";
        for (auto& p : r.path)
            file << "(" << p.first << "," << p.second << ") ";
        file << "\n\n";

        totalTime  += r.responseTimeMin;
        totalScore += r.score;
    }

    // Summary
    file << "----------------------------------------------------------------\n";
    file << "SECTION 4 — OVERALL SUMMARY\n";
    file << "----------------------------------------------------------------\n\n";

    if (!results.empty()) {
        file << "  Total Victims Handled : " << results.size() << "\n";
        file << "  Avg Response Time     : "
             << fixed << setprecision(1) << totalTime / results.size() << " minutes\n";
        file << "  Avg Score             : " << totalScore / results.size() << "/100\n";
        int avg = totalScore / results.size();
        file << "  System Rating         : "
             << (avg >= 80 ? "EXCELLENT" : avg >= 60 ? "GOOD" :
                 avg >= 40 ? "MODERATE"  : "NEEDS IMPROVEMENT") << "\n\n";
    }

    // Safety tips
    file << "----------------------------------------------------------------\n";
    file << "SECTION 5 — SAFETY RECOMMENDATIONS\n";
    file << "----------------------------------------------------------------\n\n";

    file << "  >> Install emergency call points in every danger zone\n";
    file << "  >> Increase crisis center staff during peak hours\n";
    file << "  >> Add police patrol routes near all danger zones\n";
    file << "  >> Create fast-response corridors between centers\n";
    file << "  >> Install CCTV in all danger zones\n";
    file << "  >> Train community volunteers as first-responders\n";
    file << "  >> Double patrol units between 10PM - 6AM\n\n";

    file << "================================================================\n";
    file << "                    END OF REPORT\n";
    file << "================================================================\n";

    file.close();
    cout << "  [SAVED]    Full report saved to: " << filename << "\n";
}
