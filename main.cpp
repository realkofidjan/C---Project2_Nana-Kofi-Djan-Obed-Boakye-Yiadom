#include <iostream>
#include "Airport.h"
#include <iostream>
#include "Route.h"
#include <cmath>
using namespace std;

map<string, vector<Airport>> Airport::airports = *new map<string, vector<Airport>>;
map<string, vector<Route>> Route::routes = *new map<string, vector<Route>>;
map<string, vector<Route>> Route::flights = *new map<string, vector<Route>>;
map<string, string> Route::parents = *new map<string, string>;
map<string, Airport> Airport::codes = *new map<string, Airport>;


int main() {
    Route::findRoute("test.txt");

}

