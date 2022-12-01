

#ifndef FINAL_ROUTE_H
#define FINAL_ROUTE_H

#include <string>
#include <map>
#include <queue>
#include <set>
#include "airports.h"
using namespace std;

class route {
public:
    string airlineIata, airlineId, sourceIata, sourceId, destIata, destId;
    static map<string,vector<route>> codes;
    static map<string,vector<route>> flightroute;
    static map<string,string> path;

    route(string airlineIata,string airlineId,string sourceIata,string sourceId,string destIata,string destId);
    static void getPath(string file);
    static vector<string> bfs(airports x, airports y);

    static vector<string> reconstruct(string destinationIata);
    static bool containsDeque(deque<string> deque, string x);
    static void write(string start, string destination, vector<string> flights, vector<string> pathFound);
    static bool containsSet(set<string> set, string x);

    static void config();
    static void config2();


};


#endif //FINAL_ROUTE_H
