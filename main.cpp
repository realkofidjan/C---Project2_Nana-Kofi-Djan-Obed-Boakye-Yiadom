#include "airports.h"
#include "route.h"
#include <iostream>
using namespace std;


map<string, vector<airports>> airports::portMap = *new map<string, vector<airports>>;
map<string,vector<route>> route::codes = *new map<string, vector<route>>;
map<string,vector<route>> route::flightroute = *new map<string,vector<route>>;
map<string,string> route::path = *new map<string,string>;

int main() {
    route::getPath("f.txt");

}
