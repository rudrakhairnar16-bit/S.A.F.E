================================================================
     RAPE CRISIS CENTER RESPONSE TIME SIMULATOR
     Women Safety Project — C++ Mini Project
================================================================

DESCRIPTION:
  Simulates how quickly a rape crisis center can respond to a
  victim's distress signal on a 10x10 city grid. Uses Dijkstra's
  algorithm to find the fastest route based on time of day,
  danger zones, and police presence.

================================================================
FILES:
  main.cpp                — Entry point, user input handling
  City.h / City.cpp       — 10x10 grid map management
  Victim.h / Victim.cpp   — Victim data and time factor
  CrisisCenter.h / .cpp   — Crisis center data and dispatch
  ResponseSimulator.h/.cpp— Core simulation + Dijkstra's routing
  data/city_map.txt       — City map input file
  data/report.txt         — Auto-generated simulation report

================================================================
HOW TO COMPILE:
  g++ main.cpp City.cpp Victim.cpp CrisisCenter.cpp ResponseSimulator.cpp -o simulator

HOW TO RUN:
  ./simulator   (Linux/Mac)
  simulator.exe (Windows)

================================================================
CITY MAP LEGEND:
  0 = Normal Zone
  1 = Danger Zone
  2 = Crisis Center (C)
  3 = Police Post (P)
  V = Victim (shown after input)
  > = Route Path

================================================================
FEATURES:
  >> 10x10 city grid loaded from file
  >> Up to 5 victims simulated at once
  >> Dijkstra's algorithm for fastest route
  >> Time-of-day weighted routing (night = slower)
  >> Danger zones increase response cost
  >> Police zones speed up response
  >> Response score calculated per victim
  >> Safety tips generated based on simulation
  >> Full detailed report saved to data/report.txt

================================================================
C++ CONCEPTS USED:
  >> OOP — Classes, Objects, Encapsulation
  >> STL — vector, priority_queue, tuple, pair
  >> File I/O — ifstream, ofstream
  >> Dijkstra's Algorithm
  >> 2D Vectors
  >> Input Validation
  >> Structs

================================================================
SOCIAL IMPACT:
  This project raises awareness about:
  >> Importance of crisis center locations in a city
  >> How time of day affects response time for women in crisis
  >> Need for more police presence in danger zones
  >> How faster response improves victim outcomes

================================================================
                Made with purpose — for women's safety
================================================================
